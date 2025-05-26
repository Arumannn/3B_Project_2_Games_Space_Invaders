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


gameState currentGameState = MAIN_MENU;

int main() {
    srand((unsigned)time(NULL));
    initwindow(getmaxwidth(), getmaxheight(), "Space Invaders");
    handleMainMenu();

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    Player SpaceShip_P = {screenWidth / 2, screenHeight - 120, 3, 1, 0, 0};


    Alien aliens[ALIEN_ROWS][ALIEN_COLS];
    Barrier* barrierList = NULL;
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
        switch (currentGameState)
        {
        case MAIN_MENU:
            handleMainMenu();
            break;
        case LEADERBOARD:
            showLeaderboard();
            break;
        case GUIDE:
            handleGuide();
            break;
        case EXIT:
            gameOver = 1;
            break;
        case PLAY_GAME:
            QueryPerformanceCounter(&currentTime);
            double elapsedMs = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;

            if (elapsedMs >= FRAME_TIME) {
                lastTime = currentTime;
                frameCounter++;

                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    PlaySound(NULL, 0, 0);
                    currentGameState = MAIN_MENU;
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
                checkAlienPlayerVerticalCollision(&SpaceShip_P);
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
                    currentGameState = GAMEOVER;
                    break;
                }

                setvisualpage(page);
                page = 1 - page;


                QueryPerformanceCounter(&currentTime);
                double frameDuration = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;
                if (frameDuration < FRAME_TIME) {
                    Sleep((DWORD)(FRAME_TIME - frameDuration));
                }
            }
        break;
        case GAMEOVER:
            gameOverScreen();
            break;
        default:
            break;
        }
    }

    PlaySound(NULL, 0, 0);
    cleardevice();
    closegraph();
    return 0;
}
