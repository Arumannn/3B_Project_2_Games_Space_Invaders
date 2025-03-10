#ifndef UFO_H
#define UFO_H
#include <graphics.h>
#include "alien.h" // Tambahkan ini untuk mendeklarasikan struct Alien

void drawUFO(int x, int y);
void drawBullet(int bx, int by);
void UFO(Alien aliens[]); // Perbarui deklarasi dengan parameter aliens
#endif