#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "barrier.h"
#include "alien.h"
#include "ufo.h"
#include "mainsprite.h"

extern int playerBulletX;
extern int playerBulletY;
extern int playerBulletActive;

extern int alienBulletX;
extern int alienBulletY;
extern int alienBulletActive;

extern int ufoBulletX;
extern int ufoBulletY;
extern int ufoBulletActive;

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
    Barrier* current = head;

    while (current != NULL) {
        if (current->health > 0) {
            // Periksa apakah ada peluru alien mengenai barrier ini
            if (alienBulletActive &&
                alienBulletX >= current->x &&
                alienBulletX < current->x + 8 &&
                alienBulletY >= current->y &&
                alienBulletY < current->y + 4) {

                alienBulletActive = 0;
                current->health--;
                break; // hanya satu tabrakan per frame
            }

            // Periksa apakah ada peluru pemain mengenai barrier ini
            if (playerBulletActive &&
                playerBulletX >= current->x &&
                playerBulletX < current->x + 8 &&
                playerBulletY >= current->y &&
                playerBulletY < current->y + 4) {

                playerBulletActive = 0;
                current->health--;
                break;
            }

            if (ufoBulletActive &&
                ufoBulletX >= current->x &&
                ufoBulletX < current->x + 8 &&
                ufoBulletY >= current->y &&
                ufoBulletY < current->y + 4) {

                ufoBulletActive = 0;
                current->health--;
                break;
            }
        }

        current = current->next;
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
