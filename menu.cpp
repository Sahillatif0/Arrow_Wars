#include<iostream>
#include<raylib.h>
#include<vector>
#include"menu.h"
using namespace std;
MenuItem::MenuItem(int height, int width, int Nox, int Noy, string text, Vector2 position, Texture woodie):height(height), width(width), Nox(Nox),Noy(Noy), text(text), initPosition(position), woodie(woodie), ainmation({0,0}), cainmation({0,0}){
    initScale.x = (width/400.0)*1.5, initScale.y = (height/100.0)*1.5;
    scale = inScale = initScale;
    if(text=="Singleplayer"){
        inScale.x = 2*initScale.x/3;
        inScale.y = 2*initScale.y/3;
        cainmation = {0,50};
    }
}
void MenuItem::draw(){
    width = (woodie.width-350)*scale.x, height = (woodie.height-125)*scale.y;
    position.x = float (GetScreenWidth()/2.0f - woodie.width*scale.x/2.0f + (Nox-1)*((woodie.width-250)*scale.x) - initPosition.x) - ainmation.x + cainmation.x;
    position.y = float (initPosition.y+(woodie.height-125)*scale.y*(Noy - 1)) - ainmation.y + 100 - 62.5 + cainmation.y;
    DrawTexturePro(woodie, {0,0,float (woodie.width),float (woodie.height)}, {position.x,position.y, float(woodie.width * scale.x),float(woodie.height * scale.y)},{0,0},0,WHITE);
    DrawText(text.c_str(), position.x+((woodie.width*scale.x/2.0)-MeasureText(text.c_str(), 100*scale.y)/2.0f), position.y+((woodie.height-80)*scale.y/2.0f), 100*scale.y, WHITE);
}
Menu::Menu():titleScale(0.37){
    woodie = LoadTexture("assets/woodie_menu3.png");
    bg = LoadTexture("assets/bg2.png");
    title = LoadTexture("assets/title.png");
}
void Menu::addMenuItem(int height, int width, int Nox,int Noy, string text, Vector2 position){
    menus.push_back(MenuItem(height, width, Nox, Noy, text, position, woodie));
}
void Menu::draw(){
    DrawTexturePro(bg,{0,0,float(bg.width), float(bg.height)},{0,0,GetScreenWidth()*1.0f,GetScreenHeight()*1.0f},{0,0},0,WHITE);
    DrawTexturePro(title,{0,0,float(title.width),float(title.height)}, {float(GetScreenWidth()/2 - (titleScale*title.width)/2), float(GetScreenHeight()/2 - 1.6f*(0.24/titleScale)*titleScale*title.height),titleScale*float(title.width),titleScale*float(title.height)},{0,0},0, WHITE);
    for(int i=0; i<menus.size(); i++){
        menus[i].draw();
    }
}
void Menu::selectMode(bool single,bool singleClicked){
    if(singleClicked){
        menus[1].inScale.x = (2*menus[1].initScale.x/3);
        menus[1].inScale.y = (2*menus[1].initScale.y/3);
        menus[1].cainmation = {0,50};
        menus[2].cainmation = {0,0};
        menus[2].inScale = menus[2].initScale;
    }
    else{
        menus[2].inScale.x = (2*menus[2].initScale.x/3);
        menus[2].inScale.y = (2*menus[2].initScale.y/3);
        menus[2].cainmation = {90,50};
        menus[1].cainmation = {0,0};
        menus[1].inScale = menus[1].initScale;
    }
}
void Menu::checkMouse(){
    Vector2 mouse = GetMousePosition();
    for(int i=0; i<menus.size(); i++){
        if(mouse.x>=(menus[i].position.x) && mouse.x<=menus[i].position.x+menus[i].width && mouse.y>=menus[i].position.y && mouse.y<=menus[i].position.y+menus[i].height){
            if(menus[i].scale.x<menus[i].initScale.x+0.05){
                menus[i].ainmation.y +=0.2;
                if(menus[i].Nox>1)
                    menus[i].ainmation.x +=0.4;
                menus[i].scale.x += 0.0008;
                menus[i].scale.y += 0.0008;
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                    menus[i].onClick();
                }
            }
        }
        else{
            menus[i].scale = menus[i].inScale;
                menus[i].ainmation = {0,0};
        }
    }
}
MenuItem& Menu::getItem(int i){
    return menus[i];
}
void addOptionsMenu(OptionsScreen &menu){
    menu.addMenuItem(20,75,1,1,"Resume",{0,.5f*GetScreenHeight()});
    menu.addMenuItem(20,75,1,2,"Restart",{0,.5f*GetScreenHeight()});
    menu.addMenuItem(20,75,1,3,"Sound Off",{0,.5f*GetScreenHeight()});
    menu.addMenuItem(20,75,1,4,"Save",{0,.5f*GetScreenHeight()});
}
void showOptionsMenu(OptionsScreen &menu){
    menu.draw();
    menu.checkMouse();
}
void addMenu(Menu &menu){
    menu.addMenuItem(40, 160, 1,1, "Play", {0, GetScreenHeight()/3.0f +  100});
    menu.addMenuItem(20, 75, 1,2, "Singleplayer", {150, GetScreenHeight()/3.0f + 210});
    menu.addMenuItem(20, 75, 2,2, "Multiplayer", {150, GetScreenHeight()/3.0f + 210});
    menu.addMenuItem(20, 75, 1,3, "Continue", {150, GetScreenHeight()/3.0f + 220});
    menu.addMenuItem(20, 75, 2,3, "Exit", {150, GetScreenHeight()/3.0f + 220});
}
void showMenu(Menu &menu){
    menu.draw();
    menu.checkMouse();
}