#include <graphics.h>
#include "ufo.h"


// Inisialisasi UFO
int ufoX = 100, ufoY = 100, ufoDirection = 1;
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

void moveUFO(){
    // Update dan gambar UFO
    ufoX += ufoDirection * 5;
    if (ufoX > getmaxx() - 60 || ufoX < 60) ufoDirection *= -100;
    drawUFO(ufoX, ufoY);

    // UFO menembak
    if (ufoBulletActive) {
        ufoBulletY += 10;
        drawBullet(ufoBulletX, ufoBulletY);
        if (ufoBulletY > getmaxy()) ufoBulletActive = 0;
    } else {
        ufoBulletX = ufoX;
        ufoBulletY = ufoY + 20;
        ufoBulletActive = 1;
    }
}
