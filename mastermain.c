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
#include "level.h"

// Pastikan untuk menyertakan library untuk suara
#pragma comment(lib, "winmm.lib")

void *backgroundBuffer;
int bgSize;

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
// DUA
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
    int frameCounter = 0;  // Tambahkan frameCounter di sini
    initAliens(aliens);
    initBullets();
    initScore();
    initExplosionsPlayer();
    initBarriers(barriers);
    initLevel();  // Inisialisasi level

    int gameOver = 0;
    int page = 0;

    const double TARGET_FPS = 30.0;  // FPS target (diturunkan ke 30 FPS agar lebih lambat)
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
            checkAndUpdateLevel(aliens);  // Periksa dan update level
            updateExplosionsPlayer();
            
            drawLives(SpaceShip_P.health);
            drawAliens(aliens);
            drawAlienExplosions();
            drawBullets();
            DrawSpaceShip(&SpaceShip_P);
            UFO(aliens); // Panggil UFO dengan array 2D
            
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

int main() {
    srand(time(NULL));
    showMainMenu();
    handleMainMenu(); 

    closegraph();
    return 0;
}