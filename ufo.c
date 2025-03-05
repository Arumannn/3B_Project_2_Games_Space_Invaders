#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  // PENTING supaya bisa pake GetSystemMetrics()

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
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);  // FIX ERROR DI SINI!
    outtextxy(10, 10, (char*)"UFO Shooter");      // FIX CONVERSION ERROR!
}

int main() {
    int width = GetSystemMetrics(SM_CXSCREEN);  // Ambil ukuran layar width
    int height = GetSystemMetrics(SM_CYSCREEN); // Ambil ukuran layar height

    int gd = DETECT, gm;
    initwindow(width, height, "UFO Game", 0, 0);  // INI FIXED, TEU MAKE FULLSCREEN ERROR!

    setbkcolor(BLACK);
    cleardevice();  

    int x = 100, y = 100, direction = 1;
    int bx = -1, by = -1, bulletActive = 0;
    srand(time(0));

    int page = 0; 

    while (!kbhit() || getch() != 27) {
        setactivepage(page);
        cleardevice();
        drawHeader();
        
        x += direction * 5;
        if (x > getmaxx() - 60 || x < 60) direction *= -1;
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

        setvisualpage(page);
        delay(50);
        page = 1 - page;
    }

    closegraph();
    return 0;
}
