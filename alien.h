#ifndef ALIEN_H
#define ALIEN_H

#include <graphics.h>
#include "mainsprite.h"

// Konstanta
#define ALIEN_ROWS 5
#define ALIEN_COLS 10
#define MAX_ALIENS (ALIEN_ROWS * ALIEN_COLS)

typedef struct {
    int x, y;
    int active;
} Alien;

// Variabel global
extern int BLOCK_SIZE; // Pindah ke sini sebagai extern

void initAliens(Alien aliens[]);
void drawAliens(Alien aliens[]);
void updateAliens(Alien aliens[], int *alienDir, int *alienDirLast);
void checkAlienCollisions(Alien aliens[], Bullet bullets[], int bulletCount);
void drawAlienExplosions();

#endif // ALIEN_H