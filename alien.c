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

void initAliens(Alien aliens[]) {
    BLOCK_SIZE = getmaxy() / 40;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            int i = row * ALIEN_COLS + col;
            aliens[i].x = col * BLOCK_SIZE * 2 + getmaxx() / 10;
            aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
            aliens[i].active = 1;
        }
    }

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        alienBullets[i].active = 0;
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
                bar(pupilX, pupilY + pupilSize / 4, pupilX + pupilSize, pupilY + pupilSize / 4 + pupilSize / 4);
                bar(pupilX + pupilSize / 4, pupilY, pupilX + pupilSize / 4 + pupilSize / 4, pupilY + pupilSize);
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

void updateAliens(Alien aliens[], int *alienDir) {
    int moveDown = 0;

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
            aliens[i].x += *alienDir * BLOCK_SIZE / 2;

            if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                moveDown = 1;
            }

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

    if (moveDown) {
        *alienDir *= -1;
        for (int i = 0; i < MAX_ALIENS; i++) {
            aliens[i].y += BLOCK_SIZE * 2;
        }
    }
}