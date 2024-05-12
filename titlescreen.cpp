#include<iostream>
#include<raylib.h>
#include"titlescreen.h"
using namespace std;
TitleScreen::TitleScreen():titleScale(0.3),spriteScale(0.5),ssScale(0.2),loaded(0){
    bg = LoadTexture("assets/bg22.png");
    title = LoadTexture("assets/title.png");
    sprite = LoadTexture("assets/spritesall.png");
    smallSprite  = LoadTexture("assets/redSprite.png");
}
bool TitleScreen::draw(){
    fps++;
    DrawTexturePro(bg,{0,0,float(bg.width),float(bg.height)},{0, 0,float(GetScreenWidth()),float(GetScreenHeight())},{0,0},0, WHITE);
    DrawTexturePro(title,{0,0,float(title.width),float(title.height)}, {float(GetScreenWidth()/2 - (titleScale*title.width)/2), float(GetScreenHeight()/2 - 2.0f*(0.24/titleScale)*titleScale*title.height),titleScale*float(title.width),titleScale*float(title.height)},{0,0},0, WHITE);
    DrawTexturePro(sprite,{0,0,float(sprite.width),float(sprite.height)},{GetScreenWidth()/2.0f - spriteScale*sprite.width/2, GetScreenHeight()/2.0f - 0.48f*title.height-30,spriteScale*sprite.width, spriteScale*sprite.height},{0,0},0,WHITE);
    DrawRectangleRoundedLines({GetScreenWidth()/4.0f,4*GetScreenHeight()/5.0f, GetScreenWidth()/2.0f,70},2,5,5,WHITE);
    DrawRectangleRounded({GetScreenWidth()/4.0f,4*GetScreenHeight()/5.0f, loaded*GetScreenWidth()/2.0f,70},2,5,GREEN);
    DrawTexturePro(smallSprite,{0,0,float(smallSprite.width),float(smallSprite.height)},{loaded*(GetScreenWidth()/2.0f) + GetScreenWidth()/4.0f+10,4*GetScreenHeight()/5.0f+35 - ssScale*smallSprite.height/2,ssScale*float(smallSprite.width),ssScale*float(smallSprite.height)},{0,0},0, WHITE);
    if(fps%3==0)
        loaded+=0.01;
    if(loaded>1)
        _sleep(1000);
    return loaded<=1;
}