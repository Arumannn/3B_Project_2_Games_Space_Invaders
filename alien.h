#ifndef ALIEN_H
#define ALIEN_H

#include <graphics.h>

// Konstanta
#define ALIEN_ROWS 5
#define ALIEN_COLS 10
#define MAX_ALIENS (ALIEN_ROWS * ALIEN_COLS)

// Struktur untuk alien
typedef struct {
    int x, y;
    int active;
} Alien;

// Variabel global
extern int BLOCK_SIZE; // Pindah ke sini sebagai extern

// Fungsi untuk alien
void initAliens(Alien aliens[]);
void drawAliens(Alien aliens[]);
void updateAliens(Alien aliens[], int *alienDir);

#endif // ALIEN_H