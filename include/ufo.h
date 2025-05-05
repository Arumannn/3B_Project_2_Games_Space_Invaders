#ifndef UFO_H
#define UFO_H

#include "alien.h"
#include "bullet.h"

#define MAX_UFO_BULLETS 10


extern BulletNode *ufoBulletList;
extern int ufoBulletCount;

void UFO(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);
void initUFO();
void drawExplosion(int x, int y);
void drawUFO(int x, int y);

#endif
