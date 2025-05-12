#ifndef ALIEN_H
#define ALIEN_H
#include <graphics.h>
#include "mainsprite.h"
#include "gameplay.h"

// Constants
#define ALIEN_ROWS 6
#define ALIEN_COLS 10
#define MAX_ALIEN_BULLETS 20
#define BASE_ALIEN_SPEED 0.5  // Kecepatan dasar pergerakan alien (BLOCK_SIZE per frame)
#define BASE_SHOOT_INTERVAL 5000  // Interval dasar untuk tembakan alien (semakin kecil, semakin cepat)
#define SPEED_INCREMENT 0.1  // Peningkatan kecepatan per level
#define SHOOT_INTERVAL_DECREMENT 500  // Pengurangan interval tembakan per level

// Structure for an alien
typedef struct Alien {
    int x, y;
    int active;
    struct AlienNode* node; // Pointer to the node for easier management
} Alien;

// Structure for a node in the linked list
typedef struct AlienNode {
    Alien alien;
    struct AlienNode* next;
} AlienNode;

// Structure for explosions
typedef struct {
    int x, y;
    int active;
    int lifetime;
} Explosion;

extern int BLOCK_SIZE;
extern Explosion alienExplosions[ALIEN_ROWS][ALIEN_COLS]; 
extern AlienNode* alienRows[ALIEN_ROWS]; // Array of linked lists for aliens
extern BulletNode* alienBullets; // Linked list for alien bullets

void initAliens();
void drawAliens();
void updateAliens(int *alienDirFirst, int *alienDirRest, int frameCounter);
void checkAlienCollisions(BulletNode *bullets);
void drawAlienExplosions();
void checkAlienPlayerVerticalCollision(Player *player);

// Fungsi untuk menginisialisasi level
void initLevel();

// Fungsi untuk memeriksa apakah semua alien habis dan menaikkan level
void checkAndUpdateLevel(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);

// Fungsi untuk mendapatkan kecepatan alien berdasarkan level
float getAlienSpeed();

// Fungsi untuk mendapatkan interval tembakan alien berdasarkan level
int getShootInterval();

// Fungsi untuk mendapatkan level saat ini
int getCurrentLevel();

#endif // ALIEN_H