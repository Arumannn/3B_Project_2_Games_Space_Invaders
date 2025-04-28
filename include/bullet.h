#ifndef BULLET_H
#define BULLET_H

#include <graphics.h>


#define MAX_BULLETS 3
#define MAX_ALIEN_BULLETS 20
#define MAX_UFO_BULLETS 10

#define PLAYER_BULLET 1
#define ALIEN_BULLET 2
#define UFO_BULLET 3

extern int BLOCK_SIZE;
#define PLAYER_BULLET_WIDTH (BLOCK_SIZE / 4)
#define ALIEN_BULLET_WIDTH (BLOCK_SIZE / 2)
#define UFO_BULLET_RADIUS 6


// Struktur data peluru
typedef struct {
    int x, y;        // Posisi
    int active;      // Status aktif (0 = tidak aktif, 1 = aktif)
    int speed;       // Kecepatan (default untuk pemain/alien, acak untuk UFO)
    int dx;          // Arah horizontal (khususnya untuk UFO)
    int type;        // Tipe peluru (PLAYER_BULLET, ALIEN_BULLET, UFO_BULLET)
} Bullet;

// Variabel global
extern Bullet playerBullets[MAX_BULLETS];
extern Bullet alienBullets[MAX_ALIEN_BULLETS];
extern Bullet ufoBullets[MAX_UFO_BULLETS];
extern int shootCooldown;

// Deklarasi fungsi
void initAllBullets();
void updateAllBullets();
void drawAllBullets();
void shootBullet(Bullet bullets[], int maxBullets, int x, int y, int type, 
int *cooldown, int *burstCount, int maxBurst, int burstCooldown, int postBurstCooldown);

#endif