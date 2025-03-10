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

// Fungsi untuk menggambar teks di tengah
void drawText(int x, int y, const char* text, int size, int color) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    char tempText[100];
    strncpy(tempText, text, sizeof(tempText) - 1);
    tempText[sizeof(tempText) - 1] = '\0';
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

// Fungsi untuk menampilkan menu utama dalam fullscreen
void showMainMenu() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // Ambil resolusi layar penuh
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    initwindow(screenWidth, screenHeight, "Space Invaders", -3, -3); // Fullscreen

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
}


// Fungsi untuk menangani input menu utama
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

        if (kbhit()) { // Navigasi dengan keyboard
            char key = getch();
            if (key == 13 && choice != -1) { // Enter untuk memilih
                if (choice == 1) {
                    startGame(); // Masuk ke gameplay
                    return;
                } else if (choice == 3) {
                    closegraph();
                    exit(0);
                }
            } else if (key == 72) { // Panah atas
                choice = (choice > 0) ? choice - 1 : 3;
            } else if (key == 80) { // Panah bawah
                choice = (choice < 3) ? choice + 1 : 0;
            }

            showMainMenu();
            setcolor(RED);
            rectangle(centerX, startY + (btn_height + spacing) * choice, 
                      centerX + btn_width, startY + (btn_height + spacing) * choice + btn_height);
        }
    }
}




