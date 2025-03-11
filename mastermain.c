#include <graphics.h>
#include "mainsprite.h"
#include "mainmenu.h"
#include "alien.h"
#include "score.h"
#include "ufo.h"
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>

void startGame() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);  // Inisialisasi mode grafik
    int errorcode = graphresult();  // Cek apakah ada error
    if (errorcode != grOk) {
        printf("Graphics error: %s\n", grapherrormsg(errorcode));
        return;  // Keluar dari fungsi jika mode grafik gagal
    }
    printf("START GAME dipanggil!\n");
    fflush(stdout);

    cleardevice();  

    printf("Game window initialized!\n");
    fflush(stdout);
    printf("Screen width: %d, height: %d\n", getmaxx(), getmaxy());
    fflush(stdout);

    int alienDirFirst = 1;
    int alienDirRest = 1;
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    
    printf("Inisialisasi game...\n");
    fflush(stdout);

    initAliens(aliens);
    printf("Aliens initialized!\n");
    fflush(stdout);

    initBullets();
    printf("Bullets initialized!\n");
    fflush(stdout);

    initScore();
    printf("Score initialized!\n");
    fflush(stdout);

    int gameOver = 0;
    int page = 0;

    while (!gameOver) {
        printf("Game loop running...\n");
        fflush(stdout);
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("ESC pressed! Exiting game...\n");
            fflush(stdout);
            break;
        }

        // **TEST TANPA DOUBLE BUFFERING DULU**
        // cleardevice();
        
        printf("Drawing score...\n");
        fflush(stdout);
        drawScore();

        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
                printf("Alien reached bottom! Game Over\n");
                fflush(stdout);
            }
        }

        printf("Calling SpaceshipMove...\n");
        fflush(stdout);
        SpaceshipMove(&SpaceShip_P);
        printf("Spaceship moved!\n");
        fflush(stdout);

        printf("Updating bullets...\n");
        fflush(stdout);
        updateBullets();
        
        printf("Updating aliens...\n");
        fflush(stdout);
        updateAliens(aliens, &alienDirFirst, &alienDirRest);

        printf("Drawing spaceship...\n");
        fflush(stdout);
        DrawSpaceShip(&SpaceShip_P);
        
        printf("Drawing bullets...\n");
        fflush(stdout);
        drawBullets();
        
        printf("Drawing aliens...\n");
        fflush(stdout);
        drawAliens(aliens);
        
        printf("Moving UFO...\n");
        fflush(stdout);
        moveUFO();

        delay(30);
    }

    printf("Game loop exited\n");
    fflush(stdout);
}


// void startGame() {
//     // Pastikan grafik aktif
//     if (graphresult() != grOk) {
//         printf("Graphics mode error!\n");
//         return;
//     }

//     printf("START GAME dipanggil!\n");
//     fflush(stdout);
//     printf("Game window initialized!\n");
//     fflush(stdout);
//     printf("Screen width: %d, height: %d\n", getmaxx(), getmaxy());
//     fflush(stdout);

//     delay(100); // Tunggu sebelum membersihkan layar
//     setfillstyle(SOLID_FILL, BLACK);
//     bar(0, 0, getmaxx(), getmaxy()); // Ganti cleardevice()

//     printf("2 - After cleardevice/bar\n");
//     fflush(stdout);

//     // Coba teruskan program
//     drawScore();
//     printf("3 - After drawScore\n");
//     fflush(stdout);
// }


int main() {
    int gd = VGA, gm = VGAHI;
    initgraph(&gd, &gm, (char*)"");
    showMainMenu();
    printf("close\n");
    // closegraph();
    return 0;
}