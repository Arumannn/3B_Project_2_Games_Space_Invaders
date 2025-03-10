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
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    setcolor(WHITE);
    outtextxy(10, 10, scoreText);
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