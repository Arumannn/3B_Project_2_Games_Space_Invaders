#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include <conio.h>
#include <windows.h>




int main() {
    showMainMenu();
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() / 2};
    
    SpaceShip(&SpaceShip_P);
    

    getch();
    closegraph();
}