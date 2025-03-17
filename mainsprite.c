#include <graphics.h>
#include "mainsprite.h"
#include "alien.h"
#include "ufo.h"

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

// Fungsi untuk memeriksa tabrakan dengan peluru alien/UFO
void checkPlayerCollisions(Player *player) {
    // Cek tabrakan dengan peluru alien
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
                player->health--;  // Kurangi nyawa
                alienBullets[i].active = 0;  // Nonaktifkan peluru alien

                // Tambahkan ledakan di posisi tabrakan
                for (int j = 0; j < MAX_EXPLOSIONS; j++) {
                    if (!explosions[j].active) {
                        explosions[j].x = player->X_Player;
                        explosions[j].y = player->Y_Player;
                        explosions[j].active = 1;
                        explosions[j].frame = 0;
                        break;
                    }
                }

                if (player->health <= 0) {
                    // Game over logic here
                }
            }
        }
    }

    // Cek tabrakan dengan peluru UFO
    if (ufoBulletActive) {
        int bulletLeft = ufoBulletX - 3;
        int bulletRight = ufoBulletX + 3;
        int bulletTop = ufoBulletY - 3;
        int bulletBottom = ufoBulletY + 3;

        int playerLeft = player->X_Player - 20;
        int playerRight = player->X_Player + 20;
        int playerTop = player->Y_Player;
        int playerBottom = player->Y_Player + 40;

        if (bulletRight > playerLeft && bulletLeft < playerRight &&
            bulletBottom > playerTop && bulletTop < playerBottom) {
            player->health--;  // Kurangi nyawa
            ufoBulletActive = 0;  // Nonaktifkan peluru UFO

            // Tambahkan ledakan di posisi tabrakan
            for (int j = 0; j < MAX_EXPLOSIONS; j++) {
                if (!explosions[j].active) {
                    explosions[j].x = player->X_Player;
                    explosions[j].y = player->Y_Player;
                    explosions[j].active = 1;
                    explosions[j].frame = 0;
                    break;
                }
            }

            if (player->health <= 0) {
                // Game over logic here
            }
        }
    }
}





}

void initExplosions() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosions[i].active = 0;
        explosions[i].frame = 0;
    }
}

void drawExplosions() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (explosions[i].active) {
            // Gambar ledakan berdasarkan frame
            if (explosions[i].frame < 5) {
                setcolor(YELLOW);
                setfillstyle(SOLID_FILL, YELLOW);
                fillellipse(explosions[i].x, explosions[i].y, 10, 10);
            } else if (explosions[i].frame < 10) {
                setcolor(ORANGE);
                setfillstyle(SOLID_FILL, ORANGE);
                fillellipse(explosions[i].x, explosions[i].y, 20, 20);
            } else if (explosions[i].frame < 15) {
                setcolor(RED);
                setfillstyle(SOLID_FILL, RED);
                fillellipse(explosions[i].x, explosions[i].y, 30, 30);
            } else {
                explosions[i].active = 0;  // Nonaktifkan ledakan setelah selesai
            }
        }
    }
}

void updateExplosions() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (explosions[i].active) {
            explosions[i].frame++;  // Tambahkan frame
            if (explosions[i].frame >= 20) {  // Hentikan animasi setelah 20 frame
                explosions[i].active = 0;
            }
        }
    }
}