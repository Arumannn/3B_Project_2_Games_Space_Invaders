// mainsprite.h
#ifndef MAINSPRITE_H
#define MAINSPRITE_H
#define MAX_BULLETS 3
#define MAX_EXPLOSIONS 10  // Batas maksimum ledakan yang bisa ditampilkan
#define PLAYER_HITBOX_WIDTH  80
#define PLAYER_HITBOX_HEIGHT 80
#include "gameplay.h"
#include <graphics.h>
#include <conio.h>
#include <windows.h>


typedef struct {
    int X_Player, Y_Player;
    int health;  
    int alive;
    int invincible;      
    int invincibleTimer;
    int respawning;       // 1 = respawning, 0 = normal
    int respawnTimer;      // co    
} Player;



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
void updatePlayerRespawn(Player *player);
int isColliding(int l1, int r1, int t1, int b1, int l2, int r2, int t2, int b2);
void resetPlayer(Player *player);
void drawLives(int lives);
void drawHeart(int x, int y, int size, int color);

#endif