#include "mainmenu.h"
#include <graphics.h>
#include <conio.h>
#include <windows.h>




void drawText(char *text, int x, int y, int color) {
    setcolor(color);
    outtextxy(x, y, text);
}

void drawButton(char *label, int x, int y, int width, int height, int color) {
    setcolor(color);
    rectangle(x, y, x + width, y + height);
    drawText(label, x + (width / 4), y + (height / 4), color);
}

void showMainMenu() {
    DWORD screenwidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenheight = GetSystemMetrics(SM_CXSCREEN);

    initwindow(screenwidth, screenheight, "");  // Jika BGI di luar MinGW, gunakan path lengkap

    // Set latar belakang hitam
    setbkcolor(BLACK);
    cleardevice();

    // Judul game
    setcolor(GREEN);
    drawText("SPACE", 200, 100, GREEN);
    drawText("INVADERS", 260, 150, RED);

    // Gambar tombol Play
    drawButton("PLAY!", 250, 250, 100, 50, GREEN);

    getch();  // Tunggu input sebelum keluar
}    