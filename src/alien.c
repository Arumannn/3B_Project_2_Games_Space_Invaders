#include "alien.h"
#include "mainsprite.h"
#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definisi variabel global
int BLOCK_SIZE;
AlienBullet alienBullets[MAX_ALIEN_BULLETS];
Explosion alienExplosions[MAX_ALIENS]; 



void initAliens(Alien aliens[]) {
    BLOCK_SIZE = getmaxy() / 40;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            int i = row * ALIEN_COLS + col;
            if (row == 0 || row == 1) {
                // Baris 0 dan 1: Spawn dari kiri
                aliens[i].x = col * BLOCK_SIZE * 2 + getmaxx() / 10;
                aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
                aliens[i].active = 1;
            } else {
                // Baris 2-5: Spawn dari kanan
                aliens[i].x = getmaxx() - (col + 1) * BLOCK_SIZE * 2 - getmaxx() / 10;
                aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
                aliens[i].active = 1;
            }
        }
    }

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        alienBullets[i].active = 0;
    }

    for (int i = 0; i < MAX_ALIENS; i++) {
        alienExplosions[i].active = 0;
        alienExplosions[i].lifetime = 0;
    }

    srand(time(0));
}

void drawAliens(Alien aliens[]) {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int x = aliens[i].x, y = aliens[i].y;
            int row = i / ALIEN_COLS;

            if (row == 0 || row == 1) {
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

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            bar(alienBullets[i].x, alienBullets[i].y, alienBullets[i].x + BLOCK_SIZE / 2, alienBullets[i].y + BLOCK_SIZE);
        }
    }
}

void updateAliens(Alien aliens[], int *alienDirFirst, int *alienDirRest) {
    int moveDownFirst = 0; // Untuk baris 0-1
    int moveDownRest = 0;  // Untuk baris 2-5
    static int frameCounter = 0;
    frameCounter++;

    // Update peluru alien
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            alienBullets[i].y += BLOCK_SIZE / 2;
            if (alienBullets[i].y > getmaxy()) {
                alienBullets[i].active = 0;
            }
        }
    }

    // Update pergerakan alien berdasarkan baris
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int row = i / ALIEN_COLS;

            if (row == 0 || row == 1) {
                // Baris 0 dan 1: Bergerak ke kanan dari kiri
                aliens[i].x += *alienDirFirst * BLOCK_SIZE / 2;
                if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                    moveDownFirst = 1;
                }
            }
            else if (row == 2 || row == 3) {
                // Baris 2 dan 3: Bergerak ke kiri dari kanan
                aliens[i].x += *alienDirRest * BLOCK_SIZE / 2;
                if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                    moveDownRest = 1;
                }
            }
            else if (row == 4 || row == 5) {
                // Baris 4 dan 5: Bergerak ke kiri dari kanan dengan gelombang
                aliens[i].x += *alienDirRest * BLOCK_SIZE / 2;
                aliens[i].y += (int)(sin(frameCounter * 0.1) * 5); // Gelombang kecil
                if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                    moveDownRest = 1;
                }
            }

            // Logika alien menembak
            if (rand() % 5000 < 10) {
                for (int j = 0; j < MAX_ALIEN_BULLETS; j++) {
                    if (!alienBullets[j].active) {
                        alienBullets[j].x = aliens[i].x + BLOCK_SIZE / 4;
                        alienBullets[j].y = aliens[i].y + BLOCK_SIZE;
                        alienBullets[j].active = 1;
                        break;
                    }
                }
            }
        }
    }

    // Turun untuk baris 0-1
    if (moveDownFirst) {
        *alienDirFirst *= -1;
        for (int i = 0; i < MAX_ALIENS; i++) {
            int row = i / ALIEN_COLS;
            if (row == 0 || row == 1) {
                aliens[i].y += BLOCK_SIZE * 2;
            }
        }
    }

    // Turun untuk baris 2-5
    if (moveDownRest) {
        *alienDirRest *= -1;
        for (int i = 0; i < MAX_ALIENS; i++) {
            int row = i / ALIEN_COLS;
            if (row >= 2) {
                aliens[i].y += BLOCK_SIZE * 2;
            }
        }
    }
}

void checkAlienCollisions(Alien aliens[], Bullet bullets[], int bulletCount) {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            for (int j = 0; j < bulletCount; j++) {
                if (bullets[j].active) {
                    int alienLeft = aliens[i].x;
                    int alienRight = aliens[i].x + BLOCK_SIZE;
                    int alienTop = aliens[i].y;
                    int alienBottom = aliens[i].y + BLOCK_SIZE;

                    int bulletLeft = bullets[j].x - BLOCK_SIZE / 4;
                    int bulletRight = bullets[j].x + BLOCK_SIZE / 4;
                    int bulletTop = bullets[j].y;
                    int bulletBottom = bullets[j].y + BLOCK_SIZE;

                    if (bulletRight > alienLeft && bulletLeft < alienRight &&
                        bulletBottom > alienTop && bulletTop < alienBottom) {
                        for (int k = 0; k < MAX_ALIENS; k++) {
                            if (!alienExplosions[k].active) {
                                alienExplosions[k].x = aliens[i].x + BLOCK_SIZE / 2;
                                alienExplosions[k].y = aliens[i].y + BLOCK_SIZE / 2;
                                alienExplosions[k].active = 1;
                                alienExplosions[k].lifetime = 5;
                                break;
                            }
                        }
                        aliens[i].active = 0;
                        bullets[j].active = 0;
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_ALIENS; i++) {
        if (alienExplosions[i].active) {
            alienExplosions[i].lifetime--;
            if (alienExplosions[i].lifetime <= 0) {
                alienExplosions[i].active = 0;
            }
        }
    }
}

void drawAlienExplosions() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (alienExplosions[i].active) {
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            fillellipse(alienExplosions[i].x, alienExplosions[i].y, BLOCK_SIZE, BLOCK_SIZE);
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            fillellipse(alienExplosions[i].x, alienExplosions[i].y, BLOCK_SIZE / 2, BLOCK_SIZE / 2);
            addAlienScore();
        }
    }
}