/* gameover.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameover.h"

#define LEADERBOARD_FILE "leaderboard.txt"

void showGameOver(int score) {
    char name[MAX_NAME_LENGTH];
    printf("\n\nGAME OVER\n");
    printf("Masukkan Nama: ");
    scanf("%19s", name);
    saveToLeaderboard(name, score);
    displayLeaderboard();
}

void saveToLeaderboard(const char *name, int score) {
    FILE *file = fopen(LEADERBOARD_FILE, "a");
    if (!file) {
        printf("Error membuka file leaderboard!\n");
        return;
    }
    fprintf(file, "%s %d\n", name, score);
    fclose(file);
}

void displayLeaderboard() {
    FILE *file = fopen(LEADERBOARD_FILE, "r");
    if (!file) {
        printf("Belum ada data leaderboard.\n");
        return;
    }
    
    typedef struct {
        char name[MAX_NAME_LENGTH];
        int score;
    } Entry;
    
    Entry entries[MAX_ENTRIES];
    int count = 0;
    while (fscanf(file, "%s %d", entries[count].name, &entries[count].score) == 2) {
        count++;
        if (count >= MAX_ENTRIES) break;
    }
    fclose(file);
    
    // Sorting berdasarkan skor tertinggi
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (entries[j].score > entries[i].score) {
                Entry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }
    
    printf("\nLEADERBOARD\n");
    printf("Rank | Nama | Skor\n");
    for (int i = 0; i < count; i++) {
        printf("%d | %s | %d\n", i + 1, entries[i].name, entries[i].score);
    }
}