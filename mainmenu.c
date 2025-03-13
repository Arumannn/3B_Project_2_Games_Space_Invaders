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

char playerName[30] = "";

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

// Fungsi untuk menampilkan leaderboard
void drawLeaderboard() {
    int x = getmaxwidth() - 320;
    int y = 100;
    rectangle(x, y, x + 300, y + 200);
    char names[3][30] = {"Arman", "Nazriel", "Rina"};
    int scores[3] = {2000, 1500, 1000};
    for (int i = 0; i < 3; i++) {
        char scoreText[50];
        sprintf(scoreText, "%d. %s - %d", i + 1, names[i], scores[i]);
        drawText(x + 150, y + 30 + (i * 40), scoreText, 2, WHITE);
    }
}

// Fungsi untuk memasukkan nama
void inputName() {
    cleardevice();
    drawStars();
    drawText(getmaxwidth() / 2, 100, "Masukkan Nama Anda: ", 3, WHITE);
    setcolor(WHITE);
    rectangle(400, 200, 900, 250);
    setbkcolor(BLACK);
    char tempName[30] = "";
    int index = 0;
    while (1) {
        char key = getch();
        if (key == 13) break; // Enter
        if (key == 8 && index > 0) { // Backspace
            tempName[--index] = '\0';
        } else if (index < 29) {
            tempName[index++] = key;
            tempName[index] = '\0';
        }
        strcpy(playerName, tempName);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(405, 205, WHITE);
        drawText(650, 225, playerName, 2, WHITE);
    }
    showMainMenu();
}

// Fungsi untuk menampilkan guide
void showGuide() {
    cleardevice();
    drawStars();
    drawText(getmaxwidth() / 2, 100, "GUIDE", 5, WHITE);
    drawText(getmaxwidth() / 2, 200, "Gunakan panah untuk bergerak, spasi untuk menembak.", 2, WHITE);
    drawText(getmaxwidth() / 2, 250, "Hindari tembakan musuh dan kalahkan semua alien!", 2, WHITE);
    drawText(getmaxwidth() / 2, 400, "Tekan ESC untuk kembali", 2, WHITE);
    while (getch() != 27); // Menunggu ESC ditekan
    showMainMenu();
}

// Fungsi untuk konfirmasi keluar
void confirmExit() {
    cleardevice();
    drawStars();
    drawText(getmaxwidth() / 2, 200, "Apakah Anda yakin ingin keluar?", 3, WHITE);
    drawButton(500, 300, 200, 80, LIGHTRED, "YA");
    drawButton(800, 300, 200, 80, LIGHTGREEN, "TIDAK");
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= 500 && x <= 700 && y >= 300 && y <= 380) {
                closegraph();
                exit(0);
            } else if (x >= 800 && x <= 1000 && y >= 300 && y <= 380) {
                showMainMenu();
                return;
            }
        }
    }
}

// Fungsi untuk menampilkan menu utama dalam fullscreen
void showMainMenu() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenWidth, screenHeight, "Space Invaders");
    setviewport(0, 0, screenWidth, screenHeight, 1);
    cleardevice();
    drawStars();
    drawText(screenWidth / 2, 100, "SPACE INVADERS", 8, WHITE);
    int btn_width = 400, btn_height = 80;
    int centerX = screenWidth / 2 - btn_width / 2;
    int startY = screenHeight / 2 - 160;
    int spacing = 30;
    drawButton(centerX, startY, btn_width, btn_height, LIGHTBLUE, "INPUT NAMA");
    drawButton(centerX, startY + btn_height + spacing, btn_width, btn_height, LIGHTGREEN, "START");
    drawButton(centerX, startY + (btn_height + spacing) * 2, btn_width, btn_height, YELLOW, "GUIDE");
    drawButton(centerX, startY + (btn_height + spacing) * 3, btn_width, btn_height, WHITE, "EXIT");
    drawLeaderboard();
    handleMainMenu();
}

// Fungsi untuk menangani input menu utama
void handleMainMenu() {
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= 440 && x <= 840) {
                if (y >= 220 && y <= 300) inputName();
                else if (y >= 330 && y <= 410) startGame();
                else if (y >= 440 && y <= 520) showGuide();
                else if (y >= 550 && y <= 630) confirmExit();
            }
        }
    }
}
