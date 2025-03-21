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
#include "gameover.h"

// Pastikan untuk menyertakan library untuk suara
#pragma comment(lib, "winmm.lib") // Untuk menggunakan PlaySound

void startGame() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    initwindow(screenWidth, screenHeight, "Space Invaders");
    cleardevice();

    Player SpaceShip_P = {screenWidth / 2, screenHeight - 80, 3};
    Alien aliens[ALIEN_ROWS][ALIEN_COLS]; // Array 2D untuk aliens
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

            // Periksa apakah ada alien yang mencapai batas bawah (gunakan array 2D)
            for (int row = 0; row < ALIEN_ROWS; row++) {
                for (int col = 0; col < ALIEN_COLS; col++) {
                    if (aliens[row][col].active && aliens[row][col].y >= screenHeight - BLOCK_SIZE) {
                        gameOver = 1;
                        printf("Game Over! Alien mencapai batas bawah.\n");
                        PlaySound(NULL, 0, 0); // Hentikan musik
                        exit(0);
                    }
                }
            }
            
            drawScore();
            SpaceshipMove(&SpaceShip_P);
            updateBullets();
            printf("Anda memiliki nyawa sebanyak : %d \n", SpaceShip_P.health);
            checkAlienCollisions(aliens, bullets_player, MAX_BULLETS);
            updateAliens(aliens, &alienDir, &alienDirLast);
            updateExplosionsPlayer();
            
            drawAliens(aliens);
            drawAlienExplosions();
            drawBullets();
            DrawSpaceShip(&SpaceShip_P);
            UFO(aliens); // Panggil UFO dengan array 2D
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
    // Hentikan musik saat game selesai
    PlaySound(NULL, 0, 0);
    closegraph();
    
}

int main() {
    srand(time(NULL));
    gameOverScreen();
    showMainMenu();
    handleMainMenu(); 

    closegraph();
    return 0;
}