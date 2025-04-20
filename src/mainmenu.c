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
        
        // Buat beberapa bintang lebih besar agar terlihat lebih jelas
        if (rand() % 5 == 0) { 
            setcolor(YELLOW);  // Bintang lebih besar berwarna kuning
            circle(x, y, 2);   // Ukuran lebih besar
        } else {
            setcolor(WHITE);   // Bintang kecil tetap putih
            putpixel(x, y, WHITE);
        }
    }
}


// Fungsi Leaderboard
// Struktur untuk menyimpan data leaderboard
typedef struct {
    char name[10];
    int score;
} LeaderboardEntry;

void drawLeaderboard(int yOffset) {
    int x = getmaxwidth() / 2 + 150;
    int y = yOffset; 
    int width = 350;
    int rowHeight = 40;

    FILE *file = fopen("leaderboard.txt", "r");
    if (!file) {
        drawText(x + width / 2, y, "Belum ada data!", 2, WHITE);
        return;
    }

    LeaderboardEntry entries[100]; // Maksimal 100 pemain
    int count = 0;

    // Membaca data leaderboard
    while (fscanf(file, "%s %d", entries[count].name, &entries[count].score) != EOF) {
        count++;
    }
    fclose(file);

    // Mengurutkan data berdasarkan skor tertinggi (descending order)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (entries[j].score > entries[i].score) {
                LeaderboardEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }

    // Menampilkan leaderboard
    int height = 30 + (count * rowHeight);
    setcolor(WHITE);
    rectangle(x, y, x + width, y + height);
    line(x, y + 30, x + width, y + 30);
    line(x + 60, y, x + 60, y + height);
    line(x + 220, y, x + 220, y + height);

    drawText(x + 30, y + 15, "NO", 2.5, WHITE);
    drawText(x + 140, y + 15, "PLAYER", 2.5, WHITE);
    drawText(x + 285, y + 15, "SCORE", 2.5, WHITE);

    for (int i = 0; i < count; i++) {
        char numText[5], scoreText[10];
        sprintf(numText, "%d", i + 1);
        sprintf(scoreText, "%d", entries[i].score);

        int yPos = y + 50 + (i * rowHeight);
        drawText(x + 30, yPos, numText, 2.5, WHITE);
        drawText(x + 140, yPos, entries[i].name, 2.5, WHITE);
        drawText(x + 285, yPos, scoreText, 2.5, WHITE);
    }
}

// Fungsi Gradasi Background
void drawGradientBackground() {
    int width = getmaxwidth();
    int height = getmaxheight();

    for (int y = 0; y < height; y++) {
        float t = (float)y / height; // Normalisasi 0 - 1

        int r, g, b;

        // Hitam (atas) ke Biru Tua (tengah)
        if (t < 0.5) {
            float t1 = t * 2; // Normalisasi ke 0 - 1 untuk bagian atas
            r = (int)(0 * (1 - t1) + 25 * t1);   // 0 → 25
            g = (int)(0 * (1 - t1) + 25 * t1);   // 0 → 25
            b = (int)(0 * (1 - t1) + 112 * t1);  // 0 → 112
        } 
        // Biru Tua (tengah) ke Ungu Tua (bawah)
        else {
            float t2 = (t - 0.5) * 2; // Normalisasi ke 0 - 1 untuk bagian bawah
            r = (int)(25 * (1 - t2) + 75 * t2);  // 25 → 75
            g = (int)(25 * (1 - t2) + 0 * t2);   // 25 → 0
            b = (int)(112 * (1 - t2) + 130 * t2); // 112 → 130
        }

        setcolor(COLOR(r, g, b));
        line(0, y, width, y);
    }
}

