#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mainmenu.h"


// Definisi warna kuning jika belum tersedia
#ifndef YELLOW
#define YELLOW COLOR(255, 255, 0)
#endif

// Variabel global
char playerName[30] = "";
int score = 0;

// Fungsi untuk menggambar teks di tengah
void drawText(int x, int y, const char* text, int size, int color) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    // Salin string ke buffer lokal untuk menghindari error tipe data
    char tempText[100];
    strncpy(tempText, text, sizeof(tempText) - 1);
    tempText[sizeof(tempText) - 1] = '\0';

    // Tampilkan teks di koordinat tengah
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
    for (int i = 0; i < 100; i++) {
        int x = rand() % getmaxx();
        int y = rand() % getmaxy();
        putpixel(x, y, WHITE);
    }
}

// Fungsi untuk menampilkan leaderboard
void drawLeaderboard() {
    int x = getmaxx() - 220;
    int y = 100;
    rectangle(x, y, x + 200, y + 120);
    char names[3][30] = {"Arman", "Nazriel", "Rina"};
    int scores[3] = {2000, 1500, 1000};
    for (int i = 0; i < 3; i++) {
        char scoreText[50];
        sprintf(scoreText, "%d. %s - %d", i + 1, names[i], scores[i]);
        drawText(x + 100, y + 30 + (i * 30), scoreText, 2, WHITE);
    }
}

// Deklarasi fungsi sebelum digunakan


void handleMainMenu() {
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();

    int btn_width = 400, btn_height = 80;
    int centerX = screenWidth / 2 - btn_width / 2;
    int startY = screenHeight / 2 - 160;
    int spacing = 30;

    int choice = -1; // Tidak ada pilihan awal

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) { // Deteksi klik mouse
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (x >= centerX && x <= centerX + btn_width) {
                if (y >= startY && y <= startY + btn_height) { // "INPUT NAMA"
                    printf("Input Nama dipilih\n");
                } else if (y >= startY + btn_height + spacing && y <= startY + 2 * (btn_height + spacing)) { // "START"
                    startGame(); // Masuk ke gameplay TANPA closegraph()
                    return;
                } else if (y >= startY + 2 * (btn_height + spacing) && y <= startY + 3 * (btn_height + spacing)) { // "GUIDE"
                    printf("Guide dipilih\n");
                } else if (y >= startY + 3 * (btn_height + spacing) && y <= startY + 4 * (btn_height + spacing)) { // "EXIT"
                    closegraph();
                    exit(0);
                }
            }
        }
    }
}

// Fungsi untuk menampilkan panduan
void showGuide() {
    cleardevice();
    drawStars();
    drawText(getmaxx() / 2, 100, "PANDUAN", 5, WHITE);
    drawText(getmaxx() / 2, 200, "Gunakan tombol panah untuk bergerak.", 2, WHITE);
    drawText(getmaxx() / 2, 250, "Tekan spasi untuk menembak.", 2, WHITE);
    
    // Tombol kembali
    drawButton(getmaxx() / 2 - 75, getmaxy() - 100, 150, 50, LIGHTBLUE, "BACK");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Jika tombol "BACK" diklik
            if (mx >= getmaxx() / 2 - 75 && mx <= getmaxx() / 2 + 75 &&
                my >= getmaxy() - 100 && my <= getmaxy() - 50) {
                showMainMenu();
                return;
            }
        }
    }
}

// Fungsi untuk keluar dari game dengan konfirmasi
void exitGame() {
    cleardevice();
    drawStars();
    drawText(getmaxx() / 2, getmaxy() / 2 - 50, "Apakah Anda yakin ingin keluar?", 4, WHITE);
    
    // Tombol "YA" dan "TIDAK"
    int btn_width = 150, btn_height = 50;
    int centerX = getmaxx() / 2;
    
    drawButton(centerX - 180, getmaxy() / 2 + 30, btn_width, btn_height, LIGHTRED, "YA");
    drawButton(centerX + 30, getmaxy() / 2 + 30, btn_width, btn_height, LIGHTGREEN, "TIDAK");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Jika tombol "YA" diklik, keluar dari program
            if (mx >= centerX - 180 && mx <= centerX - 180 + btn_width &&
                my >= getmaxy() / 2 + 30 && my <= getmaxy() / 2 + 30 + btn_height) {
                exit(0);
            }
            // Jika tombol "TIDAK" diklik, kembali ke menu utama
            else if (mx >= centerX + 30 && mx <= centerX + 30 + btn_width &&
                     my >= getmaxy() / 2 + 30 && my <= getmaxy() / 2 + 30 + btn_height) {
                showMainMenu();
                return;
            }
        }
    }
}

// Fungsi untuk menampilkan menu utama
void showMainMenu() {
    initwindow(getmaxwidth(), getmaxheight(), "SPACE INVADERS");
    srand(time(NULL));

    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    drawStars();
    drawText(screenWidth / 2, 80, "SPACE INVADERS", 6, WHITE);
    drawLeaderboard();

    int btn_width = 250, btn_height = 60;
    int centerX = screenWidth / 2 - btn_width / 2;
    int startY = screenHeight / 2 - 100;

    drawButton(centerX, startY, btn_width, btn_height, LIGHTBLUE, "INPUT NAMA");
    drawButton(centerX, startY + 80, btn_width, btn_height, LIGHTGREEN, "START");
    drawButton(centerX, startY + 160, btn_width, btn_height, YELLOW, "GUIDE");
    drawButton(centerX, startY + 240, btn_width, btn_height, WHITE, "EXIT");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            if (mx >= centerX && mx <= centerX + btn_width) {
                if (my >= startY && my <= startY + btn_height) {
                    cleardevice();  // Bersihkan layar sebelum masuk game
                    startGame();  // Panggil fungsi mulai game
                    return
                    setcolor(WHITE);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    outtextxy(centerX + 10, startY + 20, (char*)"Masukkan Nama:"); // Tambahkan (char*)

                    setfillstyle(SOLID_FILL, BLACK);
                    bar(centerX + 150, startY + 10, centerX + 250, startY + 50);
                    setcolor(WHITE);
                    rectangle(centerX + 150, startY + 10, centerX + 250, startY + 50);
                    setbkcolor(BLACK);
                    char input[30] = "";
                    int i = 0;
                    while (1) {
                        char ch = getch();
                        if (ch == 13) break; // Enter untuk konfirmasi
                        if (ch == 8 && i > 0) { // Backspace
                            i--;
                            input[i] = '\0';
                        } else if (ch >= 32 && ch <= 126 && i < 29) { // Input karakter
                            input[i] = ch;
                            i++;
                            input[i] = '\0';
                        }
                        setfillstyle(SOLID_FILL, BLACK);
                        bar(centerX + 155, startY + 15, centerX + 245, startY + 45);
                        setcolor(WHITE);
                        outtextxy(centerX + 160, startY + 20, input);
                    }
                    strcpy(playerName, input);
                } else if (my >= startY + 80 && my <= startY + 80 + btn_height) {
                    startGame();  // Panggil fungsi mulai game
                } else if (my >= startY + 160 && my <= startY + 160 + btn_height) {
                    showGuide();  // Panggil fungsi panduan
                } else if (my >= startY + 240 && my <= startY + 240 + btn_height) {
                    exitGame();  // Panggil fungsi keluar
                }
            }
        }
    }
}