#include<iostream>
#include<raylib.h>

using namespace std;
#ifndef TITLESCREEN_H
#define TITLESCREEN_H

class TitleScreen{
    Texture bg, title, sprite, smallSprite;
    float titleScale, spriteScale, ssScale, loaded;
    int fps;
    public:
        TitleScreen();
        bool draw();
};
#endif //MENU_H 