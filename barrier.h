#define BARRIER_H
#define BARRIER_H

int calculateBarrierStartX();
void drawBarrier(int index, int x, int y);
void drawBullet(Bullet bullet);
void checkBulletCollision(Bullet *bullet, int startX, int barrierY);
void drawAllBarriers(int startX, int barrierY);
void initializeBarrierState();