//===============================================================================================================
// Program dibuat oleh: Nazriel Ramdhani
// NIM                : 241511055
// Deskripsi File     : Semua yang berkaitan dengan alien termasuk collison, bullet dan move
// Proyek 2           : Game Space Invaders 
//===============================================================================================================

#include "alien.h"
#include "mainsprite.h"
#include "gameplay.h"
#include "mainmenu.h"
#include "barrier.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Variabel global untuk ukuran blok, ledakan alien, daftar alien, peluru alien, level, kecepatan, dan waktu tembak
int BLOCK_SIZE;
Explosion alienExplosions[ALIEN_ROWS][ALIEN_COLS];
AlienNode* alienRows[ALIEN_ROWS]; // Daftar alien per baris
BulletNode* alienBullets = NULL; // Daftar peluru alien
static int currentLevel = 1;  // Level awal permainan
static float alienSpeed = BASE_ALIEN_SPEED;  // Kecepatan gerak alien
static int shootInterval = BASE_SHOOT_INTERVAL;  // Jeda waktu tembakan alien
static int alienShootCooldown = 0;  // Waktu tunggu sebelum alien menembak lagi

// Mengatur ulang level ke awal
void initLevel() {
    currentLevel = 1;
    alienSpeed = BASE_ALIEN_SPEED;
    shootInterval = BASE_SHOOT_INTERVAL;
    alienShootCooldown = 0;
}

// Cek apakah semua alien mati, jika iya, naik level dan tingkatkan kesulitan
void checkAndUpdateLevel() {
    int allAliensDead = 1;
    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* current = alienRows[row];
        while (current != NULL) {
            if (current->alien.active) {
                allAliensDead = 0;
                break;
            }
            current = current->next;
        }
        if (!allAliensDead) break;
    }

    if (allAliensDead) {
        currentLevel++;
        if (currentLevel <= 5) { // Batasi kesulitan sampai level 5
            alienSpeed += SPEED_INCREMENT;
            shootInterval -= SHOOT_INTERVAL_DECREMENT;
            if (shootInterval < 1000) shootInterval = 1000; // Jeda tembak minimal
        }
        initAliens(); // Siapkan alien untuk level baru
        printf("Level %d - Alien Speed: %.2f, Shoot Interval: %d\n", currentLevel, alienSpeed, shootInterval);
    }
}

// Ambil kecepatan alien saat ini
float getAlienSpeed() {
    return alienSpeed;
}

// Ambil jeda tembak alien saat ini
int getShootInterval() {
    return shootInterval;
}

// Ambil level saat ini
int getCurrentLevel() {
    return currentLevel;
}

