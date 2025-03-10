#ifndef ALIEN_H
#define ALIEN_H

#include <graphics.h>
#include "mainsprite.h"

// Konstanta
#define ALIEN_ROWS 5
#define ALIEN_COLS 10
#define MAX_ALIENS (ALIEN_ROWS * ALIEN_COLS)
#define MAX_ALIEN_BULLETS 10  // Ditambahkan agar tidak error

typedef struct {
    int x, y;
    int active;
} Alien;

typedef struct {
    int x, y;
    int active;
    int lifetime; // Ditambahkan karena dipakai di alien.c
} Explosion;

// Variabel global
extern Bullet alienBullets[MAX_ALIEN_BULLETS];
extern Explosion alienExplosions[MAX_ALIEN_BULLETS];
extern int BLOCK_SIZE;  // Pastikan ini didefinisikan di file .c

// Fungsi
void initAliens(Alien aliens[]);
void drawAliens(Alien aliens[]);
void updateAliens(Alien aliens[], int *alienDirFirst, int *alienDirRest);
void checkAlienCollisions(Alien aliens[], Bullet bullets[], int bulletCount);
void drawAlienExplosions();

#endif // ALIEN_H
