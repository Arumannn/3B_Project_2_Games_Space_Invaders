#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mainmenu.h"
#include "score.h"

#define MAX_NAME_LENGTH 20

extern void drawStars(); // Menggunakan fungsi yang sudah ada di mainmenu.c

void savePlayerScore(const char *name, int score) {
    FILE *file = fopen("leaderboard.txt", "a");
    if (file) {
        fprintf(file, "%s %d\n", name, score);  // Simpan nama + skor
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
    outtextxy(screenWidth / 2 - 250, 100, (char *) "GAME OVER!!");

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

     // Gambar tombol SUBMIT (HIJAU)
     int submitButtonX1 = screenWidth / 2 - 75;
     int submitButtonY1 = 400;
     int submitButtonX2 = screenWidth / 2 + 75;
     int submitButtonY2 = 450;
     
     setfillstyle(SOLID_FILL, GREEN);
     bar(submitButtonX1, submitButtonY1, submitButtonX2, submitButtonY2);
     
     setcolor(WHITE);
     setbkcolor(GREEN);
     outtextxy(screenWidth / 2 - 30, submitButtonY1 + 15, (char *) "SUBMIT");
     setbkcolor(BLACK);
 
     char playerName[MAX_NAME_LENGTH + 1] = "";
     int index = 0;
     char ch;
     int lastIndex = -1; 
     int finalScore = getScore();
     
     while (1) {
         if (index != lastIndex) { // Hanya gambar ulang jika input berubah
             setfillstyle(SOLID_FILL, BLACK);
             bar(inputBoxX1 + 2, inputBoxY1 + 2, inputBoxX2 - 2, inputBoxY2 - 2);
             
             setcolor(WHITE);
             outtextxy(inputBoxX1 + 10, inputBoxY1 + 10, playerName);
             lastIndex = index;
         }
 
         if (kbhit()) {
             ch = getch();
             
             // Jika tekan ENTER, simpan nama & kembali ke menu
             if (ch == 13 && index > 0) {
                 savePlayerScore(playerName, finalScore);
                 closegraph();
                 showMainMenu();
                 return;
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
 
         // Cek apakah tombol SUBMIT ditekan
         if (ismouseclick(WM_LBUTTONDOWN)) {
             int x, y;
             getmouseclick(WM_LBUTTONDOWN, x, y);
             
             // Jika tombol SUBMIT ditekan, simpan nama & kembali ke menu
             if (x >= submitButtonX1 && x <= submitButtonX2 && y >= submitButtonY1 && y <= submitButtonY2) {
                 if (strlen(playerName) > 0) { // Pastikan nama tidak kosong
                     savePlayerScore(playerName, finalScore);
                     closegraph();
                     showMainMenu();
                     return;
                 }
             }
         }
     }
     
     closegraph();
 }