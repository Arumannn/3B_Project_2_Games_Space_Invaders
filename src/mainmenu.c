//===============================================================================================================
// Program dibuat oleh: Rina Permata Dewi
// NIM                : 241511061
// Deskripsi File     : Berisi dua program utama, yaitu:
//                      1. mainmenu  - untuk menampilkan menu utama
//                      2. gameover  - untuk menampilkan tampilan game over
// Proyek 2           : Game Space Invaders 
//===============================================================================================================

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

//============================================== PROGRAM MAINMENU ================================================

// Fungsi untuk menampilkan menu utama
void mainMenu() {
    cleardevice();
    
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
    outtextxy(getmaxwidth() / 2 - 100, 100, (char *)"MAIN MENU");

    getch();
    cleardevice();
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
    int numStars = 200; // Jumlah bintang
    for (int i = 0; i < numStars; i++) {
        int x = rand() % getmaxwidth();
        int y = rand() % getmaxheight();
        
        // Buat beberapa bintang lebih kecil agar terlihat lebih halus
        if (rand() % 5 == 0) { 
            setcolor(YELLOW);  // Bintang lebih besar berwarna kuning
            circle(x, y, 1);   // Ukuran lebih kecil (radius 1)
        } else {
            setcolor(WHITE);   // Bintang kecil tetap putih
            putpixel(x, y, WHITE);
        }
    }
}

// Fungsi Leaderboard
// Leaderboard: Struktur dan fungsi menggunakan linked list
typedef struct LeaderboardNode {
    char name[10];
    int score;
    struct LeaderboardNode* next;
} LeaderboardNode;

