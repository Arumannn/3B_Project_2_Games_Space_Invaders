#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "ufo.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");

    showMainMenu();
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};

    Alien aliens[MAX_ALIENS];
    int alienDirFirst = 1;  // Arah untuk baris 0-1 (kiri ke kanan)
    int alienDirRest = -1;  // Arah untuk baris 2-5 (kanan ke kiri)
    initAliens(aliens);

    initBullets();

    int gameOver = 0;
    int page = 0;

    // Kontrol frame rate
    const int TARGET_FPS = 30; // Target 30 FPS
    const int FRAME_TIME = 1000 / TARGET_FPS; // Waktu per frame dalam milidetik (33 ms)
    clock_t startTime, endTime;

    while (!gameOver) {
        startTime = clock(); // Catat waktu mulai frame

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        setactivepage(page);
        cleardevice();

        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDirFirst, &alienDirRest);
        checkAlienCollisions(aliens, bullets_player, MAX_BULLETS);

        DrawSpaceShip(&SpaceShip_P);
        drawBullets();
        drawAliens(aliens);
        drawAlienExplosions();
        UFO(aliens);

        setvisualpage(page);
        page = 1 - page;

        // Kontrol frame rate: Tunggu hingga waktu frame selesai
        endTime = clock();
        int elapsedTime = (endTime - startTime) * 1000 / CLOCKS_PER_SEC; // Konversi ke ms
        int delayTime = FRAME_TIME - elapsedTime;
        if (delayTime > 0) {
            delay(delayTime); // Delay hanya jika waktu pemrosesan kurang dari target
        }
    }

    closegraph();
    return 0;
}