#include <graphics.h>
#include "mainsprite.h"
#include "ufo.h"
#include "score.h"
#include "alien.h"
#include <stdlib.h>

// Inisialisasi UFO
float ufoX = 100.0, ufoY = 100.0;
float ufoSpeed = 2.5;
int ufoDirection = 1;    
int ufoActive = 1;
int ufoHealth = 15 + rand() % 6; // HP antara 15 - 20
const int ufoMaxHealth = 20;

// Inisialisasi peluru UFO menggunakan ADT AlienBullet
AlienBullet ufoBullets[MAX_ALIEN_BULLETS];
int ufoShootCooldown = 0;

void resetUFO() {
    ufoX = 100.0;
    ufoSpeed = 3.0;
    ufoDirection = 1;
    ufoHealth = 15 + rand() % 6; // HP antara 15 - 20
    ufoActive = 1;
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        ufoBullets[i].active = 0;
    }
}

void shootUFOBullet() {
    if (ufoShootCooldown > 0) {
        ufoShootCooldown--;
        return;
    }
    
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (!ufoBullets[i].active) {
            ufoBullets[i].x = ufoX;
            ufoBullets[i].y = ufoY + 20;
            ufoBullets[i].active = 1;
            ufoShootCooldown = 50; // Delay antara tembakan
            break;
        }
    }
}

void updateUFOBullets() {
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (ufoBullets[i].active) {
            ufoBullets[i].y += 5;
            if (ufoBullets[i].y > getmaxy()) {
                ufoBullets[i].active = 0;
            }
        }
    }
}

void drawUFOBullets() {
    setcolor(RED);
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (ufoBullets[i].active) {
            bar(ufoBullets[i].x - 2, ufoBullets[i].y, ufoBullets[i].x + 2, ufoBullets[i].y + 10);
        }
    }
}

void drawUFO(int x, int y) {
    y -= 40;
    
    // Body utama UFO
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, 60, 25);

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y - 5, 55, 20);

    setcolor(LIGHTRED);
    setfillstyle(SOLID_FILL, LIGHTRED);
    fillellipse(x, y - 10, 50, 15);

    // Jendela
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y - 20, 40, 15);
    setcolor(LIGHTCYAN);
    fillellipse(x - 15, y - 25, 8, 8);

    // Lampu LED
    for (int i = -40; i <= 40; i += 20) {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, DARKGRAY);
        fillellipse(x + i, y + 15, 6, 6);
        
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x + i, y + 15, 4, 4);
        
        setcolor(WHITE);
        fillellipse(x + i - 1, y + 13, 1, 1);
    }

    // Kaki UFO
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, 3);
    line(x - 45, y + 20, x - 55, y + 45);
    line(x + 45, y + 20, x + 55, y + 45);
    rectangle(x - 55, y + 45, x - 50, y + 50);
    rectangle(x + 50, y + 45, x + 55, y + 50);
    floodfill(x - 52, y + 47, DARKGRAY);
    floodfill(x + 52, y + 47, DARKGRAY);
    
    // Health bar di atas UFO
    int barWidth = 50;
    int barHeight = 5;
    int barX = x - barWidth / 2;
    int barY = y - 50;
    
    setcolor(WHITE);
    rectangle(barX, barY, barX + barWidth, barY + barHeight);
    
    int hpWidth = (ufoHealth * barWidth) / ufoMaxHealth;
    setfillstyle(SOLID_FILL, GREEN);
    bar(barX, barY, barX + hpWidth, barY + barHeight);
}

void UFO(Alien aliens[]) {
    if (!ufoActive) return;

    ufoX += ufoDirection * ufoSpeed;
    if (ufoX > getmaxx() - 60 || ufoX < 60) ufoDirection *= -1;
    
    drawUFO((int)ufoX, (int)ufoY);
    shootUFOBullet();
    updateUFOBullets();
    drawUFOBullets();

    for (int j = 0; j < MAX_BULLETS; j++) {
        if (bullets_player[j].active &&
            bullets_player[j].x > ufoX - 45 &&
            bullets_player[j].x < ufoX + 45 &&
            bullets_player[j].y > ufoY - 40 && 
            bullets_player[j].y < ufoY + 20) {
            
            bullets_player[j].active = 0;
            ufoHealth--;
            
            if (ufoHealth <= 0) {
                ufoActive = 0;
                addUFOScore();
                delay(10);
                resetUFO();
            }
        }
    }
}