// Fungsi untuk menambahkan node baru secara urut (descending)
void insertSorted(LeaderboardNode** head, const char* name, int score) {
    LeaderboardNode* newNode = (LeaderboardNode*)malloc(sizeof(LeaderboardNode));
    strcpy(newNode->name, name);
    newNode->score = score;
    newNode->next = NULL;

    if (*head == NULL || score > (*head)->score) {
        newNode->next = *head;
        *head = newNode;
    } else {
        LeaderboardNode* current = *head;
        while (current->next != NULL && current->next->score >= score) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Fungsi untuk menghapus seluruh linked list
void freeLeaderboard(LeaderboardNode* head) {
    LeaderboardNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Versi utama yang mendukung posisi tengah/kanan
void drawLeaderboard(int yOffset, int limit, int isCentered) {
    int width = 350;
    int x = isCentered ? (getmaxwidth() - width) / 2 : getmaxwidth() / 2 + 150;
    int y = yOffset;
    int rowHeight = 40;

    // Baca data leaderboard
    FILE* file = fopen("leaderboard.txt", "r");
    if (!file) {
        drawText(x + width / 2, y, "Belum ada data!", 2, WHITE);
        return;
    }

    LeaderboardNode* head = NULL;
    char name[10];
    int score;

    while (fscanf(file, "%s %d", name, &score) != EOF) {
        insertSorted(&head, name, score);
    }
    fclose(file);

    // Hitung jumlah entri
    int count = 0;
    LeaderboardNode* temp = head;
    while (temp && (limit > 0 ? count < limit : 1)) {
        count++;
        temp = temp->next;
    }

    int height = 30 + (count * rowHeight);

    // Gambar kotak leaderboard
    setcolor(WHITE);
    rectangle(x, y, x + width, y + height);
    line(x, y + 30, x + width, y + 30);
    line(x + 60, y, x + 60, y + height);
    line(x + 220, y, x + 220, y + height);

    drawText(x + 30, y + 15, "NO", 2.5, WHITE);
    drawText(x + 140, y + 15, "PLAYER", 2.5, WHITE);
    drawText(x + 285, y + 15, "SCORE", 2.5, WHITE);

    // Tampilkan isi data
    int i = 0;
    temp = head;
    while (temp && (limit > 0 ? i < limit : 1)) {
        char numText[5], scoreText[10];
        sprintf(numText, "%d", i + 1);
        sprintf(scoreText, "%d", temp->score);

        int yPos = y + 50 + (i * rowHeight);
        drawText(x + 30, yPos, numText, 2.5, WHITE);
        drawText(x + 140, yPos, temp->name, 2.5, WHITE);
        drawText(x + 285, yPos, scoreText, 2.5, WHITE);

        i++;
        temp = temp->next;
    }

    freeLeaderboard(head);
}

// Fungsi untuk menggambar leaderboard di sisi kanan layar
void drawLeaderboardRight(int yOffset, int limit) {
    drawLeaderboard(yOffset, limit, 0);  // isCentered = 0 (artinya: di kanan layar)
}

// Fungsi untuk menggambar tombol
void drawImageButton(const char* imageFile, int x, int y, int width, int height) {
    readimagefile(imageFile, x, y, x + width, y + height);
}

// Fungsi untuk menampilkan menu utama
void showMainMenu() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    cleardevice();
    drawStars();

    // Hitung area gambar (disesuaikan dengan ukuran sebelumnya)
    int imgWidth = 400;   // lebar gambar ditampilkan
    int imgHeight = 200;  // tinggi gambar ditampilkan
    int imgX = getmaxwidth() / 2 - imgWidth / 2;
    int imgY = 20;  // bisa kamu sesuaikan

    readimagefile("img/judul.bmp", imgX, imgY, imgX + imgWidth, imgY + imgHeight);

    // Posisi tombol
    int centerX = getmaxwidth() / 2 - 350;  // Geser tombol lebih ke kiri
    int startY = getmaxheight() / 2 - 160; // Sedikit naik agar lebih proporsional
    int buttonWidth = 300;
    int buttonHeight = 200;
    int buttonSpacing = 140; // Tambah jarak antar tombol agar lebih luas

    drawImageButton("img/start.bmp", centerX, startY, buttonWidth, buttonHeight);
    drawImageButton("img/guide.bmp", centerX, startY + buttonSpacing, buttonWidth, buttonHeight);
    drawImageButton("img/exit.bmp", centerX, startY + (buttonSpacing * 2), buttonWidth, buttonHeight);

     // Gambar leaderboard terlebih dahulu dan hitung tinggi tabel
     int leaderboardX = getmaxwidth() / 2 + 150;
     int leaderboardY = startY + 30;
     int rowHeight = 40;

     // Hitung jumlah data
    FILE* file = fopen("leaderboard.txt", "r");
    int count = 0;
    char name[10];
    int score;
    while (file && fscanf(file, "%s %d", name, &score) != EOF) {
        count++;
    }
    if (file) fclose(file);

    // Gambar leaderboard hanya 5 entri di main menu
    drawLeaderboardRight(leaderboardY, DISPLAY_MAINMENU_ENTRIES);

    // Hitung posisi Y untuk tombol LEADERBOARD
    int leaderboardHeight = 30 + (5 * rowHeight);  // Sesuaikan dengan jumlah yang ditampilkan
    int leaderboardButtonY = leaderboardY + leaderboardHeight + 20;

    // Tombol "LEADERBOARD" berada tepat di bawah tabel
    drawImageButton("img/more.bmp", leaderboardX, leaderboardButtonY - 10, 300, 200);
}

// Perbaikan handleMainMenu agar menu tetap berjalan
void handleMainMenu() {
    clearmouseclick(WM_LBUTTONDOWN);

    while (1) {  // outer loop untuk menu utama
        showMainMenu();

        // Posisi dan ukuran tombol
        int centerX = getmaxwidth() / 2 - 350;
        int startY = getmaxheight() / 2 - 160;
        int buttonWidth = 300;
        int buttonHeight = 200;
        int buttonSpacing = 140;

        int goToGame = 0;  // flag untuk jalankan game

        while (1) {  // inner loop untuk cek klik
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);
                clearmouseclick(WM_LBUTTONDOWN);

                // START
                if (x >= centerX && x <= centerX + buttonWidth &&
                    y >= startY && y <= startY + buttonHeight) {
                    Sleep(300);
                    goToGame = 1;
                    break;  // keluar dari inner loop
                }

                // GUIDE
                if (x >= centerX && x <= centerX + buttonWidth &&
                    y >= startY + buttonSpacing &&
                    y <= startY + buttonSpacing + buttonHeight) {
                    showGuide();  // kembali ke menu
                    break;
                }

                // EXIT
                if (x >= centerX && x <= centerX + buttonWidth &&
                    y >= startY + (2 * buttonSpacing) &&
                    y <= startY + (2 * buttonSpacing) + buttonHeight) {
                    if (confirmExit() == 1) exit(0);
                    break;
                }

                // LEADERBOARD
                int leaderboardX = getmaxwidth() / 2 + 150;
                int leaderboardButtonY = (getmaxheight() / 2 - 100) + 10 + 30 + (5 * 40) + 20 - 10;

                if (x >= leaderboardX && x <= leaderboardX + 350 &&
                    y >= leaderboardButtonY && y <= leaderboardButtonY + 60) {
                    showLeaderboard();
                    break;
                }
            }
        }

        // Setelah keluar dari inner loop
        if (goToGame) {
            return;  // keluar dari menu, jalankan game
        }

        // Jika tidak, maka loop ulang dan tetap di main menu
    }
}

