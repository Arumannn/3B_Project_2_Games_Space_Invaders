#ifndef MAINMENU_H
#define MAINMENU_H

void showMainMenu();  // Menampilkan menu utama
void startGame();     // Fungsi untuk memulai permainan
void handleMainMenu(); // Menangani input pada menu utama
void showGuide();     // Menampilkan aturan permainan
void showLeaderboard(); 
int confirmExit();    // Konfirmasi keluar dari permainan (mengembalikan 1 jika YA, 0 jika TIDAK)
void mainMenu(); 
void drawGradientBackground();
void drawStars();
void drawLeaderboard(int yOffset);

#endif
