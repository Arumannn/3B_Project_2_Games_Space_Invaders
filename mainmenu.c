#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>

#include "mainsprite.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include "mainmenu.h"

char playerName[30] = "";
bool nameEntered = true;

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
    for (int i = 0; i < 200; i++) {
        putpixel(rand() % getmaxwidth(), rand() % getmaxheight(), WHITE);
    }
}

void savePlayerName() {
    FILE *file = fopen("players.txt", "a");
    if (file) {
        fprintf(file, "%s 0\n", playerName);
        fclose(file);
    }
}

void handleNameInput(int x, int y, int width, int height) {
    setcolor(WHITE);
    rectangle(x, y, x + width, y + height);
    setbkcolor(BLACK);
    drawText(x + width / 2, y + height / 2, "Enter Name:", 2, LIGHTGRAY);
    
    char tempName[30] = "";
    int index = 0;
    while (1) {
        char key = getch();
        if (key == 13 && index > 0) break;
        if (key == 8 && index > 0) tempName[--index] = '\0';
        else if (index < 29 && key >= 32 && key <= 126) tempName[index++] = key;
        tempName[index] = '\0';
        strcpy(playerName, tempName);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(x + 5, y + 5, WHITE);
        drawText(x + width / 2, y + height / 2, playerName, 2, WHITE);
    }
    nameEntered = true;
    savePlayerName();
}

void drawLeaderboard() {
    int x = getmaxwidth() - 320;
    int y = 100;
    rectangle(x, y, x + 300, y + 200);
    FILE *file = fopen("players.txt", "r");
    if (file == NULL) {
        drawText(x + 150, y + 50, "No Data", 2, WHITE);
        return;
    }
    char name[30];
    int score, i = 0;
    while (fscanf(file, "%s %d", name, &score) != EOF) {
        char scoreText[50];
        sprintf(scoreText, "%d. %s - %d", i + 1, name, score);
        drawText(x + 150, y + 30 + (i * 40), scoreText, 2, WHITE);
        i++;
    }
    fclose(file);
}

void displayGuide() {
    cleardevice();
    drawText(getmaxwidth() / 2, 100, "GUIDE", 5, WHITE);
    drawText(getmaxwidth() / 2, 200, "Gunakan tombol panah untuk bergerak.", 2, WHITE);
    drawText(getmaxwidth() / 2, 250, "Tekan 'SPASI' untuk menembak.", 2, WHITE);
    drawText(getmaxwidth() / 2, 300, "Hindari tembakan musuh dan kalahkan semua alien!", 2, WHITE);
    drawText(getmaxwidth() / 2, getmaxheight() - 100, "Tekan ESC untuk kembali", 2, WHITE);
    while (1) {
        if (kbhit() && getch() == 27) {
            showMainMenu();
            return;
        }
        delay(100);
    }
}

void showMainMenu() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenWidth, screenHeight, "Space Invaders");
    cleardevice();
    drawStars();
    drawText(screenWidth / 2, 100, "SPACE INVADERS", 8, WHITE);
    handleNameInput(screenWidth / 2 - 200, screenHeight / 2 - 200, 400, 50);
    
    int btn_width = 300, btn_height = 80;
    int centerX = screenWidth / 2 - btn_width / 2;
    int startY = screenHeight / 2 - 50;
    int spacing = 30;
    
    drawButton(centerX, startY, btn_width, btn_height, LIGHTGREEN, "START");
    drawButton(centerX, startY + (btn_height + spacing), btn_width, btn_height, YELLOW, "GUIDE");
    drawButton(centerX, startY + (btn_height + spacing) * 2, btn_width, btn_height, WHITE, "EXIT");
    drawLeaderboard();
}

void handleMainMenu() {
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= 400 && x <= 700) {
                if (y >= 250 && y <= 330 && nameEntered) {
                    startGame();
                    return;
                } else if (y >= 350 && y <= 430) {
                    displayGuide();
                } else if (y >= 450 && y <= 530) {
                    if (MessageBox(NULL, "Keluar dari permainan?", "Konfirmasi", MB_YESNO) == IDYES) {
                        closegraph();
                        exit(0);
                    }
                }
            }
        }
        delay(100);
    }
}