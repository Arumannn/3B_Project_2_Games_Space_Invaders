#ifndef BARRIER_H
#define BARRIER_H

#define MAX_BARRIERS 4

typedef struct {
    int x, y;
    int health;
} Barrier;

void drawBarrier(Barrier b);
void checkAlienBulletCollision(Barrier barriers[]);
void initBarriers(Barrier barriers[]);


#endif