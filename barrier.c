#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include "barrier.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define BLOCK_SIZE 4 // Ukuran setiap blok kecil
#define BARRIER_WIDTH 9  // Lebar dalam jumlah blok
#define BARRIER_HEIGHT 4 // Tinggi dalam jumlah blok
#define NUM_BARRIERS 3
#define BULLET_SPEED 5
#define BARRIER_SPACING 100

typedef struct {
    int x, y;
    int active;
} Bullet;

// Pola barrier berdasarkan gambar (0 = kosong, 1 = blok ada)
int barrierPattern[BARRIER_HEIGHT][BARRIER_WIDTH] = {
    {0, 0, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1}
};

// Menyimpan status setiap blok dalam setiap barrier
int barrierState[NUM_BARRIERS][BARRIER_HEIGHT][BARRIER_WIDTH];


// Fungsi menggambar barrier
void drawBarrier(int index, int x, int y) {
    setfillstyle(SOLID_FILL, GREEN);
    for (int i = 0; i < BARRIER_HEIGHT; i++) {
        for (int j = 0; j < BARRIER_WIDTH; j++) {
            if (barrierState[index][i][j] == 1) {
                bar(x + j * BLOCK_SIZE, 
                    y + i * BLOCK_SIZE, 
                    x + (j + 1) * BLOCK_SIZE, 
                    y + (i + 1) * BLOCK_SIZE);
            }
        }
    }
}

// Fungsi menggambar peluru


// Fungsi untuk mengecek tabrakan peluru dengan barrier


int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    // Posisi barrier di layar
    int startX = (SCREEN_WIDTH - (NUM_BARRIERS * BARRIER_WIDTH * BLOCK_SIZE + (NUM_BARRIERS - 1) * BARRIER_SPACING)) / 2;
    int barrierY = SCREEN_HEIGHT - 150;

    // Copy pola barrier ke array state
    for (int b = 0; b < NUM_BARRIERS; b++) {
        for (int i = 0; i < BARRIER_HEIGHT; i++) {
            for (int j = 0; j < BARRIER_WIDTH; j++) {
                barrierState[b][i][j] = barrierPattern[i][j];
            }
        }
    }

    Bullet bullet = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 0}; // Peluru awal (tidak aktif)

    while (1) {
        cleardevice(); // Hapus layar untuk perbaruan frame

        // Gambar barrier
        for (int i = 0; i < NUM_BARRIERS; i++) {
            drawBarrier(i, startX + i * (BARRIER_WIDTH * BLOCK_SIZE + BARRIER_SPACING), barrierY);
        }

        // Gambar peluru jika aktif
       

        // Kontrol Keyboard
        if (kbhit()) {
            char key = getch();
            if (key == ' ') { // Tombol spasi untuk menembak
                if (!bullet.active) {
                    bullet.x = SCREEN_WIDTH / 2;
                    bullet.y = SCREEN_HEIGHT - 50;
                    bullet.active = 1;
                }
            }
            if (key == 27) break; // Tombol ESC untuk keluar
        }

        delay(30);
    }
    return 0;
}