// Fungsi untuk menampilkan halaman (Guide)
void showGuide() {
    cleardevice();
    drawStars();
    
    // Menampilkan gambar panduan
    int guideWidth = 600;  // Sesuaikan ukuran gambar
    int guideHeight = 400;
    int guideX = getmaxwidth() / 2 - (guideWidth / 2);
    int guideY = 50; // Posisi gambar lebih ke bawah
    readimagefile("img/gameguide.bmp", guideX, guideY, guideX + guideWidth, guideY + guideHeight);

    // Ukuran dan posisi tombol "BACK" disamakan dengan tombol lain
    int buttonWidth = 300;
    int buttonHeight = 200;
    int centerX = getmaxwidth() / 2 - (buttonWidth / 2);
    int backY = guideY + guideHeight + 80 - 110;

    // Tombol Back di tengah
    drawImageButton("img/back.bmp", centerX, backY, buttonWidth, buttonHeight);

    // Loop untuk deteksi klik tombol "BACK"
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);

            // Cek apakah klik di tombol "BACK"
            if (x >= centerX && x <= centerX + buttonWidth &&
                y >= backY && y <= backY + buttonHeight) {
                return;  // Kembali ke handleMainMenu tanpa keluar dari loop utama
            }
        }
    }
}

// Fungsi untuk halaman LEADERBOARD
void showLeaderboard() {
    cleardevice();
    drawStars();
    int imgWidth = 400;
    int imgHeight = 200;
    int imgX = getmaxwidth() / 2 - imgWidth / 2;
    int imgY = 5;
    readimagefile("img/leaderboard.bmp", imgX, imgY, imgX + imgWidth, imgY + imgHeight);

    // Menampilkan leaderboard dimulai dari posisi y=150, hanya menampilkan 5 entri
    drawLeaderboard(150, MAX_ENTRIES, 1);  // tampilkan max 7 entri

    int backButtonWidth = 300;
    int backButtonHeight = 200;
    int backButtonX = getmaxwidth() / 2 - backButtonWidth / 2;
    int backButtonY = getmaxheight() - backButtonHeight - 20;
    drawImageButton("img/back.bmp", backButtonX, backButtonY, backButtonWidth, backButtonHeight);

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);

            // Cek apakah klik di tombol "BACK"
            if (x >= backButtonX && x <= backButtonX + backButtonWidth &&
                y >= backButtonY && y <= backButtonY + backButtonHeight) {
                return;  // Kembali ke menu utama
            }
        }
    }
}

