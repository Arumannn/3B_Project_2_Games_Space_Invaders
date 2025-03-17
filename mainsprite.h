#ifndef MAINSPRITE_H
#define MAINSPRITE_H
#define MAX_BULLETS 100
#include <graphics.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include "alien.h"
#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)  // Windows: Sleep() dalam milidetik
#else
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000)  // Linux/macOS: usleep() dalam mikrodetik
#endif


typedef struct {
    int X_Player, Y_Player;
    int Health;
    bool exploding;

} Player;

typedef struct {
    int x, y;
    int active;
} Bullet;

extern Bullet bullets_player[MAX_BULLETS];
extern int shootCooldown; // Tambahkan deklarasi untuk cooldown
extern int numEnemyBullets; // Mengacu pada variabel di `alien.c`


void initBullets();
void updateBullets();
void drawBullets();
void ShootBullet(Player *player);
void SpaceshipMove(Player *player);
void DrawSpaceShip(Player *player);
void SpaceShip(Player *player);
void CheckCollision(Player *player, AlienBullet alienBullets[]);
void resetPlayer(Player *player);

#endif