#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "ufo.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");

    showMainMenu();
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};

    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
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

        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
            }
        }

        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDir);

        // Deteksi tabrakan dan efek ledakan
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active) {
                for (int j = 0; j < MAX_BULLETS; j++) {
                    if (bullets_player[j].active &&
                        bullets_player[j].x > aliens[i].x &&
                        bullets_player[j].x < aliens[i].x + BLOCK_SIZE &&
                        bullets_player[j].y > aliens[i].y &&
                        bullets_player[j].y < aliens[i].y + BLOCK_SIZE) {
                        // Efek ledakan: lingkaran kuning dengan merah di tengah
                        setcolor(YELLOW);
                        setfillstyle(SOLID_FILL, YELLOW);
                        fillellipse(aliens[i].x + BLOCK_SIZE / 2, aliens[i].y + BLOCK_SIZE / 2, 
                                    BLOCK_SIZE, BLOCK_SIZE); // Lingkaran kuning (diameter BLOCK_SIZE * 2)
                        setcolor(RED);
                        setfillstyle(SOLID_FILL, RED);
                        fillellipse(aliens[i].x + BLOCK_SIZE / 2, aliens[i].y + BLOCK_SIZE / 2, 
                                    BLOCK_SIZE / 2, BLOCK_SIZE / 2); // Lingkaran merah (diameter BLOCK_SIZE)
                        setvisualpage(page); // Tampilkan efek seketika
                        delay(100);

                        aliens[i].active = 0;
                        bullets_player[j].active = 0;
                    }
                }
            }
        }

        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        UFO(aliens);

        setvisualpage(page);
        page = 1 - page;

        delay(10);
    }

    closegraph();
    return 0;
}