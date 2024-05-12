#include <iostream>
#include <raylib.h>
#include <cmath>
#include <vector>
#include "menu.h"
#include <cstdlib>
#include <ctime>
#include "titlescreen.h"

const double gravity = 9.8;
const int screenWidth = 1800;
const int screenHeight = 920;

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
    // cout<<"arrow: "<<arrowLeft<<" "<<arrowRight<<" "<<arrowTop<<" "<<arrowBottom<<endl;
   
    float boxLeft = Boxpos.x+190;
    float boxRight = Boxpos.x + boxWidth+190;
    float boxTop = Boxpos.y+190;
    float boxBottom = Boxpos.y + boxHeight+190;
    // cout<<"box: "<<boxLeft<<" "<<boxRight<<" "<<boxTop<<" "<<boxBottom<<endl;
   
    if (arrowLeft < boxRight && arrowRight > boxLeft && arrowTop < boxBottom && arrowBottom > boxTop) {
        return true;  
    }
    return false;
}

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

        // DrawCircle(position.x, position.y, radius, color);
        DrawTexturePro(ball, {0, 0, float(ball.width), float(ball.height)}, {position.x, position.y, ball.width * 0.1f, ball.height * 0.1f}, {0, 0}, angle, WHITE);
    }
    void move(int x){
        if (((position.x < screenWidth / 2 || (x > (screenWidth - 150) && x < ((screenWidth - 150) + initialVel.x))) && moveDir == 1) || ((position.x > screenWidth / 2 || (x < 150 && x > (150 - initialVel.x))) && moveDir == -1))
            // cout<<"posX: "<<position.x<<" posy: "<<position.y<<" t: "<<time<<endl;
            position.x += moveDir * (velocity*power* 0.5* cos((angle * PI) / 180.0));
            position.y -= (velocity* power * sin((angle * PI) / 180.0) - 0.5 * gravity * time * time);
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
        int health, angle, velocity;
        Color color;
        double power, health2;
        Arrow arrow;
        bool isLeft, isShooting, turn, settingUp, mouseDown, turnPlayed;
        Texture sprite;
        template<class, class> friend class GamePlay;
        Player(Vector2 pos, int h = 100, Color col = RED, bool isLeft = true, bool turn = false, int ang = 45, double pow = 0.6) : position(pos), health(h), health2(h), color(col), angle(ang), power(pow), isLeft(isLeft), isShooting(false), turn(turn), settingUp(false), mouseDown(false), screenDiffPos({150, 150}), velocity(screenDiffPos.x/2), turnPlayed(false){
            initial.x = isLeft ? screenDiffPos.x : (screenWidth - screenDiffPos.x);
            initial.y = screenHeight - screenDiffPos.y;
            arrowVel = {float(velocity*cos(angle*PI/180.0)), float(velocity*sin(angle*PI/180.0))};
            arrow = Arrow(initial, arrowVel,velocity, 5, angle,power, color, (isLeft) ? 1 : -1);
            mousePos = initial;
            sprite = isLeft ? LoadTexture("assets/sprites.png") : LoadTexture("assets/Bluesprites2.png");
        }
        void draw(){
            Rectangle sourceRec = {float((sprite.width / 31) * ((isLeft ? 0 : 31) - int(angle / 3))), float(sprite.height), sprite.width / 31, sprite.height};
            Rectangle destRec = {isLeft ? position.x : position.x - sprite.width / 31, position.y - 300, sprite.width / 31, sprite.height};
            DrawTexturePro(sprite, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
            float x = initial.x, y = initial.y - sprite.height / 2 - 30;
            int sign = isLeft ? 1 : -1;
            if (mouseDown && turn){
                double time = 0;
                for (int i = 30; i > 0; i--){
                    DrawCircle(x, y, (5 - 5*float(i / 30)), Fade(color, power * 0.03 * i + 0.25));
                    time += 1/20.0;
                    // cout<<"y: "<<y<<" x:"<<x<<" TIME: "<<time<<endl;
                    y -= (velocity* power * sin((angle * PI) / 180.0) - 0.5 * gravity * time * time);
                    x += sign * 0.5* (velocity*power * cos((angle * PI) / 180.0));
                }
                cout<<endl<<endl<<endl;
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
                float x = isLeft ? (screenWidth - screenDiffPos.x) : screenDiffPos.x, y = screenHeight - 200;
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
                    cout<<"1"<<endl;
                    // DrawText("HEADSHOT", screenWidth / 2 - 250, screenHeight / 2 - 30, 100, WHITE);
                    // ClearBackground(BLACK);
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    settingUp = true;
                    p2.health -= 80;
                }
                if (circleCollision(arrow.position, p2.position, arrow.radius, 300) && ((arrow.moveDir != 1 && p2.isLeft) || (arrow.moveDir == 1 && !p2.isLeft))){
                    cout<<"2"<<endl;
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    settingUp = true;
                    p2.health -= 10;
                }
                if(p2.health<0)
                    p2.health = 0;
                if(arrow.position.y > screenHeight){
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    settingUp = true;
                }
                if((arrow.position.x > screenWidth/2 && isLeft) || (arrow.position.x < screenWidth/2 && !isLeft)){
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
                    if(p2.position.x<(screenWidth - screenDiffPos.x)){
                        if((screenWidth - screenDiffPos.x)-p2.position.x<moveAmount){
                            p2.position.x+=((screenWidth - screenDiffPos.x)-p2.position.x);
                            position.x+=((screenWidth - screenDiffPos.x)-p2.position.x);
                        }
                        else{
                            p2.position.x+=moveAmount;
                            position.x+=moveAmount;
                        }}
                    else if(p2.position.x>(screenWidth - screenDiffPos.x)){
                        if(p2.position.x-(screenWidth - screenDiffPos.x)<moveAmount){
                            p2.position.x-=(p2.position.x - (screenWidth - screenDiffPos.x));
                            position.x-=(p2.position.x - (screenWidth - screenDiffPos.x));
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
        void update(bool firstShoot){
            if (health2 > health)
                health2 -= 0.3;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && turn)
                mousePos = GetMousePosition();
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && turn){
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
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && turn && firstShoot){
                shoot();
                mouseDown = false;
                turnPlayed = true;
                mousePos = initial;
            }
        }
};
class AutoPlayer: public Player{
    int smartness, targetAngle, fps;
    double targetPower;
    bool targetSet, shootTriggered;
    public:
        AutoPlayer(Vector2 pos, int h = 100, Color col = RED, bool isLeft = true, bool turn = false, int ang = 45, double pow = 0.6, int smartness = 1):Player(pos, h, col, isLeft, turn, ang, pow), smartness(smartness), targetSet(false), shootTriggered(false), fps(0){}
        template<class P> void updateArrow(P &p2){
            if(turn && !turnPlayed){
                mouseDown = true;
                if(!targetSet){
                    randAngle:
                    targetAngle = rand()%(p2.angle==-5?p2.angle+7:p2.angle+5) + p2.angle - 10;
                    if(targetAngle<-45 || targetAngle>90)
                        goto randAngle;
                    targetPower = (rand()%(int (p2.power*100)+10) + p2.power*100 - 20)/100.0;
                    if(targetPower<0)
                        targetPower = 0;
                    if(targetPower>1)
                        targetPower = 1;
                    targetSet = true;
                }
                if(targetAngle <= 90 && targetAngle >= -45 && angle!=targetAngle){
                    int diff = abs(targetAngle - angle)/5 + 1;
                    if(targetAngle>angle+diff)
                        angle+=diff;
                    else if(targetAngle<angle-diff)
                        angle-=diff;
                    else
                        angle = targetAngle;
                }
                if(targetPower <= 1 && targetPower >= 0 && !((power+0.009)>targetPower && (power-0.009)<targetPower)){
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
                    targetSet = false;
                    shootTriggered = true;
                }
            }
            if(p2.turn)
                shootTriggered = false;
            Player::updateArrow(p2);
        }
        void update(bool first){
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
    float scale, screenHeightDiff;
    int boxNo;
    public:
    template<class, class> friend class GamePlay;
    Box(float diff, int n=1):screenHeightDiff(diff),boxNo(n),scale(0.8){
        boxmain = LoadTexture("assets/box.png");
        width = 306*scale, height = 296*scale;
        position.y = float(screenHeight-(0.7)*screenHeightDiff-((boxmain.height*scale)/2+height/2)-((height+5)*(boxNo-1)));
    }
    virtual void draw(Player p1, Player p2)=0;
};
class Powerups:public Box{
    template<class, class> friend class GamePlay;
    Powerups(float diff, int n=1):Box(diff,n){
        boxmain = LoadTexture("assets/box.png");
        width = 306*scale, height = 296*scale;
        position.y = float(screenHeight-(0.7)*screenHeightDiff-((boxmain.height*scale)/2+height/2)-((height+5)*(boxNo-1)));
    }
    void draw(Player p1, Player p2)
    {
        position.x = (p2.position.x-p1.position.x)/2 + p1.position.x - (boxmain.width*scale)/2;
        DrawTexturePro(boxmain, {0, 0, float(boxmain.width), float(boxmain.height)}, {float(position.x), float(position.y), boxmain.width * scale, boxmain.height * scale}, {0, 0}, 0, WHITE);
    }
    void randompower(Player &p2){
        cout<<"1"<<endl;
        int randomNumber = rand() % 3 + 1;
        switch (randomNumber){
            case 1:{
                cout<<"2"<<endl;
                p2.health-=30;
                break;
            }
            case 2:{
                cout<<"3"<<endl;
                p2.health-=40;
                break;
            }
            case 3:{
                cout<<"4"<<endl;
                p2.health-=10000;
                break;
            }
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
};
template<class P1, class P2> 
class GamePlay:public GamePlayBase{
    P1 p1;
    P2 p2;
    Texture Skull;
    vector<Powerups> boxes;
    
public:
    GamePlay(P1 p1, P2 p2, int nBox) : p1(p1),p2(p2){
        Skull = LoadTexture("assets/crossarrows_skull.png");
        srand(time(0));
        for(int i=0;i<nBox;i++)
            boxes.push_back(Powerups(p1.screenDiffPos.y,i+1));
    }
    void drawHealthBar(){
        float healthWidthFactor = (screenWidth / 2 - (10 * 12)) / 200.0, rounded = 1.0;
        DrawRectangleRounded({(screenWidth / 4), 40, screenWidth / 2, screenHeight / 10}, 2, 5, Color({44, 0, 81, 255}));
        DrawRectangleRounded({(screenWidth / 4) + 10, 50, healthWidthFactor * 100, screenHeight / 10 - 20}, rounded, 5, Color(BLACK));
        DrawRectangleRounded({float((0.75 * screenWidth) - 10 - healthWidthFactor * 100), 50, healthWidthFactor * 100, screenHeight / 10 - 20}, rounded, 5, Color(BLACK));
        DrawRectangleRounded({(screenWidth / 4) + 10, 50, float(p1.health2) * healthWidthFactor, screenHeight / 10 - 20}, rounded, 5, Fade(p1.color, 0.5));
        DrawRectangleRounded({float((0.75 * screenWidth) - 10 - (p2.health2) * healthWidthFactor), 50, float(p2.health2) * healthWidthFactor, screenHeight / 10 - 20}, rounded, 5, Fade(p2.color, 0.5));
        DrawRectangleRounded({(screenWidth / 4) + 10, 50, float(p1.health) * healthWidthFactor, screenHeight / 10 - 20}, rounded, 5, p1.color);
        DrawRectangleRounded({float((0.75 * screenWidth) - 10 - (p2.health) * healthWidthFactor), 50, float(p2.health) * healthWidthFactor, screenHeight / 10 - 20}, rounded, 5, p2.color);
        Rectangle sourceRec = {0.0f, 0.0f, (float)Skull.width, (float)Skull.height};
        Rectangle destRec = {float((screenWidth / 2) - Skull.width * 0.125), 30.0f, Skull.width * 0.25f, Skull.height * 0.25f};
        DrawTexturePro(Skull, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
        if (p1.health == 0 || p2.health == 0 || p1.health2 == 0 || p2.health2 == 0){
            if (p1.health == 0 || p1.health2 == 0){
                DrawText("BLUE WINS", screenWidth / 2 - 250, screenHeight / 2 - 30, 100, WHITE);
                DrawText("Press Space To Continue", screenWidth / 2 - 320, screenHeight / 2 + 100, 50, WHITE);
                if (IsKeyDown(KEY_SPACE))
                {
                    ClearBackground(BLACK);
                    p1.health = 100;
                    p1.health2 = 100;
                    p2.health = 100;
                    p2.health2 = 100;
                    round++;
                }
            }
            else{
                DrawText("RED WINS", screenWidth / 2 - 250, screenHeight / 2 - 30, 100, WHITE);
                DrawText("Press Space To Continue", screenWidth / 2 - 320, screenHeight / 2 + 100, 50, WHITE);
                if (IsKeyDown(KEY_SPACE))
                {
                    ClearBackground(BLACK);
                    p1.health = 100;
                    p1.health2 = 100;
                    p2.health = 100;
                    p2.health2 = 100;
                    round++;
                }
            }
        }
    }
    void update(bool firstShoot){
        // CheckCollisionRecs(p2.arrow.position,box1.position);
        int luck = rand() % 3 ;
        if(boxCollision(boxes[0].position,p1.arrow.position,p1.arrow.radius,boxes[0].height,boxes[0].width)){
            if(!luck)
            {boxes[0].randompower(p2);}
            p1.arrow.reset();
            p1.arrow.position = p1.initial;
            p1.isShooting = false;
            p1.settingUp = true;
        }
        if(boxCollision(boxes[1].position,p1.arrow.position,p1.arrow.radius,boxes[1].height,boxes[1].width)){
            if(!luck)
            {boxes[1].randompower(p2);}
            p1.arrow.reset();
            p1.arrow.position = p1.initial;
            p1.isShooting = false;
            p1.settingUp = true;
        }
        if(boxCollision(boxes[0].position,p2.arrow.position,p2.arrow.radius,boxes[0].height,boxes[0].width)){
            if(!luck)
            {boxes[0].randompower(p1);}
            p2.arrow.reset();
            p2.arrow.position = p2.initial;
            p2.isShooting = false;
            p2.settingUp = true;
        }
        if(boxCollision(boxes[1].position,p2.arrow.position,p2.arrow.radius,boxes[1].height,boxes[1].width)){
            if(!luck)
            {boxes[1].randompower(p1);}
            p2.arrow.reset();
            p2.arrow.position = p2.initial;
            p2.isShooting = false;
            p2.settingUp = true;
        }
        p1.update(firstShoot);
        p2.update(firstShoot);
        p1.updateArrow(p2);
        p2.updateArrow(p1);
    }
    void draw(){
        drawHealthBar();
        p1.draw();
        p2.draw();
        for(int i=0;i<boxes.size();i++)
            boxes[i].draw(p1,p2);
    }
};

int main(){
    srand(time(nullptr));
    InitWindow(screenWidth, screenHeight, "ARROW WARS!");
    TitleScreen s1;
    Menu menu;
    addMenu(menu);
    Player player1({150, screenHeight - 150}, 100,{224, 16, 0, 255}, true, true);
    bool menuOn = true, menuMouseClick=false, loading= true, single=true;
    Texture2D bg = LoadTexture("back.png");
    Texture2D bottom = LoadTexture("bottom.png");
    GamePlayBase *game;
    menu.getItem(0).onClick = [&menuOn, &game,&player1, &single]{
        _sleep(100);
        menuOn = false;
        if(single)
            game = new GamePlay<Player, AutoPlayer>(player1, AutoPlayer({2 * screenWidth, screenHeight - 150}, 100, {0, 234, 255, 255}, false, false, 45, 0.6, 1), 2);
        else
            game = new GamePlay<Player, Player>(player1, Player({2 * screenWidth, screenHeight - 150}, 100, {0, 234, 255, 255}, false, false), 2);
    };
    menu.getItem(1).onClick = [&menuOn, &game,&player1, &single, &menu]{
        cout<<single<<" single"<<endl;
        if(single)
            menu.selectMode(single,true);
        single = true;

        // _sleep(100);
        // menuOn = false;
        // game = new GamePlay<Player, AutoPlayer>(player1, AutoPlayer({2 * screenWidth, screenHeight - 150}, 100, {0, 234, 255, 255}, false, false, 45, 0.6, 1), 2);

    };
    menu.getItem(2).onClick = [&menu, &single]{
        cout<<single<<" single"<<endl;
        if(!single)
            menu.selectMode(single, false);
        single = false;
        // _sleep(100);
        // menuOn = false;
        // game = new GamePlay<Player, Player>(player1, Player({2 * screenWidth, screenHeight - 150}, 100, {0, 234, 255, 255}, false, false), 2);
    };
    menu.getItem(4).onClick = []{
        cout<<"Options"<<endl;
    };
    menu.getItem(4).onClick = []{
        cout<<"Closing window!"<<endl;
        CloseWindow();
        return 0;
    };
    Rectangle sourceRec = {0.0f, 0.0f, (float)bg.width, (float)bg.height};
    Rectangle destRec = {0.0f, 0.0f, (float)screenWidth, (float)screenHeight};
    DrawTexturePro(bg, sourceRec, destRec, {0,0}, 0.0f, WHITE);
    SetTargetFPS(60);
    while (WindowShouldClose() == false){
        BeginDrawing();
        if(loading)
            loading = s1.draw();
        else if(menuOn){
            showMenu(menu);
            menu.checkMouse();
        }
        else{
            DrawTexturePro(bottom, sourceRec, destRec, {0,0}, 0.0f, WHITE);
            DrawText(TextFormat("Round %i", game->round), screenWidth / 2 - 85, screenHeight / 2 - 300, 50, WHITE);
            ClearBackground(BLACK);
            game->draw();
            if(!menuMouseClick){
                game->update(false);
                menuMouseClick = true;
            }
            else game->update(true);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}