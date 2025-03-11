#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

void startGame(){
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};

    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    int alienDirRest = 1;
    initAliens(aliens);

    initBullets();

    int gameOver = 0;
    int page = 0;

    while (!gameOver) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        setactivepage(page);
        cleardevice();

        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDirFirst, &alienDirRest);
        checkAlienCollisions(aliens, bullets_player, MAX_BULLETS);

        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        drawAlienExplosions();
        UFO(aliens);
        drawScore();

        setvisualpage(page);
        page = 1 - page;

        delay(10);
    }

    closegraph();
    return 0;
}