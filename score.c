#include "score.h"
#include <graphics.h>
#include <stdio.h>

static int score = 0;

void initScore() {
    score = 0;
}

void updateScore(int points) {
    score += points;
}

void drawScore() {
    printf("Masuk ke drawScore()\n");
    fflush(stdout);

    printf("Sebelum setcolor\n");
    fflush(stdout);
    setcolor(WHITE);
    printf("setcolor berhasil\n");
    fflush(stdout);

    printf("Sebelum menggambar kotak\n");
    fflush(stdout);
    rectangle(10, 10, 100, 50);
    printf("Gambar kotak berhasil\n");
    fflush(stdout);
}



int getScore() {
    return score;
}

// Fungsi untuk menambah skor berdasarkan objek yang dihancurkan
void addAlienScore() {
    updateScore(100);
}

void addUFOScore() {
    updateScore(10000);
}