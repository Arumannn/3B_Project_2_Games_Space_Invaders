#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "barrier.h"
#include "alien.h"
#include "ufo.h"
#include "mainsprite.h"
#include "gameplay.h"

extern BulletNode* playerBullets;
extern BulletNode* alienBullets;
extern BulletNode* ufoBullets;

void drawBarrier(Barrier* b) {
    if (b && b->health > 0) {
        int color = WHITE;
        if (b->health > 15 && b->health <= 30) color = GREEN;
        else if (b->health > 5 && b->health <= 15) color = YELLOW;
        else if (b->health > 1 && b->health <= 5) color = RED;

        setfillstyle(SOLID_FILL, color);

        bar(b->x, b->y - 5, b->x + 80, b->y + 25);

        bar(b->x + 5, b->y - 10, b->x + 75, b->y - 5);
        bar(b->x + 10, b->y - 15, b->x + 70, b->y - 10);
        bar(b->x + 15, b->y - 20, b->x + 65, b->y - 15);

        setfillstyle(SOLID_FILL, BLUE);
        bar(b->x + 15, b->y + 15, b->x + 65, b->y + 25);
        bar(b->x + 20, b->y + 10, b->x + 60, b->y + 20);
        bar(b->x + 25, b->y + 5, b->x + 55, b->y + 15);
    }
}

void checkBulletBarrierCollision(Bullet** bulletList, Barrier** barrierList) {
    Bullet* currentBullet = *bulletList;
    Bullet* prevBullet = NULL;

    while (currentBullet != NULL) {
        Barrier* currentBarrier = *barrierList;
        Barrier* prevBarrier = NULL;
        bool collided = false;

        while (currentBarrier != NULL && !collided) {
            if (currentBullet->x >= currentBarrier->x && currentBullet->x <= currentBarrier->x + 40 &&
                currentBullet->y >= currentBarrier->y && currentBullet->y <= currentBarrier->y + 20) {

                // Kurangi kesehatan barrier
                currentBarrier->health -= 10;
                collided = true;

                // Hapus bullet
                if (prevBullet == NULL) {
                    *bulletList = currentBullet->next;
                    free(currentBullet);
                    currentBullet = *bulletList;
                } else {
                    prevBullet->next = currentBullet->next;
                    free(currentBullet);
                    currentBullet = prevBullet->next;
                }

                // Jika health barrier habis, hapus dari linked list
                if (currentBarrier->health <= 0) {
                    if (prevBarrier == NULL) {
                        *barrierList = currentBarrier->next;
                        free(currentBarrier);
                        currentBarrier = *barrierList;
                    } else {
                        prevBarrier->next = currentBarrier->next;
                        free(currentBarrier);
                        currentBarrier = prevBarrier->next;
                    }
                }

                break;
            } else {
                prevBarrier = currentBarrier;
                currentBarrier = currentBarrier->next;
            }
        }

        if (!collided) {
            prevBullet = currentBullet;
            currentBullet = currentBullet->next;
        }
    }
}

void initBarriers(Barrier** head) {
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    
    int startX = (screenWidth / 4) - 110;
    int startY = screenHeight - 210;
    int gap = screenWidth / 5;

    Barrier* newBarrier = NULL;
    Barrier* lastBarrier = NULL;

    for (int i = 0; i < MAX_BARRIERS; i++) {
        newBarrier = (Barrier*) malloc(sizeof(Barrier));
        newBarrier->x = startX + (i * gap);
        newBarrier->y = startY;
        newBarrier->health = 30;
        newBarrier->next = NULL;

        if (*head == NULL) {
            *head = newBarrier;
        } else {
            lastBarrier->next = newBarrier;
        }
        lastBarrier = newBarrier;
    }
}

void freeBarriers(Barrier* head) {
    Barrier* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void checkAlienBulletCollision(Barrier* barrierHead) {
    checkBulletBarrierCollision(&playerBullets, &barrierHead);
    checkBulletBarrierCollision(&alienBullets, &barrierHead);
    checkBulletBarrierCollision(&ufoBullets, &barrierHead);
}