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
    drawLeaderboardRight(leaderboardY, 5);  // Hanya menampilkan 5 entri di halaman utama

    // Hitung posisi Y untuk tombol LEADERBOARD
    int leaderboardHeight = 30 + (5 * rowHeight);  // Sesuaikan dengan jumlah yang ditampilkan
    int leaderboardButtonY = leaderboardY + leaderboardHeight + 20;

    // Tombol "LEADERBOARD" berada tepat di bawah tabel
    drawImageButton("img/more.bmp", leaderboardX, leaderboardButtonY, 350, 200);
}

// Perbaikan handleMainMenu agar menu tetap berjalan
void handleMainMenu() {
    clearmouseclick(WM_LBUTTONDOWN);

    while (1) {  // **Loop utama agar menu tetap berjalan**
        showMainMenu();

        // Posisi dan ukuran tombol (disamakan dengan showMainMenu)
        int centerX = getmaxwidth() / 2 - 350;  // Geser tombol lebih ke kiri
        int startY = getmaxheight() / 2 - 160; // Sedikit naik agar lebih proporsional
        int buttonWidth = 300;
        int buttonHeight = 200;
        int buttonSpacing = 140;

        while (1) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);
                clearmouseclick(WM_LBUTTONDOWN);

                // Cek apakah klik di tombol "START"
                if (x >= centerX && x <= centerX + buttonWidth &&
                    y >= startY && y <= startY + buttonHeight) {
                    Sleep(500);  
                    startGame();
                    break;  // **Keluar dari loop input dan kembali ke Main Menu setelah Game selesai**
                }

                // Cek apakah klik di tombol "GUIDE"
                if (x >= centerX && x <= centerX + buttonWidth &&
                    y >= startY + buttonSpacing && y <= startY + buttonSpacing + buttonHeight) {
                    showGuide();
                    break;  // **Setelah kembali dari Guide, ulangi loop agar tetap di Main Menu**
                }

                // Cek apakah klik di tombol "EXIT"
                if (x >= centerX && x <= centerX + buttonWidth &&
                    y >= startY + (2 * buttonSpacing) && 
                    y <= startY + (2 * buttonSpacing) + buttonHeight) {
                        if (confirmExit() == 1) {
                            exit(0);
                        } else {
                            break;  // **Tetap di Main Menu jika memilih "NO"**
                        }
                }

                // Cek apakah klik di tombol "LEADERBOARD" di sisi kanan layar
                int leaderboardX = getmaxwidth() / 2 + 150;
                int rowHeight = 40;
                int count = 0;
                FILE* file = fopen("leaderboard.txt", "r");
                char name[10];
                int score;
                while (file && fscanf(file, "%s %d", name, &score) != EOF) count++;
                if (file) fclose(file);

                int leaderboardHeight = 30 + (5 * rowHeight);
                int leaderboardButtonY = (getmaxheight() / 2 - 100) + 10 + leaderboardHeight + 20;

                if (x >= leaderboardX && x <= leaderboardX + 350 &&
                    y >= leaderboardButtonY && y <= leaderboardButtonY + 60) {
                    showLeaderboard();
                    break;
                }

            }
        }
    }
}


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
    
   

// Fungsi untuk LEADERBOARD
void showLeaderboard() {
    cleardevice();
    drawStars();
    drawText(getmaxwidth() / 2, 100, "LEADERBOARD", 5, WHITE);

    // Menampilkan leaderboard dimulai dari posisi y=150, hanya menampilkan 5 entri
    drawLeaderboard(150, 0, 1);  // Menggunakan limit 5 untuk menampilkan 5 entri leaderboard

    drawButton(getmaxwidth() / 2 - 100, getmaxheight() - 100, 200, 50, MAGENTA, "BACK");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);

            // Cek apakah klik di tombol "BACK"
            if (x >= getmaxwidth() / 2 - 100 && x <= getmaxwidth() / 2 + 100 &&
                y >= getmaxheight() - 100 && y <= getmaxheight() - 50) {
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

// Tampilan layar GAME OVER
void gameOverScreen() {
    // Inisialisasi Window dan Layar 
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    
    cleardevice();

    // Background 
    drawStars();

    // Teks "GAME OVER!!" 
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 10);

    int textWidth = textwidth((char *) "GAME OVER!!");
    int textHeight = textheight((char*)"GAME OVER!!");
    outtextxy((screenWidth - textWidth) / 2, (screenHeight / 4) - (textHeight / 2), (char*)"GAME OVER!!");

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

    // Tombol "SUBMIT" 
    int buttonWidth = 200, buttonHeight = 50;
    int submitButtonX1 = (screenWidth - buttonWidth) / 2;
    int submitButtonY1 = inputBoxY2 + 30;
    int submitButtonX2 = submitButtonX1 + buttonWidth;
    int submitButtonY2 = submitButtonY1 + buttonHeight;
     
    setfillstyle(SOLID_FILL, CYAN);
    bar(submitButtonX1, submitButtonY1, submitButtonX2, submitButtonY2);

    setcolor(WHITE);
    setbkcolor(CYAN);
    
    int textSubmitWidth = textwidth((char*)"SUBMIT");
    int textSubmitHeight = textheight((char*)"SUBMIT");
    outtextxy((submitButtonX1 + submitButtonX2) / 2 - textSubmitWidth / 2,
              (submitButtonY1 + submitButtonY2) / 2 - textSubmitHeight / 2, 
              (char*)"SUBMIT");

    setbkcolor(BLACK);

    // Input Nama Pemain Secara Dinamis
    char playerName[MAX_NAME_LENGTH + 1] = "";
    int index = 0;
    char ch;
    int lastIndex = -1; 
    int finalScore = getScore();
    printf("otw loop gameover\n");

    
    while (1) {
        printf("Masuk ke gameover\n");
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
            if (ch == 13 && index > 0) {
                savePlayerScore(playerName, finalScore);
                cleardevice();
                showMainMenu();
                return;
            } 
            else if (ch == 8 && index > 0) {
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
    }
}