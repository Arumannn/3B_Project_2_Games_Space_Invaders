#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef YELLOW
#define YELLOW COLOR(255, 255, 0)
#endif

char playerName[30] = "";
int score = 0;
int isGameRunning = 0;

void drawText(int x, int y, const char* text, int size, int color);
void drawButton(int x, int y, int width, int height, int color, const char *label);
void drawStars();
void showMainMenu();
void startGame();
void exitGame();
void showGuide();

void drawText(int x, int y, const char* text, int size, int color) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    char tempText[100];
    strncpy(tempText, text, sizeof(tempText) - 1);
    tempText[sizeof(tempText) - 1] = '\0';

    outtextxy(x, y, tempText);
}

void drawButton(int x, int y, int width, int height, int color, const char *label) {
    setcolor(WHITE);
    rectangle(x, y, x + width, y + height);
    setfillstyle(SOLID_FILL, color);
    floodfill(x + 1, y + 1, WHITE);
    setbkcolor(color);
    drawText(x + width / 2, y + height / 2, label, 3, BLACK);
    setbkcolor(BLACK);
}

void drawStars() {
    cleardevice();
    for (int i = 0; i < 100; i++) {
        int x = rand() % getmaxx();
        int y = rand() % getmaxy();
        putpixel(x, y, WHITE);
    }

    // Set latar belakang
    setbkcolor(BLACK);
    cleardevice();
    drawStars();
    drawText(getmaxx() / 2, getmaxy() / 2, "GAME DIMULAI!", 5, WHITE);
    delay(2000);
}

void exitGame() {
    cleardevice();
    drawStars();
    drawText(getmaxx() / 2, getmaxy() / 2 - 50, "Apakah Anda yakin ingin keluar?", 4, WHITE);
    int btn_width = 150, btn_height = 50;
    int centerX = getmaxx() / 2;
    drawButton(centerX - 180, getmaxy() / 2 + 30, btn_width, btn_height, LIGHTRED, "YA");
    drawButton(centerX + 30, getmaxy() / 2 + 30, btn_width, btn_height, LIGHTGREEN, "TIDAK");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            if (mx >= centerX - 180 && mx <= centerX - 180 + btn_width &&
                my >= getmaxy() / 2 + 30 && my <= getmaxy() / 2 + 30 + btn_height) {
                exit(0);
            } else if (mx >= centerX + 30 && mx <= centerX + 30 + btn_width &&
                       my >= getmaxy() / 2 + 30 && my <= getmaxy() / 2 + 30 + btn_height) {
                showMainMenu();
                return;
            }
        }
    }
}

void showGuide() {
    cleardevice();
    drawStars();
    drawText(getmaxx() / 2, 100, "PANDUAN", 5, WHITE);
    drawText(getmaxx() / 2, 200, "Gunakan tombol panah untuk bergerak.", 2, WHITE);
    drawText(getmaxx() / 2, 250, "Tekan spasi untuk menembak.", 2, WHITE);
    drawButton(getmaxx() / 2 - 75, getmaxy() - 100, 150, 50, LIGHTBLUE, "BACK");
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            if (mx >= getmaxx() / 2 - 75 && mx <= getmaxx() / 2 + 75 &&
                my >= getmaxy() - 100 && my <= getmaxy() - 50) {
                showMainMenu();
                return;
            }
        }
    }
}

void showMainMenu() {
    // Pastikan jendela grafik sudah dibuat
    if (!ismouseclick(WM_LBUTTONDOWN)) {
        initwindow(800, 600, "SPACE INVADERS");
    }

    cleardevice();
    drawStars();
    drawText(getmaxx() / 2, 80, "SPACE INVADERS", 6, WHITE);

    int btn_width = 250, btn_height = 60;
    int centerX = getmaxx() / 2 - btn_width / 2;
    int startY = getmaxy() / 2 - 100;

    drawButton(centerX, startY, btn_width, btn_height, LIGHTBLUE, "INPUT NAMA");
    drawButton(centerX, startY + 80, btn_width, btn_height, LIGHTGREEN, "START");
    drawButton(centerX, startY + 160, btn_width, btn_height, YELLOW, "GUIDE");
    drawButton(centerX, startY + 240, btn_width, btn_height, WHITE, "EXIT");

    while (1) {
        delay(50); // Tambahkan delay agar loop tidak membebani CPU

        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            if (mx >= centerX && mx <= centerX + btn_width) {
                if (my >= startY && my <= startY + btn_height) {
                    // Input Nama
                    setcolor(WHITE);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    char text[] = "Masukkan Nama:";
                    outtextxy(centerX + 10, startY + 20, text);



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