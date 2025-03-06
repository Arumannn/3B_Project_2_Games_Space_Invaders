#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "mainmenu.h"

// Fungsi untuk menggambar teks di tengah koordinat tertentu
void drawText(int x, int y, const char* text, int size, int color) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    char tempText[100];
    strncpy(tempText, text, sizeof(tempText) - 1);
    tempText[sizeof(tempText) - 1] = '\0';

    outtextxy(x, y, tempText);
}

// Fungsi untuk menggambar alien
void drawAlien(int x, int y, int size, int color) {
    setcolor(color);
    for (int i = 0; i < size / 10; i++) {
        rectangle(x + i, y + i, x + size - i, y + size - i);
    }
    floodfill(x + size / 2, y + size / 2, color);
}

// Fungsi untuk menggambar bintang berbentuk segi lima
void drawStar(int x, int y, int size, int color) {
    setcolor(color);
    int starPoints[] = {
        x, y - size,
        x + size, y + size / 2,
        x - size, y + size / 2,
        x + size / 2, y + size,
        x - size / 2, y + size,
        x, y - size
    };
    drawpoly(6, starPoints);
    floodfill(x, y, color);
}

// Fungsi untuk menggambar bintang di latar belakang
void drawStars(int numStars) {
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    for (int i = 0; i < numStars; i++) {
        int x = rand() % screenWidth;
        int y = rand() % screenHeight;
        drawStar(x, y, 5, WHITE);
    }
}

// Fungsi untuk menampilkan menu utama dalam mode fullscreen
void showMainMenu() {
    int gd = DETECT, gm;
    initwindow(getmaxwidth(), getmaxheight(), "Space Invaders");

    if (graphresult() != grOk) {
        printf("Graphics initialization failed\n");
        exit(1);
    }

    setbkcolor(BLACK);
    cleardevice();

    int screenWidth = getmaxx();
    int screenHeight = getmaxy();

    drawStars(100);

    drawText(screenWidth / 2, screenHeight / 6, "SPACE INVADERS", 6, WHITE);

    int btn_width = screenWidth / 5;
    int btn_height = screenHeight / 12;
    int btn_x = (screenWidth - btn_width) / 2;
    int btn_y = screenHeight / 3;

    setcolor(WHITE);
    rectangle(btn_x, btn_y, btn_x + btn_width, btn_y + btn_height);
    line(btn_x, btn_y, btn_x + btn_width, btn_y + btn_height);
    line(btn_x + btn_width, btn_y, btn_x, btn_y + btn_height);
    floodfill(btn_x + 1, btn_y + 1, WHITE);

    setbkcolor(BLACK);
    drawText(btn_x + btn_width / 2, btn_y + btn_height / 2, "Play", 4, WHITE);

    int alien_size = screenWidth / 12;
    int alien_y = btn_y + btn_height + screenHeight / 10;

    drawAlien(screenWidth / 3 - alien_size / 2, alien_y, alien_size, WHITE);
    drawAlien(screenWidth / 2 - alien_size / 2, alien_y, alien_size, WHITE);
    drawAlien(2 * screenWidth / 3 - alien_size / 2, alien_y, alien_size, WHITE);

    int isRunning = 1;
    while (isRunning) {
        if (kbhit()) {
            char key = getch();
            if (key == 27) {
                isRunning = 0;
            }
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);
    
            if (mx >= btn_x && mx <= btn_x + btn_width &&
                my >= btn_y && my <= btn_y + btn_height) {
                cleardevice();
                drawStars(100);
                drawText(screenWidth / 2, screenHeight / 2, "Game Started...", 4, WHITE);
                delay(2000);
                isRunning = 0;
            }
        }
    }
}
