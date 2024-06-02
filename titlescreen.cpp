#include<iostream>
#include<raylib.h>
#include"titlescreen.h"
using namespace std;
TitleScreen::TitleScreen():titleScale(0.3),spriteScale(0.5),ssScale(0.2),loaded(0),fps(0),fps2(0),redWalkPos({100, GetScreenHeight()/2.0f+200}), blueWalkPos({GetScreenWidth()-250.0f, GetScreenHeight()/2.0f+200}),boolRedSit(false),boolBlueSit(false), redReached(false),blueReached(false), blueFlying(false), redFlying(false), Bframes(15), Rframes(15){
    bg = LoadTexture("assets/bg22.png");
    title = LoadTexture("assets/title.png");
    sprite = LoadTexture("assets/spritesall.png");
    smallSprite  = LoadTexture("assets/redSprite.png");
    walkRed = LoadTexture("assets/walkRed2.png");
    walkBlue = LoadTexture("assets/walkBlue.png");
    sitRed = LoadTexture("assets/sitRed.png");
    sitBlue = LoadTexture("assets/sitBlue.png");
    titlePos = {float(GetScreenWidth()/2 - (titleScale*title.width)/2), float(GetScreenHeight()/2 - 2.0f*(0.24/titleScale)*titleScale*title.height)};
    changeInPos = 0.0f;
}
// bool TitleScreen::draw(){
//     fps++;
//     DrawTexturePro(bg,{0,0,float(bg.width),float(bg.height)},{0, 0,float(GetScreenWidth()),float(GetScreenHeight())},{0,0},0, WHITE);
//     DrawTexturePro(title,{0,0,float(title.width),float(title.height)}, {float(GetScreenWidth()/2 - (titleScale*title.width)/2), float(GetScreenHeight()/2 - 2.0f*(0.24/titleScale)*titleScale*title.height),titleScale*float(title.width),titleScale*float(title.height)},{0,0},0, WHITE);
//     DrawTexturePro(sprite,{0,0,float(sprite.width),float(sprite.height)},{GetScreenWidth()/2.0f - spriteScale*sprite.width/2, GetScreenHeight()/2.0f - 0.48f*title.height-30,spriteScale*sprite.width, spriteScale*sprite.height},{0,0},0,WHITE);
//     DrawRectangleRoundedLines({GetScreenWidth()/4.0f,4*GetScreenHeight()/5.0f, GetScreenWidth()/2.0f,70},2,5,5,WHITE);
//     DrawRectangleRounded({GetScreenWidth()/4.0f,4*GetScreenHeight()/5.0f, loaded*GetScreenWidth()/2.0f,70},2,5,GREEN);
//     DrawTexturePro(smallSprite,{0,0,float(smallSprite.width),float(smallSprite.height)},{loaded*(GetScreenWidth()/2.0f) + GetScreenWidth()/4.0f+10,4*GetScreenHeight()/5.0f+35 - ssScale*smallSprite.height/2,ssScale*float(smallSprite.width),ssScale*float(smallSprite.height)},{0,0},0, WHITE);
//     if(fps%3==0)
//         loaded+=0.01;
//     if(loaded>1)
//         _sleep(1000);
//     return true;
// }


