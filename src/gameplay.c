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

#define MAX_ENTRIES 5  // Maksimum 5 pemain
#define MAX_NAME_LENGTH 10 // Maksimum panjang nama

// Struktur untuk menyimpan data leaderboard
typedef struct LeaderboardEntry {
    char name[MAX_NAME_LENGTH];
    int score;
    struct LeaderboardEntry *next;
} LeaderboardEntry;


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
    Barrier* barrierHead = NULL;
    int alienDir = 1;
    int alienDirLast = 1;
    int frameCounter = 0;  
    initAliens(aliens);
    initAllBullets();
    initScore();
    initExplosionsPlayer();
    initBarriers(&barrierHead);
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
            drawCustomBackground();  

            drawScore();
            SpaceshipMove(&SpaceShip_P);
            updateBullets();
            checkAlienBulletCollision(barrierHead);
            printf("Anda memiliki nyawa sebanyak : %d \n", SpaceShip_P.health);
            checkAlienCollisions(aliens, playerBullets);
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
            Barrier* current = barrierHead;
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

// PROGRAM GAME OVER
void savePlayerScore(const char *name, int score) {
    LeaderboardEntry *head = NULL;
    LeaderboardEntry *current = NULL;
    LeaderboardEntry *prev = NULL;
    int found = 0;

    // Baca file dan isi ke linked list
    FILE *file = fopen("leaderboard.txt", "r");
    if (file) {
        char tempName[MAX_NAME_LENGTH];
        int tempScore;

        while (fscanf(file, "%s %d", tempName, &tempScore) == 2) {
            LeaderboardEntry *newNode = (LeaderboardEntry*)malloc(sizeof(LeaderboardEntry));
            strcpy(newNode->name, tempName);
            newNode->score = tempScore;
            newNode->next = NULL;

            // Tambahkan ke linked list
            if (!head) {
                head = newNode;
            } else {
                current->next = newNode;
            }
            current = newNode;
        }
        fclose(file);
    }

    // Cek apakah nama sudah ada
    current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            found = 1;
            if (score > current->score) {
                current->score = score; // update skor lebih tinggi
            }
            break;
        }
        current = current->next;
    }

    // Jika nama belum ada, tambahkan node baru
    if (!found) {
        LeaderboardEntry *newNode = (LeaderboardEntry*)malloc(sizeof(LeaderboardEntry));
        strcpy(newNode->name, name);
        newNode->score = score;
        newNode->next = NULL;

        // Tambahkan di akhir
        if (!head) {
            head = newNode;
        } else {
            current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Urutkan linked list berdasarkan skor (descending)
    for (LeaderboardEntry *i = head; i != NULL; i = i->next) {
        for (LeaderboardEntry *j = i->next; j != NULL; j = j->next) {
            if (j->score > i->score) {
                // tukar data
                char tempName[MAX_NAME_LENGTH];
                int tempScore;

                strcpy(tempName, i->name);
                tempScore = i->score;

                strcpy(i->name, j->name);
                i->score = j->score;

                strcpy(j->name, tempName);
                j->score = tempScore;
            }
        }
    }

    // Simpan ke file
    file = fopen("leaderboard.txt", "w");
    if (file) {
        int count = 0;
        current = head;
        while (current && count < MAX_ENTRIES) {
            fprintf(file, "%s %d\n", current->name, current->score);
            current = current->next;
            count++;
        }
        fclose(file);
    }

    // Bebaskan memori linked list
    while (head) {
        LeaderboardEntry *temp = head;
        head = head->next;
        free(temp);
    }
}

// Tampilan layar GAME OVER
void gameOverScreen() {
    // Inisialisasi Window dan Layar 
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    
    cleardevice();

    // Background 
    drawGradientBackground();
    drawStars();

    // Teks "GAME OVER!!" 
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
    
    // Label "MASUKKAN NAMA:" 
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(inputBoxX1, inputBoxY1 - 30, (char*)"MASUKKAN NAMA:");

    // Tombol "SUBMIT" 
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

    // Input Nama Pemain Secara Dinamis
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

        // Tombol Keyboard (ENTER dan BACKSPACE) 
        if (kbhit()) {
            ch = getch();
            if (ch == 13 && index > 0) {
                savePlayerScore(playerName, finalScore);
                cleardevice();
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

        // Klik Tombol SUBMIT 
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= submitButtonX1 && x <= submitButtonX2 && y >= submitButtonY1 && y <= submitButtonY2) {
                if (strlen(playerName) > 0) {
                    savePlayerScore(playerName, finalScore);
                    cleardevice();
                    showMainMenu();
                    return;
                }
            }
        }
    }
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