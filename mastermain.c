#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include "barrier.h"

int numEnemyBullets = 0; // Definisi

int countActiveEnemyBullets(Bullet enemyBullets[], int maxBullets) {
    int count = 0;
    for (int i = 0; i < maxBullets; i++) {
        if (enemyBullets[i].active) {
            count++;
        }
    }
    return count;
}


void startGame() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    initwindow(screenWidth, screenHeight, "Space Invaders");
    cleardevice();
    
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80, 3};
    Alien aliens[MAX_ALIENS];
    Bullet alienBullets[MAX_ALIEN_BULLETS] = {0};
    
    
    int alienDir = 1;
    int alienDirLast = 1;
    initAliens(aliens);
    initBullets();
    initScore();

    int gameOver = 0;
    int page = 0;

    // Target 30 FPS (33.33ms per frame)
    const double TARGET_FPS = 30.0;
    const double FRAME_TIME = 1000.0 / TARGET_FPS; // 33.33ms dalam milidetik
    LARGE_INTEGER frequency, lastTime;
    QueryPerformanceFrequency(&frequency); // Dapatkan frekuensi timer
    QueryPerformanceCounter(&lastTime);    // Waktu mulai

    while (!gameOver) {
        // Catet waktu sekarang
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        double elapsedMs = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;

        // Jalankan frame kalo waktunya udah cukup
        if (elapsedMs >= FRAME_TIME) {
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
            checkAlienCollisions(aliens, bullets_player, MAX_BULLETS);
            CheckCollision(&SpaceShip_P, alienBullets, &numEnemyBullets);
            updateAliens(aliens, &alienDir, &alienDirLast);
            DrawSpaceShip(&SpaceShip_P);
            drawBullets();
            drawAliens(aliens);
            drawAlienExplosions();
            UFO(aliens);
            barBarrier();
            setvisualpage(page);
            page = 1 - page;

            // Update waktu terakhir
            lastTime = currentTime;
        } else {
            // Kalo belum cukup waktunya, delay sisanya
            double sleepTime = FRAME_TIME - elapsedMs;
            if (sleepTime > 0) {
                Sleep((DWORD)sleepTime); // Delay sisanya
            }
        }
    }

    closegraph();
}

int main() {
    

    showMainMenu();
    handleMainMenu();  // Memastikan menu utama bisa berpindah ke game

    closegraph();
    return 0;
}
