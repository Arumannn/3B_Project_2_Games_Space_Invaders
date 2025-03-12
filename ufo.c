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
// **Tambahkan fungsi untuk reset UFO dengan HP random**


void resetUFO() {
ufoX = 100.0;
ufoY = 100.0;
ufoSpeed = 3.0;
ufoDirection = 1;
ufoHealth = 15 + rand() % 6;
ufoActive = 1;
}

void drawUFO(int x, int y) {
// Body utama UFO dengan efek gradasi merah metalik
setcolor(RED);
setfillstyle(SOLID_FILL, RED);
fillellipse(x, y, 80, 35);

setcolor(RED);
setfillstyle(SOLID_FILL, RED);
fillellipse(x, y - 5, 75, 30);

setcolor(LIGHTRED);
setfillstyle(SOLID_FILL, LIGHTRED);
fillellipse(x, y - 10, 70, 25);

// Jendela dengan efek refleksi
setcolor(WHITE);
setfillstyle(SOLID_FILL, CYAN);
fillellipse(x, y - 25, 50, 25);
setcolor(LIGHTCYAN);
fillellipse(x - 20, y - 30, 12, 12);

// Lampu LED dengan efek glow
for (int i = -55; i <= 55; i += 25) {
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(x + i, y + 20, 8, 8);
    
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x + i, y + 20, 6, 6);
    
    setcolor(WHITE);
    fillellipse(x + i - 2, y + 18, 2, 2);
}
// Kaki lebih futuristik
setcolor(DARKGRAY);
setlinestyle(SOLID_LINE, 0, 3);
line(x - 60, y + 30, x - 75, y + 60);
line(x + 60, y + 30, x + 75, y + 60);
rectangle(x - 75, y + 60, x - 65, y + 70);
rectangle(x + 65, y + 60, x + 75, y + 70);
floodfill(x - 70, y + 65, DARKGRAY);
floodfill(x + 70, y + 65, DARKGRAY);

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
                delay(10); // Bisa diatur ulang
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
