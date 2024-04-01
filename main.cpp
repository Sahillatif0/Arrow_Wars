#include <iostream>
#include <raylib.h>
#include <cmath>
#include <vector>

const double gravity = 9.8;
const int screenWidth = 800;
const int screenHeight = 600;
Camera2D camera;

using namespace std;

bool circleCollision(Vector2 pos1, Vector2 pos2, int rad1, int rad2){
    return (sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2)) < rad1 + rad2);
}

class Arrow{
    Vector2 position, velocity, initialVel;
    Color color;
    double time;
    int moveDir, radius, angle;
    public:
        friend class Player;
        Arrow(Vector2 pos= {0,0}, Vector2 vel={0,0},int rad=5, int ang=45, Color col=RED, int moveDir=1):position(pos),velocity(vel),radius(rad), angle(ang),color(col),time(0),moveDir(moveDir),initialVel(velocity){}
        void draw(){
            DrawCircle(position.x, position.y, radius, color);
        }
        void move(int x){
            if(((position.x < screenWidth/2 || (x>700 && x<(700+initialVel.x))) && moveDir==1) || ((position.x > screenWidth/2 || (x<100&&x>(100-initialVel.x))) && moveDir==-1))
                position.x += moveDir*(velocity.x * cos((angle*PI)/180.0));
            position.y -= (velocity.y * sin((angle*PI)/180.0) - 0.5 * gravity * time * time);
            velocity.y -= gravity * time;
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
        Vector2 position,initial,arrowVel, mousePos;
        int health,radius,angle,Textx,recentHitTimer;
        Color color;
        double power;
        Arrow arrow;
        bool isLeft, isShooting, turn, settingUp, mouseDown;
        PlayerKeys keys;
        int upKeyCounter = 0;
        int downKeyCounter = 0;
        int rightKeyCounter = 0;
        int leftKeyCounter = 0;
        Player(Vector2 pos= {0,0}, int h=100,int r=50, Color col=RED,PlayerKeys k = PlayerKeys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER),bool isLeft=true,bool isShooting=false,bool turn=false,int ang=45,double pow=0.6):position(pos),health(h),radius(r),color(col),angle(ang),power(pow),keys(k),isLeft(isLeft),isShooting(isShooting),turn(turn),Textx((isLeft)? 20 : screenWidth-120),settingUp(false),recentHitTimer(0),mouseDown(false){
            initial.x = isLeft?100:700;
            initial.y = 500;
            arrow = Arrow(initial,{70,60},5,45,color,(isLeft)?1:-1);
            arrowVel = {70,60};
            mousePos = initial;
        }
        void draw(){
            DrawCircle(position.x, position.y, radius, color);
            float x=position.x,y=position.y;
            int sign = isLeft?1:-1;
            if(mouseDown && turn)
                for(int i=10;i>=0;i--){
                    y -= (arrowVel.y*power * sin((angle*PI)/180.0) - 0.5 * gravity * ((10-i)*0.000001) * ((10-i)*0.000001));
                    x += sign*((arrowVel.x*power) * cos((angle*PI)/180.0));
                    DrawCircle(x, y, (7 - float (i/2)), Fade(color, power*0.1*i));
                }
            drawText();
        }
        void drawText(){
            int x = Textx;
            string angle_str = "Angle: " + to_string(angle);
            string pow_str = "Power: "+to_string(int(power*100));
            if(recentHitTimer>0){
                Color color2 = recentHitTimer%3==0?RED:BLUE;
                DrawRectangle(x, 20, health, 20, color2);
                recentHitTimer--;
            }
            else
                DrawRectangle(x, 20, health, 20, color);
            DrawRectangleLines(x-2, 18, 102, 22, WHITE);
            DrawText(angle_str.c_str(), x, 50, 20, WHITE);
            DrawText(pow_str.c_str(), x, 80, 20, WHITE);
        }
        void shoot(){
            if(turn){
                arrow.velocity.x *= power;
                arrow.velocity.y *= power;
                arrow.draw();
                isShooting = true;
            }
        }
        void updateArrow(Player &p2){
            if(turn){
                float x = isLeft?(screenWidth-150):50, y = screenHeight-200;
                string dist_str = to_string(abs(int (p2.position.x-arrow.position.x)))+"m";
                DrawText(dist_str.c_str(), x, y, 20, WHITE);
                DrawLineEx(Vector2{x, y+30}, Vector2{x + 50, y+30}, 2, p2.color);
                if(isLeft)
                    DrawTriangle(Vector2{x + 50, y+30}, Vector2{x + 40, y + 20}, Vector2{x + 40, y + 40}, p2.color);
                else
                    DrawTriangle(Vector2{x, y+20}, Vector2{x - 10, y + 30}, Vector2{x, y + 40}, p2.color);
            }
            if(p2.recentHitTimer>0 && p2.health>0)
                p2.health -= 2;
            if(isShooting){
            arrow.time += 1.0/60.0;
            arrow.angle = angle;
            arrow.draw();
            arrow.move(p2.position.x);
            arrow.velocity.x = arrow.initialVel.x*power;
            if(circleCollision(arrow.position,p2.position,arrow.radius, p2.radius) && ((arrow.moveDir!=1 && p2.isLeft) || (arrow.moveDir==1 && !p2.isLeft))){
                arrow.reset();
                arrow.position = initial;
                isShooting = false;
                turn = false;
                p2.turn = true;
                settingUp = true;
                p2.recentHitTimer = 10;
            }
            if(p2.health<0 && p2.radius>0){
                p2.health = 0;
                p2.radius-=5;
            }
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
            float moveAmount = (arrow.velocity.x*cos((angle*PI)/180.0))/2;
            if(p2.isLeft){
                if(p2.position.x>100){
                    if(p2.position.x-100<moveAmount){
                        p2.position.x-=(p2.position.x-100);
                        position.x-=(p2.position.x-100);
                    }
                    else{
                        p2.position.x-=moveAmount;
                        position.x-=moveAmount;
                    }
                }
                else if(p2.position.x<100){
                    if(100-p2.position.x<moveAmount){
                        p2.position.x+=(100-p2.position.x);
                        position.x+=(100-p2.position.x);
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
                if(p2.position.x<700){
                    if(700-p2.position.x<moveAmount){
                        p2.position.x+=(700-p2.position.x);
                        position.x+=(700-p2.position.x);
                    }
                    else{
                        p2.position.x+=moveAmount;
                        position.x+=moveAmount;
                    }}
                else if(p2.position.x>700){
                    if(p2.position.x-700<moveAmount){
                        p2.position.x-=(p2.position.x - 700);
                        position.x-=(p2.position.x - 700);
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
            // if (IsKeyPressed(keys.shoot)) shoot();
            // if(IsKeyPressed(keys.up) && (angle < 90)) angle++;
            // if(IsKeyPressed(keys.down) && (angle > 0)) angle--;
            // if(IsKeyPressed(keys.right) && (power < 1.0)) power += 0.01;
            // if(IsKeyPressed(keys.left) && (power > 0.0)) power -= 0.01;
            // if(IsKeyDown(keys.up) && (angle < 90)) {
            //     upKeyCounter++;
            //     if(upKeyCounter % 7 == 0) angle++;
            // } else upKeyCounter = 0;
            // if(IsKeyDown(keys.down) && (angle > 0)) {
            //     downKeyCounter++;
            //     if(downKeyCounter % 7 == 0)angle--;
            // } else downKeyCounter = 0;
            // if(IsKeyDown(keys.right) && (power < 1.0)) {
            //     rightKeyCounter++;
            //     if(rightKeyCounter % 7 == 0) power += 0.01;
            // } else rightKeyCounter = 0;
            // if(IsKeyDown(keys.left) && (power > 0.0)) {
            //     leftKeyCounter++;
            //     if(leftKeyCounter % 7 == 0) power -= 0.01;
            // } else leftKeyCounter = 0;
            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && turn){
                mouseDown = true;
                Vector2 currMousePos = GetMousePosition();
                double npower = isLeft?((mousePos.x - currMousePos.x)/200):((currMousePos.x - mousePos.x)/200);
                if(npower<1.0 && npower>=0.0)
                    power = npower;
                else
                    power = 1.0;
                int nangle = (currMousePos.y - mousePos.y)/2;
                if(nangle<90)
                    angle = nangle;
                else
                    angle = 90;
            }
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && turn){
                shoot();
                mouseDown = false;
                mousePos = initial;
            }
        }
};
int main () {
    PlayerKeys player1Keys(KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE);
    PlayerKeys player2Keys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER);
    Player player1({100,500}, 100,50, RED,player1Keys,true,false,true);
    Player player2({1500,500}, 100,50, BLUE,player2Keys,false,false,false);
    camera.offset = {screenWidth/2, screenHeight/2};
    camera.target = {screenWidth/2, screenHeight/2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    InitWindow(screenWidth, screenHeight, "ARROW WARS!");
    SetTargetFPS(60);
    while (WindowShouldClose() == false){
        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BLACK);
        player1.draw();
        player2.draw();
        player1.update();
        player2.update();
        player1.updateArrow(player2);
        player2.updateArrow(player1);
        if(player1.health==0 || player2.health==0){
            ClearBackground(GRAY);
            if(player1.health==0)
            DrawText("BLUE WINS",screenWidth/2-50,screenHeight/2,20,WHITE);
            else
            DrawText("RED WINS",screenWidth/2-50,screenHeight/2,20,WHITE);
        }
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}