// Fungsi untuk menampilkan menu utama
void showMainMenu() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    cleardevice();
    drawGradientBackground();
    drawStars();

    // Judul tetap di posisinya
    int titleX = getmaxwidth() / 2;
    int titleY = 120; 
    drawText(titleX, titleY, "SPACE INVADERS", 8, WHITE);

    // Posisi tombol
    int centerX = getmaxwidth() / 2 - 400;  // Geser tombol lebih ke kiri
    int startY = getmaxheight() / 2 - 100; // Sedikit naik agar lebih proporsional
    int buttonWidth = 400;
    int buttonHeight = 80;
    int buttonSpacing = 130; // Tambah jarak antar tombol agar lebih luas

    drawButton(centerX, startY, buttonWidth, buttonHeight, RGB(0, 180, 255), "START");
    drawButton(centerX, startY + buttonSpacing, buttonWidth, buttonHeight, RGB(160, 90, 200), "GUIDE");
    drawButton(centerX, startY + (buttonSpacing * 2), buttonWidth, buttonHeight, RGB(120, 150, 255), "EXIT");

    // Leaderboard diposisikan sejajar dengan tombol
    drawLeaderboard(startY + 10); // Tambahkan sedikit offset agar lebih rapi
}

// Perbaikan handleMainMenu agar menu tetap berjalan
void handleMainMenu() {
    int centerX = getmaxwidth() / 2 - 400;
    int startY = getmaxheight() / 2 - 100;
    int buttonWidth = 400;
    int buttonHeight = 80;
    int buttonSpacing = 110;

    
    clearmouseclick(WM_LBUTTONDOWN);

    while (1) {  // **Loop utama agar menu tetap berjalan**
        showMainMenu();

        while (1) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);

                // Cek apakah klik di tombol "START"
                if (x >= centerX && x <= centerX + buttonWidth &&
                    y >= startY && y <= startY + buttonHeight) {
                    Sleep(500);  
                    clearmouseclick(WM_LBUTTONDOWN);
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
                    y >= startY + 2 * buttonSpacing && y <= startY + 2 * buttonSpacing + buttonHeight) {
                        if (confirmExit() == 1) {
                            exit(0);
                        } else {
                            break;  // **Tetap di Main Menu jika memilih "NO"**
                        }
                }
            }
        }
    }
}

void showGuide() {
    cleardevice();
    drawGradientBackground();
    drawStars();
    drawText(getmaxwidth() / 2, 100, "GUIDE", 5, WHITE);
    drawText(getmaxwidth() / 2, 200, "Gunakan Tombol A dan D atau panah untuk bergerak", 3, WHITE);
    drawText(getmaxwidth() / 2, 250, "Tekan spasi untuk menembak", 3, WHITE);
    drawText(getmaxwidth() / 2, 300, "Hindari tembakan musuh", 3, WHITE);
    drawButton(getmaxwidth() / 2 - 100, 400, 200, 50, MAGENTA, "BACK");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);

            // Cek apakah klik di tombol "BACK"
            if (x >= getmaxwidth() / 2 - 100 && x <= getmaxwidth() / 2 + 100 &&
                y >= 400 && y <= 450) {
                return;  // **Kembali ke handleMainMenu tanpa keluar dari loop utama**
            }
        }
    }
}

// Fungsi untuk EXIT
int confirmExit() {
    cleardevice();
    drawGradientBackground();
    drawStars();
    drawText(getmaxwidth() / 2, 200, "Anda yakin ingin keluar?", 3, WHITE);
    drawButton(getmaxwidth() / 2 - 120, 300, 100, 50, CYAN, "YES");
    drawButton(getmaxwidth() / 2 + 20, 300, 100, 50, MAGENTA, "NO");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);

            // Jika klik tombol YES (Keluar dari game)
            if (x >= getmaxwidth() / 2 - 120 && x <= getmaxwidth() / 2 - 20 && y >= 300 && y <= 350) {
              cleardevice(); // Menutup jendela grafis
                return 1;      // Kembali ke handleMainMenu untuk exit(0)
            }

            // Jika klik tombol NO (Kembali ke Main Menu)
            if (x >= getmaxwidth() / 2 + 20 && x <= getmaxwidth() / 2 + 120 && y >= 300 && y <= 350) {
                cleardevice(); // Bersihkan layar sebelum kembali
                return 0;      // Kembali ke main menu
            }
        }
    }
}