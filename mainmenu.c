#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
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

// Fungsi untuk menggambar alien sederhana
void drawAlien(int x, int y, int size, int color) {
    setcolor(color);
    rectangle(x, y, x + size, y + size);
    floodfill(x + size / 2, y + size / 2, color);
}

// Fungsi untuk menggambar bintang kecil sebagai background
void drawStars(int numStars) {
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    
    for (int i = 0; i < numStars; i++) {
        int x = rand() % screenWidth;
        int y = rand() % screenHeight;
        putpixel(x, y, WHITE);
    }
}

// Fungsi untuk menampilkan score di pojok kanan atas
void drawScore(int score) {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    drawText(getmaxx() - 100, 20, scoreText, 2, WHITE);
}

// Fungsi untuk menampilkan menu utama dalam mode fullscreen
void showMainMenu() {
    int gd = DETECT, gm;
    initwindow(getmaxwidth(), getmaxheight(), "Space Invaders");
    srand(time(NULL));

    if (graphresult() != grOk) {
        printf("Graphics initialization failed\n");
        exit(1);
    }

    setbkcolor(BLACK);
    cleardevice();

    int screenWidth = getmaxx();
    int screenHeight = getmaxy();

    drawStars(200);
    drawText(screenWidth / 2, screenHeight / 6, "SPACE INVADERS", 6, WHITE);
    drawScore(0);

    int btn_width = screenWidth / 5;
    int btn_height = screenHeight / 10;
    int btn_x = (screenWidth - btn_width) / 2;
    int btn_y = screenHeight / 3;
    
    setcolor(GREEN);
    rectangle(btn_x, btn_y, btn_x + btn_width, btn_y + btn_height);
    setfillstyle(SOLID_FILL, GREEN);
    floodfill(btn_x + 1, btn_y + 1, GREEN);
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
                drawText(screenWidth / 2, screenHeight / 2, "Game Started...", 4, WHITE);
                delay(2000);
                isRunning = 0;
            }
        }
    }
}
