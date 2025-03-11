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
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    initwindow(getmaxwidth(), getmaxheight(), "SPACE INVADERS");

    printf("Game window initialized!\n"); // Debug

    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    printf("Screen width: %d, height: %d\n", screenWidth, screenHeight); // Debug
   
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

    while (!gameOver) {
        printf("Game loop running...\n"); 
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        setactivepage(page);
        cleardevice();
        drawScore();

        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
            }
        }

        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDirFirst, &alienDirRest);
        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        moveUFO();

        setvisualpage(page);
        page = 1 - page;

        delay(10);
    }

   
} 

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");
    showMainMenu();
    closegraph();
    return 0;
}
