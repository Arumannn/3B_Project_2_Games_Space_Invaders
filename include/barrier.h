#ifndef BARRIER_H
#define BARRIER_H

typedef struct {
    int x, y;
    int health;
}Barrier;

void drawBarrier(Barrier barriers[]);
void barBarrier();
void checkAlienBulletCollision(Barrier barriers[]);

#endif