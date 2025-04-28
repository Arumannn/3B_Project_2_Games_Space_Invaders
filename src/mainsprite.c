#include <graphics.h>
#include "mainsprite.h"
#include "alien.h"
#include "ufo.h"
#include "gameplay.h"
#include "mainmenu.h"

// Definisi variabel global
Explosion playerExplosions[MAX_EXPLOSIONS];

void DrawSpaceShip(Player *player) {
    int x = player->X_Player;
    int y = player->Y_Player;

    // Body of the spaceship with more details
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    int body[] = {x, y, x - 20, y + 50, x + 20, y + 50, x, y};
    fillpoly(4, body);

    // Detailed Central Blue Window
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y + 12, 6, 10);
    fillellipse(x, y + 28, 4, 7);
    
    // Orange Stripes for more details
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    bar(x - 15, y + 32, x - 8, y + 35);
    bar(x + 8, y + 32, x + 15, y + 35);

    // Left Wing - More pixel-accurate
    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    int leftWing[] = {x - 20, y + 50, x - 38, y + 65, x - 20, y + 70, x - 20, y + 50};
    fillpoly(4, leftWing);
    
    // Right Wing - More pixel-accurate
    int rightWing[] = {x + 20, y + 50, x + 38, y + 65, x + 20, y + 70, x + 20, y + 50};
    fillpoly(4, rightWing);

    // Thruster Base
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    int thruster[] = {x - 8, y + 50, x + 8, y + 50, x + 5, y + 72, x - 5, y + 72, x - 8, y + 50};
    fillpoly(5, thruster);

    // Flame Details - Multi-layered for more effect
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int flame[] = {x - 5, y + 72, x + 5, y + 72, x, y + 85, x - 5, y + 72};
    fillpoly(4, flame);
    
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int smallFlame[] = {x - 3, y + 77, x + 3, y + 77, x, y + 92, x - 3, y + 77};
    fillpoly(4, smallFlame);

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int tinyFlame[] = {x - 2, y + 82, x + 2, y + 82, x, y + 96, x - 2, y + 82};
    fillpoly(4, tinyFlame);
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
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        showMainMenu();
    }
}

void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        playerBullets[i].active = 0;
    }
}

void ShootBullet(Player *player) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!playerBullets[i].active) {
            PlaySound(TEXT("sound/Shooting_Audio.wav"), NULL, SND_FILENAME | SND_ASYNC);
            playerBullets[i].x = player->X_Player;
            playerBullets[i].y = player->Y_Player - 10;
            playerBullets[i].active = 1;
            break;
        }
    }
}

void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (playerBullets[i].active) {
            playerBullets[i].y -= BLOCK_SIZE;
            if (playerBullets[i].y < 0) {
                playerBullets[i].active = 0;
            }
        }
    }
    if (shootCooldown > 0) {
        shootCooldown--;
    }
}

void drawBullets() {
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (playerBullets[i].active) {
            bar(playerBullets[i].x - BLOCK_SIZE / 4, playerBullets[i].y, 
                playerBullets[i].x + BLOCK_SIZE / 4, playerBullets[i].y + BLOCK_SIZE);
        }
    }
}

