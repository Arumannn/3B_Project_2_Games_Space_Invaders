#ifndef ALIEN_H
#define ALIEN_H
#include <graphics.h>
#include "mainsprite.h"
// Konstanta
#define ALIEN_ROWS 6
#define ALIEN_COLS 10
#define MAX_ALIENS (ALIEN_ROWS * ALIEN_COLS)
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
    int lifetime; // Tambahkan penghitung frame
} Explosion;

extern Explosion alienExplosions[MAX_ALIENS]; // Static agar hanya digunakan di alien.c

extern int BLOCK_SIZE;
extern AlienBullet alienBullets[MAX_ALIEN_BULLETS];
void initAliens(Alien aliens[]);
void drawAliens(Alien aliens[]);
void updateAliens(Alien aliens[], int *alienDir, int *alienDirLast);
void checkAlienCollisions(Alien aliens[], Bullet bullets[], int bulletCount);
void drawAlienExplosions();
#endif // ALIEN_H