#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include "barrier.h"
#include "gameplay.h"

int main() {
    srand(time(NULL));
    initwindow(getmaxwidth(), getmaxheight(), "Space Invaders");
    showMainMenu();
    handleMainMenu(); 

    closegraph();
    return 0;
}