bool TitleScreen::draw(){
    DrawTexturePro(bg,{0,0,float(bg.width),float(bg.height)},{0, 0,float(GetScreenWidth()),float(GetScreenHeight())},{0,0},0, WHITE);
    fps++;
    if(redWalkPos.x<float(GetScreenWidth()/2 - (titleScale*title.width)/2-150))
        redWalkPos.x++;
    else if(!redReached && redWalkPos.y>float(GetScreenHeight()/2)-(titleScale*title.height)*0.3-100){
        redFlying = true;
        redWalkPos.y-=6;
    }
    else if(!boolRedSit && redWalkPos.y<=float(GetScreenHeight()/2)-(titleScale*title.height)*0.3-100){
        cout<<"value is: "<<redWalkPos.y<<endl;
        redFlying = false;
        redReached = true;
        redWalkPos.x+=2;
        redWalkPos.y+=2;
    }
    else if(!boolRedSit && redReached && redWalkPos.y<=float(GetScreenHeight()/2)-(titleScale*title.height)*0.3+30){
        redWalkPos.x+=2;
        redWalkPos.y+=2;
    }
    else
        boolRedSit = true;
    if(blueWalkPos.x>float(GetScreenWidth()/2 - ((titleScale*title.width)/2)+title.width*titleScale-50))
        blueWalkPos.x-=1.25;
    else if(!blueReached && blueWalkPos.y>float(GetScreenHeight()/2)-(titleScale*title.height*0.3+100)){
        blueFlying = true;
        blueWalkPos.y-=6;
    }
    else if(!boolBlueSit && blueWalkPos.y<=float(GetScreenHeight()/2)-(titleScale*title.height*0.3+100)){
        blueFlying = false;
        blueReached = true;
        blueWalkPos.x-=2;
        blueWalkPos.y+=2;
    }
    else if(!boolBlueSit && blueReached && blueWalkPos.y<=float(GetScreenHeight()/2)-(titleScale*title.height)*0.3+30){
        blueWalkPos.x-=2;
        blueWalkPos.y+=2;
    }
    else
        boolBlueSit = true;
    fps2=blueFlying?2:fps2;
    float yVelocity = 0.01* 2.0f*(0.24/titleScale)*titleScale*title.height;
    if(fps>100 && fps<=200)
        changeInPos += yVelocity;
    else if(fps>400 && fps<=500){
        changeInPos -= yVelocity;
        redWalkPos.y -= yVelocity;
        blueWalkPos.y -= yVelocity;
    }
    DrawTexturePro(title,{0,0,float(title.width),float(title.height)}, {titlePos.x, titlePos.y+changeInPos,titleScale*float(title.width),titleScale*float(title.height)},{0,0},0, WHITE);
    if(boolRedSit)
        DrawTexturePro(sitRed,{0,0,256,144},{redWalkPos.x,redWalkPos.y,1024*0.2f,576*0.2f},{0,0},0,WHITE);
    else
        DrawTexturePro(walkRed,{(fps2%Bframes+2)*256.0f,0,256,144},{redWalkPos.x,redWalkPos.y,1024*.2f,576*.2f},{0,0},0,WHITE);
    if(boolBlueSit)
        DrawTexturePro(sitBlue,{0,0,256,144},{blueWalkPos.x,blueWalkPos.y,1024*.2f,576*.2f},{0,0},0,WHITE);
    else
        DrawTexturePro(walkBlue,{(fps2%Rframes+2)*256.0f,0,256,144},{blueWalkPos.x,blueWalkPos.y,1024*.2f,576*.2f},{0,0},0,WHITE);
    if(fps%6==0)
        fps2++;
    if(fps>550){
        DrawTexturePro(sprite,{0,0,float(sprite.width),float(sprite.height)},{GetScreenWidth()/2.0f - spriteScale*sprite.width/2, GetScreenHeight()/2.0f - 0.48f*title.height-30,spriteScale*sprite.width, spriteScale*sprite.height},{0,0},0,WHITE);
        DrawRectangleRoundedLines({GetScreenWidth()/4.0f,4*GetScreenHeight()/5.0f, GetScreenWidth()/2.0f,70},2,5,5,WHITE);
        DrawRectangleRounded({GetScreenWidth()/4.0f,4*GetScreenHeight()/5.0f, loaded*GetScreenWidth()/2.0f,70},2,5,GREEN);
        DrawTexturePro(walkRed,{(fps2%Rframes+2)*256.0f,0,256,144},{loaded*(GetScreenWidth()/2.0f) + GetScreenWidth()/4.0f-40,4*GetScreenHeight()/5.0f+30 - ssScale*smallSprite.height/2,1024*ssScale*0.67f,576*ssScale*0.67f},{0,0},0,WHITE);
        // DrawTexturePro(walkRed,{0,0,float(smallSprite.width),float(smallSprite.height)},{loaded*(GetScreenWidth()/2.0f) + GetScreenWidth()/4.0f+10,4*GetScreenHeight()/5.0f+35 - ssScale*smallSprite.height/2,ssScale*float(smallSprite.width),ssScale*float(smallSprite.height)},{0,0},0, WHITE);
        if(fps%3==0)
            loaded+=0.01;
        if(loaded>1)
            _sleep(1000);
    }
    return loaded<=1;
}
// int main(){
//     InitWindow(1800,920,"TitleScreen");
//     SetTargetFPS(60);
//     TitleScreen titleScreen;
//     while(!WindowShouldClose()){
//         BeginDrawing();
//         ClearBackground(WHITE);
//         if(!titleScreen.draw()){
//             CloseWindow();
//         }
//         EndDrawing();
//     }
//     return 0;
// }