// Siapkan alien di awal permainan atau level baru
void initAliens() {
    BLOCK_SIZE = getmaxy() / 40; // Tentukan ukuran blok berdasarkan layar

    srand(time(0)); // Inisialisasi angka acak

    // Buat alien di setiap baris
    for (int row = 0; row < ALIEN_ROWS; row++) {
        alienRows[row] = NULL;
        AlienNode* tail = NULL;

        for (int col = 0; col < ALIEN_COLS; col++) {
            AlienNode* newNode = (AlienNode*)malloc(sizeof(AlienNode)); // Buat alien baru
            if (!newNode) {
                printf("Memory allocation failed!\n");
                exit(1);
            }

            newNode->alien.active = 1; // Alien hidup
            newNode->alien.node = newNode;
            newNode->next = NULL;

            // Atur posisi alien berdasarkan baris
            if (row == 0 || row == 1) {
                newNode->alien.x = col * BLOCK_SIZE * 2 + getmaxx() / 10; // Baris 0,1 dari kiri
                newNode->alien.y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
            } else if (row == 2 || row == 3) {
                newNode->alien.x = getmaxx() - (col + 1) * BLOCK_SIZE * 2 - getmaxx() / 10; // Baris 2,3 dari kanan
                newNode->alien.y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
            } else {
                newNode->alien.x = col * BLOCK_SIZE * 2 + getmaxx() / 10; // Baris 4,5 dari kiri, geser ke bawah
                newNode->alien.y = (row + 1) * BLOCK_SIZE * 2 + getmaxy() / 5;
            }

            // Tambahkan alien ke daftar
            if (alienRows[row] == NULL) {
                alienRows[row] = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

    alienBullets = NULL; // Kosongkan daftar peluru

    // Matikan semua ledakan
    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            alienExplosions[row][col].active = 0;
            alienExplosions[row][col].lifetime = 0;
        }
    }

    alienShootCooldown = 0; // Reset waktu tunggu tembak
}

// Gambar alien dan peluru di layar
void drawAliens() {
    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* current = alienRows[row];
        int col = 0;
        while (current != NULL) {
            if (current->alien.active) {
                int x = current->alien.x, y = current->alien.y;

                // Gambar alien sesuai baris
                if (row == 0 || row == 1) {
                    setcolor(LIGHTGREEN);
                    setfillstyle(SOLID_FILL, LIGHTGREEN);
                    fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLOCK_SIZE / 2);
                    // Tambah mata dan duri
                    int eyeSize = BLOCK_SIZE / 2;
                    int eyeX = x + BLOCK_SIZE / 4;
                    int eyeY = y + BLOCK_SIZE / 4;
                    setcolor(WHITE);
                    setfillstyle(SOLID_FILL, WHITE);
                    fillellipse(eyeX + eyeSize / 2, eyeY + eyeSize / 2, eyeSize / 2, eyeSize / 2);
                    int pupilSize = eyeSize / 2;
                    int pupilX = eyeX + eyeSize / 4;
                    int pupilY = eyeY + eyeSize / 4;
                    setcolor(BLACK);
                    setfillstyle(SOLID_FILL, BLACK);
                    bar(pupilX, pupilY + pupilSize / 4, pupilX + pupilSize, pupilY + pupilSize / 2);
                    bar(pupilX + pupilSize / 4, pupilY, pupilX + pupilSize / 2, pupilY + pupilSize);
                    int spikeSize = BLOCK_SIZE / 4;
                    int radius = BLOCK_SIZE / 2 + spikeSize / 2;
                    setcolor(GREEN);
                    setfillstyle(SOLID_FILL, GREEN);
                    for (int j = 0; j < 8; j++) {
                        float angle = j * (3.14159265 / 4);
                        int spikeX = x + BLOCK_SIZE / 2 + cos(angle) * radius - spikeSize / 2;
                        int spikeY = y + BLOCK_SIZE / 2 + sin(angle) * radius - spikeSize / 2;
                        bar(spikeX, spikeY, spikeX + spikeSize, spikeY + spikeSize);
                    }
                } else if (row == 2 || row == 3) {
                    setcolor(LIGHTBLUE);
                    setfillstyle(SOLID_FILL, LIGHTBLUE);
                    ellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, 0, 360, BLOCK_SIZE / 3, BLOCK_SIZE / 2);
                    // Tambah mata dan kaki
                    int eyeSize = BLOCK_SIZE / 2;
                    int eyeX = x + BLOCK_SIZE / 2;
                    int eyeY = y + BLOCK_SIZE / 2;
                    setcolor(WHITE);
                    setfillstyle(SOLID_FILL, WHITE);
                    fillellipse(eyeX, eyeY, eyeSize / 2, eyeSize / 2);
                    int pupilSize = eyeSize / 3;
                    int pupilX = eyeX;
                    int pupilY = eyeY;
                    setcolor(BLACK);
                    setfillstyle(SOLID_FILL, BLACK);
                    fillellipse(pupilX, pupilY, pupilSize, pupilSize);
                    setcolor(LIGHTBLUE);
                    line(x + BLOCK_SIZE / 4, y + BLOCK_SIZE, x + BLOCK_SIZE / 4, y + 6 * BLOCK_SIZE / 5);
                    line(x + 3 * BLOCK_SIZE / 4, y + BLOCK_SIZE, x + 3 * BLOCK_SIZE / 4, y + 6 * BLOCK_SIZE / 5);
                } else if (row == 4 || row == 5) {
                    setcolor(LIGHTRED);
                    setfillstyle(SOLID_FILL, LIGHTRED);
                    fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLOCK_SIZE / 2);
                    // Tambah mata dan antena
                    int eyeSize = BLOCK_SIZE / 2;
                    int eyeX = x + BLOCK_SIZE / 2;
                    int eyeY = y + BLOCK_SIZE / 2;
                    setcolor(WHITE);
                    setfillstyle(SOLID_FILL, WHITE);
                    fillellipse(eyeX, eyeY, eyeSize / 2, eyeSize / 2);
                    int pupilSize = eyeSize / 3;
                    int pupilX = eyeX;
                    int pupilY = eyeY;
                    setcolor(BLACK);
                    setfillstyle(SOLID_FILL, BLACK);
                    fillellipse(pupilX, pupilY, pupilSize, pupilSize);
                    setcolor(LIGHTRED);
                    line(x + BLOCK_SIZE / 3, y, x + BLOCK_SIZE / 4, y - BLOCK_SIZE / 3);
                    line(x + 2 * BLOCK_SIZE / 3, y, x + 3 * BLOCK_SIZE / 4, y - BLOCK_SIZE / 3);
                    setfillstyle(SOLID_FILL, LIGHTRED);
                    fillellipse(x + BLOCK_SIZE / 4, y - BLOCK_SIZE / 3, 2, 2);
                    fillellipse(x + 3 * BLOCK_SIZE / 4, y - BLOCK_SIZE / 3, 2, 2);
                    setcolor(LIGHTRED);
                    setfillstyle(SOLID_FILL, LIGHTRED);
                    fillellipse(x + BLOCK_SIZE / 2 - BLOCK_SIZE / 6, y + BLOCK_SIZE, 2, 2);
                    fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE, 2, 2);
                    fillellipse(x + BLOCK_SIZE / 2 + BLOCK_SIZE / 6, y + BLOCK_SIZE, 2, 2);
                }
            }
            current = current->next;
            col++;
        }
    }

    // Gambar peluru alien
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    BulletNode* currentBullet = alienBullets;
    while (currentBullet != NULL) {
        if (currentBullet->bullet.active) {
            bar(currentBullet->bullet.x, currentBullet->bullet.y, 
                currentBullet->bullet.x + BLOCK_SIZE / 2, currentBullet->bullet.y + BLOCK_SIZE);
        }
        currentBullet = currentBullet->next;
    }
}

