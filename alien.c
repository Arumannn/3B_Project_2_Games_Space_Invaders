#include "alien.h"
#include "mainsprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <graphics.h>

// Definisi variabel global
int BLOCK_SIZE;
Bullet alienBullets[MAX_ALIEN_BULLETS];
Explosion alienExplosions[MAX_ALIEN_BULLETS];

void initAliens(Alien aliens[]) {
    BLOCK_SIZE = getmaxy() / 40;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            int i = row * ALIEN_COLS + col;
            aliens[i].x = (row < 2) ? (col * BLOCK_SIZE * 2 + getmaxx() / 10)
                                    : (getmaxx() - (col + 1) * BLOCK_SIZE * 2 - getmaxx() / 10);
            aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
            aliens[i].active = 1;
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
            setcolor(GREEN);
            setfillstyle(SOLID_FILL, GREEN);
            fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLOCK_SIZE / 2);

            int eyeSize = BLOCK_SIZE / 2;
            int eyeX = x + BLOCK_SIZE / 4;
            int eyeY = y + BLOCK_SIZE / 4;
            setcolor(WHITE);
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(eyeX + eyeSize / 2, eyeY + eyeSize / 2, eyeSize / 2, eyeSize / 2);

            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            bar(eyeX + eyeSize / 4, eyeY, eyeX + 3 * eyeSize / 4, eyeY + eyeSize);
        }
    }
}

void updateAliens(Alien aliens[], int *alienDirFirst, int *alienDirRest) {
    int moveDownFirst = 0, moveDownRest = 0;
    static int frameCounter = 0;
    frameCounter++;

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            alienBullets[i].y += BLOCK_SIZE / 2;
            if (alienBullets[i].y > getmaxy()) {
                alienBullets[i].active = 0;
            }
        }
    }

    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int row = i / ALIEN_COLS;
            int *dir = (row < 2) ? alienDirFirst : alienDirRest;
            aliens[i].x += *dir * BLOCK_SIZE / 2;
            if (row >= 4) aliens[i].y += (int)(sin(frameCounter * 0.1) * 5);

            if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                (row < 2) ? (moveDownFirst = 1) : (moveDownRest = 1);
            }
        }
    }

    if (moveDownFirst || moveDownRest) {
        *alienDirFirst *= (moveDownFirst) ? -1 : 1;
        *alienDirRest *= (moveDownRest) ? -1 : 1;
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active) aliens[i].y += BLOCK_SIZE * 2;
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
        }
    }
}
