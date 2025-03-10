#include "mainmenu.h"
#include "alien.h"
#include "ufo.h"
#include "score.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

void startGame() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // Ambil resolusi layar penuh
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    // ✅ Tambahkan kembali mode grafik setelah keluar dari menu
    initwindow(screenWidth, screenHeight, "Space Invaders", -3, -3);
    
    cleardevice();

    Player SpaceShip_P = {screenWidth / 2, screenHeight - 80};
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    initAliens(aliens);
    initBullets();
    initScore();

    int gameOver = 0;
    int page = 0;

    while (!gameOver) {
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
        updateAliens(aliens, &alienDir);
        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        UFO(aliens);

        setvisualpage(page);
        page = 1 - page;

        delay(10);
    }

    closegraph();
}

int main() {
    

    showMainMenu();
    handleMainMenu();  // Memastikan menu utama bisa berpindah ke game

    closegraph();
}

/*#include "mainmenu.h"
#include "alien.h"
#include "ufo.h"
#include "score.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

// Struktur untuk menyimpan posisi ledakan
typedef struct {
    int x, y;
    int active;
} Explosion;

Explosion explosions[MAX_ALIENS]; // Array untuk menyimpan ledakan

void initExplosions() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        explosions[i].active = 0;
    }
}

void drawExplosions() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (explosions[i].active) {
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            fillellipse(explosions[i].x, explosions[i].y, BLOCK_SIZE, BLOCK_SIZE); // Lingkaran kuning
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            fillellipse(explosions[i].x, explosions[i].y, BLOCK_SIZE / 2, BLOCK_SIZE / 2); // Lingkaran merah
            explosions[i].active = 0; // Hanya tampil satu frame
        }
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");

    showMainMenu();
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};

    Alien aliens[MAX_ALIENS];
    int alienDirFirst = 1;  // Arah untuk baris 0-1 (kiri ke kanan)
    int alienDirRest = -1;  // Arah untuk baris 2-5 (kanan ke kiri)
    initAliens(aliens);
    initBullets();
    initExplosions(); // Inisialisasi ledakan

    int gameOver = 0;
    int page = 0;

    // Kontrol frame rate
    const int TARGET_FPS = 30; // Target 30 FPS
    const int FRAME_TIME = 1000 / TARGET_FPS; // Waktu per frame dalam milidetik (33 ms)
    clock_t startTime, endTime;

    while (!gameOver) {
        startTime = clock(); // Catat waktu mulai frame

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        setactivepage(page);
        cleardevice();

        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
            }
        }

        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDir);

        // Deteksi tabrakan dan tambahkan ledakan
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active) {
                for (int j = 0; j < MAX_BULLETS; j++) {
                    if (bullets_player[j].active &&
                        bullets_player[j].x > aliens[i].x &&
                        bullets_player[j].x < aliens[i].x + BLOCK_SIZE &&
                        bullets_player[j].y > aliens[i].y &&
                        bullets_player[j].y < aliens[i].y + BLOCK_SIZE) {
                        // Tambahkan ledakan ke array
                        for (int k = 0; k < MAX_ALIENS; k++) {
                            if (!explosions[k].active) {
                                explosions[k].x = aliens[i].x + BLOCK_SIZE / 2;
                                explosions[k].y = aliens[i].y + BLOCK_SIZE / 2;
                                explosions[k].active = 1;
                                break;
                            }
                        }
                        aliens[i].active = 0;
                        bullets_player[j].active = 0;
                    }
                }
            }
        }

        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        drawExplosions(); // Gambar efek ledakan
        UFO(aliens);

        setvisualpage(page);
        page = 1 - page;

        // Kontrol frame rate: Tunggu hingga waktu frame selesai
        endTime = clock();
        int elapsedTime = (endTime - startTime) * 1000 / CLOCKS_PER_SEC; // Konversi ke ms
        int delayTime = FRAME_TIME - elapsedTime;
        if (delayTime > 0) {
            delay(delayTime); // Delay hanya jika waktu pemrosesan kurang dari target
        }
    }

    closegraph();
}

int main() {
    

    showMainMenu();
    handleMainMenu();  // Memastikan menu utama bisa berpindah ke game

    closegraph();
    return 0;
}*/