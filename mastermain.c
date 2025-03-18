#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include "barrier.h"

// Pastikan untuk menyertakan library untuk suara
#pragma comment(lib, "winmm.lib") // Untuk menggunakan PlaySound

void startGame() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
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

    const double TARGET_FPS = 30.0;
    const double FRAME_TIME = 1000.0 / TARGET_FPS;
    LARGE_INTEGER frequency, lastTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    // Mulai musik latar saat game dimulai
    PlaySound(TEXT("sound/background_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while (!gameOver) {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        double elapsedMs = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;

        if (elapsedMs >= FRAME_TIME) {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                // Hentikan musik saat keluar dari game
                PlaySound(NULL, 0, 0);
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
            updateAliens(aliens, &alienDir, &alienDirLast);
            DrawSpaceShip(&SpaceShip_P);
            drawBullets();
            drawAliens(aliens);
            drawAlienExplosions();

            if (ufoActive) {
                UFO(aliens);
            }

            barBarrier();
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
    // Hentikan musik saat game selesai
    PlaySound(NULL, 0, 0);
    closegraph();
}

int main() {
    srand(time(NULL));
    showMainMenu();
    handleMainMenu();

    closegraph();
    return 0;
}