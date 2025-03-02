#include <graphics.h>
#include <stdlib.h>
#include <string.h>
#include "mainmenu.h"

// Fungsi untuk inisialisasi graphics
void initGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");  // Path tidak diperlukan di sebagian besar compiler modern
    setbkcolor(BLACK);
    cleardevice();
}

// Fungsi untuk menggambar tombol
void drawButton(int x, int y, int width, int height, const char *text) {
    setcolor(WHITE);
    rectangle(x, y, x + width, y + height);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    floodfill(x + 1, y + 1, WHITE);

    // Menampilkan teks di tengah tombol
    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);

    // Gunakan array untuk kompatibilitas
    char textArray[100];
    strncpy(textArray, text, sizeof(textArray) - 1);
    textArray[sizeof(textArray) - 1] = '\0';

    // Dapatkan ukuran teks
    int textWidth = textwidth(textArray);
    int textHeight = textheight(textArray);

    // Tempatkan teks di tengah tombol
    outtextxy(x + (width - textWidth) / 2, y + (height - textHeight) / 2, textArray);
}

// Fungsi untuk menggambar judul
void drawTitle() {
    setcolor(WHITE);
    settextstyle(BOLD_FONT, HORIZ_DIR, 5);
    outtextxy(200, 50, "SPACE INVADERS");
}

// Fungsi untuk menampilkan menu utama
void showMainMenu() {
    cleardevice();
    drawTitle();

    // Menampilkan tombol
    drawButton(250, 150, 200, 50, "Play");
    drawButton(250, 220, 200, 50, "Instructions");
    drawButton(250, 290, 200, 50, "Credits");

    getch();  // Menunggu input sebelum menutup
}
