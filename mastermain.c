#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "ufo.h"
#include "barrier.h"
#include "gameplay.h"

#pragma comment(lib, "winmm.lib")

extern BulletNode* playerBullets;
Barrier* barrierList = NULL; // Memindahkan barrierList menjadi global

int main() {
    srand((unsigned)time(NULL));
    initwindow(getmaxwidth(), getmaxheight(), "Space Invaders");

    while (1) { 
        handleMainMenu();  

        // Inisialisasi ulang game setiap user klik "START"
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        Player SpaceShip_P = {screenWidth / 2, screenHeight - 120, 3, 1, 0, 0};

        Alien aliens[ALIEN_ROWS][ALIEN_COLS];
        int alienDir = 1, alienDirLast = 1, frameCounter = 0;

        initAliens();
        initScore();
        initExplosionsPlayer();
        initBarriers(&barrierList);
        initLevel();
        initUFO();

        PlaySound(TEXT("sound/backsound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

        LARGE_INTEGER frequency, lastTime, currentTime;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&lastTime);

        const double TARGET_FPS = 30.0;
        const double FRAME_TIME = 1000.0 / TARGET_FPS;

        int gameOver = 0;
        int page = 0;

        while (!gameOver) {
            QueryPerformanceCounter(&currentTime);
            double elapsedMs = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;

            if (elapsedMs >= FRAME_TIME) {
                lastTime = currentTime;
                frameCounter++;

                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    PlaySound(NULL, 0, 0);
                    gameOver = 1;
                    break;
                }

                setactivepage(page);
                cleardevice();
                drawStars();
                drawScore();
                SpaceshipMove(&SpaceShip_P);
                updateBullets();
                checkBarrierBulletCollision(barrierList);
                checkAlienCollisions(playerBullets);
                updateAliens(&alienDir, &alienDirLast, frameCounter);
                checkAlienPlayerVerticalCollision(&SpaceShip_P, &gameOver);
                checkAndUpdateLevel();
                updateExplosionsPlayer();
                updatePlayerRespawn(&SpaceShip_P);

                drawLives(SpaceShip_P.health);
                drawAliens();
                drawAlienExplosions();
                drawBullets();
                DrawSpaceShip(&SpaceShip_P);
                UFO(aliens);
                drawExplosionsPlayer();
                checkPlayerCollisions(&SpaceShip_P);

                for (Barrier* b = barrierList; b != NULL; b = b->next) {
                    drawBarrier(b);
                }

                if (SpaceShip_P.health <= 0) {
                    setvisualpage(page);
                    gameOverScreen(); 
                    gameOver = 1;     
                }

                setvisualpage(page);
                page = 1 - page;

                QueryPerformanceCounter(&currentTime);
                double frameDuration = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;
                if (frameDuration < FRAME_TIME) {
                    Sleep((DWORD)(FRAME_TIME - frameDuration));
                }
            }
        }

        PlaySound(NULL, 0, 0);
        cleardevice();
        freeBarriers(barrierList); // Free barriers when game loop ends
        barrierList = NULL; // Reset barrierList
    }

    closegraph(); 
    return 0;
}