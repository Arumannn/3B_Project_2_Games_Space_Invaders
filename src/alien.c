#include "alien.h"
#include "mainsprite.h"
#include "score.h"
#include "level.h"
#include "gameplay.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definisi variabel global
int BLOCK_SIZE;
AlienBullet alienBullets[MAX_ALIEN_BULLETS];
Explosion alienExplosions[ALIEN_ROWS][ALIEN_COLS];


void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]) {
    BLOCK_SIZE = getmaxy() / 40;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (row == 0 || row == 1) {
                // Baris 0 dan 1: Spawn dari kiri
                aliens[row][col].x = col * BLOCK_SIZE * 2 + getmaxx() / 10;
                aliens[row][col].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
                aliens[row][col].active = 1;
            } else {
                // Baris 2-5: Spawn dari kanan
                aliens[row][col].x = getmaxx() - (col + 1) * BLOCK_SIZE * 2 - getmaxx() / 10;
                aliens[row][col].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
                aliens[row][col].active = 1;
            }
        }
    }

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        alienBullets[i].active = 0;
    }

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            alienExplosions[row][col].active = 0;
            alienExplosions[row][col].lifetime = 0;
        }
    }

    srand(time(0));
}

void drawAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]) {
    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (aliens[row][col].active) {
                int x = aliens[row][col].x, y = aliens[row][col].y;

                if (row == 0 || row == 1) {
                    // Desain alien baris 0 dan 1 (seperti asli)
                    setcolor(LIGHTGREEN);
                    setfillstyle(SOLID_FILL, LIGHTGREEN);
                    fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLOCK_SIZE / 2);
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
                }
                else if (row == 2 || row == 3) {
                    // Desain alien baris 2 dan 3 (seperti asli)
                    setcolor(LIGHTBLUE);
                    setfillstyle(SOLID_FILL, LIGHTBLUE);
                    ellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, 0, 360, BLOCK_SIZE / 3, BLOCK_SIZE / 2);
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
                }
                else if (row == 4 || row == 5) {
                    // Desain alien baris 4 dan 5 (seperti asli)
                    setcolor(LIGHTRED);
                    setfillstyle(SOLID_FILL, LIGHTRED);
                    fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLOCK_SIZE / 2);
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
        }
    }

    // Gambar peluru alien (tidak berubah)
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            bar(alienBullets[i].x, alienBullets[i].y, alienBullets[i].x + BLOCK_SIZE / 2, alienBullets[i].y + BLOCK_SIZE);
        }
    }
}

void updateAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS], int *alienDirFirst, int *alienDirRest, int frameCounter) {
    int moveDownFirst = 0;
    int moveDownRest = 0;

    // Update pergerakan peluru alien
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            alienBullets[i].y += BLOCK_SIZE / 2;
            if (alienBullets[i].y > getmaxy()) {
                alienBullets[i].active = 0;
            }
        }
    }

    // Dapatkan kecepatan dan interval tembakan berdasarkan level
    float currentAlienSpeed = getAlienSpeed();
    int currentShootInterval = getShootInterval();

    // Hitung posisi Y awal untuk baris 4 (sebagai batas atas pergerakan baris 4 dan 5)
    int baseYRow4 = 4 * BLOCK_SIZE * 2 + getmaxy() / 5;  // Posisi Y awal baris 4
    int minY = baseYRow4 - BLOCK_SIZE;  // Batas atas (sedikit di atas posisi awal baris 4)

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (aliens[row][col].active) {
                if (row == 0 || row == 1) {
                    aliens[row][col].x += *alienDirFirst * (BLOCK_SIZE * currentAlienSpeed);
                    if (aliens[row][col].x <= 0 || aliens[row][col].x >= getmaxx() - BLOCK_SIZE) {
                        moveDownFirst = 1;
                    }
                } else if (row == 2 || row == 3) {
                    aliens[row][col].x += *alienDirRest * (BLOCK_SIZE * currentAlienSpeed);
                    if (aliens[row][col].x <= 0 || aliens[row][col].x >= getmaxx() - BLOCK_SIZE) {
                        moveDownRest = 1;
                    }
                } else if (row == 4 || row == 5) {
                    aliens[row][col].x += *alienDirRest * (BLOCK_SIZE * currentAlienSpeed);
                    // Pergerakan sinusoidal ke atas dan bawah
                    int originalY = row * BLOCK_SIZE * 2 + getmaxy() / 5;  // Posisi Y awal alien
                    int newY = aliens[row][col].y + (int)(sin(frameCounter * 0.1) * 3);
                    // Batasi pergerakan ke atas agar tidak melewati batas minimum
                    if (newY < minY) {
                        newY = minY;
                    }
                    aliens[row][col].y = newY;
                    if (aliens[row][col].x <= 0 || aliens[row][col].x >= getmaxx() - BLOCK_SIZE) {
                        moveDownRest = 1;
                    }
                }

                // Logika tembakan alien dengan interval berdasarkan level
                if (rand() % currentShootInterval < 10) {
                    for (int j = 0; j < MAX_ALIEN_BULLETS; j++) {
                        if (!alienBullets[j].active) {
                            alienBullets[j].x = aliens[row][col].x + BLOCK_SIZE / 4;
                            alienBullets[j].y = aliens[row][col].y + BLOCK_SIZE;
                            alienBullets[j].active = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (moveDownFirst) {
        *alienDirFirst *= -1;
        for (int row = 0; row < 2; row++) {
            for (int col = 0; col < ALIEN_COLS; col++) {
                aliens[row][col].y += BLOCK_SIZE * 2;
            }
        }
    }

    if (moveDownRest) {
        *alienDirRest *= -1;
        for (int row = 2; row < ALIEN_ROWS; row++) {
            for (int col = 0; col < ALIEN_COLS; col++) {
                aliens[row][col].y += BLOCK_SIZE * 2;
            }
        }
    }
}

void checkAlienCollisions(Alien aliens[ALIEN_ROWS][ALIEN_COLS], Bullet bullets[], int bulletCount) {
    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (aliens[row][col].active) {
                for (int j = 0; j < bulletCount; j++) {
                    if (bullets[j].active) {
                        int alienLeft = aliens[row][col].x;
                        int alienRight = aliens[row][col].x + BLOCK_SIZE;
                        int alienTop = aliens[row][col].y;
                        int alienBottom = aliens[row][col].y + BLOCK_SIZE;

                        int bulletLeft = bullets[j].x - BLOCK_SIZE / 4;
                        int bulletRight = bullets[j].x + BLOCK_SIZE / 4;
                        int bulletTop = bullets[j].y;
                        int bulletBottom = bullets[j].y + BLOCK_SIZE;

                        if (bulletRight > alienLeft && bulletLeft < alienRight &&
                            bulletBottom > alienTop && bulletTop < alienBottom) {
                            aliens[row][col].active = 0;
                            bullets[j].active = 0;
                            alienExplosions[row][col].x = aliens[row][col].x + BLOCK_SIZE / 2;
                            alienExplosions[row][col].y = aliens[row][col].y + BLOCK_SIZE / 2;
                            alienExplosions[row][col].active = 1;
                            alienExplosions[row][col].lifetime = 5;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (alienExplosions[row][col].active) {
                alienExplosions[row][col].lifetime--;
                if (alienExplosions[row][col].lifetime <= 0) {
                    alienExplosions[row][col].active = 0;
                }
            }
        }
    }
}

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
                addAlienScore();
            }
        }
    }
}

void checkAlienPlayerVerticalCollision(Alien aliens[ALIEN_ROWS][ALIEN_COLS], Player *player) {
    // Posisi vertikal sprite pemain
    int playerTop = player->Y_Player;
    int playerBottom = player->Y_Player + 40; // Tinggi sprite pemain (sesuai DrawSpaceShip)

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            if (aliens[row][col].active) {
                // Posisi vertikal alien
                int alienTop = aliens[row][col].y;
                int alienBottom = aliens[row][col].y + BLOCK_SIZE; // Tinggi alien

                // Periksa apakah ada tumpang tindih vertikal
                if (alienBottom >= playerTop && alienTop <= playerBottom) {
                    // Alien sejajar dengan sprite pemain, game over
                    gameOverScreen();
                    return; // Keluar dari fungsi dan akhiri permainan
                }
            }
        }
    }
}