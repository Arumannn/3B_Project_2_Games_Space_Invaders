#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, ""); // Inisialisasi graphics mode

    showMainMenu();
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};

    // Inisialisasi aliens
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    initAliens(aliens);
    // Inisialisasi peluru
    initBullets();
    // Inisialisasi Score
    initScore();

    

    int gameOver = 0;
    int page = 0; // Untuk double buffering

    while (!gameOver) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }
    
        setactivepage(page);
        cleardevice();
    
        // Cek game over
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
            }
        }
    
        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDir);
        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        UFO(); 
    
        // Tampilkan buffer yang sudah digambar
        setvisualpage(page);
        page = 1 - page;
    
        delay(10);
    }
    
    closegraph();
    return 0;
}
