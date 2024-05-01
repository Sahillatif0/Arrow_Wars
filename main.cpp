#include <iostream>
#include <raylib.h>
#include <cmath>
#include <vector>

const double gravity = 9.8;
const int screenWidth = 1800;
const int screenHeight = 920;

using namespace std;

bool circleCollision(Vector2 pos1, Vector2 pos2, int rad1, int rad2){
    return (sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2)) < rad1 + rad2);
}

class Arrow{
    Vector2 position, velocity, initialVel;
    Color color;
    Texture ball;
    double time;
    int moveDir, radius, angle;
    public:
        friend class Player;
        Arrow(Vector2 pos= {0,0}, Vector2 vel={0,0},int rad=5, int ang=45, Color col=RED, int moveDir=1):position(pos),velocity(vel),radius(rad), angle(ang),color(col),time(0),moveDir(moveDir),initialVel(velocity){
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
            position.x += moveDir * (velocity.x * cos((angle * PI) / 180.0));
        if (angle >= 0){
                position.y -= (velocity.y * sin((angle * PI) / 180.0) - 0.5 * gravity * time * time);
                velocity.y -= gravity * time;
            }
        else
            position.y -= velocity.y * sin((angle * PI) / 180.0) * time;
    }
    void reset(){
        velocity = initialVel;
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
        int health, radius, angle, Textx, recentHitTimer;
        Color color;
        double power, health2;
        Arrow arrow;
        bool isLeft, isShooting, turn, settingUp, mouseDown;
        Texture sprite;
        friend class GamePlay;
        Player(Vector2 pos, int h = 100, int r = 50, Color col = RED, bool isLeft = true, bool isShooting = false, bool turn = false, int ang = 45, double pow = 0.6) : position(pos), health(h), health2(h), radius(r), color(col), angle(ang), power(pow), isLeft(isLeft), isShooting(isShooting), turn(turn), Textx((isLeft) ? 20 : screenWidth - 120), settingUp(false), recentHitTimer(0), mouseDown(false), screenDiffPos({150, 150}){
            initial.x = isLeft ? screenDiffPos.x : (screenWidth - screenDiffPos.x);
            initial.y = screenHeight - screenDiffPos.y;
            arrow = Arrow(initial, {screenDiffPos.x / 2 + 20, screenDiffPos.x / 2 + 10}, 5, 45, color, (isLeft) ? 1 : -1);
            arrowVel = {screenDiffPos.x / 2 + 20, screenDiffPos.x / 2 + 10};
            mousePos = initial;
            sprite = isLeft ? LoadTexture("assets/sprites.png") : LoadTexture("assets/Bluesprites2.png");
        }
        void draw(){
            Rectangle sourceRec = {float((sprite.width / 31) * ((isLeft ? 0 : 31) - int(angle / 3))), float(sprite.height), sprite.width / 31, sprite.height};
            Rectangle destRec = {isLeft ? position.x : position.x - sprite.width / 31, position.y - 300, sprite.width / 31, sprite.height};
            DrawTexturePro(sprite, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
            float x = position.x, y = position.y - sprite.height / 2 - 30;
            int sign = isLeft ? 1 : -1;
            if (mouseDown && turn)
                for (int i = 10; i >= 0; i--)
                {
                    y -= (arrowVel.y * power * sin((angle * PI) / 180.0) - 0.5 * gravity * ((10 - i) * 0.000001) * ((10 - i) * 0.000001));
                    x += sign * ((arrowVel.x * power) * cos((angle * PI) / 180.0));
                    DrawCircle(x, y, (7 - float(i / 2)), Fade(color, power * 0.1 * i));
                }
            drawText();
        }
        void drawText(){
            string angle_str = "Angle: " + to_string(angle);
            string pow_str = "Power: " + to_string(int(power * 100));
            if (isLeft){
            DrawText(angle_str.c_str(), position.x + 130, position.y - 350, 20, WHITE);
            DrawText(pow_str.c_str(), position.x + 130, position.y - 320, 20, WHITE);
            }
        else{
            DrawText(angle_str.c_str(), position.x - 190, position.y - 350, 20, WHITE);
            DrawText(pow_str.c_str(), position.x - 190, position.y - 320, 20, WHITE);
            }
        }
        void shoot(){
            if (turn){
                arrow.velocity.x *= power;
                arrow.velocity.y *= power;
                arrow.update(position.x, position.y - sprite.height / 2 - 30);
                arrow.draw();
                isShooting = true;
            }
        }
        void updateArrow(Player &p2){
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
                arrow.time += 1.0 / 60.0;
                arrow.angle = angle;
                arrow.draw();
                arrow.move(p2.position.x);
                arrow.velocity.x = arrow.initialVel.x * power;
                if (circleCollision(arrow.position, p2.position, arrow.radius, 300) && ((arrow.moveDir != 1 && p2.isLeft) || (arrow.moveDir == 1 && !p2.isLeft))){
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    turn = false;
                    p2.turn = true;
                    settingUp = true;
                    recentHitTimer = 100;
                    p2.health -= 30;
                }
                if(p2.health<0)
                    p2.health = 0;
                if(arrow.position.y > screenHeight){
                    arrow.reset();
                    arrow.position = initial;
                    isShooting = false;
                    turn = false;
                    p2.turn = true;
                    settingUp = true;
                }
                if((arrow.position.x > screenWidth/2 && isLeft) || (arrow.position.x < screenWidth/2 && !isLeft)){
                    position.x-= arrow.moveDir*(arrow.velocity.x*cos((angle*PI)/180.0));
                    p2.position.x-= arrow.moveDir*(arrow.velocity.x*cos((angle*PI)/180.0));
                }
            }
            if(settingUp){
                float moveAmount = (arrow.velocity.x)/2;
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
                    else
                        settingUp = false;
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
                    else
                        settingUp = false;
                }
            } else {
                settingUp = false;
            }
        }
        void update(){
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
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && turn){
                shoot();
                mouseDown = false;
                mousePos = initial;
            }
        }
};
class GamePlay{
    Player p1, p2;
    Texture Skull;

public:
    static int round;
    GamePlay(Player p1, Player p2) : p1(p1), p2(p2){
        Skull = LoadTexture("assets/crossarrows_skull.png");
    }
    void drawHealthBar(){
        float healthWidthFactor = (screenWidth / 2 - (10 * 12)) / 200.0, rounded = 1.0;
        DrawRectangleRounded({(screenWidth / 4), 40, screenWidth / 2, screenHeight / 10}, 2, 5, Color({44, 0, 81, 255}));
        DrawRectangleRounded({(screenWidth / 4) + 10, 50, healthWidthFactor * 100, screenHeight / 10 - 20}, rounded, 5, Color(BLACK));
        DrawRectangleRounded({float((0.75 * screenWidth) - 10 - healthWidthFactor * 100), 50, healthWidthFactor * 100, screenHeight / 10 - 20}, rounded, 5, Color({45, 18, 60, 150}));
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
    void update(){
        p1.update();
        p2.update();
        p1.updateArrow(p2);
        p2.updateArrow(p1);
    }
    void draw(){
        drawHealthBar();
        p1.draw();
        p2.draw();
    }
};
int GamePlay::round = 1;
int main(){
    InitWindow(screenWidth, screenHeight, "ARROW WARS!");
    Player player1({150, screenHeight - 150}, 100, 50, {224, 16, 0, 255}, true, false, true);
    Player player2({2 * screenWidth, screenHeight - 150}, 100, 50, {0, 234, 255, 255}, false, false, false);
    // Texture2D bg = LoadTexture("bg.png");
    GamePlay game(player1, player2);
    SetTargetFPS(60);
    while (WindowShouldClose() == false){
        // Rectangle sourceRec = {0.0f, 0.0f, (float)bg.width, (float)bg.height};
        // Rectangle destRec = {0.0f, 0.0f, (float)screenWidth, (float)screenHeight};
        // DrawTexturePro(bg, sourceRec, destRec, {0,0}, 0.0f, WHITE);
        BeginDrawing();
        DrawText(TextFormat("Round %i", GamePlay::round), screenWidth / 2 - 85, screenHeight / 2 - 300, 50, WHITE);
        ClearBackground(BLACK);
        game.update();
        game.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}