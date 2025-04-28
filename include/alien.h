#ifndef ALIEN_H
#define ALIEN_H
#include <graphics.h>
#include "mainsprite.h"
#include "bullet.h"

// Konstanta
#define ALIEN_ROWS 6
#define ALIEN_COLS 10
#define MAX_ALIEN_BULLETS 20

typedef struct {
    int x, y;
    int active;
} Alien;


// Struktur dan array untuk ledakan
typedef struct {
    int x, y;
    int active;
    int lifetime;
} Explosion;

extern int BLOCK_SIZE;
extern Explosion alienExplosions[ALIEN_ROWS][ALIEN_COLS]; 

void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);
void drawAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);
void updateAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS], int *alienDir, int *alienDirLast, int frameCounter);
void checkAlienCollisions(Alien aliens[ALIEN_ROWS][ALIEN_COLS], Bullet bullets[], int bulletCount);
void drawAlienExplosions();
void checkAlienPlayerVerticalCollision(Alien aliens[ALIEN_ROWS][ALIEN_COLS], Player *player); // Fungsi baru

#endif // ALIEN_H