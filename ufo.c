#include <graphics.h>
#include "ufo.h"
#include "alien.h"

// Inisialisasi UFO
float ufoX = 100.0, ufoY = 70.0; // Geser ke bawah sedikit (sebelumnya 30.0)
float ufoSpeed = 2.5;
int ufoDirection = 1;    
int page = 0; // Untuk double buffering

// Inisialisasi peluru UFO
int ufoBulletX = -1, ufoBulletY = -1, ufoBulletActive = 0;

void drawUFO(int x, int y) {
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    ellipse(x, y, 0, 360, 60, 25);
    floodfill(x, y, RED);
    
    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    fillellipse(x, y - 15, 25, 15);
    
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x, y, 30, 10);
    
    setcolor(DARKGRAY);
    line(x - 40, y + 15, x - 50, y + 35);
    line(x + 40, y + 15, x + 50, y + 35);
    
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    for (int i = -40; i <= 40; i += 20) {
        fillellipse(x + i, y + 5, 5, 5);
    }
}

void drawBullet(int bx, int by) {
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(bx, by, 3, 3);
}

void UFO(Alien aliens[]) {
    // Update dan gambar UFO
    ufoX += ufoDirection * ufoSpeed;
    if (ufoX > getmaxx() - 60 || ufoX < 60) ufoDirection *= -1;
    drawUFO((int)ufoX, (int)ufoY);

    // UFO menembak
    if (ufoBulletActive) {
        ufoBulletY += 10;
        drawBullet(ufoBulletX, ufoBulletY);
        if (ufoBulletY > getmaxy()) ufoBulletActive = 0;
    } else {
        ufoBulletX = (int)ufoX;
        ufoBulletY = (int)ufoY + 20;
        ufoBulletActive = 1;
    }

    // Deteksi tabrakan dengan alien
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int ufoLeft = (int)ufoX - 60;   // Batas kiri UFO
            int ufoRight = (int)ufoX + 60;  // Batas kanan UFO
            int ufoTop = (int)ufoY - 25;    // Batas atas UFO
            int ufoBottom = (int)ufoY + 25; // Batas bawah UFO

            int alienLeft = aliens[i].x;
            int alienRight = aliens[i].x + BLOCK_SIZE;
            int alienTop = aliens[i].y;
            int alienBottom = aliens[i].y + BLOCK_SIZE;

            // Cek apakah ada tabrakan
            if (ufoRight > alienLeft && ufoLeft < alienRight &&
                ufoBottom > alienTop && ufoTop < alienBottom) {
                // Tabrakan terjadi, nonaktifkan alien
                aliens[i].active = 0; // Alien dihancurkan saat tabrakan
            }
        }
    }
}