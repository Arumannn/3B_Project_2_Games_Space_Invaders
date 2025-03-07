#include <graphics.h>
#include "mainsprite.h"
#include "ufo.h"
#include "score.h"
#include "alien.h"


 // Inisialisasi UFO
float ufoX = 100.0, ufoY = 100.0;
float ufoSpeed = 2.5;
int ufoDirection = 1;    
int page = 0; // Untuk double buffering
int ufoActive = 1;
int ufoHealth;  // Tambahkan variabel untuk nyawa/hit UFO

// Inisialisasi peluru UFO
int ufoBulletX = -1, ufoBulletY = -1, ufoBulletActive = 0;

Alien aliens[MAX_ALIENS];

// **Tambahkan fungsi untuk reset UFO dengan HP random**
void resetUFO() {
    ufoX = 100.0;
    ufoY = 100.0;
    ufoSpeed = 2.5;
    ufoDirection = 1;
    ufoHealth = 15 + rand() % 6; // Random antara 15-20 hit
    ufoActive = 1;
}

void drawUFO(int x, int y) {
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    ellipse(x, y, 0, 360, 60, 25);
    floodfill(x, y, RED);

    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    fillellipse(x, y - 15, 25, 15);

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x, y, 30, 10);

    setcolor(DARKGRAY);
    line(x - 40, y + 15, x - 50, y + 35);
    line(x + 40, y + 15, x + 50, y + 35);

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    for (int i = -40; i <= 40; i += 20) {
        fillellipse(x + i, y + 5, 5, 5);
    }

    

    
}

void drawBullet(int bx, int by) {
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(bx, by, 3, 3);
}

void UFO(Alien aliens[]) {
    if (!ufoActive) return; // Jika UFO sudah mati, tidak usah update

    // Update posisi UFO
    ufoX += ufoDirection * ufoSpeed;
    if (ufoX > getmaxx() - 60 || ufoX < 60) ufoDirection *= -1;
    drawUFO((int)ufoX, (int)ufoY);

    // Cek jika UFO terkena tembakan
    for (int j = 0; j < MAX_BULLETS; j++) {
        if (bullets_player[j].active &&
            bullets_player[j].x > ufoX - 60 &&
            bullets_player[j].x < ufoX + 60 &&
            bullets_player[j].y > ufoY - 25 &&
            bullets_player[j].y < ufoY + 25) {
            
            bullets_player[j].active = 0; // Nonaktifkan peluru
            ufoHealth--; // Kurangi nyawa UFO

            if (ufoHealth <= 0) { // Jika nyawa habis, matikan UFO
                ufoActive = 0;
                addUFOScore(); // Tambah skor
                
                // Spawn ulang UFO setelah beberapa waktu
                delay(1000); // Bisa diatur ulang
                resetUFO();
            }
        }
    }

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

    // Deteksi tabrakan dengan alien
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int ufoLeft = (int)ufoX - 60;
            int ufoRight = (int)ufoX + 60;
            int ufoTop = (int)ufoY - 25;
            int ufoBottom = (int)ufoY + 25;

            int alienLeft = aliens[i].x;
            int alienRight = aliens[i].x + BLOCK_SIZE;
            int alienTop = aliens[i].y;
            int alienBottom = aliens[i].y + BLOCK_SIZE;

            if (ufoRight > alienLeft && ufoLeft < alienRight &&
                ufoBottom > alienTop && ufoTop < alienBottom) {
                aliens[i].active = 0;
            }
        }
    }
}