// Fungsi untuk EXIT dengan gambar konfirmasi dan tombol YES/NO
int confirmExit() {
    cleardevice();
    drawStars();

    // Ukuran dan posisi gambar konfirmasi
    int questionWidth = 600;
    int questionHeight = 300;
    int questionX = getmaxwidth() / 2 - (questionWidth / 2);
    int questionY = 50;

    // Gambar konfirmasi "Anda yakin ingin keluar?"
    readimagefile("img/question.bmp", questionX, questionY, questionX + questionWidth, questionY + questionHeight);

    // Ukuran dan posisi tombol YES dan NO (disamakan dengan tombol BACK)
    int buttonWidth = 300;
    int buttonHeight = 200;
    int yesX = getmaxwidth() / 2 - buttonWidth - 20;  // YES di kiri
    int noX = getmaxwidth() / 2 + 20;                // NO di kanan
    int buttonY = questionY + questionHeight - 100;

    // Gambar tombol YES dan NO
    readimagefile("img/yes.bmp", yesX, buttonY, yesX + buttonWidth, buttonY + buttonHeight);
    readimagefile("img/no.bmp", noX, buttonY, noX + buttonWidth, buttonY + buttonHeight);

    // Loop untuk deteksi klik tombol YES atau NO
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);

            // Jika klik tombol YES (Keluar dari game)
            if (x >= yesX && x <= yesX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight) {
                cleardevice(); // Menutup jendela grafis
                return 1;      // Kembali ke handleMainMenu untuk exit(0)
            }

            // Jika klik tombol NO (Kembali ke Main Menu)
            if (x >= noX && x <= noX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight) {
                cleardevice(); // Bersihkan layar sebelum kembali
                return 0;      // Kembali ke main menu
            }
        }
    }
}

//============================================== PROGRAM GAMEOVER ================================================

// Menyimpan skor pemain ke leaderboard.txt dan mengurutkan entri berdasarkan skor tertinggi
void savePlayerScore(const char *name, int score) {
    LeaderboardEntry *head = NULL;

    // Membaca leaderboard dari file
    FILE *leaderboardFile = fopen("leaderboard.txt", "r");
    if (leaderboardFile) {
        char tempName[MAX_NAME_LENGTH];
        int tempScore;
        while (fscanf(leaderboardFile, "%s %d", tempName, &tempScore) == 2) {
            LeaderboardEntry *newNode = (LeaderboardEntry *)malloc(sizeof(LeaderboardEntry));
            strcpy(newNode->name, tempName);
            newNode->score = tempScore;
            newNode->next = head;
            head = newNode;
        }
        fclose(leaderboardFile);
    }

    // Cek apakah nama sudah ada di linked list
    LeaderboardEntry *current = head;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->score += score;  // Tambahkan skor ke pemain yang sama
            found = 1;
            break;
        }
        current = current->next;
    }

    // Jika belum ditemukan, tambahkan sebagai entri baru
    if (!found) {
        LeaderboardEntry *newEntry = (LeaderboardEntry *)malloc(sizeof(LeaderboardEntry));
        strncpy(newEntry->name, name, MAX_NAME_LENGTH);
        newEntry->name[MAX_NAME_LENGTH - 1] = '\0';
        newEntry->score = score;
        newEntry->next = head;
        head = newEntry;
    }

    // Mengurutkan secara descending
    LeaderboardEntry *sorted = NULL;
    while (head) {
        LeaderboardEntry *currentEntry = head;
        head = head->next;

        if (!sorted || currentEntry->score > sorted->score) {
            currentEntry->next = sorted;
            sorted = currentEntry;
        } else {
            LeaderboardEntry *currentSorted = sorted;
            while (currentSorted->next && currentSorted->next->score >= currentEntry->score) {
                currentSorted = currentSorted->next;
            }
            currentEntry->next = currentSorted->next;
            currentSorted->next = currentEntry;
        }
    }

    // Menulis kembali leaderboard ke file
    leaderboardFile = fopen("leaderboard.txt", "w");
    if (leaderboardFile) {
        int count = 0;
        LeaderboardEntry *current = sorted;
        while (current && count < MAX_ENTRIES) {
            fprintf(leaderboardFile, "%s %d\n", current->name, current->score);
            current = current->next;
            count++;
        }
        fclose(leaderboardFile);
    }

    // Menghapus semua node linked list
    while (sorted) {
        LeaderboardEntry *temp = sorted;
        sorted = sorted->next;
        free(temp);
    }
}

