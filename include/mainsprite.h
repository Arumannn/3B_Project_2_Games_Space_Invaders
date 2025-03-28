// mainsprite.h
#ifndef MAINSPRITE_H
#define MAINSPRITE_H
#define MAX_BULLETS 3
#define MAX_EXPLOSIONS 10  // Batas maksimum ledakan yang bisa ditampilkan

#include <graphics.h>
#include <conio.h>
#include <windows.h>


typedef struct {
    int X_Player, Y_Player;
    int health;  // Tambahkan variabel nyawa
} Player;

typedef struct {
    int x, y;
    int active;
} Bullet;


extern Bullet bullets_player[MAX_BULLETS];
extern int shootCooldown;

void initBullets();
void updateBullets();
void drawBullets();
void ShootBullet(Player *player);
void SpaceshipMove(Player *player);
void DrawSpaceShip(Player *player);
void checkPlayerCollisions(Player *player);  
void updateExplosionsPlayer();
void drawExplosionsPlayer();
void initExplosionsPlayer();
void resetPlayer(Player *player);
void drawLives(int lives);
void drawHeart(int x, int y, int size, int color);

#endif