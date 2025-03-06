#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "ufo.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

int main() {
    showMainMenu();
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};

    // Inisialisasi aliens
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    initAliens(aliens);

    // Inisialisasi peluru
    initBullets();

    srand(time(0));

    int gameOver = 0;
    while (!gameOver) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        // Cek kalau aliens nyampe bawah (game over)
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
            }
        }

        cleardevice();
        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDir);
        DrawSpaceShip(&SpaceShip_P); 
        drawBullets();
        drawAliens(aliens);
        moveUFO();

        

        delay(30);
    }

    closegraph();
    return 0;
}
