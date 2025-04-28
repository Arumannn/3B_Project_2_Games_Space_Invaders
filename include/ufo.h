#ifndef UFO_H
#define UFO_H
#include "bullet.h"
#include "alien.h"
#define MAX_UFO_BULLETS 10
extern int ufoRespawnDelay;

void drawUFO(int x, int y);
void drawBullet(int bx, int by);
void UFO(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);
void initUFO();
void drawExplosion(int x, int y);

#endif