#include<iostream>
#include<raylib.h>
#include<vector>
#include"menu.h"
using namespace std;
int MenuScreenWidth = 1800;
int MenuScreenHeight = 920;
MenuItem::MenuItem(int height, int width, int Nox, int Noy, string text, Vector2 position, Texture woodie):height(height), width(width), Nox(Nox),Noy(Noy), text(text), initPosition(position), woodie(woodie), ainmation({0,0}){
    initScale.x = width/400.0, initScale.y = height/100.0;
    scale = initScale;
}
void MenuItem::draw(){
    width = woodie.width*scale.x, height = woodie.height*scale.y;
    position.x = float (MenuScreenWidth/2.0f - woodie.width*scale.x/2.0f + (Nox-1)*(woodie.width*scale.x+10) - initPosition.x) - ainmation.x;
    position.y = float (initPosition.y+woodie.height*scale.y*(Noy - 1)) - ainmation.y;
    DrawTexturePro(woodie, {0,0,float (woodie.width),float (woodie.height)}, {position.x,position.y, float(woodie.width * scale.x),float(woodie.height * scale.y)},{0,0},0,WHITE);
    DrawText(text.c_str(), position.x+((woodie.width*scale.x/2.0)-MeasureText(text.c_str(), 100*scale.y)/2.0f), position.y+((woodie.height-80)*scale.y/2.0f), 100*scale.y, WHITE);
}
Menu::Menu(){
    woodie = LoadTexture("assets/woodie_menu1.png");
    bg = LoadTexture("assets/bg1.png");
}
void Menu::addMenuItem(int height, int width, int Nox,int Noy, string text, Vector2 position){
    menus.push_back(MenuItem(height, width, Nox, Noy, text, position, woodie));
}
void Menu::draw(){
    for(int i=0; i<menus.size(); i++){
        menus[i].draw();
    }
}
void Menu::selectItem(int No){
    cout<<"Selected Item: "<<No<<endl;
}
void Menu::checkMouse(){
    Vector2 mouse = GetMousePosition();
    for(int i=0; i<menus.size(); i++){
        if(mouse.x>=menus[i].position.x && mouse.x<=menus[i].position.x+menus[i].width && mouse.y>=menus[i].position.y && mouse.y<=menus[i].position.y+menus[i].height){
            if(menus[i].scale.x<menus[i].initScale.x+0.05){
                menus[i].ainmation.y +=0.2;
                if(menus[i].Nox>1)
                    menus[i].ainmation.x +=0.4;
                menus[i].scale.x += 0.0008;
                menus[i].scale.y += 0.0008;
            }
        }
        else{
            menus[i].scale = menus[i].initScale;
            menus[i].ainmation = {0,0};
        }
    }
}
void addMenu(Menu &menu){
    menu.addMenuItem(40, 160, 1,1, "Play", {0, MenuScreenHeight/3.0f +  100});
    menu.addMenuItem(20, 75, 1,2, "Singleplayer", {95, MenuScreenHeight/3.0f + 160});
    menu.addMenuItem(20, 75, 2,2, "Multiplayer", {95, MenuScreenHeight/3.0f + 160});
    menu.addMenuItem(20, 75, 1,3, "Options", {95, MenuScreenHeight/3.0f + 170});
    menu.addMenuItem(20, 75, 2,3, "Exit", {95, MenuScreenHeight/3.0f + 170});
}
void showMenu(Menu &menu){
    DrawTexturePro(menu.bg,{0,0,float(menu.bg.width), float(menu.bg.height)},{0,0,MenuScreenWidth*1.0f,MenuScreenHeight*1.0f},{0,0},0,WHITE);
    DrawText("ARROW WARS", MenuScreenWidth/2.0f - MeasureText("ARROW WARS", 100)/2.0f,180,100,WHITE);
    menu.draw();
    menu.checkMouse();
}
// int main() {
//     InitWindow(MenuScreenWidth, MenuScreenHeight, "Menu");
//     Menu menu;
//     addMenu(menu);
//     while(!WindowShouldClose()){
//         BeginDrawing();
//         showMenu(menu);
//         EndDrawing();
//     }
//     CloseWindow();
//     return 0;
// }