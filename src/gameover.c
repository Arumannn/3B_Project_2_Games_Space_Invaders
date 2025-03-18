#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mainmenu.h" 

#define MAX_NAME_LENGTH 20

extern void drawStars(); // Menggunakan fungsi yang sudah ada di mainmenu.c

void savePlayerName(const char *name) {
    FILE *file = fopen("leaderboard.txt", "a");
    if (file) {
        fprintf(file, "%s\n", name);
        fclose(file);
    }
}

void gameOverScreen() {
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    
    initwindow(screenWidth, screenHeight, "Game Over");
    cleardevice();
    drawStars(); // Menggunakan background bintang yang sama dengan menu utama

    // Tampilkan tulisan "GAME OVER"
    setcolor(RED);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 10);
    outtextxy(screenWidth / 2 - 250, 100, (char *) "GAME OVER");

    // Gambar kotak input nama
    int inputBoxX1 = screenWidth / 2 - 150;
    int inputBoxY1 = 300;
    int inputBoxX2 = screenWidth / 2 + 150;
    int inputBoxY2 = 350;
    
    setcolor(WHITE);
    rectangle(inputBoxX1, inputBoxY1, inputBoxX2, inputBoxY2);
    
    // Tampilkan tulisan "MASUKKAN NAMA:"
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(inputBoxX1, inputBoxY1 - 30, (char *) "MASUKKAN NAMA:");

    // Gambar tombol BACK
    int backButtonX1 = screenWidth / 2 - 75;
    int backButtonY1 = 400;
    int backButtonX2 = screenWidth / 2 + 75;
    int backButtonY2 = 450;
    
    setfillstyle(SOLID_FILL, RED);
    bar(backButtonX1, backButtonY1, backButtonX2, backButtonY2);
    
    setcolor(WHITE);
    outtextxy(screenWidth / 2 - 30, backButtonY1 + 15, (char *) "BACK");

    char playerName[MAX_NAME_LENGTH + 1] = "";
    int index = 0;
    char ch;

    while (1) {
        if (kbhit()) {
            ch = getch();

            // Jika tekan ENTER, simpan nama & keluar
            if (ch == 13 && index > 0) {
                savePlayerName(playerName);
                break;
            } 
            // Jika tekan BACKSPACE, hapus karakter terakhir
            else if (ch == 8 && index > 0) {
                index--;
                playerName[index] = '\0';
            } 
            // Jika karakter valid, tambahkan ke input
            else if (index < MAX_NAME_LENGTH && ch >= 32 && ch <= 126) {
                playerName[index++] = ch;
                playerName[index] = '\0';
            }
        }

        // Perbarui isi kotak input tanpa flicker
        setfillstyle(SOLID_FILL, BLACK);
        bar(inputBoxX1 + 2, inputBoxY1 + 2, inputBoxX2 - 2, inputBoxY2 - 2);

        setcolor(WHITE);
        outtextxy(inputBoxX1 + 10, inputBoxY1 + 10, playerName);

        // Cek apakah tombol BACK ditekan
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            // Jika tombol BACK ditekan, kembali ke main menu
            if (x >= backButtonX1 && x <= backButtonX2 && y >= backButtonY1 && y <= backButtonY2) {
                closegraph();
                mainMenu();
                return;
            }
        }
    }
    
    closegraph();
}
