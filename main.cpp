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
    public:
        Vector2 position;
        Vector2 velocity;
        double initialVelX;
        Color color;
        double time;
        int moveDir,radius, angle;
        Arrow(Vector2 pos= {0,0}, Vector2 vel={0,0},int rad=5, int ang=45, Color col=RED, int moveDir=1):position(pos),velocity(vel),radius(rad), angle(ang),color(col),time(0),moveDir(moveDir),initialVelX(velocity.x){}
        void draw(){
            DrawCircle(position.x, position.y, radius, color);
        }
        void move(){
            position.x += moveDir*(velocity.x * cos((angle*PI)/180.0));
            position.y -= (velocity.y * sin((angle*PI)/180.0) - 0.5 * gravity * time * time);
            velocity.y -= gravity * time;
        }
};
class PlayerKeys{
    public:
        int up, down, left, right, shoot;
        PlayerKeys(int up, int down, int left, int right, int shoot):up(up),down(down),left(left),right(right),shoot(shoot){}
};
class Player{
    public:
        Vector2 position;
        int health,radius,angle;
        Color color;
        double power;
        vector <Arrow> arrows;
        bool isLeft;
        PlayerKeys keys;
        int shootKeyCounter = 0;
        int upKeyCounter = 0;
        int downKeyCounter = 0;
        int rightKeyCounter = 0;
        int leftKeyCounter = 0;
        Player(Vector2 pos= {0,0}, int h=100,int r=50, Color col=RED,PlayerKeys k = PlayerKeys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER),bool isLeft=true,int ang=45,double pow=0.6):position(pos),health(h),radius(r),color(col),angle(ang),power(pow),keys(k),isLeft(isLeft){}
        void draw(){
            int x = (isLeft)? 20 : screenWidth-120;
            DrawCircle(position.x, position.y, radius, color);
            string health_str = "Health: " + to_string(health);
            string angle_str = "Angle: " + to_string(angle);
            string pow_str = "Power: "+to_string(int(power*100));
            DrawText(health_str.c_str(), x, 20, 20, WHITE);
            DrawText(angle_str.c_str(), x, 50, 20, WHITE);
            DrawText(pow_str.c_str(), x, 80, 20, WHITE);
        }
        void shoot(){
            Arrow arrow(position, {30.0*power,40.0*power},5, angle, color, isLeft?1:-1);
            arrows.push_back(arrow);
        }
        void updateArrows(Player &p2){
            for (int i = 0; i < arrows.size(); i++){
            arrows[i].time += 1.0/60.0;
            arrows[i].draw();
            arrows[i].move();
            arrows[i].velocity.x = arrows[i].initialVelX*power;
            if(circleCollision(arrows[i].position,p2.position,arrows[i].radius, p2.radius) && ((arrows[i].moveDir!=1 && p2.isLeft) || (arrows[i].moveDir==1 && !p2.isLeft))){
                p2.health -= 1;
                arrows.erase(arrows.begin() + i);
            }
            if(p2.health<0 && p2.radius>0){
                p2.health = 0;
                p2.radius-=5;
            }
            if(arrows[i].position.x > screenWidth || arrows[i].position.x < 0  || arrows[i].position.y > screenHeight){
                arrows.erase(arrows.begin() + i);
            }
        }
        }
        void update(){
            if (IsKeyPressed(keys.shoot)) shoot();
            if(IsKeyPressed(keys.up) && (angle < 90)) angle++;
            if(IsKeyPressed(keys.down) && (angle > 0)) angle--;
            if(IsKeyPressed(keys.right) && (power < 1.0)) power += 0.01;
            if(IsKeyPressed(keys.left) && (power > 0.0)) power -= 0.01;
            if(IsKeyDown(keys.shoot)) {
                shootKeyCounter++;
                if(shootKeyCounter % 10 == 0) shoot();
            } else shootKeyCounter = 0;
            if(IsKeyDown(keys.up) && (angle < 90)) {
                upKeyCounter++;
                if(upKeyCounter % 10 == 0) angle++;
            } else upKeyCounter = 0;
            if(IsKeyDown(keys.down) && (angle > 0)) {
                downKeyCounter++;
                if(downKeyCounter % 10 == 0)angle--;
            } else downKeyCounter = 0;
            if(IsKeyDown(keys.right) && (power < 1.0)) {
                rightKeyCounter++;
                if(rightKeyCounter % 10 == 0) power += 0.01;
            } else rightKeyCounter = 0;
            if(IsKeyDown(keys.left) && (power > 0.0)) {
                leftKeyCounter++;
                if(leftKeyCounter % 10 == 0) power -= 0.01;
            } else leftKeyCounter = 0;
        }
};
int main () {
    PlayerKeys player1Keys(KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE);
    PlayerKeys player2Keys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER);
    Player player1({100,500}, 100,50, RED,player1Keys,true);
    Player player2({700,500}, 100,50, BLUE,player2Keys,false);
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
        player1.updateArrows(player2);
        player2.updateArrows(player1);
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