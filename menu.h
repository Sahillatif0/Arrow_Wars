#include<iostream>
#include<raylib.h>
#include<vector>

using namespace std;
#ifndef MENU_H
#define MENU_H

class MenuItem{
    int height, width, Nox, Noy;
    string text;
    Vector2 initPosition, position, initScale,scale, ainmation;
    Texture woodie;
    friend class Menu;
    public:
        MenuItem(int height, int width, int Nox, int Noy, string text, Vector2 position, Texture woodie);
        void draw();
};
class Menu{
    vector<MenuItem> menus;
    Texture bg, woodie;
    public:
        Menu();
        void addMenuItem(int height, int width, int Nox,int Noy, string text, Vector2 position);
        void draw();
        void selectItem(int No);
        void checkMouse();
        friend void showMenu(Menu &menu);
        
};
void addMenu(Menu &menu);
void showMenu(Menu &menu);
#endif //MENU_H 