// mainsprite.h
#ifndef MAINSPRITE_H
#define MAINSPRITE_H
#define MAX_BULLETS 100
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

typedef struct {
    int x, y;
    int active;
    int frame;  // Frame animasi ledakan
} Explosion;  // Struktur untuk ledakan

extern Bullet bullets_player[MAX_BULLETS];
extern int shootCooldown;
extern Explosion explosions[MAX_EXPLOSIONS];  // Array untuk menyimpan ledakan

void initBullets();
void updateBullets();
void drawBullets();
void ShootBullet(Player *player);
void SpaceshipMove(Player *player);
void DrawSpaceShip(Player *player);
void SpaceShip(Player *player);
void checkPlayerCollisions(Player *player);  // Tambahkan deklarasi fungsi
void drawExplosions();  // Fungsi untuk menggambar ledakan
void updateExplosions();  // Fungsi untuk memperbarui animasi ledakan

#endif