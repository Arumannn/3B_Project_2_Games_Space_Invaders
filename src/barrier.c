#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "barrier.h"
#include "alien.h"
#include "ufo.h"
#include "mainsprite.h"
#include "gameover.h"


extern BulletNode* playerBullets;
extern BulletNode* alienBullets;
extern BulletNode* ufoBulletList;

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

void checkBarrierBulletCollision(Barrier* head) {
    Barrier* currentBarrier;
    BulletNode* bullet;

    bullet = playerBullets;
    while (bullet != NULL) {
        if (bullet->bullet.active) {
            int bulletLeft = bullet->bullet.x;
            int bulletRight = bullet->bullet.x + 3;
            int bulletTop = bullet->bullet.y;
            int bulletBottom = bullet->bullet.y + 5;
    
            currentBarrier = head;
            while (currentBarrier != NULL) {
                if (currentBarrier->health > 0) {
                    int barrierLeft = currentBarrier->x;
                    int barrierRight = currentBarrier->x + 8;
                    int barrierTop = currentBarrier->y;
                    int barrierBottom = currentBarrier->y + 4;
    
                    if (bulletRight > barrierLeft && bulletLeft < barrierRight &&
                            bulletBottom > barrierTop && bulletTop < barrierBottom) {
    
                        bullet->bullet.active = 0;
                        currentBarrier->health--;
                        goto next_player_bullet;
                    }
                }
                currentBarrier = currentBarrier->next;
            }
        }
    next_player_bullet:
        bullet = bullet->next;
    }

    bullet = alienBullets;
    while (bullet != NULL) {
        if (bullet->bullet.active) {
            int bulletLeft = bullet->bullet.x;
            int bulletRight = bullet->bullet.x + 3;
            int bulletTop = bullet->bullet.y;
            int bulletBottom = bullet->bullet.y + 5;
    
            currentBarrier = head;
            while (currentBarrier != NULL) {
                if (currentBarrier->health > 0) {
                    int barrierLeft = currentBarrier->x;
                    int barrierRight = currentBarrier->x + 8;
                    int barrierTop = currentBarrier->y;
                    int barrierBottom = currentBarrier->y + 4;
    
                    if (bulletRight > barrierLeft && bulletLeft < barrierRight &&
                        bulletBottom > barrierTop && bulletTop < barrierBottom) {
    
                        bullet->bullet.active = 0;
                        currentBarrier->health--;
                        goto next_alien_bullet;
                    }
                }
                currentBarrier = currentBarrier->next;
            }
        }
    next_alien_bullet:
        bullet = bullet->next;
    }

    bullet = ufoBulletList;
    while (bullet != NULL) {
        if (bullet->bullet.active) {
            int bulletLeft = bullet->bullet.x;
            int bulletRight = bullet->bullet.x + 3;
            int bulletTop = bullet->bullet.y;
            int bulletBottom = bullet->bullet.y + 5;
    
            currentBarrier = head;
            while (currentBarrier != NULL) {
                if (currentBarrier->health > 0) {
                    int barrierLeft = currentBarrier->x;
                    int barrierRight = currentBarrier->x + 8;
                    int barrierTop = currentBarrier->y;
                    int barrierBottom = currentBarrier->y + 4;
    
                    if (bulletRight > barrierLeft && bulletLeft < barrierRight &&
                        bulletBottom > barrierTop && bulletTop < barrierBottom) {
    
                        bullet->bullet.active = 0;
                        currentBarrier->health--;
                        goto next_ufo_bullet;
                    }
                }
                currentBarrier = currentBarrier->next;
            }
        }
    next_ufo_bullet:
        bullet = bullet->next;
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
