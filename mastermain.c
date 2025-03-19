#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include "barrier.h"

void startGame() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    initwindow(screenWidth, screenHeight, "Space Invaders");
    cleardevice();

    Player SpaceShip_P = {screenWidth / 2, screenHeight - 80, 3};  // Inisialisasi nyawa
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    int alienDirLast = 1;
    initAliens(aliens);
    initBullets();
    initScore();
    initExplosionsPlayer();

    int gameOver = 0;
    int page = 0;

    const double TARGET_FPS = 30.0;
    const double FRAME_TIME = 1000.0 / TARGET_FPS;
    LARGE_INTEGER frequency, lastTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    while (!gameOver) {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        double elapsedMs = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;

        if (elapsedMs >= FRAME_TIME) {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                break;
            }
            setactivepage(page);
            cleardevice();         

            for (int i = 0; i < MAX_ALIENS; i++) {
                printf("Alien %d - Active: %d, Y: %d\n", i, aliens[i].active, aliens[i].y);
                if (aliens[i].active && aliens[i].y >= screenHeight - BLOCK_SIZE) {
                    gameOver = 1;
                    printf("Game Over! Alien mencapai batas bawah.\n");
                }
            }
            
            
            // Render
            drawScore();
            SpaceshipMove(&SpaceShip_P);
            updateBullets();
            checkAlienCollisions(aliens, bullets_player, MAX_BULLETS);
            updateAliens(aliens, &alienDir, &alienDirLast);
            updateExplosionsPlayer();
            
            drawAliens(aliens);
            drawAlienExplosions();
            drawBullets();
            DrawSpaceShip(&SpaceShip_P);
            UFO(aliens);
            barBarrier();
            
            
            drawExplosionsPlayer();
           
            checkPlayerCollisions(&SpaceShip_P);

            setvisualpage(page);
            page = 1 - page;

            lastTime = currentTime;
        } else {
            double sleepTime = FRAME_TIME - elapsedMs;
            if (sleepTime > 0) {
                Sleep((DWORD)sleepTime);
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
