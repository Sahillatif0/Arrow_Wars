#include<iostream>
#include<raylib.h>

using namespace std;
#ifndef TITLESCREEN_H
#define TITLESCREEN_H

class TitleScreen{
    Texture bg, title, sprite, smallSprite, walkRed, walkBlue, sitRed, sitBlue;
    float titleScale, spriteScale, ssScale, loaded, changeInPos;
    int fps, fps2, Bframes, Rframes;
    Vector2 redWalkPos, blueWalkPos, titlePos;
    bool boolRedSit, boolBlueSit, redReached, blueReached, redFlying, blueFlying;
    public:
        TitleScreen();
        bool draw();
};
void animateTexture(Texture,Rectangle, Rectangle, Rectangle, Vector2,int);
#endif //MENU_H 