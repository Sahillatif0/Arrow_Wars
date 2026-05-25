#include <iostream>
#include <raylib.h>
#include <cmath>
#include <vector>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include "menu.h"
#include "constants.h"
#include "sound_manager.h"
#include "ai_learning.h"
#include "background.h"
#include <cstdlib>
#include <ctime>
#include "titlescreen.h"
#include <fstream>

using namespace std;


bool hscircleCollision(Vector2 pos1, Vector2 pos2, int rad1, int rad2){
    DrawCircle(pos2.x,pos2.y-400,50,WHITE);
    return (sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - (pos2.y-400), 2)) < rad1 + 30);
}
bool circleCollision(Vector2 pos1, Vector2 pos2, int rad1, int rad2){
    return (sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2)) < rad1 + rad2);
}
bool boxCollision(Vector2 Boxpos, Vector2 arrowPos,int arrowradius, double boxHeight, double boxWidth){
    float arrowLeft = arrowPos.x - arrowradius*2;
    float arrowRight = arrowPos.x + arrowradius*2;
    float arrowTop = arrowPos.y - arrowradius*2;
    float arrowBottom = arrowPos.y + arrowradius*2;
   
    float boxLeft = Boxpos.x+190;
    float boxRight = Boxpos.x + boxWidth+190;
    float boxTop = Boxpos.y+190;
    float boxBottom = Boxpos.y + boxHeight+190;
   
    if (arrowLeft < boxRight && arrowRight > boxLeft && arrowTop < boxBottom && arrowBottom > boxTop) {
        return true;  
    }
    return false;
}
class data{
    int health1, health2, round;
    bool isLeft;
    public:
    int geth1(){
        return health1;
    }
    int geth2(){
        return health2;
    }
    int getround(){
        return round;
    }
    bool getisleft(){
        return isLeft;
    }
    void seth1(int a){
        health1=a;
    }
    void seth2(int a){
        health2=a;
    }
    void setil(bool a){
        isLeft=a;
    }
    void setround(int r){
        round = r;
    }
    void savedata()
    {
        ofstream outfile;
        outfile.open("playerdata.txt");
        if(outfile.is_open()){
            outfile << health1 << endl << health2 << endl << round;
            outfile.close();
        }
    }
    void readdata()
    {
        ifstream infile;
        infile.open("playerdata.txt");
        if(infile.is_open()){
            infile >> health1;
            infile >> health2;
            infile >> round;
            infile.close();
        }
    }
};

