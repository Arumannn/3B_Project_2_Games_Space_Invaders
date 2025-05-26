#ifndef MAINMENU_H
#define MAINMENU_H

#define MAX_NAME_LENGTH 10 // Maksimum panjang nama

#define MAX_ENTRIES 7
#define DISPLAY_MAINMENU_ENTRIES 5

// Struktur untuk menyimpan data leaderboard
typedef struct LeaderboardEntry {
    char name[MAX_NAME_LENGTH];
    int score;
    struct LeaderboardEntry *next;
} LeaderboardEntry;

void showMainMenu();  // Menampilkan menu utama
void startGame();     // Fungsi untuk memulai permainan
void handleMainMenu(); // Menangani input pada menu utama
void showGuide();     // Menampilkan aturan permainan
void showLeaderboard(); 
int confirmExit();    // Konfirmasi keluar dari permainan (mengembalikan 1 jika YA, 0 jika TIDAK)
void mainMenu(); 
void drawImageButton();
void drawStars();
void drawLeaderboard(int yOffset);

void gameOverScreen();
void savePlayerScore(const char *name, int score);
void createCustomBackground();


#endif
