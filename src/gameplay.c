#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include "mainmenu.h"
#include "gameover.h"





// Pastikan untuk menyertakan library untuk suara
#pragma comment(lib, "winmm.lib")

void *backgroundBuffer;
int bgSize;
static int currentLevel = 1;  // Level awal
static float alienSpeed = BASE_ALIEN_SPEED;  // Kecepatan awal alien
static int shootInterval = BASE_SHOOT_INTERVAL;  // Interval tembakan awal

static int score = 0;
static int lastLevel = -1;  
static int blinkCounter = 0;  

extern BulletNode *playerBullets;

// Fungsi untuk membuat background hanya sekali
// SATU
void createCustomBackground() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    setactivepage(1);
    cleardevice();

    // Gradasi biru ke hitam
    for (int i = 0; i < screenHeight; i++) {
        int color = COLOR(0, 0, 255 * i / screenHeight);
        setcolor(color);
        line(0, i, screenWidth, i);
    }

    // Bintang acak
    for (int i = 0; i < 100; i++) {
        int x = rand() % screenWidth;
        int y = rand() % screenHeight;
        putpixel(x, y, WHITE);
    }

    // Simpan background ke buffer
    bgSize = imagesize(0, 0, screenWidth, screenHeight);
    backgroundBuffer = malloc(bgSize);
    getimage(0, 0, screenWidth, screenHeight, backgroundBuffer);
}

// Fungsi untuk menampilkan background dari buffer
void drawCustomBackground() {
    putimage(0, 0, backgroundBuffer, COPY_PUT);
}

void startGame() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Buat background hanya sekali
    createCustomBackground();

    Player SpaceShip_P = {screenWidth / 2, screenHeight - 120, 3, 1, 0, 0};
    Alien aliens[ALIEN_ROWS][ALIEN_COLS]; // Array 2D untuk aliens
    Barrier* barrierList = NULL;
    int alienDir = 1;
    int alienDirLast = 1;
    int frameCounter = 0;  
    initAliens();
    initScore();
    initExplosionsPlayer();
    initBarriers(&barrierList);
    initLevel();  

    int gameOver = 0;
    int page = 0;
    
    const double TARGET_FPS = 30.0;  
    const double FRAME_TIME = 1000.0 / TARGET_FPS;
    
    LARGE_INTEGER frequency, lastTime, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    PlaySound(TEXT("sound/backsound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    // **Spawn UFO pertama dengan posisi dan waktu acak**
    srand(time(NULL));
    initUFO();
    int ufoRespawnDelay = (rand() % 5 + 3) * 30; // 3-8 detik dalam frame 30FPS
    
    while (!gameOver) {
        QueryPerformanceCounter(&currentTime);
        double elapsedMs = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;

        if (elapsedMs >= FRAME_TIME) {
            lastTime = currentTime; // Update waktu frame terakhir
            frameCounter++;  // Increment frameCounter

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                PlaySound(NULL, 0, 0);
                break;
            }

            setactivepage(page);
            cleardevice();         
            //drawCustomBackground();  

            drawScore();
            SpaceshipMove(&SpaceShip_P);
            updateBullets();
            printf("Anda memiliki nyawa sebanyak : %d \n", SpaceShip_P.health);
            checkBarrierBulletCollision(barrierList);
            checkAlienCollisions(playerBullets);
            updateAliens(&alienDir, &alienDirLast, frameCounter);
            checkAlienPlayerVerticalCollision(&SpaceShip_P);  // Periksa tabrakan vertikal
            checkAndUpdateLevel(aliens);  // Periksa dan update level
            updateExplosionsPlayer();
            updatePlayerRespawn(&SpaceShip_P);

            drawLives(SpaceShip_P.health);
            drawAliens();
            drawAlienExplosions();
            drawBullets();
            DrawSpaceShip(&SpaceShip_P);
            UFO(aliens); 
            
            if (SpaceShip_P.health <= 0) {
                setvisualpage(page);
                gameOverScreen();  
                return;  
            }
            drawExplosionsPlayer();
            checkPlayerCollisions(&SpaceShip_P);
            Barrier* current = barrierList;
            while (current != NULL) {
                drawBarrier(current);
                current = current->next;
            }

            setvisualpage(page);
            page = 1 - page;

            // **Tambahkan Delay jika game berjalan terlalu cepat**
            QueryPerformanceCounter(&currentTime);
            double frameEndTime = (double)(currentTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;
            
            if (frameEndTime < FRAME_TIME) {
                Sleep((DWORD)(FRAME_TIME - frameEndTime));  // Tunggu hingga frame selesai
            }
        }
    }

    PlaySound(NULL, 0, 0);  // Hentikan musik saat game selesai
    
    free(backgroundBuffer);  // Bebaskan memori background

    cleardevice();
}


void initLevel() {
    currentLevel = 1;
    alienSpeed = BASE_ALIEN_SPEED;
    shootInterval = BASE_SHOOT_INTERVAL;
}

void checkAndUpdateLevel(Alien aliens[ALIEN_ROWS][ALIEN_COLS]) {
    // Periksa apakah semua alien habis
    int allAliensDead = 1;
    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (aliens[row][col].active) {
                allAliensDead = 0;
                break;
            }
        }
        if (!allAliensDead) break;
    }

    // Jika semua alien habis, naikkan level dan respawn
    if (allAliensDead) {
        currentLevel++;
        alienSpeed += SPEED_INCREMENT;  // Tingkatkan kecepatan pergerakan
        shootInterval -= SHOOT_INTERVAL_DECREMENT;  // Kurangi interval tembakan (lebih cepat)
        if (shootInterval < 1000) shootInterval = 1000;  // Batasi agar tidak terlalu cepat

        // Respawn alien dengan memanggil initAliens
        initAliens();

        printf("Level %d - Alien Speed: %.2f, Shoot Interval: %d\n", currentLevel, alienSpeed, shootInterval);
    }
}

float getAlienSpeed() {
    return alienSpeed;
}

int getShootInterval() {
    return shootInterval;
}

int getCurrentLevel() {
    return currentLevel;
}


void initScore() {
    score = 0;
    lastLevel = -1; 
    blinkCounter = 0; 
}

void updateScore(int points) {
    score += points;
}

void drawScore() {
    char scoreText[20];
    char levelText[20];
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2); 

    sprintf(scoreText, "Score: %d", score);
    setcolor(WHITE);
    outtextxy(10, 10, scoreText);
    
    int currentLevel = getCurrentLevel();
    if (lastLevel != currentLevel) {
        lastLevel = currentLevel;
        blinkCounter = 30;  
    }

    sprintf(levelText, "Level: %d", currentLevel);
    
    int textWidth = textwidth(levelText);
    int screenWidth = getmaxx();
    int levelX = (screenWidth - textWidth) / 2;  
    int levelY = 10;  

    if (blinkCounter > 0) {
        if (blinkCounter % 10 < 5) {
            setcolor(CYAN);  
        } else {
            setcolor(WHITE);
        }
        blinkCounter--;
    } else {
        setcolor(WHITE);  
    }
    
    outtextxy(levelX, levelY, levelText);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

int getScore() {
    return score;
}

void addAlienScore() {
    updateScore(100);
}

void addUFOScore() {
    updateScore(1000);
}