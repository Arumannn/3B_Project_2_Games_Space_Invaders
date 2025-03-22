#include "level.h"
#include <stdio.h>

static int currentLevel = 1;  // Level awal
static float alienSpeed = BASE_ALIEN_SPEED;  // Kecepatan awal alien
static int shootInterval = BASE_SHOOT_INTERVAL;  // Interval tembakan awal

void initLevel() {
    currentLevel = 1;
    alienSpeed = BASE_ALIEN_SPEED;
    shootInterval = BASE_SHOOT_INTERVAL;
}

void checkAndUpdateLevel(Alien aliens[ALIEN_ROWS][ALIEN_COLS]) {
    // Periksa apakah semua alien habis
    int allAliensDead = 1;
    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (aliens[row][col].active) {
                allAliensDead = 0;
                break;
            }
        }
        if (!allAliensDead) break;
    }

    // Jika semua alien habis, naikkan level dan respawn
    if (allAliensDead) {
        currentLevel++;
        alienSpeed += SPEED_INCREMENT;  // Tingkatkan kecepatan pergerakan
        shootInterval -= SHOOT_INTERVAL_DECREMENT;  // Kurangi interval tembakan (lebih cepat)
        if (shootInterval < 1000) shootInterval = 1000;  // Batasi agar tidak terlalu cepat

        // Respawn alien dengan memanggil initAliens
        initAliens(aliens);

        printf("Level %d - Alien Speed: %.2f, Shoot Interval: %d\n", currentLevel, alienSpeed, shootInterval);
    }
}

float getAlienSpeed() {
    return alienSpeed;
}

int getShootInterval() {
    return shootInterval;
}

int getCurrentLevel() {
    return currentLevel;
}