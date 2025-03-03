#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
#include "ufo.h"

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

void drawHeader() {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(getmaxx() / 2 - 50, 10, "UFO Shooter");
}

int main() {
    int gd = DETECT, gm;
    initwindow(getmaxwidth(), getmaxheight(), "UFO Game", -3, -3);
    setbkcolor(BLACK);
    cleardevice();
    
    int x = 100, y = 100;
    int direction = 1;
    int bx = -1, by = -1;
    int bulletActive = 0;
    int playerX = getmaxx() / 2, playerY = getmaxy() - 50;
    int px = -1, py = -1;
    int playerBulletActive = 0;
    int score = 0;
    
    srand(time(0));
    
    while (!kbhit()) {
        cleardevice();
        drawHeader();
        
        x += direction * 5;
        if (x > getmaxx() - 60 || x < 60) {
            direction *= -1;
        }
        drawUFO(x, y);
        
        if (bulletActive) {
            by += 10;
            drawBullet(bx, by);
            if (by > getmaxy()) bulletActive = 0;
        } else {
            bx = x;
            by = y + 20;
            bulletActive = 1;
        }
        
        if (playerBulletActive) {
            py -= 10;
            drawBullet(px, py);
            if (py < 0) playerBulletActive = 0;
        }
        
        if (kbhit()) {
            char ch = getch();
            if (ch == ' ') {
                if (!playerBulletActive) {
                    px = playerX;
                    py = playerY - 20;
                    playerBulletActive = 1;
                }
            } else if (ch == 'a' && playerX > 50) {
                playerX -= 10;
            } else if (ch == 'd' && playerX < getmaxx() - 50) {
                playerX += 10;
            }
        }
        
        if (playerBulletActive && px > x - 50 && px < x + 50 && py > y - 20 && py < y + 20) {
            score++;
            x = rand() % (getmaxx() - 120) + 60;
            y = rand() % 100 + 50;
            playerBulletActive = 0;
        }
        
        setcolor(WHITE);
        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        outtextxy(10, 40, scoreText);
        
        delay(50);
    }
    
    closegraph();
    return 0;
}
