#ifndef MAINMENU_H
#define MAINMENU_H

// Fungsi Menu Utama
void showMainMenu();  // Menampilkan menu utama
void startGame();     // Fungsi untuk memulai permainan
void handleMainMenu(); // Menangani input pada menu utama
void showGuide();     // Menampilkan aturan permainan
void showLeaderboard(); 
int confirmExit();    // Konfirmasi keluar dari permainan (1 = YA, 0 = TIDAK)
void mainMenu(); 

// Fungsi Gambar dan Desain
void drawImageButton();
void drawStars();
void drawLeaderboard(int yOffset);
void createCustomBackground();
void drawCustomBackground();

#endif
