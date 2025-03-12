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
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // Ambil resolusi layar penuh
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    // ✅ Tambahkan kembali mode grafik setelah keluar dari menu
    initwindow(screenWidth, screenHeight, "Space Invaders");
    
    cleardevice();

    Player SpaceShip_P = {screenWidth / 2, screenHeight - 80};
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    int alienDirLast = 1;
    initAliens(aliens);
    initBullets();
    initScore();

    int gameOver = 0;
    int page = 0;

    while (!gameOver) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        setactivepage(page);
        cleardevice();
        drawScore();

        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= screenHeight - BLOCK_SIZE) {
                gameOver = 1;
            }
        }

        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDir, &alienDirLast);
        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        UFO(aliens);

        setvisualpage(page);
        page = 1 - page;

        delay(10);
    }

    closegraph();
}

int main() {
    

    showMainMenu();
    handleMainMenu();  // Memastikan menu utama bisa berpindah ke game

    closegraph();
    return 0;
}
