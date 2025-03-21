#include <graphics.h>
#include "mainsprite.h"
#include "alien.h"
#include "ufo.h"
#include "gameover.h"

// Definisi variabel global
Bullet bullets_player[MAX_BULLETS];
Explosion playerExplosions[MAX_EXPLOSIONS];
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
        //cleardevice();
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

// Fungsi untuk memeriksa tabrakan dengan peluru alien/UFO
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
                playerExplosions[i].active = 0;  // **Matikan ledakan setelah animasi selesai**
                playerExplosions[i].lifetime = 0;
            }
            playerExplosions[i].lifetime++;
        }
    }
}


void updateExplosionsPlayer() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (playerExplosions[i].active) {
            playerExplosions[i].lifetime++;
            if (playerExplosions[i].lifetime >= 20) {
                playerExplosions[i].active = 0;  // **Matikan ledakan setelah 20 frame**
                playerExplosions[i].lifetime = 0;  // **Reset lifetime agar bisa digunakan lagi**
            }
        }
    }
}


void resetPlayer(Player *player) {
    // **Tambahkan ledakan di posisi terakhir pemain**
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

    // **Tampilkan animasi ledakan selama 3 detik**
    for (int i = 0; i < 90; i++) {  // 90 frame (dengan asumsi 30 FPS)
        updateExplosionsPlayer();
        drawExplosionsPlayer();
        
    }

    player->X_Player = getmaxx() / 2;
    player->Y_Player = getmaxy() - 80;
    
    
    // **Kurangi nyawa pemain**
    printf("Respawn! Nyawa tersisa: %d\n", player->health);
   

    if (player->health == 0) {
        printf("Game Over!\n");
        closegraph();
        gameOverScreen();

    }
}