// Cek tabrakan alien dengan penghalang
void checkAlienBarrierCollisions(Barrier* barrierList) {
    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* currentAlien = alienRows[row];
        int col = 0;
        while (currentAlien != NULL) {
            if (currentAlien->alien.active) {
                int alienLeft = currentAlien->alien.x;
                int alienRight = currentAlien->alien.x + BLOCK_SIZE;
                int alienTop = currentAlien->alien.y;
                int alienBottom = currentAlien->alien.y + BLOCK_SIZE;

                Barrier* currentBarrier = barrierList;
                while (currentBarrier != NULL) {
                    if (currentBarrier->health > 0) {
                        int barrierLeft = currentBarrier->x;
                        int barrierRight = currentBarrier->x + 80;
                        int barrierTop = currentBarrier->y - 20;
                        int barrierBottom = currentBarrier->y + 25;

                        if (alienRight > barrierLeft && alienLeft < barrierRight &&
                            alienBottom > barrierTop && alienTop < barrierBottom) {
                            currentAlien->alien.active = 0; // Matikan alien
                            currentBarrier->health--; // Kurangi nyawa penghalang
                            // Tambah ledakan
                            alienExplosions[row][col].x = currentAlien->alien.x + BLOCK_SIZE / 2;
                            alienExplosions[row][col].y = currentAlien->alien.y + BLOCK_SIZE / 2;
                            alienExplosions[row][col].active = 1;
                            alienExplosions[row][col].lifetime = 10;
                            break;
                        }
                    }
                    currentBarrier = currentBarrier->next;
                }
            }
            currentAlien = currentAlien->next;
            col++;
        }
    }
}

