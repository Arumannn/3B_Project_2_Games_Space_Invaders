#include "bullet.h"
#include <windows.h>

// Definisi variabel global
Bullet playerBullets[MAX_BULLETS];
Bullet alienBullets[MAX_ALIEN_BULLETS];
Bullet ufoBullets[MAX_UFO_BULLETS];
int shootCooldown = 0;

void initAllBullets() {
    // Inisialisasi peluru pemain
    for (int i = 0; i < MAX_BULLETS; i++) {
        playerBullets[i].active = 0;
    }
    // Inisialisasi peluru alien
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        alienBullets[i].active = 0;
    }
    // Inisialisasi peluru UFO
    for (int i = 0; i < MAX_UFO_BULLETS; i++) {
        ufoBullets[i].active = 0;
    }
    shootCooldown = 0;
}

void updateAllBullets() {
    // Perbarui peluru pemain
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (playerBullets[i].active) {
            playerBullets[i].y -= BLOCK_SIZE;
            if (playerBullets[i].y < 0) {
                playerBullets[i].active = 0;
            }
        }
    }
    // Perbarui peluru alien
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            alienBullets[i].y += BLOCK_SIZE / 2;
            if (alienBullets[i].y > getmaxy()) {
                alienBullets[i].active = 0;
            }
        }
    }
    // Perbarui peluru UFO
    for (int i = 0; i < MAX_UFO_BULLETS; i++) {
        if (ufoBullets[i].active) {
            ufoBullets[i].y += ufoBullets[i].speed;
            ufoBullets[i].x += ufoBullets[i].dx;
            if (ufoBullets[i].y > getmaxy() || ufoBullets[i].x < 0 || ufoBullets[i].x > getmaxx()) {
                ufoBullets[i].active = 0;
            }
        }
    }
    // Kurangi cooldown pemain
    if (shootCooldown > 0) {
        shootCooldown--;
    }
}

void drawAllBullets() {
    // Gambar peluru pemain
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (playerBullets[i].active) {
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            bar(playerBullets[i].x - PLAYER_BULLET_WIDTH, playerBullets[i].y, 
                playerBullets[i].x + PLAYER_BULLET_WIDTH, playerBullets[i].y + BLOCK_SIZE);
        }
    }
    // Gambar peluru alien
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            bar(alienBullets[i].x, alienBullets[i].y, 
                alienBullets[i].x + ALIEN_BULLET_WIDTH, alienBullets[i].y + BLOCK_SIZE);
        }
    }
    // Gambar peluru UFO
    for (int i = 0; i < MAX_UFO_BULLETS; i++) {
        if (ufoBullets[i].active) {
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            fillellipse(ufoBullets[i].x, ufoBullets[i].y, UFO_BULLET_RADIUS, UFO_BULLET_RADIUS);
            setcolor(14); // Oranye
            setfillstyle(SOLID_FILL, 14);
            fillellipse(ufoBullets[i].x, ufoBullets[i].y, UFO_BULLET_RADIUS - 2, UFO_BULLET_RADIUS - 2);
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            fillellipse(ufoBullets[i].x, ufoBullets[i].y, UFO_BULLET_RADIUS - 4, UFO_BULLET_RADIUS - 4);
        }
    }
}

void shootBullet(Bullet bullets[], int maxBullets, int x, int y, int type, 
                 int *cooldown, int *burstCount, int maxBurst, int burstCooldown, int postBurstCooldown) {
    // Periksa cooldown
    if (cooldown && *cooldown > 0) {
        (*cooldown)--;
        return;
    }

    // Cari slot peluru kosong
    for (int i = 0; i < maxBullets; i++) {
        if (!bullets[i].active) {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].active = 1;
            bullets[i].type = type;

            // Atur properti berdasarkan tipe
            if (type == PLAYER_BULLET) {
                bullets[i].speed = BLOCK_SIZE;
                bullets[i].dx = 0;
                if (cooldown) *cooldown = 3;
                PlaySound(TEXT("sound/Shooting_Audio.wav"), NULL, SND_ASYNC);
            } else if (type == ALIEN_BULLET) {
                bullets[i].speed = BLOCK_SIZE / 2;
                bullets[i].dx = 0;
            } else if (type == UFO_BULLET) {
                bullets[i].speed = 2 + rand() % 4;
                bullets[i].dx = -3 + (rand() % 7);
                if (burstCount && cooldown) {
                    (*burstCount)++;
                    if (*burstCount < maxBurst) {
                        *cooldown = burstCooldown;
                    } else {
                        *burstCount = 0;
                        *cooldown = postBurstCooldown;
                    }
                }
            }
            break;
        }
    }
}