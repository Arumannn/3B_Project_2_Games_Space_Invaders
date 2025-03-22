#ifndef ALIEN_H
#define ALIEN_H
#include <graphics.h>
#include "mainsprite.h"

// Konstanta
#define ALIEN_ROWS 6
#define ALIEN_COLS 10
#define MAX_ALIEN_BULLETS 20

typedef struct {
    int x, y;
    int active;
} Alien;

typedef struct {
    int x, y;
    int active;
    int speed;     
    int dx;        
} AlienBullet;

// Struktur dan array untuk ledakan
typedef struct {
    int x, y;
    int active;
    int lifetime;
} Explosion;

extern int BLOCK_SIZE;
extern AlienBullet alienBullets[MAX_ALIEN_BULLETS];
extern Explosion alienExplosions[ALIEN_ROWS][ALIEN_COLS]; 

void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);
void drawAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);
void updateAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS], int *alienDir, int *alienDirLast, int frameCounter); // Tambahkan parameter frameCounter
void checkAlienCollisions(Alien aliens[ALIEN_ROWS][ALIEN_COLS], Bullet bullets[], int bulletCount);
void drawAlienExplosions();

#endif // ALIEN_H