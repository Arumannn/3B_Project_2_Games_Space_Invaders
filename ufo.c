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
void resetUFO();

void resetUFO() {
    ufoX = 100.0; // Reset posisi horizontal aja
    ufoSpeed = 3.0;
    ufoDirection = 1;
    ufoHealth = 15 + rand() % 6;
    ufoActive = 1;
}

void drawUFO(int x, int y) {
    // Geser ke atas lebih jauh, dari -20 jadi -40
    y -= 40;

    // Body utama UFO (ukuran tetap seperti sebelumnya)
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, 60, 25);

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y - 5, 55, 20);

    setcolor(LIGHTRED);
    setfillstyle(SOLID_FILL, LIGHTRED);
    fillellipse(x, y - 10, 50, 15);

    // Jendela
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y - 20, 40, 15);
    setcolor(LIGHTCYAN);
    fillellipse(x - 15, y - 25, 8, 8);

    // Lampu LED
    for (int i = -40; i <= 40; i += 20) {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, DARKGRAY);
        fillellipse(x + i, y + 15, 6, 6);
        
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x + i, y + 15, 4, 4);
        
        setcolor(WHITE);
        fillellipse(x + i - 1, y + 13, 1, 1);
    }

    // Kaki
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, 3);
    line(x - 45, y + 20, x - 55, y + 45);
    line(x + 45, y + 20, x + 55, y + 45);
    rectangle(x - 55, y + 45, x - 50, y + 50);
    rectangle(x + 50, y + 45, x + 55, y + 50);
    floodfill(x - 52, y + 47, DARKGRAY);
    floodfill(x + 52, y + 47, DARKGRAY);
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
            bullets_player[j].x > ufoX - 45 &&
            bullets_player[j].x < ufoX + 45 &&
            bullets_player[j].y > ufoY - 40 && // Dari -20 jadi -40
            bullets_player[j].y < ufoY + 20) {

            bullets_player[j].active = 0; // Nonaktifkan peluru
            ufoHealth--; // Kurangi nyawa UFO

            if (ufoHealth <= 0) { // Jika nyawa habis, matikan UFO
                ufoActive = 0;
                addUFOScore(); // Tambah skor
                
                // Spawn ulang UFO setelah beberapa waktu
                delay(10); // Bisa diatur ulang
                resetUFO(); // Cuma reset X, health, dll, gak ubah Y
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
        ufoBulletY = (int)ufoY + 15; // Dari +20 jadi +15
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