#ifndef GAMEOVER_H
#define GAMEOVER_H
#define BASE_ALIEN_SPEED 0.5  // Kecepatan dasar pergerakan alien (BLOCK_SIZE per frame)
#define BASE_SHOOT_INTERVAL 5000  // Interval dasar untuk tembakan alien (semakin kecil, semakin cepat)
#define SPEED_INCREMENT 0.1  // Peningkatan kecepatan per level
#define SHOOT_INTERVAL_DECREMENT 500  // Pengurangan interval tembakan per level

#include "alien.h"
#include "bullet.h"

void initScore();
void updateScore(int points);
void drawScore();
int getScore();
void addAlienScore();
void addUFOScore();
void drawLevel(); 

void drawStars();
void gameOverScreen();
void savePlayerScore(const char *name, int score);
void createCustomBackground();
void drawCustomBackground();
void startGame();

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

#endif