void checkPlayerCollisions(Player *player) {
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            int bulletLeft = alienBullets[i].x;
            int bulletRight = alienBullets[i].x + BLOCK_SIZE / 2;
            int bulletTop = alienBullets[i].y;
            int bulletBottom = alienBullets[i].y + BLOCK_SIZE;

            int playerLeft = player->X_Player - 20;
            int playerRight = player->X_Player + 20;
            int playerTop = player->Y_Player;
            int playerBottom = player->Y_Player + 40;

            if (bulletRight > playerLeft && bulletLeft < playerRight &&
                bulletBottom > playerTop && bulletTop < playerBottom) {
                resetPlayer(player);
                player->health--;
                printf("Health Sekarang : %d\n", player->health);
                alienBullets[i].active = 0;
                
                for (int j = 0; j < MAX_EXPLOSIONS; j++) {
                    if (!playerExplosions[j].active) {
                        playerExplosions[j].x = player->X_Player;
                        playerExplosions[j].y = player->Y_Player;
                        playerExplosions[j].active = 1;
                        playerExplosions[j].lifetime = 0;
                        break;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < MAX_UFO_BULLETS; i++) {
        if (ufoBullets[i].active) {
            int bulletLeft = ufoBullets[i].x - 3;
            int bulletRight = ufoBullets[i].x + 3;
            int bulletTop = ufoBullets[i].y - 3;
            int bulletBottom = ufoBullets[i].y + 3;

            int playerLeft = player->X_Player - 20;
            int playerRight = player->X_Player + 20;
            int playerTop = player->Y_Player;
            int playerBottom = player->Y_Player + 40;

            if (bulletRight > playerLeft && bulletLeft < playerRight &&
                bulletBottom > playerTop && bulletTop < playerBottom) {
                resetPlayer(player);
                player->health--;
                printf("Health Sekarang : %d\n", player->health);
                ufoBullets[i].active = 0;
            }
        }
    }
}

void initExplosionsPlayer() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        playerExplosions[i].active = 0;
        playerExplosions[i].lifetime = 0;
    }
}

void drawExplosionsPlayer() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (playerExplosions[i].active) {
            if (playerExplosions[i].lifetime < 5) {
                setcolor(YELLOW);
                setfillstyle(SOLID_FILL, YELLOW);
                fillellipse(playerExplosions[i].x, playerExplosions[i].y, 10, 10);
            } else if (playerExplosions[i].lifetime < 10) {
                setcolor(YELLOW);
                setfillstyle(SOLID_FILL, YELLOW);
                fillellipse(playerExplosions[i].x, playerExplosions[i].y, 20, 20);
            } else if (playerExplosions[i].lifetime < 15) {
                setcolor(RED);
                setfillstyle(SOLID_FILL, RED);
                fillellipse(playerExplosions[i].x, playerExplosions[i].y, 30, 30);
            } else {
                playerExplosions[i].active = 0;  // Matikan ledakan setelah animasi selesai
                playerExplosions[i].lifetime = 0;
            }
            playerExplosions[i].lifetime++;
        }
    }
}

void updateExplosionsPlayer() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (playerExplosions[i].active) {
            if (playerExplosions[i].lifetime == 0) { // Putar suara hanya sekali
                PlaySound(TEXT("sound/Player_Explosion.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
            playerExplosions[i].lifetime++;
            if (playerExplosions[i].lifetime >= 20) {
                playerExplosions[i].active = 0;  // Matikan ledakan setelah 20 frame
                playerExplosions[i].lifetime = 0;  // Reset lifetime agar bisa digunakan lagi
            }
        }
    }
}

void resetPlayer(Player *player) {
    printf("Respawn! Nyawa tersisa: %d\n", player->health);
    for (int j = 0; j < MAX_EXPLOSIONS; j++) {
        if (!playerExplosions[j].active) {
            playerExplosions[j].x = player->X_Player;
            playerExplosions[j].y = player->Y_Player;
            playerExplosions[j].active = 1;
            playerExplosions[j].lifetime = 0;
            break;
        }
    }

    for (int i = 0; i < 90; i++) {  // 90 frame (dengan asumsi 30 FPS)
        updateExplosionsPlayer();
        drawExplosionsPlayer();
    }

    player->X_Player = getmaxx() / 2;
    player->Y_Player = getmaxy() - 100;
    
    printf("Respawn! Nyawa tersisa: %d\n", player->health);
}

void drawHeart(int x, int y, int size, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, color);

    fillellipse(x, y, size, size / 2);

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y - size / 3, size / 2, size / 3);

    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    for (int i = -size / 2; i <= size / 2; i += size / 4) {
        fillellipse(x + i, y + size / 2, size / 6, size / 6);
    }
}

void drawLives(int lives) {
    int screenWidth = getmaxx();
    int startX = screenWidth - 50;  // Pojok kanan atas
    int startY = 50;  // Tinggi tetap
    int heartSize = 20;
    int spacing = 40;  // Jarak antar heart

    for (int i = 0; i < lives; i++) {
        drawHeart(startX - (i * spacing), startY, heartSize, RED);
    }
}