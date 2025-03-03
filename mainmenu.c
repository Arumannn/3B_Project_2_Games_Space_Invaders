#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "mainmenu.h"

// Fungsi untuk menggambar teks di tengah koordinat tertentu
void drawText(int x, int y, const char* text, int size, int color) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    // Salin string ke buffer lokal untuk menghindari error tipe data
    char tempText[100];
    strncpy(tempText, text, sizeof(tempText) - 1);
    tempText[sizeof(tempText) - 1] = '\0'; // Pastikan null-terminated

    // Tampilkan teks di koordinat tengah
    outtextxy(x, y, tempText);
}

// Fungsi untuk menggambar alien sederhana
void drawAlien(int x, int y, int size, int color) {
    setcolor(color);
    
    // Kepala alien
    rectangle(x, y, x + size, y + size);
    floodfill(x + size / 2, y + size / 2, color);

    // Mata
    setcolor(BLACK);
    rectangle(x + size / 4, y + size / 3, x + size / 4 + size / 5, y + size / 3 + size / 5);
    rectangle(x + size - size / 4 - size / 5, y + size / 3, x + size - size / 4, y + size / 3 + size / 5);
    floodfill(x + size / 4 + 1, y + size / 3 + 1, BLACK);
    floodfill(x + size - size / 4, y + size / 3 + 1, BLACK);

    // Kaki
    setcolor(color);
    rectangle(x + size / 4, y + size, x + size / 4 + size / 5, y + size + size / 4);
    rectangle(x + size - size / 4 - size / 5, y + size, x + size - size / 4, y + size + size / 4);
    floodfill(x + size / 4 + 1, y + size + 1, color);
    floodfill(x + size - size / 4, y + size + 1, color);
}

// Fungsi untuk menampilkan menu utama dalam mode fullscreen
void showMainMenu() {
    int gd = DETECT, gm;
    initwindow(getmaxwidth(), getmaxheight(), "Space Invaders");

    if (graphresult() != grOk) {
        printf("Graphics initialization failed\n");
        exit(1);
    }

    // Set latar belakang
    setbkcolor(BLACK);
    cleardevice();

    int screenWidth = getmaxx();
    int screenHeight = getmaxy();

    // Tampilkan judul "SPACE INVADERS" besar
    drawText(screenWidth / 2, screenHeight / 6, "SPACE INVADERS", 6, GREEN);

    // Buat tombol "Play" yang lebih besar dan tengah
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

    // Gambar 3 alien secara simetris di bawah tombol Play
    int alien_size = screenWidth / 12;
    int alien_y = btn_y + btn_height + screenHeight / 10;

    drawAlien(screenWidth / 3 - alien_size / 2, alien_y, alien_size, WHITE);
    drawAlien(screenWidth / 2 - alien_size / 2, alien_y, alien_size, WHITE);
    drawAlien(2 * screenWidth / 3 - alien_size / 2, alien_y, alien_size, WHITE);

    // Loop untuk menangkap input pengguna
    int isRunning = 1;
    while (isRunning) {
        if (kbhit()) {
            char key = getch();
            if (key == 27) { // ESC untuk keluar
                isRunning = 0;
            }
        }

        // Cek jika tombol "Play" diklik
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
