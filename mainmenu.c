#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mainsprite.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include "mainmenu.h"

// Fungsi untuk menggambar teks di tengah
void drawText(int x, int y, const char* text, int size, int color) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    char tempText[100];
    strncpy(tempText, text, sizeof(tempText) - 1);
    tempText[sizeof(tempText) - 1] = '\0';

    outtextxy(x, y, tempText);
}

// Fungsi untuk menggambar footer di setiap halaman
void drawFooter() {
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    int footerHeight = 20;  // Kembalikan ke ukuran awal
    int footerY = screenHeight - footerHeight;  

    // Latar belakang footer (putih)
    setfillstyle(SOLID_FILL, WHITE);
    bar(0, footerY, screenWidth, screenHeight);

    // Garis batas atas footer (hitam)
    setcolor(BLACK);
    line(0, footerY, screenWidth, footerY);  

    // Teks di tengah dengan perhitungan posisi yang lebih akurat
    setbkcolor(WHITE);
    setcolor(BLACK);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);  // Ukuran teks kembali ke awal
    int textWidth = textwidth("Space Invaders | Kelompok 3 | Proyek 2025");
    int textHeight = textheight("A");  // Untuk memastikan teks sejajar vertikal
    outtextxy((screenWidth - textWidth) / 2, footerY + (footerHeight - textHeight) / 2, "Space Invaders | Kelompok 3 | Proyek 2025");

    setbkcolor(BLACK); // Kembalikan latar belakang ke hitam setelah footer ditulis
}

// Fungsi untuk menggambar tombol
void drawButton(int x, int y, int width, int height, int color, const char *label) {
    setcolor(WHITE);
    rectangle(x, y, x + width, y + height);
    setfillstyle(SOLID_FILL, color);
    floodfill(x + 1, y + 1, WHITE);
    setbkcolor(color);
    drawText(x + width / 2, y + height / 2, label, 3, BLACK);
    setbkcolor(BLACK);
}

// Fungsi untuk menggambar background bintang
void drawStars() {
    cleardevice();
    for (int i = 0; i < 200; i++) {
        int x = rand() % getmaxwidth();
        int y = rand() % getmaxheight();
        putpixel(x, y, WHITE);
    }
}

// Fungsi Leaderboard
void drawLeaderboard() {
    int x = getmaxwidth() - 320; // Geser lebih ke kiri agar seimbang
    int y = 150;  
    int width = 280; // Lebarkan tabel
    int height = 150; 

    setcolor(GREEN);
    rectangle(x, y, x + width, y + height); // Kotak utama leaderboard

    // Garis horizontal untuk memisahkan header
    line(x, y + 30, x + width, y + 30);

    // Garis vertikal pemisah kolom (disesuaikan agar lebih proporsional)
    line(x + 50, y, x + 50, y + height);  // Kolom "No"
    line(x + 180, y, x + 180, y + height); // Kolom "Player" (dilebarkan sedikit)

    // Header leaderboard (digeser sedikit ke bawah agar tengah)
    drawText(x + 25, y + 8, "NO", 2, WHITE);
    drawText(x + 115, y + 8, "PLAYER", 2, WHITE);
    drawText(x + 210, y + 8, "SCORE", 2, WHITE);

    // Data leaderboard
    char names[3][30] = {"Arman", "Nazriel", "Rina"};
    int scores[3] = {2000, 1500, 1000};

    for (int i = 0; i < 3; i++) {
        char numText[5], scoreText[10];
        sprintf(numText, "%d", i + 1);
        sprintf(scoreText, "%d", scores[i]);

        // Posisi teks lebih direnggangkan agar lebih rapi
        drawText(x + 20, y + 45 + (i * 35), numText, 2, WHITE); // No
        drawText(x + 100, y + 45 + (i * 35), names[i], 2, WHITE); // Player
        drawText(x + 225, y + 45 + (i * 35), scoreText, 2, WHITE); // Score
    }
}

// Fungsi untuk menampilkan menu utama
void showMainMenu() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    initwindow(screenWidth, screenHeight, "Space Invaders");
    cleardevice();
    drawStars();

    // Judul tetap di posisinya
    int titleX = getmaxwidth() / 2;
    int titleY = 120; // Sedikit lebih naik agar terlihat lebih seimbang
    drawText(titleX, titleY, "SPACE INVADERS", 8, WHITE);

    // Posisi tombol lebih dinaikkan agar dekat dengan judul
    int centerX = getmaxwidth() / 2 - 200;
    int startY = getmaxheight() / 2 - 120; // Sebelumnya -80, sekarang lebih naik
    int buttonWidth = 400;
    int buttonHeight = 80;
    int buttonSpacing = 110; // Jarak antar tombol lebih proporsional

    drawButton(centerX, startY, buttonWidth, buttonHeight, LIGHTGREEN, "START");
    drawButton(centerX, startY + buttonSpacing, buttonWidth, buttonHeight, YELLOW, "GUIDE");
    drawButton(centerX, startY + (buttonSpacing * 2), buttonWidth, buttonHeight, WHITE, "EXIT");

    // Leaderboard tetap di pojok kanan atas
    drawLeaderboard();
    drawFooter(); 
    handleMainMenu();
}

// Fungsi untuk menangani input menu utama
void handleMainMenu() {
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= 400 && x <= 800) {
                if (y >= 300 && y <= 380) startGame();
                else if (y >= 420 && y <= 500) showGuide();
                else if (y >= 540 && y <= 620) if (confirmExit()) exit(0);
            }
        }
    }
}

// Fungsi untuk menampilkan aturan game
void showGuide() {
    drawStars();
    drawText(getmaxwidth() / 2, 100, "GUIDE", 5, WHITE);
    drawText(getmaxwidth() / 2, 200, "Gunakan panah untuk bergerak", 3, WHITE);
    drawText(getmaxwidth() / 2, 250, "Tekan spasi untuk menembak", 3, WHITE);
    drawText(getmaxwidth() / 2, 300, "Hindari tembakan musuh", 3, WHITE);
    drawButton(getmaxwidth() / 2 - 100, 400, 200, 50, RED, "BACK");
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= getmaxwidth() / 2 - 100 && x <= getmaxwidth() / 2 + 100 && y >= 400 && y <= 450) {
                showMainMenu();
                return;
            }
        }
    }
}

// Fungsi untuk konfirmasi keluar
int confirmExit() {
    drawStars();
    drawText(getmaxwidth() / 2, 200, "Anda yakin ingin keluar?", 3, WHITE);
    drawButton(getmaxwidth() / 2 - 120, 300, 100, 50, RED, "YES");
    drawButton(getmaxwidth() / 2 + 20, 300, 100, 50, GREEN, "NO");
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= getmaxwidth() / 2 - 120 && x <= getmaxwidth() / 2 - 20 && y >= 300 && y <= 350) return 1;
            if (x >= getmaxwidth() / 2 + 20 && x <= getmaxwidth() / 2 + 120 && y >= 300 && y <= 350) return 0;
        }
    }
}
