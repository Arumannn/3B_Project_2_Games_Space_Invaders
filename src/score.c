#include "score.h"
#include "level.h"
#include <graphics.h>
#include <stdio.h>

static int score = 0;
static int lastLevel = -1;  
static int blinkCounter = 0;  

void initScore() {
    score = 0;
    lastLevel = -1; 
    blinkCounter = 0; 
}

void updateScore(int points) {
    score += points;
}

void drawScore() {
    char scoreText[20];
    char levelText[20];
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2); 

    sprintf(scoreText, "Score: %d", score);
    setcolor(WHITE);
    outtextxy(10, 10, scoreText);
    
    int currentLevel = getCurrentLevel();
    if (lastLevel != currentLevel) {
        lastLevel = currentLevel;
        blinkCounter = 30;  
    }

    sprintf(levelText, "Level: %d", currentLevel);
    
    int textWidth = textwidth(levelText);
    int screenWidth = getmaxx();
    int levelX = (screenWidth - textWidth) / 2;  
    int levelY = 10;  

    if (blinkCounter > 0) {
        if (blinkCounter % 10 < 5) {
            setcolor(CYAN);  
        } else {
            setcolor(WHITE);
        }
        blinkCounter--;
    } else {
        setcolor(WHITE);  
    }
    
    outtextxy(levelX, levelY, levelText);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

int getScore() {
    return score;
}

void addAlienScore() {
    updateScore(10);
}

void addUFOScore() {
    updateScore(100);
}