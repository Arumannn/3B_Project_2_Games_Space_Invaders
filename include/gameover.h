#ifndef GAMEOVER_H
#define GAMEOVER_H

#define MAX_ENTRIES 5       // Maksimum 5 pemain di leaderboard
#define MAX_NAME_LENGTH 10  // Maksimum panjang nama pemain

// Struktur untuk menyimpan data leaderboard
typedef struct LeaderboardEntry {
    char name[MAX_NAME_LENGTH];
    int score;
    struct LeaderboardEntry *next;
} LeaderboardEntry;

// Fungsi Game Over
void gameOverScreen();
void savePlayerScore(const char *name, int score);
void createCustomBackground();
void drawCustomBackground();
void startGame();

#endif