class Arrow{
    Vector2 position, initialVel;
    Color color;
    Texture ball;
    double time, power;
    int moveDir, radius, angle, velocity;
    public:
    template<class, class> friend class GamePlay;
        friend class Player;
        Arrow(Vector2 pos= {0,0}, Vector2 vel={0,0},int velo=50, int rad=5, int ang=45,int pow=0.6, Color col=RED, int moveDir=1):position(pos),velocity(velo),radius(rad), angle(ang),power(pow),color(col),time(0),moveDir(moveDir),initialVel(vel){
            ball = LoadTexture("assets/ball.png");
        }
        void update(float x, float y){
            position.x = x;
            position.y = y;
        }
        void draw(){
        DrawTexturePro(ball, {0, 0, float(ball.width), float(ball.height)}, {position.x, position.y, ball.width * 0.1f, ball.height * 0.1f}, {0, 0}, angle, WHITE);
    }
    void move(int x){
        if (((position.x < SCREEN_WIDTH / 2 || (x > (SCREEN_WIDTH - 150) && x < ((SCREEN_WIDTH - 150) + initialVel.x))) && moveDir == 1) || ((position.x > SCREEN_WIDTH / 2 || (x < 150 && x > (150 - initialVel.x))) && moveDir == -1))
            position.x += moveDir * (velocity*power* 0.5* cos((angle * PI) / 180.0));
        position.y -= (velocity* power * sin((angle * PI) / 180.0) - 0.5 * GRAVITY * time * time);
    }
    void reset(){
        time = 0;
    }
};
class PlayerKeys{
    public:
        int up, down, left, right, shoot;
        PlayerKeys(int up, int down, int left, int right, int shoot):up(up),down(down),left(left),right(right),shoot(shoot){}
};
class Player{
    public:
        Vector2 position, initial, arrowVel, mousePos, screenDiffPos;
        Vector2 hitOffset, recoilVelocity, basePosition;
        int health, angle, velocity;
        Color color;
        double power, health2;
        Arrow arrow;
        bool isLeft, isShooting, turn, settingUp, mouseDown, turnPlayed;
        bool isHit;
        float hitTimer, hitDuration;
        Texture sprite;
        template<class, class> friend class GamePlay;
        Player(Vector2 pos, int h = PLAYER_HEALTH, Color col = RED, bool isLeft = true, bool turn = false, int ang = BASE_ANGLE, double pow = BASE_POWER) : position(pos), health(h), health2(h), color(col), angle(ang), power(pow), isLeft(isLeft), isShooting(false), turn(turn), settingUp(false), mouseDown(false), screenDiffPos({SCREEN_DIFF_X, SCREEN_DIFF_Y}), velocity(SCREEN_DIFF_X/2), turnPlayed(false), isHit(false), hitTimer(0), hitDuration(0.5f), basePosition(pos), hitOffset({0, 0}), recoilVelocity({0, 0}){
            initial.x = isLeft ? SCREEN_DIFF_X : (SCREEN_WIDTH - SCREEN_DIFF_X);
            initial.y = SCREEN_HEIGHT - SCREEN_DIFF_Y;
            arrowVel = {float(velocity*cos(angle*PI/180.0)), float(velocity*sin(angle*PI/180.0))};
            arrow = Arrow(initial, arrowVel,velocity, ARROW_RADIUS, angle,power, color, (isLeft) ? 1 : -1);
            mousePos = initial;
            sprite = isLeft ? LoadTexture("assets/sprites.png") : LoadTexture("assets/Bluesprites2.png");
        }
        void triggerHit() {
            if (!isHit) {
                isHit = true;
                hitTimer = hitDuration;
                recoilVelocity.x = isLeft ? -8.0f : 8.0f;
                recoilVelocity.y = -5.0f;
            }
        }
        void updateHitAnimation(float deltaTime) {
            if (isHit) {
                hitTimer -= deltaTime;
                hitOffset.x += recoilVelocity.x * deltaTime * 60;
                hitOffset.y += recoilVelocity.y * deltaTime * 60;
                recoilVelocity.y += 15.0f * deltaTime;
                if (hitTimer <= 0) {
                    isHit = false;
                    hitTimer = 0;
                    hitOffset = {0, 0};
                    recoilVelocity = {0, 0};
                }
            }
        }
        void draw(){
            Rectangle sourceRec = {float((sprite.width / 31) * ((isLeft ? 0 : 31) - int(angle / 3))), float(sprite.height), sprite.width / 31, sprite.height};
            Rectangle destRec = {isLeft ? position.x : position.x - sprite.width / 31, position.y - 300 + hitOffset.y, sprite.width / 31, sprite.height};
            DrawTexturePro(sprite, sourceRec, destRec, {0, 0}, 0.0f, WHITE);

            if (isHit) {
                float flash = (hitTimer / hitDuration) * 0.5f;
                DrawRectangleRec(destRec, Fade(WHITE, flash));
            }

            float x = initial.x, y = initial.y - sprite.height / 2 - 30;
            int sign = isLeft ? 1 : -1;
            if (mouseDown && turn){
                double time = 0;
                for (int i = 30; i > 0; i--){
                    DrawCircle(x, y, (5 - 5*float(i / 30)), Fade(color, power * 0.03 * i + 0.25));
                    time += 1/20.0;
                    y -= (velocity* power * sin((angle * PI) / 180.0) - 0.5 * GRAVITY * time * time);
                    x += sign * 0.5* (velocity*power * cos((angle * PI) / 180.0));
                }
            }
            drawText();
        }
        void drawText(){
            string angle_str = "Angle: " + to_string(angle);
            string pow_str = "Power: " + to_string(int(power * 100));
            DrawText(angle_str.c_str(), position.x + (isLeft? 130:(-190)), position.y - 350, 20, WHITE);
            DrawText(pow_str.c_str(), position.x + (isLeft? 130:(-190)), position.y - 320, 20, WHITE);
        }
        void shoot(){
            if (turn){
                arrow.update(position.x, position.y - sprite.height / 2 - 30);
                arrow.draw();
                isShooting = true;
            }
        }
        template <class P> void updateArrow(P &p2){
            if (turn){
                float x = isLeft ? (SCREEN_WIDTH - screenDiffPos.x) : screenDiffPos.x, y = SCREEN_HEIGHT - 200;
                string dist_str = to_string(abs(int(p2.position.x - arrow.position.x))) + "m";
                DrawText(dist_str.c_str(), x, y, 20, WHITE);
                DrawLineEx(Vector2{x, y + 30}, Vector2{x + 50, y + 30}, 2, p2.color);
                if (isLeft)
                    DrawTriangle(Vector2{x + 50, y + 30}, Vector2{x + 40, y + 20}, Vector2{x + 40, y + 40}, p2.color);
                else
                    DrawTriangle(Vector2{x, y + 20}, Vector2{x - 10, y + 30}, Vector2{x, y + 40}, p2.color);
            }
            if (isShooting){
                arrow.angle = angle;
                arrow.power = power;
                arrow.draw();
                arrow.time += 1.0 / 20.0;
                arrow.move(p2.position.x);
                if (hscircleCollision(arrow.position, p2.position, arrow.radius, 300) && ((arrow.moveDir != 1 && p2.isLeft) || (arrow.moveDir == 1 && !p2.isLeft))){
                    p2.triggerHit();
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    settingUp = true;
                    p2.health -= HEADSHOT_DAMAGE;
                    g_soundManager.play("hitting");
                    g_aiLearning.recordPlayerShot(angle, power, HEADSHOT_DAMAGE, true);
                }
                if (circleCollision(arrow.position, p2.position, arrow.radius, 300) && ((arrow.moveDir != 1 && p2.isLeft) || (arrow.moveDir == 1 && !p2.isLeft))){
                    p2.triggerHit();
                    g_soundManager.play("hitting");
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    settingUp = true;
                    p2.health -= BODYYSHOT_DAMAGE;
                    g_aiLearning.recordPlayerShot(angle, power, BODYYSHOT_DAMAGE, true);
                }
                if(p2.health<0)
                    p2.health = 0;
                if(arrow.position.y > SCREEN_HEIGHT){
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    settingUp = true;
                }
                if((arrow.position.x > SCREEN_WIDTH/2 && isLeft) || (arrow.position.x < SCREEN_WIDTH/2 && !isLeft)){
                    position.x-= arrow.moveDir*(arrow.velocity*power*cos((angle*PI)/180.0));
                    p2.position.x-= arrow.moveDir*(arrow.velocity*power*cos((angle*PI)/180.0));
                }
            }
            if(settingUp){
                float moveAmount = (arrow.velocity);
                if(p2.isLeft){
                    if(p2.position.x>screenDiffPos.x){
                        if(p2.position.x-screenDiffPos.x<moveAmount){
                            p2.position.x-=(p2.position.x-screenDiffPos.x);
                            position.x-=(p2.position.x-screenDiffPos.x);
                        }
                        else{
                            p2.position.x-=moveAmount;
                            position.x-=moveAmount;
                        }
                    }
                    else if(p2.position.x<screenDiffPos.x){
                        if(screenDiffPos.x-p2.position.x<moveAmount){
                            p2.position.x+=(screenDiffPos.x-p2.position.x);
                            position.x+=(screenDiffPos.x-p2.position.x);
                        }
                        else{
                            p2.position.x+=moveAmount;
                            position.x+=moveAmount;
                        }
                    }
                    else{
                        settingUp = false;
                        if(turn)
                            turn = false,p2.turn = true,turnPlayed = false,p2.turnPlayed = false;
                        else
                            turn = true, p2.turn = false,turnPlayed=false,p2.turnPlayed = false;
                    }
                }
                else{
                    if(p2.position.x<(SCREEN_WIDTH - screenDiffPos.x)){
                        if((SCREEN_WIDTH - screenDiffPos.x)-p2.position.x<moveAmount){
                            p2.position.x+=((SCREEN_WIDTH - screenDiffPos.x)-p2.position.x);
                            position.x+=((SCREEN_WIDTH - screenDiffPos.x)-p2.position.x);
                        }
                        else{
                            p2.position.x+=moveAmount;
                            position.x+=moveAmount;
                        }}
                    else if(p2.position.x>(SCREEN_WIDTH - screenDiffPos.x)){
                        if(p2.position.x-(SCREEN_WIDTH - screenDiffPos.x)<moveAmount){
                            p2.position.x-=(p2.position.x - (SCREEN_WIDTH - screenDiffPos.x));
                            position.x-=(p2.position.x - (SCREEN_WIDTH - screenDiffPos.x));
                        }
                        else{
                            p2.position.x-=moveAmount;
                            position.x-=moveAmount;
                        }}
                    else{
                        settingUp = false;
                        if(turn)
                            turn = false,p2.turn = true,turnPlayed = false,p2.turnPlayed = false;
                        else
                            turn = true, p2.turn = false,turnPlayed=false,p2.turnPlayed = false;
                    }
                }
            }else
                settingUp = false;
        }
        void update(bool firstShoot, float deltaTime){
            updateHitAnimation(deltaTime);
            if (health2 > health)
                health2 -= 0.3;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && turn && GetMousePosition().y>150)
                mousePos = GetMousePosition();
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && turn && GetMousePosition().y>150){
                mouseDown = true;
                Vector2 currMousePos = GetMousePosition();
                double npower = isLeft ? ((mousePos.x - currMousePos.x) / 300) : ((currMousePos.x - mousePos.x) / 300);
                if (npower < 1.0 && npower >= 0.0)
                    power = npower;
                else
                    power = 1.0;
                int nangle = (currMousePos.y - mousePos.y) / 2;
                if (nangle < 90 && nangle >= -45)
                    angle = nangle;
                
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && turn && firstShoot && GetMousePosition().y>150){
                shoot();
                g_soundManager.play("throw");
                mouseDown = false;
                turnPlayed = true;
                g_aiLearning.recordPlayerShot(angle, power, 0, false);
                mousePos = initial;
            }
        }
};
class AutoPlayer: public Player{
    int smartness, targetAngle, fps;
    double targetPower;
    bool targetSet, shootTriggered;
    int myShotsFired;
    int myShotsHit;

public:
    AutoPlayer(Vector2 pos, int h = PLAYER_HEALTH, Color col = RED, bool isLeft = true, bool turn = false, int ang = BASE_ANGLE, double pow = BASE_POWER, int smart = 1):Player(pos, h, col, isLeft, turn, ang, pow), smartness(smart), targetSet(false), shootTriggered(false), fps(0), myShotsFired(0), myShotsHit(0){}
        template<class P> void updateArrow(P &p2){
            if(turn && !turnPlayed){
                mouseDown = true;
                if(!targetSet){
                    // Direct targeting as fallback - aim at player position
                    int level = g_aiLearning.getTotalRounds();

                    // In early rounds or with no data, use direct targeting
                    int directAngle = -30 + (isLeft ? 5 : -5); // Aim upward toward opponent
                    int directPower = 60 + (rand() % 20);

                    if (level >= 3) {
                        // Use learning after 3+ rounds
                        int predictedAngle = g_aiLearning.predictBestAngle(p2.angle, p2.power);
                        int predictedPower = g_aiLearning.predictBestPower(p2.power);

                        targetAngle = predictedAngle + (rand() % 11) - 5; // Smaller random offset
                        targetPower = predictedPower / 100.0;
                    } else {
                        // Direct targeting for early rounds - more accurate
                        targetAngle = directAngle + (rand() % 21) - 10;
                        targetPower = directPower / 100.0;
                    }

                    if(targetAngle < MIN_ANGLE) targetAngle = MIN_ANGLE;
                    if(targetAngle > MAX_ANGLE) targetAngle = MAX_ANGLE;
                    if(targetPower < MIN_POWER) targetPower = MIN_POWER;
                    if(targetPower > MAX_POWER) targetPower = MAX_POWER;

                    targetSet = true;
                }
                if(targetAngle <= MAX_ANGLE && targetAngle >= MIN_ANGLE && angle!=targetAngle){
                    int diff = abs(targetAngle - angle)/5 + 1;
                    if(targetAngle>angle+diff)
                        angle+=diff;
                    else if(targetAngle<angle-diff)
                        angle-=diff;
                    else
                        angle = targetAngle;
                }
                if(targetPower <= MAX_POWER && targetPower >= MIN_POWER && !((power+0.009)>targetPower && (power-0.009)<targetPower)){
                    double diff = abs(targetPower - power)/5.0;
                    if(targetPower>(power+diff))
                        power+=diff;
                    else if(targetPower<(power-diff))
                        power-=diff;
                    else
                        power = targetPower;
                    if(power>0.99 && targetPower==1)
                        power = 1;
                }
                if(((power+0.009)>targetPower && (power-0.009)<targetPower) && angle==targetAngle && !shootTriggered){
                    mouseDown = false;
                    turnPlayed = true;
                    shoot();
                    g_soundManager.play("throw");
                    myShotsFired++;
                    targetSet = false;
                    shootTriggered = true;
                }
            }
            if(p2.turn)
                shootTriggered = false;
            if(!p2.isShooting && p2.turnPlayed && p2.health2 < p2.health) {
                myShotsHit++;
            }
            Player::updateArrow(p2);
        }
        void update(bool first, float deltaTime){
            updateHitAnimation(deltaTime);
            if (health2 > health)
                health2 -= 0.3;
        }

};
class Powerups;
class Box{
    friend class Powerups;
    Vector2 position;
    Texture boxmain;
    double height, width;
    float scale, SCREEN_HEIGHTDiff;
    int boxNo;
    bool sound;
    public:
    template<class, class> friend class GamePlay;
    Box(float diff, int n=1):SCREEN_HEIGHTDiff(diff),boxNo(n),scale(0.8), sound(true){
        boxmain = LoadTexture("assets/box.png");
        width = 306*scale, height = 296*scale;
        position.y = float(SCREEN_HEIGHT-(0.7)*SCREEN_HEIGHTDiff-((boxmain.height*scale)/2+height/2)-((height+5)*(boxNo-1)));
    }
    virtual void draw(Player p1, Player p2)=0;
};
class Powerups:public Box{
    public:
    template<class, class> friend class GamePlay;
    Powerups(float diff, int n=1):Box(diff,n){
        boxmain = LoadTexture("assets/box.png");
        width = 306*scale, height = 296*scale;
        position.y = float(SCREEN_HEIGHT-(0.7)*SCREEN_HEIGHTDiff-((boxmain.height*scale)/2+height/2)-((height+5)*(boxNo-1)));
    }
    void draw(Player p1, Player p2)
    {
        position.x = (p2.position.x-p1.position.x)/2 + p1.position.x - (boxmain.width*scale)/2;
        DrawTexturePro(boxmain, {0, 0, float(boxmain.width), float(boxmain.height)}, {float(position.x), float(position.y), boxmain.width * scale, boxmain.height * scale}, {0, 0}, 0, WHITE);
    }
    void randompower(Player &p2){
        int randomNumber = rand() % 6 + 1;
        switch (randomNumber){
            case 1:
                p2.health -= POWERUP_DAMAGE_1;
                g_soundManager.play("boxHit");
                break;
            case 2:
                p2.health -= POWERUP_DAMAGE_2;
                g_soundManager.play("boxHit");
                break;
            case 3:
                g_soundManager.play("fatality");
                if(p2.health < POWERUP_HEAL_CAP_1)
                    p2.health += POWERUP_HEAL_1;
                break;
            case 4:
                g_soundManager.play("boxHit");
                if(p2.health < POWERUP_HEAL_CAP_2)
                    p2.health += POWERUP_HEAL_2;
                break;
        }
    }
};
class GamePlayBase{
    public:
        int round;
        GamePlayBase():round(1){}
        virtual void draw() = 0;
        virtual void update(bool) = 0;
        virtual void drawHealthBar() = 0;
        virtual bool getMenuOn() = 0;
        virtual void checkWin() = 0;
};
template<class P1, class P2>
class GamePlay:public GamePlayBase{
    P1 p1;
    P2 p2;
    Texture Skull, pause;
    vector<Powerups> boxes;
    data a;
    OptionsScreen opMenu;
    bool options, menuOn, frShoot, soundEnabled;
    int fps;
    float deltaTime;
    Background background;
    SoundManager soundManager;
public:
    GamePlay(P1 p1, P2 p2, int nBox) : p1(p1),p2(p2),options(false), menuOn(false),frShoot(true), soundEnabled(true), deltaTime(1.0f/60.0f){
        soundManager.load("hitting", "hitting_sound.mp3");
        soundManager.load("throw", "throw.mp3");
        soundManager.load("boxHit", "box_hit.mp3");
        soundManager.load("fatality", "fatality.mp3");
        soundManager.load("wallHit", "wall_hit.mp3");
        addOptionsMenu(opMenu);
        opMenu.getItem(0).onClick = [this]{
            this->options = false;
            this->frShoot = false;
        };
        opMenu.getItem(1).onClick = [this]{
            this->restartGame();
            this->options = false;
            this->frShoot = false;
        };
        
        opMenu.getItem(2).onClick = [this]{
            this->soundOnOff();
        };
        opMenu.getItem(3).onClick = [this]{
           CloseWindow();
           exit(0);
        };
        Skull = LoadTexture("assets/crossarrows_skull.png");
        pause = LoadTexture("assets/pause2.png");
        srand(time(0));
        for(int i=0;i<nBox;i++)
            boxes.push_back(Powerups(p1.screenDiffPos.y,i+1));
    }
    void operator++(){
        round++;
    }
    void restartGame(){
        p1.health = PLAYER_HEALTH;
        p1.health2 = PLAYER_HEALTH;
        p2.health = PLAYER_HEALTH;
        p2.health2 = PLAYER_HEALTH;
        round = 1;
    }
    void soundOnOff(){
        if(soundEnabled){
            opMenu.getItem(2).setText("Sound On");
            soundEnabled = false;
            soundManager.setEnabled(false);
        }
        else{
            opMenu.getItem(2).setText("Sound Off");
            soundEnabled = true;
            soundManager.setEnabled(true);
        }


    }
    bool getMenuOn(){return menuOn;}
    void drawHealthBar(){
        float healthWidthFactor = (SCREEN_WIDTH / 2 - (10 * 12)) / 200.0, rounded = 1.0;
        DrawRectangleRounded({(SCREEN_WIDTH / 4), 40, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10}, 2, 5, Color({44, 0, 81, 255}));
        DrawRectangleRounded({(SCREEN_WIDTH / 4) + 10, 50, healthWidthFactor * 100, SCREEN_HEIGHT / 10 - 20}, rounded, 5, Color(BLACK));
        DrawRectangleRounded({float((0.75 * SCREEN_WIDTH) - 10 - healthWidthFactor * 100), 50, healthWidthFactor * 100, SCREEN_HEIGHT / 10 - 20}, rounded, 5, Color(BLACK));
        DrawRectangleRounded({(SCREEN_WIDTH / 4) + 10, 50, float(p1.health2) * healthWidthFactor, SCREEN_HEIGHT / 10 - 20}, rounded, 5, Fade(p1.color, 0.5));
        DrawRectangleRounded({float((0.75 * SCREEN_WIDTH) - 10 - (p2.health2) * healthWidthFactor), 50, float(p2.health2) * healthWidthFactor, SCREEN_HEIGHT / 10 - 20}, rounded, 5, Fade(p2.color, 0.5));
        DrawRectangleRounded({(SCREEN_WIDTH / 4) + 10, 50, float(p1.health) * healthWidthFactor, SCREEN_HEIGHT / 10 - 20}, rounded, 5, p1.color);
        DrawRectangleRounded({float((0.75 * SCREEN_WIDTH) - 10 - (p2.health) * healthWidthFactor), 50, float(p2.health) * healthWidthFactor, SCREEN_HEIGHT / 10 - 20}, rounded, 5, p2.color);
        Rectangle sourceRec = {0.0f, 0.0f, (float)Skull.width, (float)Skull.height};
        Rectangle destRec = {float((SCREEN_WIDTH / 2) - Skull.width * 0.125), 30.0f, Skull.width * 0.25f, Skull.height * 0.25f};
        DrawTexturePro(Skull, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
        }
        void checkWin(){
        if (p1.health <= 0 || p2.health <= 0 || p1.health2 <= 0 || p2.health2 <= 0){
            if (p1.health <= 0 || p1.health2 <= 0){
                DrawText("BLUE WINS", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 30, 100, WHITE);
                DrawText("Press Space To Continue", SCREEN_WIDTH / 2 - 320, SCREEN_HEIGHT / 2 + 100, 50, WHITE);
                if (IsKeyDown(KEY_SPACE))
                {
                    ClearBackground(BLACK);
                    p1.health = PLAYER_HEALTH;
                    p1.health2 = PLAYER_HEALTH;
                    p2.health = PLAYER_HEALTH;
                    p2.health2 = PLAYER_HEALTH;
                    g_aiLearning.onRoundEnd(true);
                    ++(*this);
                    return ;
                }
            }
            else{
                DrawText("RED WINS", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 30, 100, WHITE);
                DrawText("Press Space To Continue", SCREEN_WIDTH / 2 - 320, SCREEN_HEIGHT / 2 + 100, 50, WHITE);
                if (IsKeyDown(KEY_SPACE))
                {
                    ClearBackground(BLACK);
                    p1.health = PLAYER_HEALTH;
                    p1.health2 = PLAYER_HEALTH;
                    p2.health = PLAYER_HEALTH;
                    p2.health2 = PLAYER_HEALTH;
                    g_aiLearning.onRoundEnd(false);
                    ++(*this);
                    return;
                }
            }
        }
        return ;
    }
    void update(bool firstShoot){
        a.seth1(p1.health);
        a.seth2(p2.health);
        a.setround(round);
        if(p1.turn==1||p1.turn==0)
       { a.setil(p1.turn);}
        a.savedata();
        fps++;
        fps%=60;
        if(!options){
        int luck = rand() % 3 ;
        if(boxCollision(boxes[0].position,p1.arrow.position,p1.arrow.radius,boxes[0].height,boxes[0].width)){
            if(!luck)
            {boxes[0].randompower(p2);}
            p1.arrow.reset();
            p1.arrow.position = p1.initial;
            p1.isShooting = false;
            p1.settingUp = true;
            g_soundManager.play("wallHit");
        }
        if(boxCollision(boxes[1].position,p1.arrow.position,p1.arrow.radius,boxes[1].height,boxes[1].width)){
            if(!luck)
            {boxes[1].randompower(p2);}
            p1.arrow.reset();
            p1.arrow.position = p1.initial;
            p1.isShooting = false;
            p1.settingUp = true;
            g_soundManager.play("wallHit");
        }
        if(boxCollision(boxes[0].position,p2.arrow.position,p2.arrow.radius,boxes[0].height,boxes[0].width)){
            if(!luck)
            {boxes[0].randompower(p1);}
            p2.arrow.reset();
            p2.arrow.position = p2.initial;
            p2.isShooting = false;
            p2.settingUp = true;
            g_soundManager.play("wallHit");
        }
        if(boxCollision(boxes[1].position,p2.arrow.position,p2.arrow.radius,boxes[1].height,boxes[1].width)){
            if(!luck)
            {boxes[1].randompower(p1);}
            p2.arrow.reset();
            p2.arrow.position = p2.initial;
            p2.isShooting = false;
            p2.settingUp = true;
            g_soundManager.play("wallHit");
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMousePosition().y<150){
            if(GetMousePosition().x>SCREEN_WIDTH-130 && GetMousePosition().x<SCREEN_WIDTH-30 && GetMousePosition().y>50 && GetMousePosition().y<150){
                options = true;
            }
        }
        if(firstShoot)
            firstShoot = frShoot;
        p1.update(firstShoot, deltaTime);
        p2.update(firstShoot, deltaTime);
        p1.updateArrow(p2);
        p2.updateArrow(p1);
        if(!frShoot && fps%59==0)
            frShoot = true;
        background.update(p1.position.x - SCREEN_WIDTH/2);
        checkWin();
        }
    }
    void draw(){
        if(options){
            showOptionsMenu(opMenu);
        }
        else{
        background.draw();
        DrawTexturePro(pause,{0,0,float(pause.width), float(pause.height)},{SCREEN_WIDTH - 130, 50,0.1f*float(pause.width),0.1f*float(pause.width)},{0,0} , 0, WHITE);
        drawHealthBar();
        p1.draw();
        p2.draw();
        for(int i=0;i<boxes.size();i++)
            boxes[i].draw(p1,p2);
        }
    }
};
class ArcadeGameplay:public GamePlayBase{
    
};
int main(){
    data d;
    d.readdata();
    int h1 = d.geth1();
    int h2 = d.geth2();
    bool il = d.getisleft();
    srand(time(nullptr));
    InitAudioDevice();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ARROW WARS!");
    // TitleScreen s1; // COMMENTED OUT FOR TESTING
    Menu menu;
    addMenu(menu);
    Player player1({150, SCREEN_HEIGHT - 150}, 100,{224, 16, 0, 255}, true, true);
    bool menuMouseClick=false, single=true;
    GameState gameState = GameState::PLAYING; // SKIP TO PLAYING FOR TESTING
    bool loading = true;
    OptionsScreen opMenu;
    Texture2D bg = LoadTexture("back.png");
    Texture2D bottom = LoadTexture("bottom.png");
    unique_ptr<GamePlayBase> game = make_unique<GamePlay<Player, AutoPlayer>>(player1, AutoPlayer({2 * SCREEN_WIDTH, SCREEN_HEIGHT - 150}, 100, {0, 234, 255, 255}, false, false, 45, 0.6, 1), 2);
    // Menu callbacks commented out for testing
    // menu.getItem(0).onClick = [&gameState, &game,&player1, &single]{
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //     gameState = GameState::PLAYING;
    //     if(single)
    //         game = make_unique<GamePlay<Player, AutoPlayer>>(player1, AutoPlayer({2 * SCREEN_WIDTH, SCREEN_HEIGHT - 150}, 100, {0, 234, 255, 255}, false, false, 45, 0.6, 1), 2);
    //     else
    //         game = make_unique<GamePlay<Player, Player>>(player1, Player({2 * SCREEN_WIDTH, SCREEN_HEIGHT - 150}, 100, {0, 234, 255, 255}, false, false), 2);
    // };
    // menu.getItem(1).onClick = [&gameState, &game,&player1, &single, &menu]{
    //     if(single)
    //         menu.selectMode(single,true);
    //     single = true;
    // };
    // menu.getItem(2).onClick = [&menu, &single]{
    //     if(!single)
    //         menu.selectMode(single, false);
    //     single = false;
    // };
    // menu.getItem(3).onClick = [&gameState, &game,&player1, &single,&h1,&h2,&il,&d]{
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //     gameState = GameState::PLAYING;
    //     if(single)
    //        { game = make_unique<GamePlay<Player, AutoPlayer>>(Player({150, SCREEN_HEIGHT - 150}, h1,{224, 16, 0, 255}, true, true), AutoPlayer({2 * SCREEN_WIDTH, SCREEN_HEIGHT - 150}, h2, {0, 234, 255, 255}, false, false, 45, 0.6, 1), 2);
    //         game->round=d.getround();}
    //     else
    //        { game = make_unique<GamePlay<Player, Player>>(Player({150, SCREEN_HEIGHT - 150}, h1,{224, 16, 0, 255}, true, true), Player({2 * SCREEN_WIDTH, SCREEN_HEIGHT - 150}, h2, {0, 234, 255, 255}, false, false), 2);
    //         game->round=d.getround();}
    // };
    // menu.getItem(4).onClick = []{
    //     CloseWindow();
    //     exit(0);
    // };
    Rectangle sourceRec = {0.0f, 0.0f, (float)bg.width, (float)bg.height};
    Rectangle destRec = {0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
    DrawTexturePro(bg, sourceRec, destRec, {0,0}, 0.0f, WHITE);
    SetTargetFPS(TARGET_FPS);
    g_soundManager.load("bgMusic", "bgmusic.mp3");
    g_soundManager.play("bgMusic");
    while (WindowShouldClose() == false){
        BeginDrawing();
        // Skip to GAME PLAY for testing
        DrawText(TextFormat("Round %i", game->round), SCREEN_WIDTH / 2 - 85, SCREEN_HEIGHT / 2 - 300, 50, WHITE);
        game->draw();
        if(!menuMouseClick){
            game->update(false);
            menuMouseClick = true;
        }
        else game->update(true);
        EndDrawing();
    }
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}