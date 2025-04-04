#ifndef UFO_H
#define UFO_H
#include "alien.h"
#define MAX_UFO_BULLETS 10
extern AlienBullet ufoBullets[MAX_UFO_BULLETS];
extern int ufoRespawnDelay;

void drawUFO(int x, int y);
void drawBullet(int bx, int by);
void UFO(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);
void initUFO();

#endif