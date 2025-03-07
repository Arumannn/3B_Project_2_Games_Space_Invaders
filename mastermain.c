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
    initgraph(&gd, &gm, ""); // Inisialisasi graphics mode

    showMainMenu();
    Player SpaceShip_P = {getmaxx() / 2, getmaxy() - 80};

    // Inisialisasi aliens
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    initAliens(aliens);

    // Inisialisasi peluru
    initBullets();

    // Inisialisasi UFO
    float ufoX = 100.0, ufoY = 100.0;
    float ufoSpeed = 2.5;
    int ufoDirection = 1;    

    // Inisialisasi peluru UFO
    int ufoBulletX = -1, ufoBulletY = -1, ufoBulletActive = 0;

    int gameOver = 0;
    int page = 0; // Untuk double buffering

    while (!gameOver) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        // Gunakan double buffering untuk mencegah kedap-kedip
        setactivepage(page);
        cleardevice();

        // Cek kalau aliens nyampe bawah (game over)
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active && aliens[i].y >= getmaxy() - BLOCK_SIZE) {
                gameOver = 1;
            }
        }

        SpaceshipMove(&SpaceShip_P);
        updateBullets();
        updateAliens(aliens, &alienDir);
        DrawSpaceShip(&SpaceShip_P); 
        drawBullets();
        drawAliens(aliens);

        // Update dan gambar UFO
        ufoX += ufoDirection * ufoSpeed;
        if (ufoX > getmaxx() - 60 || ufoX < 60) ufoDirection *= -1;
        drawUFO((int)ufoX, (int)ufoY);

        // UFO menembak
        if (ufoBulletActive) {
            ufoBulletY += 10;
            drawBullet(ufoBulletX, ufoBulletY);
            if (ufoBulletY > getmaxy()) ufoBulletActive = 0;
        } else {
            ufoBulletX = (int)ufoX;
            ufoBulletY = (int)ufoY + 20;
            ufoBulletActive = 1;
        }

        // Tampilkan buffer yang sudah digambar
        setvisualpage(page);
        page = 1 - page; // Tukar halaman untuk frame berikutnya

        delay(10);
    }

    closegraph();
    return 0;
}
