#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mainsprite.h"
#include "alien.h"
#include "ufo.h"
#include "gameplay.h"
#include "mainmenu.h"
#include "gameover.h"

//------------------------------------------PROGRAM GAMEOVER-----------------------------------------------------

void savePlayerScore(const char *name, int score) {
    LeaderboardEntry *head = NULL;
    LeaderboardEntry *current = NULL;
    LeaderboardEntry *prev = NULL;
    int found = 0;

    // Baca file dan isi ke linked list
    FILE *file = fopen("leaderboard.txt", "r");
    if (file) {
        char tempName[MAX_NAME_LENGTH];
        int tempScore;

        while (fscanf(file, "%s %d", tempName, &tempScore) == 2) {
            LeaderboardEntry *newNode = (LeaderboardEntry*)malloc(sizeof(LeaderboardEntry));
            strcpy(newNode->name, tempName);
            newNode->score = tempScore;
            newNode->next = NULL;

            // Tambahkan ke linked list
            if (!head) {
                head = newNode;
            } else {
                current->next = newNode;
            }
            current = newNode;
        }
        fclose(file);
    }

    // Cek apakah nama sudah ada
    current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            found = 1;
            if (score > current->score) {
                current->score = score; // update skor lebih tinggi
            }
            break;
        }
        current = current->next;
    }

    // Jika nama belum ada, tambahkan node baru
    if (!found) {
        LeaderboardEntry *newNode = (LeaderboardEntry*)malloc(sizeof(LeaderboardEntry));
        strcpy(newNode->name, name);
        newNode->score = score;
        newNode->next = NULL;

        // Tambahkan di akhir
        if (!head) {
            head = newNode;
        } else {
            current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Urutkan linked list berdasarkan skor (descending)
    for (LeaderboardEntry *i = head; i != NULL; i = i->next) {
        for (LeaderboardEntry *j = i->next; j != NULL; j = j->next) {
            if (j->score > i->score) {
                // tukar data
                char tempName[MAX_NAME_LENGTH];
                int tempScore;

                strcpy(tempName, i->name);
                tempScore = i->score;

                strcpy(i->name, j->name);
                i->score = j->score;

                strcpy(j->name, tempName);
                j->score = tempScore;
            }
        }
    }

    // Simpan ke file
    file = fopen("leaderboard.txt", "w");
    if (file) {
        int count = 0;
        current = head;
        while (current && count < MAX_ENTRIES) {
            fprintf(file, "%s %d\n", current->name, current->score);
            current = current->next;
            count++;
        }
        fclose(file);
    }

    // Bebaskan memori linked list
    while (head) {
        LeaderboardEntry *temp = head;
        head = head->next;
        free(temp);
    }
}

void gameOverScreen(int finalScore) {
    // Inisialisasi Window dan Layar 
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    
    cleardevice();

    // Background 
    drawStars();

    // Gambar "GAME OVER" 
    int imgWidth = 400; // Tentukan ukuran gambar
    int imgHeight = 100;
    int imgX = screenWidth / 2 - imgWidth / 2;
    int imgY = screenHeight / 4 - imgHeight / 2;

    // Path gambar
    const char *imagePath = "img/gameover.bmp";
    const char *submitImagePath = "img/submit.bmp";

    // Periksa apakah file gambar game over ada
    FILE *fileCheck = fopen(imagePath, "r");
    if (fileCheck) {
        fclose(fileCheck);
        readimagefile(imagePath, imgX, imgY, imgX + imgWidth, imgY + imgHeight);
    } else {
        printf("Gambar game over tidak ditemukan: %s\n", imagePath);
        delay(2000); // Beri waktu untuk membaca pesan
    }

    // Kotak input nama
    int inputBoxX1 = screenWidth / 2 - 150;
    int inputBoxY1 = 300;
    int inputBoxX2 = screenWidth / 2 + 150;
    int inputBoxY2 = 350;
    
    setcolor(WHITE);
    rectangle(inputBoxX1, inputBoxY1, inputBoxX2, inputBoxY2);
    
    // Label "MASUKKAN NAMA:" 
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(inputBoxX1, inputBoxY1 - 30, (char*)"MASUKKAN NAMA:");

    // Gambar tombol "SUBMIT"
    int buttonWidth = 200, buttonHeight = 50;
    int submitButtonX1 = (screenWidth - buttonWidth) / 2;
    int submitButtonY1 = inputBoxY2 + 30;
    int submitButtonX2 = submitButtonX1 + buttonWidth;
    int submitButtonY2 = submitButtonY1 + buttonHeight;

    // Periksa apakah file gambar submit ada
    fileCheck = fopen(submitImagePath, "r");
    if (fileCheck) {
        fclose(fileCheck);
        readimagefile(submitImagePath, submitButtonX1, submitButtonY1, submitButtonX2, submitButtonY2);
    } else {
        setfillstyle(SOLID_FILL, MAGENTA);
        bar(submitButtonX1, submitButtonY1, submitButtonX2, submitButtonY2);
        setcolor(WHITE);
        setbkcolor(MAGENTA);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
        outtextxy(submitButtonX1 + 45, submitButtonY1 + 10, (char*)"SUBMIT");
    }

    // Variabel untuk input nama pemain
    char playerName[MAX_NAME_LENGTH] = "";
    int index = 0;
    int lastIndex = 0;
    char ch;

    while (1) {
        // Update kotak input nama
        if (index != lastIndex) {
            setfillstyle(SOLID_FILL, BLACK);
            bar(inputBoxX1 + 2, inputBoxY1 + 2, inputBoxX2 - 2, inputBoxY2 - 2);
            setcolor(WHITE);
            outtextxy(inputBoxX1 + 10, inputBoxY1 + 10, playerName);
            lastIndex = index;
        }

        // Tombol Keyboard (ENTER dan BACKSPACE)
        if (kbhit()) {
            ch = getch();
            if (ch == 13 && index > 0) {  // ENTER
                savePlayerScore(playerName, finalScore);
                cleardevice();
                showMainMenu();
                return;
            } 
            else if (ch == 8 && index > 0) {  // BACKSPACE
                index--;
                playerName[index] = '\0';
            } 
            else if (index < MAX_NAME_LENGTH && ch >= 32 && ch <= 126) {
                playerName[index++] = ch;
                playerName[index] = '\0';
            }
        }

        // Klik Tombol SUBMIT
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= submitButtonX1 && x <= submitButtonX2 && y >= submitButtonY1 && y <= submitButtonY2) {
                if (strlen(playerName) > 0) {
                    savePlayerScore(playerName, finalScore);
                    cleardevice();
                    showMainMenu();
                    return;
                }
            }
        }

        delay(10); // Tambahkan delay untuk menghindari penggunaan CPU yang tinggi
    }
}
