#ifndef BARRIER_H
#define BARRIER_H
#define MAX_BARRIERS 4

typedef struct {
    int x, y;
    int health = 20;
}Barrier;

void drawBarrier(Barrier barriers[]);
void barBarrier();
void checkAlienBulletCollision(Barrier barriers[MAX_BARRIERS]);

#endif