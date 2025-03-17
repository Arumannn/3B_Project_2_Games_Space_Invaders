#include <graphics.h>
#include <math.h>
#include "alien.h"
#include "mainsprite.h"

// Definisi variabel global
Bullet bullets_player[MAX_BULLETS];
int shootCooldown = 0;


void DrawSpaceShip(Player *player) {
    int x = player->X_Player;
    int y = player->Y_Player;

    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    int body[] = {x, y, x - 20, y + 40, x + 20, y + 40, x, y};
    fillpoly(4, body);

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y + 10, 7, 10);

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int leftWing[] = {x - 20, y + 40, x - 40, y + 60, x - 20, y + 60, x - 20, y + 40};
    fillpoly(4, leftWing);

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int rightWing[] = {x + 20, y + 40, x + 40, y + 60, x + 20, y + 60, x + 20, y + 40};
    fillpoly(4, rightWing);

    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    bar(x - 10, y + 40, x + 10, y + 50);

    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    int thruster[] = {x - 5, y + 50, x + 5, y + 50, x, y + 70, x - 5, y + 50};
    fillpoly(4, thruster);
}

void SpaceshipMove(Player *player) {
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000) && player->X_Player > 40) {
        player->X_Player -= 15;
    }
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) && player->X_Player < getmaxx() - 40) {
        player->X_Player += 15;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && shootCooldown <= 0) {
        ShootBullet(player);
        shootCooldown = 3;
    }
}

void SpaceShip(Player *player) {
    while (1) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }
        cleardevice();
        SpaceshipMove(player);
        updateBullets();
        drawBullets();
        delay(30);
    }
}

void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets_player[i].active = 0;
    }
}

void ShootBullet(Player *player) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets_player[i].active) {
            bullets_player[i].x = player->X_Player;
            bullets_player[i].y = player->Y_Player - 10;
            bullets_player[i].active = 1;
            break;
        }
    }
}

void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets_player[i].active) {
            extern int BLOCK_SIZE;
            bullets_player[i].y -= BLOCK_SIZE;
            if (bullets_player[i].y < 0) {
                bullets_player[i].active = 0;
            }
        }
    }
    if (shootCooldown > 0) {
        shootCooldown--;
    }
}

void drawBullets() {
    extern int BLOCK_SIZE;
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets_player[i].active) {
            bar(bullets_player[i].x - BLOCK_SIZE / 4, bullets_player[i].y, 
                bullets_player[i].x + BLOCK_SIZE / 4, bullets_player[i].y + BLOCK_SIZE);
        }
    }
}

void drawExplosion(int x, int y, int frame) {
    int maxRadius = BLOCK_SIZE; // Ukuran maksimum ledakan
    int currentRadius = frame * (maxRadius / 5); // Ledakan berkembang tiap frame

    // Efek percikan
    setcolor(YELLOW);
    for (int i = 0; i < 8; i++) {
        float angle = i * (3.14159265 / 4); // 8 arah percikan
        int dx = cos(angle) * currentRadius;
        int dy = sin(angle) * currentRadius;
        line(x, y, x + dx, y + dy);
    }

    // Efek fragmen alien
    setcolor(WHITE);
    for (int i = 0; i < 4; i++) {
        int fragX = x + (rand() % (2 * currentRadius)) - currentRadius;
        int fragY = y + (rand() % (2 * currentRadius)) - currentRadius;
        fillellipse(fragX, fragY, 2, 2); // Fragmen kecil
    }

    // Gelombang kejut
    if (frame < 3) {
        setcolor(LIGHTGRAY);
        circle(x, y, currentRadius);
    }
}



void CheckCollision(Player *player, AlienBullet alienBullets[]) {
    if (player->Health <= 0) return;

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            if (alienBullets[i].x >= player->X_Player - 20 &&
                alienBullets[i].x <= player->X_Player + 20 &&
                alienBullets[i].y >= player->Y_Player &&
                alienBullets[i].y <= player->Y_Player + 40) {
                
                alienBullets[i].active = 0;
                player->Health--;
                
                if (player->Health > 0) {
                    resetPlayer(player);
                }
            }
        }
    }
}

void resetPlayer(Player *player){
    player->X_Player = getmaxx() / 2;    
    player->Y_Player = getmaxy() - 80;
    
    if (player->Health > 0) {
        player->Health -= 1;
    }
    SpaceshipMove(player);

}