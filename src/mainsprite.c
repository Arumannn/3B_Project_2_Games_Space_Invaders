//===============================================================================================================
// Program dibuat oleh: Arman Yusuf Rifandi
// NIM                : 241511038
// Deskripsi File     : Semua yang berkaitan dengan spaceship/player termasuk collison, bullet dan move
// Proyek 2           : Game Space Invaders 
//===============================================================================================================
#include <graphics.h>
#include "mainsprite.h"
#include "alien.h"
#include "ufo.h"
#include "gameplay.h"
#include "mainmenu.h"

// Variabel Global
Explosion playerExplosions[MAX_EXPLOSIONS]; 
BulletNode *playerBullets = NULL;
extern BulletNode* alienBullets;
extern BulletNode* ufoBulletList;
int shootCooldown = 0;


// Menggambar bagaimana bentuk dan wujud Player
void DrawSpaceShip(Player *player) {
    if (!player->alive) return;
    if (player->invincible && (player->invincibleTimer / 5) % 2 == 0) return; // Drawing pada Spaceship akan di skip jika player sedang Invincible

    int x = player->X_Player;
    int y = player->Y_Player;
    int imgWidth = 80;
    int imgHeight = 80;

    readimagefile("img/spaceship_1.bmp", x - imgWidth/2, y, x + imgWidth/2, y + imgHeight);
}



// Menginilisasi pergerakan pada player
void SpaceshipMove(Player *player) {
    if (player->respawning) return; 

    if ((GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000) && player->X_Player > 40) {
        player->X_Player -= 15;
    }
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) && player->X_Player < getmaxx() - 40) {
        player->X_Player += 15;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && shootCooldown <= 0) {
        ShootBullet(player);
        shootCooldown = 3; // Batas dari peluru yang bisa dikeluarkan
    }
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        showMainMenu(); // Akan dianggap gameover ketika player menekekan Escape
    }
    if (player->invincible) {
        player->invincibleTimer--; // Menjadikan player kebal terhadap peluru
        if (player->invincibleTimer <= 0) {
            player->invincible = 0; 
        }
    }
}

