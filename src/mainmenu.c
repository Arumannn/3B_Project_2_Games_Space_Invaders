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

void mainMenu() {
    initwindow(getmaxwidth(), getmaxheight(), "Main Menu");
    cleardevice();
    
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
    outtextxy(getmaxwidth() / 2 - 100, 100, (char *)"MAIN MENU");

    getch();
    closegraph();
}

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
void drawLeaderboard(int yOffset) {
    int x = getmaxwidth() / 2 + 50; // Geser leaderboard agar sisi kanan simetris dengan tombol
    int y = yOffset; // Posisi leaderboard berdasarkan parameter

    int width = 350; 
    int height = 200;

    setcolor(GREEN);
    rectangle(x, y, x + width, y + height); // Kotak utama leaderboard

    // Garis horizontal untuk memisahkan header
    line(x, y + 30, x + width, y + 30);  // Naikkan pemisah header sedikit

    // Garis vertikal pemisah kolom
    line(x + 60, y, x + 60, y + height);  // Kolom "No"
    line(x + 220, y, x + 220, y + height); // Kolom "Player"

    // Header leaderboard
    drawText(x + 30, y + 15, "NO", 2.5, WHITE);
    drawText(x + 140, y + 15, "PLAYER", 2.5, WHITE);
    drawText(x + 285, y + 15, "SCORE", 2.5, WHITE);

    // Data leaderboard
    char names[3][30] = {"Arman", "Nazriel", "Rina"};
    int scores[3] = {2000, 1500, 1000};

    for (int i = 0; i < 3; i++) {
        char numText[5], scoreText[10];
        sprintf(numText, "%d", i + 1);
        sprintf(scoreText, "%d", scores[i]);

        drawText(x + 30, y + 45 + (i * 40), numText, 2.5, WHITE); // No
        drawText(x + 140, y + 45 + (i * 40), names[i], 2.5, WHITE); // Player
        drawText(x + 285, y + 45 + (i * 40), scoreText, 2.5, WHITE); // Score
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

    // Geser tombol lebih ke kiri
    int centerX = getmaxwidth() / 2 - 350;  // Geser tombol ke kiri
    int startY = getmaxheight() / 2 - 120;
    int buttonWidth = 400;
    int buttonHeight = 80;
    int buttonSpacing = 110; // Jarak antar tombol lebih proporsional

    drawButton(centerX, startY, buttonWidth, buttonHeight, LIGHTGREEN, "START");
    drawButton(centerX, startY + buttonSpacing, buttonWidth, buttonHeight, YELLOW, "GUIDE");
    drawButton(centerX, startY + (buttonSpacing * 2), buttonWidth, buttonHeight, WHITE, "EXIT");

    // Geser leaderboard lebih ke bawah agar seimbang dengan tombol
    drawLeaderboard(startY + (buttonSpacing * 3) + 50); 

    handleMainMenu();
}

// Fungsi untuk menangani input menu utama
void handleMainMenu() {
    int centerX = getmaxwidth() / 2 - 200;
    int startY = getmaxheight() / 2 - 120;
    int buttonWidth = 400;
    int buttonHeight = 80;
    int buttonSpacing = 110;

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            // Cek apakah klik di tombol "START"
            if (x >= centerX && x <= centerX + buttonWidth &&
                y >= startY && y <= startY + buttonHeight) {
                startGame();
                return;
            }

            // Cek apakah klik di tombol "GUIDE"
            if (x >= centerX && x <= centerX + buttonWidth &&
                y >= startY + buttonSpacing && y <= startY + buttonSpacing + buttonHeight) {
                showGuide();
                return;
            }

            // Cek apakah klik di tombol "EXIT"
            if (x >= centerX && x <= centerX + buttonWidth &&
                y >= startY + 2 * buttonSpacing && y <= startY + 2 * buttonSpacing + buttonHeight) {
                    if (confirmExit() == 1) {
                        exit(0);
                    } else {
                        showMainMenu();
                    }
                return;
            }
        }
    }
}

// Fungsi untuk menampilkan aturan game
void showGuide() {
    drawStars();
    drawText(getmaxwidth() / 2, 100, "GUIDE", 5, WHITE);
    drawText(getmaxwidth() / 2, 200, "Gunakan Tombol A dan D atau panah untuk bergerak", 3, WHITE);
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

// Fungsi untuk EXIT
int confirmExit() {
    cleardevice();
    drawStars();
    drawText(getmaxwidth() / 2, 200, "Anda yakin ingin keluar?", 3, WHITE);
    drawButton(getmaxwidth() / 2 - 120, 300, 100, 50, RED, "YES");
    drawButton(getmaxwidth() / 2 + 20, 300, 100, 50, GREEN, "NO");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);

            // Jika klik tombol YES (Keluar dari game)
            if (x >= getmaxwidth() / 2 - 120 && x <= getmaxwidth() / 2 - 20 && y >= 300 && y <= 350) {
                closegraph(); // Menutup jendela grafis
                return 1;      // Kembali ke handleMainMenu untuk exit(0)
            }

            // Jika klik tombol NO (Kembali ke Main Menu)
            if (x >= getmaxwidth() / 2 + 20 && x <= getmaxwidth() / 2 + 120 && y >= 300 && y <= 350) {
                cleardevice(); // Bersihkan layar sebelum kembali
                return 0;      // Kembali ke main menu
            }
        }
    }
}