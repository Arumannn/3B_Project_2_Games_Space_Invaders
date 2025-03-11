#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

void startGame() {
    cleardevice();
    printf("START GAME dipanggil!\n");
    printf("Game window initialized!\n");
    printf("Screen width: %d, height: %d\n", getmaxx(), getmaxy());
   
    int alienDirFirst = 1;
    int alienDirRest = 1;
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    initAliens(aliens);
    initBullets();
    initScore();

    int gameOver = 0;
    int page = 0;

    while (TRUE) {
        printf("Game loop...\n"); 
        // if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        //     printf("Break");
        //     break;
        // }

        //setactivepage(page);
        cleardevice();
        printf("1");
        drawScore();
        printf("2");

        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
                printf("3");
            }
        }
        printf("4");
        SpaceshipMove(&SpaceShip_P);
        printf("5");
        updateBullets();
        printf("6");
        updateAliens(aliens, &alienDirFirst, &alienDirRest);
        printf("7");
        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        moveUFO();

        setvisualpage(page);
        page = 1 - page;

        delay(30);
    }
    printf("end");
} 

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    showMainMenu();
    printf("close\n");
    closegraph();
    return 0;
}