// Mengeluarkan bullet didepan player ketika menekana spasi
void ShootBullet(Player *player) {
    int bulletCount = 0;
    BulletNode *current = playerBullets;
    while (current) {
        bulletCount++;
        current = current->next;
    }
    if (bulletCount < MAX_BULLETS) {
        BulletNode *newBullet = (BulletNode *)malloc(sizeof(BulletNode)); 
        if (!newBullet) return;

        newBullet->bullet.x = player->X_Player;
        newBullet->bullet.y = player->Y_Player - 10; // Hanya memunculkan bulletnya saja tanpa ada pergerakan
        newBullet->bullet.active = 1;
        newBullet->next = playerBullets;
        playerBullets = newBullet;

        PlaySound(TEXT("sound/Shooting_Audio.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
}


// Pergerakan bullet maju mendekati alien
void updateBullets() {
    BulletNode *current = playerBullets;
    BulletNode *prev = NULL;

    while (current) {
        current->bullet.y -= BLOCK_SIZE;
        if (current->bullet.y < 0) {
            BulletNode *temp = current;
            if (!prev) playerBullets = current->next;
            else prev->next = current->next;
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
    if (shootCooldown > 0) shootCooldown--;
}


//Drawing pada peluru
void drawBullets() {
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);

    BulletNode *current = playerBullets;
    while (current) {
        if (current->bullet.active) {
            bar(current->bullet.x - BLOCK_SIZE / 4, current->bullet.y, 
                current->bullet.x + BLOCK_SIZE / 4, current->bullet.y + BLOCK_SIZE);
        }
        current = current->next;
    }
}


// Menciptakan tabrakan pada hitbox peluru dan player
int isColliding(int l1, int r1, int t1, int b1, int l2, int r2, int t2, int b2) {
    return !(r1 < l2 || l1 > r2 || b1 < t2 || t1 > b2);
}


// Membuat efek tabrakan ketika player terkena peluru
void checkPlayerCollisions(Player *player) {
    if (!player->alive || player->respawning || player->invincible) return;

    // Inisaliasi hitbox pada player
    int playerLeft   = player->X_Player - PLAYER_HITBOX_WIDTH / 2;
    int playerRight  = player->X_Player + PLAYER_HITBOX_WIDTH / 2;
    int playerTop    = player->Y_Player + 20;
    int playerBottom = player->Y_Player + 20 + PLAYER_HITBOX_HEIGHT;

    BulletNode *currentAlienBullet = alienBullets;
    while (currentAlienBullet != NULL) {
        if (currentAlienBullet->bullet.active) {
            // Inisliasi hitbox pada peluru
            int bulletLeft   = currentAlienBullet->bullet.x;
            int bulletRight  = currentAlienBullet->bullet.x + BLOCK_SIZE / 2;
            int bulletTop    = currentAlienBullet->bullet.y;
            int bulletBottom = currentAlienBullet->bullet.y + BLOCK_SIZE;

            // Memeriksa hasil dari hitbox tersebut
            if (isColliding(playerLeft, playerRight, playerTop, playerBottom,
                            bulletLeft, bulletRight, bulletTop, bulletBottom)) {
                currentAlienBullet->bullet.active = 0;
                player->health--;
                resetPlayer(player);
                return;
            }
        }
        currentAlienBullet = currentAlienBullet->next;
    }

    BulletNode *currentUfoBullet = ufoBulletList;
    while (currentUfoBullet != NULL) {
        if (currentUfoBullet->bullet.active) {
            //Inisilasasi hitbox pada ufo bullet
            int bulletLeft   = currentUfoBullet->bullet.x - 3;
            int bulletRight  = currentUfoBullet->bullet.x + 3;
            int bulletTop    = currentUfoBullet->bullet.y - 3;
            int bulletBottom = currentUfoBullet->bullet.y + 3;

            // Menentukan hasil dari hitbox ufo bullet dan player
            if (isColliding(playerLeft, playerRight, playerTop, playerBottom,
                            bulletLeft, bulletRight, bulletTop, bulletBottom)) {
                currentUfoBullet->bullet.active = 0;
                player->health--;
                resetPlayer(player);
                return;
            }
        }
        currentUfoBullet = currentUfoBullet->next;
    }
}


/// Memangil kembali player ke posisi semula ketika bermain dikarenkan terkena collison
void resetPlayer(Player *player) {
    player->alive = 0;
    player->respawning = 1;
    player->respawnTimer = 30; // Jeda untuk respawn player

    for (int j = 0; j < MAX_EXPLOSIONS; j++) {
        if (!playerExplosions[j].active) {
            //Memanggil design ledakan dan efeknya
            playerExplosions[j].x = player->X_Player;
            playerExplosions[j].y = player->Y_Player;
            playerExplosions[j].active = 1;
            playerExplosions[j].lifetime = 0;
            break;
        }
    }
}

void updateExplosionsPlayer() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (playerExplosions[i].active) {
            if (playerExplosions[i].lifetime == 0) {
                PlaySound(TEXT("sound/Player_Explosion.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
            playerExplosions[i].lifetime++; // Update efek ledakan
            if (playerExplosions[i].lifetime >= 20) {
                playerExplosions[i].active = 0;
                playerExplosions[i].lifetime = 0;
            }
        }
    }
}

// Update Player ketika respawn
void updatePlayerRespawn(Player *player) {
    if (player->respawning) {
        updateExplosionsPlayer();
        drawExplosionsPlayer();

        player->respawnTimer--;
        if (player->respawnTimer <= 0) {
            if (player->health > 0) {
                player->X_Player = getmaxx() / 2;
                player->Y_Player = getmaxy() - 100;
                player->alive = 1;
                player->invincible = 1;
                player->invincibleTimer = 90;
                player->respawning = 0;
            } else {
                player->respawning = 0;
                player->alive = 0;
                gameOverScreen(); 
            }
        }
    }
}

// Inisasi Ledakan player
void initExplosionsPlayer() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        playerExplosions[i].active = 0;
        playerExplosions[i].lifetime = 0;
    }
}

// Desing ledakan player
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
                playerExplosions[i].active = 0;
                playerExplosions[i].lifetime = 0;
            }
            playerExplosions[i].lifetime++;
        }
    }
}

//Menggambar nyawa
void drawLives(int lives) {
    int screenWidth = getmaxx();
    int startX = screenWidth - 50;
    int startY = 40;
    int heartSize = 40;
    int spacing = 40;

    for (int i = 0; i < lives; i++) {
        int x = startX - (i * spacing);
        int y = startY;
        int imgWidth = heartSize;
        int imgHeight = heartSize;

        readimagefile("img/lives.bmp", x - imgWidth/2, y - imgHeight/2, x + imgWidth/2, y + imgHeight/2);
    }
}


