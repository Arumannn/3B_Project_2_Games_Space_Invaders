#ifndef UFO_H
#define UFO_H
#include "alien.h"

extern int ufoActive;
extern int ufoBulletX, ufoBulletY, ufoBulletActive;

void drawUFO(int x, int y);
void drawBullet(int bx, int by);
void UFO(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);

#endif