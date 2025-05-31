#ifndef ALIEN_H
#define ALIEN_H
#include <graphics.h>
#include "mainsprite.h"
#include "barrier.h" 

// Constants
#define ALIEN_ROWS 6
#define ALIEN_COLS 10
#define MAX_ALIEN_BULLETS 20
#define BASE_ALIEN_SPEED 0.5  // Kecepatan dasar pergerakan alien (BLOCK_SIZE per frame)
#define BASE_SHOOT_INTERVAL 5000  // Interval dasar untuk tembakan alien (semakin kecil, semakin cepat)
#define SPEED_INCREMENT 0.1  // Peningkatan kecepatan per level
#define SHOOT_INTERVAL_DECREMENT 800  // Pengurangan interval tembakan per level

// struct untuk wadah alien
typedef struct Alien {
    int x, y;
    int active;
    struct AlienNode* node; // Pointer ke node alien
} Alien;

// struct untuk node alien pada linked list
typedef struct AlienNode {
    Alien alien;
    struct AlienNode* next;
} AlienNode;

// struct untuk ledakan
typedef struct {
    int x, y;
    int active;
    int lifetime;
} Explosion;

extern int BLOCK_SIZE;
extern Explosion alienExplosions[ALIEN_ROWS][ALIEN_COLS]; 
extern AlienNode* alienRows[ALIEN_ROWS]; // Array of linked lists untuk aliens
extern BulletNode* alienBullets; // Linked list untuk peluru alien 
extern Barrier* barrierList; 

void initAliens();
void drawAliens();
void updateAliens(int *alienDirFirst, int *alienDirRest, int frameCounter);
void checkAlienCollisions(BulletNode *bullets);
void drawAlienExplosions();
void checkAlienPlayerVerticalCollision(Player *player, int *gameOver);
void initLevel();
void checkAndUpdateLevel();
float getAlienSpeed();
int getShootInterval();
int getCurrentLevel();
int isUFOActive();
int isBonusUFOActive();

#endif // ALIEN_H