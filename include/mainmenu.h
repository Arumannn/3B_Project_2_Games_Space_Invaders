//===============================================================================================================
// Program dibuat oleh: Rina Permata Dewi
// NIM                : 241511061
// Proyek 2           : Game Space Invaders 
//===============================================================================================================

#ifndef MAINMENU_H
#define MAINMENU_H

// Panjang maksimum nama pemain (digunakan pada leaderboard)
#define MAX_NAME_LENGTH 10

// Jumlah maksimum entri pada leaderboard
#define MAX_ENTRIES 7

// Jumlah entri menu utama yang ditampilkan pada layar
#define DISPLAY_MAINMENU_ENTRIES 5

// Struktur data untuk menyimpan entri leaderboard
typedef struct LeaderboardEntry {
    char name[MAX_NAME_LENGTH];         // Nama pemain
    int score;                          // Skor pemain
    struct LeaderboardEntry *next;      // Pointer ke entri berikutnya (linked list)
} LeaderboardEntry;

// Fungsi untuk menampilkan menu utama
void showMainMenu();

// Fungsi untuk memulai permainan
void startGame();

// Fungsi untuk menangani input dan navigasi pada menu utama
void handleMainMenu();

// Fungsi untuk menampilkan panduan atau aturan permainan
void showGuide();

// Fungsi untuk menampilkan leaderboard (daftar skor tertinggi)
void showLeaderboard();

// Fungsi untuk menampilkan dialog konfirmasi keluar dari permainan
// Mengembalikan nilai 1 jika pemain memilih "YA", 0 jika "TIDAK"
int confirmExit();

// Fungsi utama menu (entry point tampilan menu utama)
void mainMenu();

// Fungsi untuk menggambar tombol menu menggunakan gambar
void drawImageButton();

// Fungsi untuk menggambar latar belakang bintang
void drawStars();

// Fungsi untuk menggambar leaderboard dengan posisi vertikal disesuaikan (yOffset)
void drawLeaderboard(int yOffset);

// Fungsi untuk menampilkan layar Game Over
void gameOverScreen();

// Fungsi untuk menyimpan skor pemain ke leaderboard
void savePlayerScore(const char *name, int score);

// Fungsi untuk membuat latar belakang kustom di tampilan menu
void createCustomBackground();

#endif // MAINMENU_H