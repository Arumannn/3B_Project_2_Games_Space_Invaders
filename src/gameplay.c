#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "ufo.h"
#include "barrier.h"
#include "gameplay.h"
#include "mainmenu.h"

static int score = 0;
static int lastLevel = -1;  //ini jangan di hapus (Nazriel)
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
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 5); 

    sprintf(scoreText, "Score: %d", score);
    setcolor(WHITE);
    outtextxy(100, 40, scoreText);
    
    int currentLevel = getCurrentLevel();
    if (lastLevel != currentLevel) {
        lastLevel = currentLevel;
        blinkCounter = 30;  
    }

    sprintf(levelText, "Level: %d", currentLevel);
    
    int textWidth = textwidth(levelText);
    int screenWidth = getmaxx();
    int levelX = (screenWidth) / 2;  
    int levelY = 40;  

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
    updateScore(100);
}

void addUFOScore() {
    updateScore(1000);
}

void addBonusScore() {
    score += 250;
}