// Perbarui posisi alien dan peluru
void updateAliens(int *alienDirFirst, int *alienDirRest, int frameCounter) {
    int moveDownFirst = 0;
    int moveDownRest = 0;

    // Perbarui posisi peluru alien
    BulletNode* currentBullet = alienBullets;
    BulletNode* prevBullet = NULL;
    while (currentBullet != NULL) {
        if (currentBullet->bullet.active) {
            currentBullet->bullet.y += BLOCK_SIZE / 2; // Gerakkan peluru ke bawah
            if (currentBullet->bullet.y > getmaxy()) { // Hapus peluru jika keluar layar
                BulletNode* temp = currentBullet;
                if (prevBullet == NULL) {
                    alienBullets = currentBullet->next;
                } else {
                    prevBullet->next = currentBullet->next;
                }
                currentBullet = currentBullet->next;
                free(temp);
                continue;
            }
        }
        prevBullet = currentBullet;
        currentBullet = currentBullet->next;
    }

    // Kurangi waktu tunggu tembak
    if (alienShootCooldown > 0) {
        alienShootCooldown--;
    }

    float currentAlienSpeed = getAlienSpeed();
    int currentShootInterval = getShootInterval();
    int hasShot = 0;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* current = alienRows[row];
        while (current != NULL) {
            if (current->alien.active) {
                // Gerakkan alien berdasarkan arah
                if (row == 0 || row == 1 || row == 4 || row == 5) {
                    current->alien.x += *alienDirFirst * (BLOCK_SIZE * currentAlienSpeed);
                    if (current->alien.x <= 0 || current->alien.x >= getmaxx() - BLOCK_SIZE) {
                        moveDownFirst = 1; // Turun jika menyentuh tepi
                    }
                } else if (row == 2 || row == 3) {
                    current->alien.x += *alienDirRest * (BLOCK_SIZE * currentAlienSpeed);
                    if (current->alien.x <= 0 || current->alien.x >= getmaxx() - BLOCK_SIZE) {
                        moveDownRest = 1; // Turun jika menyentuh tepi
                    }
                }

                // Logika tembak alien
                if (!hasShot && alienShootCooldown <= 0 && rand() % 100 < 5) { // 5% peluang tembak
                    int bulletCount = 0;
                    BulletNode* countBullet = alienBullets;
                    while (countBullet != NULL) {
                        bulletCount++;
                        countBullet = countBullet->next;
                    }
                    if (bulletCount < MAX_ALIEN_BULLETS) { // Batasi jumlah peluru
                        BulletNode* newBullet = (BulletNode*)malloc(sizeof(BulletNode));
                        if (newBullet) {
                            newBullet->bullet.x = current->alien.x + BLOCK_SIZE / 4;
                            newBullet->bullet.y = current->alien.y + BLOCK_SIZE;
                            newBullet->bullet.active = 1;
                            newBullet->next = alienBullets;
                            alienBullets = newBullet;
                            alienShootCooldown = currentShootInterval / 100; // Atur waktu tunggu
                            hasShot = 1; // Hanya satu alien menembak
                        }
                    }
                }
            }
            current = current->next;
        }
    }

    // Cek tabrakan alien dengan penghalang
    checkAlienBarrierCollisions(barrierList);

    // Turunkan alien jika menyentuh tepi
    if (moveDownFirst) {
        *alienDirFirst *= -1;
        for (int row = 0; row < ALIEN_ROWS; row++) {
            if (row == 0 || row == 1 || row == 4 || row == 5) {
                AlienNode* current = alienRows[row];
                while (current != NULL) {
                    current->alien.y += BLOCK_SIZE * 2;
                    current = current->next;
                }
            }
        }
    }

    if (moveDownRest) {
        *alienDirRest *= -1;
        for (int row = 2; row <= 3; row++) {
            AlienNode* current = alienRows[row];
            while (current != NULL) {
                current->alien.y += BLOCK_SIZE * 2;
                current = current->next;
            }
        }
    }
}

