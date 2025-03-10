#ifndef ALIEN_H
#define ALIEN_H

#include <graphics.h>

// Konstanta
#define ALIEN_ROWS 6
#define ALIEN_COLS 10
#define MAX_ALIENS (ALIEN_ROWS * ALIEN_COLS)
#define MAX_ALIEN_BULLETS 20 // Sama seperti versi dua file

// Struktur untuk alien
typedef struct {
    int x, y;
    int active;
} Alien;

// Struktur untuk peluru alien (sama seperti versi dua file)
typedef struct {
    int x, y;
    int active;
} AlienBullet;

// Variabel global
extern int BLOCK_SIZE;
extern AlienBullet alienBullets[MAX_ALIEN_BULLETS]; // Array untuk peluru alien

// Fungsi untuk alien
void initAliens(Alien aliens[]);
void drawAliens(Alien aliens[]);
void updateAliens(Alien aliens[], int *alienDir); // Tanpa parameter Player untuk saat ini

#endif // ALIEN_H