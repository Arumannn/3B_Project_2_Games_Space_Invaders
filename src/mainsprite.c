#include <graphics.h>
#include "mainsprite.h"
#include "alien.h"
#include "ufo.h"
#include "gameplay.h"
#include "mainmenu.h"

// Global Variables
Explosion playerExplosions[MAX_EXPLOSIONS];
BulletNode *playerBullets = NULL;
extern BulletNode* alienBullets;
extern BulletNode* ufoBulletList;
int shootCooldown = 0;


void DrawSpaceShip(Player *player) {
    if (!player->alive) return;

    if (player->invincible) {
        if ((player->invincibleTimer / 5) % 2 == 0) {
            return; // Flicker effect
        }
    }

    int x = player->X_Player;
    int y = player->Y_Player;

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    int body[] = {x, y, x - 20, y + 50, x + 20, y + 50, x, y};
    fillpoly(4, body);

    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y + 12, 6, 10);
    fillellipse(x, y + 28, 4, 7);

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    bar(x - 15, y + 32, x - 8, y + 35);
    bar(x + 8, y + 32, x + 15, y + 35);

    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    int leftWing[] = {x - 20, y + 50, x - 38, y + 65, x - 20, y + 70, x - 20, y + 50};
    fillpoly(4, leftWing);
    int rightWing[] = {x + 20, y + 50, x + 38, y + 65, x + 20, y + 70, x + 20, y + 50};
    fillpoly(4, rightWing);

    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    int thruster[] = {x - 8, y + 50, x + 8, y + 50, x + 5, y + 72, x - 5, y + 72, x - 8, y + 50};
    fillpoly(5, thruster);

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int flame[] = {x - 5, y + 72, x + 5, y + 72, x, y + 85, x - 5, y + 72};
    fillpoly(4, flame);
    int smallFlame[] = {x - 3, y + 77, x + 3, y + 77, x, y + 92, x - 3, y + 77};
    fillpoly(4, smallFlame);
    int tinyFlame[] = {x - 2, y + 82, x + 2, y + 82, x, y + 96, x - 2, y + 82};
    fillpoly(4, tinyFlame);
}

void SpaceshipMove(Player *player) {
    if (player->respawning) return; // Cannot move during respawn

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
    if (player->invincible) {
        player->invincibleTimer--;
        if (player->invincibleTimer <= 0) {
            player->invincible = 0;
        }
    }
}

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
        newBullet->bullet.y = player->Y_Player - 10;
        newBullet->bullet.active = 1;
        newBullet->next = playerBullets;
        playerBullets = newBullet;

        PlaySound(TEXT("sound/Shooting_Audio.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
}

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
void checkPlayerCollisions(Player *player) {
    if (!player->alive || player->respawning) return;

    // Cek peluru alien dari linked list
    BulletNode *currentAlienBullet = alienBullets;
    while (currentAlienBullet != NULL) {
        if (currentAlienBullet->bullet.active) {
            int bulletLeft = currentAlienBullet->bullet.x;
            int bulletRight = currentAlienBullet->bullet.x + BLOCK_SIZE / 2;
            int bulletTop = currentAlienBullet->bullet.y;
            int bulletBottom = currentAlienBullet->bullet.y + BLOCK_SIZE;
            int playerLeft = player->X_Player - 20;
            int playerRight = player->X_Player + 20;
            int playerTop = player->Y_Player;
            int playerBottom = player->Y_Player + 40;

            if (bulletRight > playerLeft && bulletLeft < playerRight &&
                bulletBottom > playerTop && bulletTop < playerBottom) {
                currentAlienBullet->bullet.active = 0;
                player->health--;
                resetPlayer(player);
                return;
            }
        }
        currentAlienBullet = currentAlienBullet->next;
    }

    // Cek peluru UFO dari linked list
    BulletNode *currentUfoBullet = ufoBulletList;
    while (currentUfoBullet != NULL) {
        if (currentUfoBullet->bullet.active) {
            int bulletLeft = currentUfoBullet->bullet.x - 3;
            int bulletRight = currentUfoBullet->bullet.x + 3;
            int bulletTop = currentUfoBullet->bullet.y - 3;
            int bulletBottom = currentUfoBullet->bullet.y + 3;
            int playerLeft = player->X_Player - 20;
            int playerRight = player->X_Player + 20;
            int playerTop = player->Y_Player;
            int playerBottom = player->Y_Player + 40;

            if (bulletRight > playerLeft && bulletLeft < playerRight &&
                bulletBottom > playerTop && bulletTop < playerBottom) {
                currentUfoBullet->bullet.active = 0;
                player->health--;
                resetPlayer(player);
                return;
            }
        }
        currentUfoBullet = currentUfoBullet->next;
    }
}

void resetPlayer(Player *player) {
    player->alive = 0;
    player->respawning = 1;
    player->respawnTimer = 10; // Frames

    for (int j = 0; j < MAX_EXPLOSIONS; j++) {
        if (!playerExplosions[j].active) {
            playerExplosions[j].x = player->X_Player;
            playerExplosions[j].y = player->Y_Player;
            playerExplosions[j].active = 1;
            playerExplosions[j].lifetime = 0;
            break;
        }
    }
    PlaySound(TEXT("sound/Player_Explosion.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void updateExplosionsPlayer() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (playerExplosions[i].active) {
            if (playerExplosions[i].lifetime == 0) {
                PlaySound(TEXT("sound/Player_Explosion.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
            playerExplosions[i].lifetime++;
            if (playerExplosions[i].lifetime >= 20) {
                playerExplosions[i].active = 0;
                playerExplosions[i].lifetime = 0;
            }
        }
    }
}
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
                playerExplosions[i].active = 0;
                playerExplosions[i].lifetime = 0;
            }
            playerExplosions[i].lifetime++;
        }
    }
}

void drawLives(int lives) {
    int screenWidth = getmaxx();
    int startX = screenWidth - 50;
    int startY = 50;
    int heartSize = 20;
    int spacing = 40;

    for (int i = 0; i < lives; i++) {
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(startX - (i * spacing), startY, heartSize, heartSize / 2);
    }
}

