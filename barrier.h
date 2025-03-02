#ifndef BARRIER_H
#define BARRIER_H

void drawBarrier(int index, int x, int y);
void drawBullet(Bullet bullet);
void checkBulletCollision(Bullet *bullet, int startX, int barrierY);

#endif