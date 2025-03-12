// ufo.h
#ifndef UFO_H
#define UFO_H
#include <graphics.h>
#include "alien.h" // Tambahkan ini untuk mendeklarasikan struct Alien

extern float ufoX;
extern float ufoY;
extern float ufoSpeed;
extern int ufoDirection;
extern int page;
extern int ufoActive;
extern int ufoHealth;

// Deklarasi variabel peluru UFO
extern int ufoBulletX;
extern int ufoBulletY;
extern int ufoBulletActive;

extern Alien aliens[MAX_ALIENS]; // Asumsikan MAX_ALIENS sudah didefinisikan






void drawUFO(int x, int y);
void drawBullet(int bx, int by);
void UFO(Alien aliens[]);
void resetUFO();

#endif
