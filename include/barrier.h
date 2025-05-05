#ifndef BARRIER_H
#define BARRIER_H

#define MAX_BARRIERS 4

typedef struct Barrier {
    int x, y;
    int health;
    struct Barrier* next; // Tambahkan pointer ke next
} Barrier;

void drawBarrier(Barrier* b);
void checkAlienBulletCollision(Barrier* barrierHead);
void initBarriers(Barrier** head);
void freeBarriers(Barrier* head);
void checkBulletBarrierCollision(Bullet** bulletList, Barrier** barrierList);

#endif
