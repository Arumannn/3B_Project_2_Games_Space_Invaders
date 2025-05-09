#include "alien.h"
#include "mainsprite.h"
#include "gameplay.h"
#include "mainmenu.h"
#include "malloc.h"
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Global variables
int BLOCK_SIZE;
Explosion alienExplosions[ALIEN_ROWS][ALIEN_COLS];
AlienNode* alienRows[ALIEN_ROWS]; // Array of linked lists for each row
BulletNode* alienBullets = NULL; // Linked list for alien bullets

void initAliens() {
    BLOCK_SIZE = getmaxy() / 40;

    // Initialize random seed
    srand(time(0));

    // Initialize alien rows
    for (int row = 0; row < ALIEN_ROWS; row++) {
        alienRows[row] = NULL; // Start with empty list
        AlienNode* tail = NULL;

        for (int col = 0; col < ALIEN_COLS; col++) {
            // Allocate new node
            AlienNode* newNode = (AlienNode*)malloc(sizeof(AlienNode));
            if (!newNode) {
                printf("Memory allocation failed!\n");
                exit(1);
            }

            // Initialize alien data
            newNode->alien.active = 1;
            newNode->alien.node = newNode; // Store pointer to node for easy access
            newNode->next = NULL;

            if (row == 0 || row == 1) {
                // Rows 0 and 1: Spawn from left
                newNode->alien.x = col * BLOCK_SIZE * 2 + getmaxx() / 10;
                newNode->alien.y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
            } else {
                // Rows 2-5: Spawn from right
                newNode->alien.x = getmaxx() - (col + 1) * BLOCK_SIZE * 2 - getmaxx() / 10;
                newNode->alien.y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
            }

            // Add to linked list
            if (alienRows[row] == NULL) {
                alienRows[row] = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

    // Initialize alien bullets
    alienBullets = NULL;

    // Initialize explosions
    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            alienExplosions[row][col].active = 0;
            alienExplosions[row][col].lifetime = 0;
        }
    }
}

void drawAliens() {
    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* current = alienRows[row];
        int col = 0; // Track column for explosion array
        while (current != NULL) {
            if (current->alien.active) {
                int x = current->alien.x, y = current->alien.y;

                if (row == 0 || row == 1) {
                    // Design for rows 0 and 1
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
                } else if (row == 2 || row == 3) {
                    // Design for rows 2 and 3
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
                } else if (row == 4 || row == 5) {
                    // Design for rows 4 and 5
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
            current = current->next;
            col++;
        }
    }

    // Draw alien bullets
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

void updateAliens(int *alienDirFirst, int *alienDirRest, int frameCounter) {
    int moveDownFirst = 0;
    int moveDownRest = 0;

    // Update alien bullets
    BulletNode* currentBullet = alienBullets;
    BulletNode* prevBullet = NULL;
    while (currentBullet != NULL) {
        if (currentBullet->bullet.active) {
            currentBullet->bullet.y += BLOCK_SIZE / 2;
            if (currentBullet->bullet.y > getmaxy()) {
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

    // Get speed and shoot interval based on level
    float currentAlienSpeed = getAlienSpeed();
    int currentShootInterval = getShootInterval();

    // Calculate initial Y position for row 4 (upper bound for rows 4 and 5)
    int baseYRow4 = 4 * BLOCK_SIZE * 2 + getmaxy() / 5;
    int minY = baseYRow4 - BLOCK_SIZE;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* current = alienRows[row];
        while (current != NULL) {
            if (current->alien.active) {
                if (row == 0 || row == 1) {
                    current->alien.x += *alienDirFirst * (BLOCK_SIZE * currentAlienSpeed);
                    if (current->alien.x <= 0 || current->alien.x >= getmaxx() - BLOCK_SIZE) {
                        moveDownFirst = 1;
                    }
                } else if (row == 2 || row == 3) {
                    current->alien.x += *alienDirRest * (BLOCK_SIZE * currentAlienSpeed);
                    if (current->alien.x <= 0 || current->alien.x >= getmaxx() - BLOCK_SIZE) {
                        moveDownRest = 1;
                    }
                } else if (row == 4 || row == 5) {
                    current->alien.x += *alienDirRest * (BLOCK_SIZE * currentAlienSpeed);
                    // Sinusoidal movement
                    int originalY = row * BLOCK_SIZE * 2 + getmaxy() / 5;
                    int newY = current->alien.y + (int)(sin(frameCounter * 0.1) * 3);
                    if (newY < minY) {
                        newY = minY;
                    }
                    current->alien.y = newY;
                    if (current->alien.x <= 0 || current->alien.x >= getmaxx() - BLOCK_SIZE) {
                        moveDownRest = 1;
                    }
                }

                // Alien shooting logic
                if (rand() % currentShootInterval < 10) {
                    // Count active bullets
                    int bulletCount = 0;
                    BulletNode* countBullet = alienBullets;
                    while (countBullet != NULL) {
                        bulletCount++;
                        countBullet = countBullet->next;
                    }
                    if (bulletCount < MAX_ALIEN_BULLETS) {
                        BulletNode* newBullet = (BulletNode*)malloc(sizeof(BulletNode));
                        if (newBullet) {
                            newBullet->bullet.x = current->alien.x + BLOCK_SIZE / 4;
                            newBullet->bullet.y = current->alien.y + BLOCK_SIZE;
                            newBullet->bullet.active = 1;
                            newBullet->next = alienBullets;
                            alienBullets = newBullet;
                        }
                    }
                }
            }
            current = current->next;
        }
    }

    if (moveDownFirst) {
        *alienDirFirst *= -1;
        for (int row = 0; row < 2; row++) {
            AlienNode* current = alienRows[row];
            while (current != NULL) {
                current->alien.y += BLOCK_SIZE * 2;
                current = current->next;
            }
        }
    }

    if (moveDownRest) {
        *alienDirRest *= -1;
        for (int row = 2; row < ALIEN_ROWS; row++) {
            AlienNode* current = alienRows[row];
            while (current != NULL) {
                current->alien.y += BLOCK_SIZE * 2;
                current = current->next;
            }
        }
    }
}

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
                            currentAlien->alien.active = 0;
                            currentBullet->bullet.active = 0;
                            // Set explosion
                            alienExplosions[row][col].x = currentAlien->alien.x + BLOCK_SIZE / 2;
                            alienExplosions[row][col].y = currentAlien->alien.y + BLOCK_SIZE / 2;
                            alienExplosions[row][col].active = 1;
                            alienExplosions[row][col].lifetime = 10;
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
                    alienExplosions[row][col].active = 0;
                }
            }
        }
    }
}

void checkAlienPlayerVerticalCollision(Player *player) {
    int playerTop = player->Y_Player;
    int playerBottom = player->Y_Player + 40;

    for (int row = 0; row < ALIEN_ROWS; row++) {
        AlienNode* current = alienRows[row];
        while (current != NULL) {
            if (current->alien.active) {
                int alienTop = current->alien.y;
                int alienBottom = current->alien.y + BLOCK_SIZE;

                if (alienBottom >= playerTop && alienTop <= playerBottom) {
                    gameOverScreen();
                    return;
                }
            }
            current = current->next;
        }
    }
}