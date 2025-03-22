#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mainmenu.h"
#include "score.h"

#define MAX_ENTRIES 10  // Maksimum 10 pemain
#define MAX_NAME_LENGTH 20 // Maksimum panjang nama

typedef struct {
    char name[MAX_NAME_LENGTH + 1];
    int score;
} LeaderboardEntry;

extern void drawStars(); // Menggunakan fungsi yang sudah ada di mainmenu.c

// Fungsi untuk menyimpan skor dan mengurutkan leaderboard
void savePlayerScore(const char *name, int score) {
    LeaderboardEntry entries[MAX_ENTRIES];
    int count = 0;
    int found = 0; // Menandakan apakah pemain sudah ada di leaderboard

    FILE *file = fopen("leaderboard.txt", "r");
    if (file) {
        while (fscanf(file, "%s %d", entries[count].name, &entries[count].score) != EOF) {
            if (strcmp(entries[count].name, name) == 0) {
                // Jika pemain sudah ada, hanya update skor jika lebih tinggi
                if (score > entries[count].score) {
                    entries[count].score = score;
                }
                found = 1;
            }
            count++;
            if (count >= MAX_ENTRIES) break;
        }
        fclose(file);
    }

    // Jika pemain tidak ditemukan, tambahkan pemain baru
    if (!found && count < MAX_ENTRIES) {
        strcpy(entries[count].name, name);
        entries[count].score = score;
        count++;
    }

    // Sorting leaderboard berdasarkan skor tertinggi
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (entries[j].score > entries[i].score) {
                LeaderboardEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }

    // Simpan leaderboard ke file
    file = fopen("leaderboard.txt", "w");
    if (file) {
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s %d\n", entries[i].name, entries[i].score);
        }
        fclose(file);
    }
}


// Tampilan layar GAME OVER
void gameOverScreen() {
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    
    initwindow(screenWidth, screenHeight, "Game Over");
    cleardevice();
    drawGradientBackground();
    drawStars(); // Menggunakan background bintang yang sama dengan menu utama

    // Tampilkan tulisan "GAME OVER"
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 10);

    int textWidth = textwidth("GAME OVER!!");
    int textHeight = textheight("GAME OVER!!");

    outtextxy((screenWidth - textWidth) / 2, (screenHeight / 4) - (textHeight / 2), "GAME OVER!!");


    // Kotak input nama
    int inputBoxX1 = screenWidth / 2 - 150;
    int inputBoxY1 = 300;
    int inputBoxX2 = screenWidth / 2 + 150;
    int inputBoxY2 = 350;
    
    setcolor(WHITE);
    rectangle(inputBoxX1, inputBoxY1, inputBoxX2, inputBoxY2);
    
    // Label "MASUKKAN NAMA:"
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(inputBoxX1, inputBoxY1 - 30, (char *) "MASUKKAN NAMA:");

    // Tombol SUBMIT
    int buttonWidth = 200, buttonHeight = 50;
    int submitButtonX1 = (screenWidth - buttonWidth) / 2;
    int submitButtonY1 = inputBoxY2 + 30;
    int submitButtonX2 = submitButtonX1 + buttonWidth;
    int submitButtonY2 = submitButtonY1 + buttonHeight;
     
    setfillstyle(SOLID_FILL, CYAN);
    bar(submitButtonX1, submitButtonY1, submitButtonX2, submitButtonY2);

    setcolor(WHITE);
    setbkcolor(CYAN);
    
    int textSubmitWidth = textwidth("SUBMIT");
    int textSubmitHeight = textheight("SUBMIT");

    outtextxy((submitButtonX1 + submitButtonX2) / 2 - textSubmitWidth / 2,
              (submitButtonY1 + submitButtonY2) / 2 - textSubmitHeight / 2, 
              "SUBMIT");

    setbkcolor(BLACK);

    // Input nama pemain
    char playerName[MAX_NAME_LENGTH + 1] = "";
    int index = 0;
    char ch;
    int lastIndex = -1; 
    int finalScore = getScore();
    
    while (1) {
        // Tampilkan input nama hanya jika berubah
        if (index != lastIndex) {
            setfillstyle(SOLID_FILL, BLACK);
            bar(inputBoxX1 + 2, inputBoxY1 + 2, inputBoxX2 - 2, inputBoxY2 - 2);
            
            setcolor(WHITE);
            outtextxy(inputBoxX1 + 10, inputBoxY1 + 10, playerName);
            lastIndex = index;
        }

        // Cek input keyboard
        if (kbhit()) {
            ch = getch();
            
            if (ch == 13 && index > 0) { // ENTER ditekan
                savePlayerScore(playerName, finalScore);
                closegraph();
                showMainMenu();
                return;
            } 
            else if (ch == 8 && index > 0) { // BACKSPACE
                index--;
                playerName[index] = '\0';
            } 
            else if (index < MAX_NAME_LENGTH && ch >= 32 && ch <= 126) { // Tambahkan karakter
                playerName[index++] = ch;
                playerName[index] = '\0';
            }
        }

        // Cek klik tombol SUBMIT
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            if (x >= submitButtonX1 && x <= submitButtonX2 && y >= submitButtonY1 && y <= submitButtonY2) {
                if (strlen(playerName) > 0) {
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
