#include <graphics.h>
#include "mainsprite.h"
#include "ufo.h"
#include "alien.h"
#include "gameplay.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define FIREBALL_SPEED 12

// Variabel peluru player (harus didefinisikan di tempat lain)
extern BulletNode *playerBullets;

// Inisialisasi UFO
float ufoX = 100.0, ufoY = 150.0;
float ufoSpeed = 10.0;
int ufoDirection = 1;
int ufoActive = 1;
int ufoHealth = 20;
const int ufoMaxHealth = 20;
int ufoRespawnDelay = 0;

// Peluru UFO (linked list)
BulletNode *ufoBulletList = NULL;
int ufoBulletCount = 0;

int ufoBurstCount = 0;
int ufoShootCooldown = 0;

void someFunction() {
    BulletNode* newNode = (BulletNode*)malloc(sizeof(BulletNode)); // Deklarasi pointer
    
    // kode lainnya yang menggunakan newNode 
}    

void shootUFOBullet() {
    if (!ufoActive || ufoShootCooldown > 0) {
        ufoShootCooldown--;
        return;
    }

    if (ufoBurstCount >= 3) {
        ufoBurstCount = 0;
        ufoShootCooldown = 80;
        return;
    }

    if (ufoBulletCount >= MAX_UFO_BULLETS) return;

    int offsetX = (rand() % 21) - 10;
    BulletNode* newBullet = (BulletNode*)malloc(sizeof(BulletNode));

    if (newBullet == NULL) return;

    newBullet->bullet.x = ufoX + offsetX; 
    newBullet->bullet.y = ufoY + 20;
    newBullet->bullet.active = 1;
    newBullet->bullet.speed = (rand() % 4) + 8;
    int direction = rand() % 3 - 1;
    newBullet->bullet.dx = direction * 2;
    newBullet->bullet.type = UFO_BULLET;
    newBullet->next = ufoBulletList;
    ufoBulletList = newBullet;
    ufoBulletCount++;

    ufoBurstCount++;
    ufoShootCooldown = 10;
}

void updateUFOBullets() {
    BulletNode *current = ufoBulletList;
    BulletNode *prev = NULL;
    while (current != NULL) {
        current->bullet.y += current->bullet.speed;
        current->bullet.x += current->bullet.dx;

        if (current->bullet.y > getmaxy() ||
            current->bullet.x < 0 ||
            current->bullet.x > getmaxx()) {
            BulletNode *toDelete = current;
            if (prev == NULL) {
                ufoBulletList = current->next;
                current = ufoBulletList;
            } else {
                prev->next = current->next;
                current = prev->next;
            }
            free(toDelete);
            ufoBulletCount--;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void drawUFOBullets() {
    BulletNode *current = ufoBulletList;
    while (current != NULL) {
        int bx = current->bullet.x;
        int by = current->bullet.y;

        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(bx, by, 6, 6);

        setcolor(14);
        setfillstyle(SOLID_FILL, 14);
        fillellipse(bx, by, 4, 4);

        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(bx, by, 2, 2);

        current = current->next;
    }
}

void UFO(Alien aliens[ALIEN_ROWS][ALIEN_COLS]) {
    if (!ufoActive) {
        if (ufoRespawnDelay > 0) {
            ufoRespawnDelay--;
        } else {
            ufoX = (rand() % (getmaxx() - 120)) + 60;
            ufoY = 100;
            ufoHealth = ufoMaxHealth;
            ufoActive = 1;
            ufoDirection = (rand() % 2) == 0 ? 1 : -1;
        }
        return;
    }

    ufoX += ufoDirection * ufoSpeed;
    if (ufoX > getmaxx() - 60 || ufoX < 60) ufoDirection *= -1;

    drawUFO((int)ufoX, (int)ufoY);
    shootUFOBullet();
    updateUFOBullets();
    drawUFOBullets();

    BulletNode *current = playerBullets;
    while (current != NULL) {
        if (current->bullet.active &&
            current->bullet.x > ufoX - 45 &&
            current->bullet.x < ufoX + 45 &&
            current->bullet.y > ufoY - 40 &&
            current->bullet.y < ufoY + 20) {

            current->bullet.active = 0;
            ufoHealth--;

            if (ufoHealth <= 0) {
                drawExplosion((int)ufoX, (int)ufoY);
                PlaySound(TEXT("sound/UFO_Died.wav"), NULL, SND_FILENAME | SND_ASYNC);
                ufoActive = 0;
                addUFOScore();
                ufoRespawnDelay = (rand() % 5 + 3) * 30;
                break;
            }
        }
        current = current->next;
    }
}

void initUFO() {
    srand(time(NULL));
    ufoHealth = 15 + rand() % 6;
}

void drawExplosion(int x, int y) {
    for (int radius = 10; radius <= 70; radius += 7) {
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(x, y, radius, radius);

        setcolor(14);
        setfillstyle(SOLID_FILL, 14);
        fillellipse(x, y, radius - 7, radius - 7);

        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x, y, radius - 14, radius - 14);

        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(x, y, radius - 21, radius - 21);

        delay(5);
    }
}

void drawUFO(int x, int y) {
    if (!ufoActive) return;
    y -= 40;

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, 60, 25);

    setcolor(LIGHTRED);
    setfillstyle(SOLID_FILL, LIGHTRED);
    fillellipse(x, y - 10, 50, 15);

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y - 20, 40, 15);
    setcolor(LIGHTCYAN);
    fillellipse(x - 15, y - 25, 8, 8);

    for (int i = -40; i <= 40; i += 20) {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, DARKGRAY);
        fillellipse(x + i, y + 15, 6, 6);

        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x + i, y + 15, 4, 4);
    }

    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, 3);
    line(x - 45, y + 20, x - 55, y + 45);
    line(x + 45, y + 20, x + 55, y + 45);
    rectangle(x - 55, y + 45, x - 50, y + 50);
    rectangle(x + 50, y + 45, x + 55, y + 50);
    floodfill(x - 52, y + 47, DARKGRAY);
    floodfill(x + 52, y + 47, DARKGRAY);

    int barWidth = 50;
    int barHeight = 5;
    int barX = x - barWidth / 2;
    int barY = y - 50;

    setcolor(WHITE);
    rectangle(barX, barY, barX + barWidth, barY + barHeight);

    int hpWidth = (ufoHealth * barWidth) / ufoMaxHealth;
    if (hpWidth > 0) {
        setfillstyle(SOLID_FILL, GREEN);
        bar(barX, barY, barX + hpWidth, barY + barHeight);
    }
}
