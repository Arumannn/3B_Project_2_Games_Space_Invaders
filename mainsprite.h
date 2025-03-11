#ifndef MAINSPRITE_H
#define MAINSPRITE_H
#define MAX_BULLETS 5
#include <graphics.h>
#include <conio.h>
#include <windows.h>

typedef struct {
    int X_Player, Y_Player;
} Player;

typedef struct {
    int x, y;
    int active;
} Bullet;

extern Bullet bullets_player[MAX_BULLETS];
extern int shootCooldown; // Tambahkan deklarasi untuk cooldown

void initBullets();
void updateBullets();
void drawBullets();
void ShootBullet(Player *player);
void SpaceshipMove(Player *player);
void DrawSpaceShip(Player *player);
void SpaceShip(Player *player);

#endif