// Menampilkan layar "Game Over" dan input nama untuk disimpan ke leaderboard
void gameOverScreen() {
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    cleardevice();
    drawStars();

    // Menampilkan gambar "GAME OVER"
    int imgWidth = 400, imgHeight = 200;
    int imgX = screenWidth / 2 - imgWidth / 2;
    int imgY = screenHeight / 4 - imgHeight / 2 - 30;
    const char *gameOverImage = "img/gameover.bmp";
    FILE *imageFile = fopen(gameOverImage, "r");
    if (imageFile) {
        fclose(imageFile);
        readimagefile(gameOverImage, imgX, imgY, imgX + imgWidth, imgY + imgHeight);
    }

    // Menampilkan label "MASUKKAN NAMA"
    int labelWidth = 300, labelHeight = 100;
    int labelX = screenWidth / 2 - labelWidth / 2;
    int labelY = imgY + imgHeight + 20;
    imageFile = fopen("img/label_nama.bmp", "r");
    if (imageFile) {
        fclose(imageFile);
        readimagefile("img/label_nama.bmp", labelX, labelY, labelX + labelWidth, labelY + labelHeight);
    }

    // Kotak input
    int inputBoxX1 = screenWidth / 2 - 150;
    int inputBoxY1 = labelY + labelHeight + 10;
    int inputBoxX2 = screenWidth / 2 + 150;
    int inputBoxY2 = inputBoxY1 + 50;
    setcolor(WHITE);
    rectangle(inputBoxX1, inputBoxY1, inputBoxX2, inputBoxY2);

    // Tombol Submit
    int buttonWidth = 300, buttonHeight = 200;
    int submitX1 = screenWidth / 2 - buttonWidth / 2;
    int submitY1 = inputBoxY2 + 40;
    int submitX2 = submitX1 + buttonWidth;
    int submitY2 = submitY1 + buttonHeight;
    const char *submitImage = "img/submit.bmp";
    imageFile = fopen(submitImage, "r");
    if (imageFile) {
        fclose(imageFile);
        readimagefile(submitImage, submitX1, submitY1, submitX2, submitY2);
    }

    // Input nama
    char playerName[MAX_NAME_LENGTH + 1] = "";
    int index = 0, lastIndex = -1;
    int finalScore = getScore();
    while (kbhit()) getch(); // Bersihkan buffer

    while (1) {
        // Tampilkan nama yang diketik
        if (index != lastIndex) {
            setfillstyle(SOLID_FILL, BLACK);
            bar(inputBoxX1 + 2, inputBoxY1 + 2, inputBoxX2 - 2, inputBoxY2 - 2);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            setbkcolor(BLACK);
            setcolor(WHITE);
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            outtextxy(inputBoxX1 + 10, (inputBoxY1 + inputBoxY2) / 2, playerName);
            lastIndex = index;
        }

        // Keyboard input
        if (kbhit()) {
            char ch = getch();
            if (ch == 13 && index > 0) { // ENTER
                savePlayerScore(playerName, finalScore);
                cleardevice();
                showMainMenu();
                return;
            } else if (ch == 8 && index > 0) { // BACKSPACE
                index--;
                playerName[index] = '\0';
            } else if (index < MAX_NAME_LENGTH && ch >= 32 && ch <= 126) {
                playerName[index++] = ch;
                playerName[index] = '\0';
            }
        }

        // Mouse click pada tombol SUBMIT
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);
            if (x >= submitX1 && x <= submitX2 && y >= submitY1 && y <= submitY2 && strlen(playerName) > 0) {
                savePlayerScore(playerName, finalScore);
                cleardevice();
                showLeaderboard();
                cleardevice();
                showMainMenu();
                break;
            }
        }
    }
}