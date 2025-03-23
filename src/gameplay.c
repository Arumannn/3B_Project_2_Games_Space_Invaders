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
#include "score.h"
#include "ufo.h"
#include "barrier.h"
#include "gameplay.h"
#include "level.h"
#include "mainmenu.h"
#include "score.h"

#define MAX_ENTRIES 5  // Maksimum 5 pemain
#define MAX_NAME_LENGTH 10 // Maksimum panjang nama

// Struktur untuk menyimpan data leaderboard
typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} LeaderboardEntry;


// Pastikan untuk menyertakan library untuk suara
#pragma comment(lib, "winmm.lib")

void *backgroundBuffer;
int bgSize;
static int currentLevel = 1;  // Level awal
static float alienSpeed = BASE_ALIEN_SPEED;  // Kecepatan awal alien
static int shootInterval = BASE_SHOOT_INTERVAL;  // Interval tembakan awal

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
    
    initwindow(screenWidth, screenHeight, "Space Invaders");

    // Buat background hanya sekali
    createCustomBackground();

    Player SpaceShip_P = {screenWidth / 2, screenHeight - 120, 3};
    Alien aliens[ALIEN_ROWS][ALIEN_COLS]; // Array 2D untuk aliens
    Barrier barriers[MAX_BARRIERS];
    int alienDir = 1;
    int alienDirLast = 1;
    int frameCounter = 0;  
    initAliens(aliens);
    initBullets();
    initScore();
    initExplosionsPlayer();
    initBarriers(barriers);
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
    ufoRespawnDelay = (rand() % 5 + 3) * 30; // 3-8 detik dalam frame 30FPS
    
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
            drawCustomBackground();  

            drawScore();
            SpaceshipMove(&SpaceShip_P);
            updateBullets();
            checkAlienBulletCollision(barriers);
            printf("Anda memiliki nyawa sebanyak : %d \n", SpaceShip_P.health);
            checkAlienCollisions(aliens, bullets_player, MAX_BULLETS);
            updateAliens(aliens, &alienDir, &alienDirLast, frameCounter);  // Kirim frameCounter
            checkAlienPlayerVerticalCollision(aliens, &SpaceShip_P);  // Periksa tabrakan vertikal
            checkAndUpdateLevel(aliens);  // Periksa dan update level
            updateExplosionsPlayer();
            
            drawLives(SpaceShip_P.health);
            drawAliens(aliens);
            drawAlienExplosions();
            drawBullets();
            DrawSpaceShip(&SpaceShip_P);
            UFO(aliens); 
            
            if (SpaceShip_P.health <= 0) {
                gameOverScreen();  
                return;  
            }
            drawExplosionsPlayer();
            checkPlayerCollisions(&SpaceShip_P);
            for (int i = 0; i < MAX_BARRIERS; i++) {
                drawBarrier(barriers[i]);
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

    closegraph();
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
        initAliens(aliens);

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



void savePlayerScore(const char *name, int score) {
    LeaderboardEntry entries[MAX_ENTRIES];
    int count = 0;
    int found = 0; // Menandakan apakah pemain sudah ada di leaderboard

    FILE *file = fopen("leaderboard.txt", "r");
    if (file) {
        while (fscanf(file, "%s %d", entries[count].name, &entries[count].score) == 2) {
            if (strcmp(entries[count].name, name) == 0) {
                // Jika pemain sudah ada, hanya update skor jika lebih tinggi
                if (score > entries[count].score) {
                    entries[count].score = score;
                }
                found = 1;
            }
            count++;
            if (count >= MAX_ENTRIES) break;
        }
        fclose(file);
    }

    // Jika pemain tidak ditemukan dan leaderboard masih bisa menampung
    if (!found && count < MAX_ENTRIES) {
        strcpy(entries[count].name, name);
        entries[count].score = score;
        count++;
    }

    // Sorting leaderboard berdasarkan skor tertinggi
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (entries[j].score > entries[i].score) {
                LeaderboardEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }

    // Jika leaderboard sudah penuh, hapus pemain dengan skor terendah
    if (count > MAX_ENTRIES) {
        count = MAX_ENTRIES;
    }

    // Simpan leaderboard ke file
    file = fopen("leaderboard.txt", "w");
    if (file) {
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s %d\n", entries[i].name, entries[i].score);
        }
        fclose(file);
    }
}

// Tampilan layar GAME OVER
void gameOverScreen() {
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    
    initwindow(screenWidth, screenHeight, "Game Over");
    cleardevice();
    drawGradientBackground();
    drawStars();

    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 10);

    int textWidth = textwidth((char *) "GAME OVER!!");
    int textHeight = textheight((char*)"GAME OVER!!");
    outtextxy((screenWidth - textWidth) / 2, (screenHeight / 4) - (textHeight / 2), (char*)"GAME OVER!!");

    // Kotak input nama
    int inputBoxX1 = screenWidth / 2 - 150;
    int inputBoxY1 = 300;
    int inputBoxX2 = screenWidth / 2 + 150;
    int inputBoxY2 = 350;
    
    setcolor(WHITE);
    rectangle(inputBoxX1, inputBoxY1, inputBoxX2, inputBoxY2);
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(inputBoxX1, inputBoxY1 - 30, (char*)"MASUKKAN NAMA:");

    int buttonWidth = 200, buttonHeight = 50;
    int submitButtonX1 = (screenWidth - buttonWidth) / 2;
    int submitButtonY1 = inputBoxY2 + 30;
    int submitButtonX2 = submitButtonX1 + buttonWidth;
    int submitButtonY2 = submitButtonY1 + buttonHeight;
     
    setfillstyle(SOLID_FILL, CYAN);
    bar(submitButtonX1, submitButtonY1, submitButtonX2, submitButtonY2);

    setcolor(WHITE);
    setbkcolor(CYAN);
    
    int textSubmitWidth = textwidth((char*)"SUBMIT");
    int textSubmitHeight = textheight((char*)"SUBMIT");
    outtextxy((submitButtonX1 + submitButtonX2) / 2 - textSubmitWidth / 2,
              (submitButtonY1 + submitButtonY2) / 2 - textSubmitHeight / 2, 
              (char*)"SUBMIT");

    setbkcolor(BLACK);

    char playerName[MAX_NAME_LENGTH + 1] = "";
    int index = 0;
    char ch;
    int lastIndex = -1; 
    int finalScore = getScore();
    
    while (1) {
        if (index != lastIndex) {
            setfillstyle(SOLID_FILL, BLACK);
            bar(inputBoxX1 + 2, inputBoxY1 + 2, inputBoxX2 - 2, inputBoxY2 - 2);
            setcolor(WHITE);
            outtextxy(inputBoxX1 + 10, inputBoxY1 + 10, playerName);
            lastIndex = index;
        }

        if (kbhit()) {
            ch = getch();
            if (ch == 13 && index > 0) {
                savePlayerScore(playerName, finalScore);
                closegraph();
                showMainMenu();
                return;
            } 
            else if (ch == 8 && index > 0) {
                index--;
                playerName[index] = '\0';
            } 
            else if (index < MAX_NAME_LENGTH && ch >= 32 && ch <= 126) {
                playerName[index++] = ch;
                playerName[index] = '\0';
            }
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= submitButtonX1 && x <= submitButtonX2 && y >= submitButtonY1 && y <= submitButtonY2) {
                if (strlen(playerName) > 0) {
                    savePlayerScore(playerName, finalScore);
                    closegraph();
                    showMainMenu();
                    return;
                }
            }
        }
    }
}