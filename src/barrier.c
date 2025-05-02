#include <graphics.h>
#include <conio.h>
#include <stdlib.h> // untuk malloc
#include "barrier.h"
#include "alien.h"
#include "ufo.h"
#include "mainsprite.h"

extern AlienBullet alienBullets[MAX_ALIEN_BULLETS];
extern Bullet bullets_player[MAX_BULLETS];

// Menggambar barrier
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

void checkAlienBulletCollision(Barrier* head) {
    Barrier* current;
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets_player[i].active) {
            int bulletLeft = bullets_player[i].x;
            int bulletTop = bullets_player[i].y;
            int bulletRight = bullets_player[i].x + 10;
            int bulletBottom = bullets_player[i].y + 10;

            current = head;
            while (current != NULL) {
                if (current->health > 0) {
                    int BarrierLeft = current->x;
                    int BarrierRight = current->x + 80;
                    int BarrierTop = current->y - 5;
                    int BarrierBottom = current->y + 25;

                    if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                        bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                        current->health--;
                        bullets_player[i].active = 0;
                        drawBarrier(current);
                    }
                }
                current = current->next;
            }
        }
    }

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            int bulletLeft = alienBullets[i].x;
            int bulletRight = alienBullets[i].x + BLOCK_SIZE / 2;
            int bulletTop = alienBullets[i].y;
            int bulletBottom = alienBullets[i].y + BLOCK_SIZE;

            current = head;
            while (current != NULL) {
                if (current->health > 0) {
                    int BarrierLeft = current->x;
                    int BarrierRight = current->x + 80;
                    int BarrierTop = current->y - 5;
                    int BarrierBottom = current->y + 25;

                    if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                        bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                        current->health--;
                        alienBullets[i].active = 0;
                        drawBarrier(current);
                    }
                }
                current = current->next;
            }
        }
    }

    for (int i = 0; i < MAX_UFO_BULLETS; i++) {
        if (ufoBullets[i].active) {
            int bulletLeft = ufoBullets[i].x - 3;
            int bulletRight = ufoBullets[i].x + 3;
            int bulletTop = ufoBullets[i].y - 3;
            int bulletBottom = ufoBullets[i].y + 3;

            current = head;
            while (current != NULL) {
                if (current->health > 0) {
                    int BarrierLeft = current->x;
                    int BarrierRight = current->x + 80;
                    int BarrierTop = current->y - 5;
                    int BarrierBottom = current->y + 25;

                    if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                        bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                        current->health--;
                        ufoBullets[i].active = 0;
                        drawBarrier(current);
                    }
                }
                current = current->next;
            }
        }
    }
}

// Inisialisasi linked list barrier
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
