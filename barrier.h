#ifndef BARRIER_H
#define BARRIER_H

int calculateBarrierStartX();
void drawBarrier(int index, int x, int y);
void checkBulletCollision(Bullet *bullet, int startX, int barrierY);

#endif