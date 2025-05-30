#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "barrier.h"
#include "alien.h"
#include "ufo.h"
#include "mainsprite.h"

extern BulletNode* playerBullets;
extern BulletNode* alienBullets;
extern BulletNode* ufoBulletList;

void drawBarrier(Barrier* b) {
    if (b && b->health > 0) {
        const char* imgFile;
        if (b->health > 15 && b->health <= 30) {
            imgFile = "img/barrierHijau.bmp";
        } else if (b->health > 5 && b->health <= 15) {
            imgFile = "img/barrierKuning.bmp";
        } else if (b->health > 1 && b->health <= 5) {
            imgFile = "img/barrierMerah.bmp";
        }
        readimagefile(imgFile, b->x, b->y + 20, b->x + 80, b->y + 20);
    }
}

void checkBarrierBulletCollision(Barrier* head) {
    BulletNode* bullet;
    Barrier* currentBarrier;

    BulletNode* bulletLists[] = {playerBullets, alienBullets};

    for (int i = 0; i < 2; ++i) {
        bullet = bulletLists[i];
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
                        int barrierRight = currentBarrier->x + 80;
                        int barrierTop = currentBarrier->y - 20;
                        int barrierBottom = currentBarrier->y + 25;

                        if (bulletRight > barrierLeft && bulletLeft < barrierRight &&
                            bulletBottom > barrierTop && bulletTop < barrierBottom) {
                            bullet->bullet.active = 0;
                            currentBarrier->health--;
                            break;
                        }
                    }
                    currentBarrier = currentBarrier->next;
                }
            }
            bullet = bullet->next;
        }
    }
    
    BulletNode* currentUfoBullet = ufoBulletList;
    while (currentUfoBullet != NULL) {
        if (currentUfoBullet->bullet.active) {
            int bulletLeft   = currentUfoBullet->bullet.x - 3;
            int bulletRight  = currentUfoBullet->bullet.x + 3;
            int bulletTop    = currentUfoBullet->bullet.y - 3;
            int bulletBottom = currentUfoBullet->bullet.y + 3;

            currentBarrier = head;
            while (currentBarrier != NULL) {
                if (currentBarrier->health > 0) {
                    int barrierLeft   = currentBarrier->x;
                    int barrierRight  = currentBarrier->x + 80;
                    int barrierTop    = currentBarrier->y - 20;
                    int barrierBottom = currentBarrier->y + 25;

                    if (bulletRight > barrierLeft && bulletLeft < barrierRight &&
                        bulletBottom > barrierTop && bulletTop < barrierBottom) {
                        currentUfoBullet->bullet.active = 0;
                        currentBarrier->health--;
                        break;
                    }
                }
                currentBarrier = currentBarrier->next;
            }
        }
        currentUfoBullet = currentUfoBullet->next;
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
