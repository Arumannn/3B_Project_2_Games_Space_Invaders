#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include "barrier.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define BLOCK_SIZE 6 // Ukuran setiap blok kecil
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
void drawBullet(Bullet bullet) {
    if (bullet.active) {
        setfillstyle(SOLID_FILL, WHITE);
        bar(bullet.x - 2, bullet.y - 5, bullet.x + 2, bullet.y + 5);
    }
}

// Fungsi untuk mengecek tabrakan peluru dengan barrier
void checkBulletCollision(Bullet *bullet, int startX, int barrierY) {
    for (int b = 0; b < NUM_BARRIERS; b++) {
        int barrierX = startX + b * (BARRIER_WIDTH * BLOCK_SIZE + 20);

        for (int i = 0; i < BARRIER_HEIGHT; i++) {
            for (int j = 0; j < BARRIER_WIDTH; j++) {
                if (barrierState[b][i][j] == 1) { // Jika blok masih ada
                    int bx1 = barrierX + j * BLOCK_SIZE;
                    int by1 = barrierY + i * BLOCK_SIZE;
                    int bx2 = bx1 + BLOCK_SIZE;
                    int by2 = by1 + BLOCK_SIZE;

                    // Cek apakah peluru menyentuh blok
                    if (bullet->x >= bx1 && bullet->x <= bx2 && bullet->y >= by1 && bullet->y <= by2) {
                        barrierState[b][i][j] = 0; // Hancurkan blok
                        bullet->active = 0; // Matikan peluru
                        return;
                    }
                }
            }
        }
    }
}