// Cek tabrakan peluru pemain dengan alien
void checkAlienCollisions(BulletNode *bullets) {
    BulletNode *currentBullet = bullets;
    while (currentBullet != NULL) {
        if (currentBullet->bullet.active) {
            for (int row = 0; row < ALIEN_ROWS; row++) {
                AlienNode* currentAlien = alienRows[row];
                int col = 0;
                while (currentAlien != NULL) {
                    if (currentAlien->alien.active) {
                        int bulletLeft = currentBullet->bullet.x;
                        int bulletRight = currentBullet->bullet.x + BLOCK_SIZE / 2;
                        int bulletTop = currentBullet->bullet.y;
                        int bulletBottom = currentBullet->bullet.y + BLOCK_SIZE;

                        int alienLeft = currentAlien->alien.x;
                        int alienRight = currentAlien->alien.x + BLOCK_SIZE;
                        int alienTop = currentAlien->alien.y;
                        int alienBottom = currentAlien->alien.y + BLOCK_SIZE;

                        if (bulletRight > alienLeft && bulletLeft < alienRight &&
                            bulletBottom > alienTop && bulletTop < alienBottom) {
                            currentAlien->alien.active = 0; // Matikan alien
                            currentBullet->bullet.active = 0; // Matikan peluru
                            // Tambah ledakan
                            alienExplosions[row][col].x = currentAlien->alien.x + BLOCK_SIZE / 2;
                            alienExplosions[row][col].y = currentAlien->alien.y + BLOCK_SIZE / 2;
                            alienExplosions[row][col].active = 1;
                            alienExplosions[row][col].lifetime = 10;
                            addAlienScore(); // Tambah skor
                        }
                    }
                    currentAlien = currentAlien->next;
                    col++;
                }
            }
        }
        currentBullet = currentBullet->next;
    }
}

// Gambar efek ledakan alien
void drawAlienExplosions() {
    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (alienExplosions[row][col].active) {
                setcolor(YELLOW);
                setfillstyle(SOLID_FILL, YELLOW);
                fillellipse(alienExplosions[row][col].x, alienExplosions[row][col].y, BLOCK_SIZE, BLOCK_SIZE);
                setcolor(RED);
                setfillstyle(SOLID_FILL, RED);
                fillellipse(alienExplosions[row][col].x, alienExplosions[row][col].y, BLOCK_SIZE / 2, BLOCK_SIZE / 2);
                alienExplosions[row][col].lifetime--;
                if (alienExplosions[row][col].lifetime <= 0) {
                    alienExplosions[row][col].active = 0; // Matikan ledakan
                }
            }
        }
    }
}

// Cek tabrakan alien dengan pemain
void checkAlienPlayerVerticalCollision(Player *player, int *gameOver) {
    int playerTop = player->Y_Player;
    int playerBottom = player->Y_Player + 40;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* current = alienRows[row];
        while (current != NULL) {
            if (current->alien.active) {
                int alienTop = current->alien.y;
                int alienBottom = current->alien.y + BLOCK_SIZE;

                if (alienBottom >= playerTop && alienTop <= playerBottom) {
                    *gameOver = 1; // Permainan selesai
                    setvisualpage(getactivepage());
                    gameOverScreen(); // Tampilkan layar game over
                    return;
                }
            }
            current = current->next;
        }
    }
}