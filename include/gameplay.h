#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#define BASE_ALIEN_SPEED 0.5  // Kecepatan dasar pergerakan alien (BLOCK_SIZE per frame)
#define BASE_SHOOT_INTERVAL 5000  // Interval dasar untuk tembakan alien (semakin kecil, semakin cepat)
#define SPEED_INCREMENT 0.1  // Peningkatan kecepatan per level
#define SHOOT_INTERVAL_DECREMENT 500  // Pengurangan interval tembakan per level


typedef struct {
    int x, y;        // Posisi
    int active;      // Status aktif (0 = tidak aktif, 1 = aktif)
    int speed;       // Kecepatan (default untuk pemain/alien, acak untuk UFO)
    int dx;          // Arah horizontal (khususnya untuk UFO)
    int type;        // Tipe peluru (PLAYER_BULLET, ALIEN_BULLET, UFO_BULLET)
} Bullet;

typedef struct BulletNode {
    Bullet bullet;
    struct BulletNode *next;
} BulletNode;


void initScore();
void updateScore(int points);
void drawScore();
int getScore();
void addAlienScore();
void addUFOScore();
void drawLevel(); 


// Fungsi untuk mendapatkan kecepatan alien berdasarkan level
float getAlienSpeed();

// Fungsi untuk mendapatkan interval tembakan alien berdasarkan level
int getShootInterval();

// Fungsi untuk mendapatkan level saat ini
int getCurrentLevel();

#endif
