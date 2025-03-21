#include <graphics.h>
#include <conio.h>
#include "barrier.h"
#include "alien.h"

#define MAX_BARRIERS 4

extern AlienBullet alienBullets[MAX_ALIEN_BULLETS];

// Fungsi untuk menggambar barrier
void drawBarrier(Barrier b) {
    if (b.health > 0) {
        // Menentukan warna berdasarkan health
        int color;
        if (b.health == 3) color = GREEN;
        else if (b.health == 2) color = YELLOW;
        else if (b.health == 1) color = RED;
        
        setfillstyle(SOLID_FILL, color);
        
        // Badan utama barrier
        bar(b.x, b.y - 5, b.x + 80, b.y + 25);
        
        // Membuat lengkungan dengan blok bertingkat lebih besar
        bar(b.x + 5, b.y - 10, b.x + 75, b.y - 5);
        bar(b.x + 10, b.y - 15, b.x + 70, b.y - 10);
        bar(b.x + 15, b.y - 20, b.x + 65, b.y - 15);
  
        
        // Potongan di tengah bawah lebih besar
        setfillstyle(SOLID_FILL, BLACK);
        bar(b.x + 15, b.y + 15, b.x + 65, b.y + 25);
        bar(b.x + 20, b.y + 10, b.x + 60, b.y + 20);
        bar(b.x + 25, b.y + 5, b.x + 55, b.y + 15);
    }
}

void damageBarrier(Barrier *b) {
    if (b->health > 0) {
        b->health--;
    }
}

void checkAlienBulletCollision(Barrier barriers[]) {
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            int bulletLeft = alienBullets[i].x;
            int bulletRight = alienBullets[i].x + BLOCK_SIZE / 2;
            int bulletTop = alienBullets[i].y;
            int bulletBottom = alienBullets[i].y + BLOCK_SIZE;
    
            int BarrierLeft = barriers->x;
            int BarrierRight = barriers->x + 80;
            int BarrierTop = barriers->y - 5;
            int BarrierBottom = barriers->y + 25;
    
                if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                    bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                    damageBarrier(barriers);
                    alienBullets[i].active = 0;
                }
            }
        }
    }

void barBarrier(){
    // Menggunakan getmaxx dan getmaxy untuk layar fullscreen
    int screenWidth = getmaxx();
    int screenHeight =  getmaxy();
    Barrier barriers[MAX_BARRIERS];
    // Posisi barrier berdasarkan ukuran layar
    int startX = (screenWidth / 4) - 110;
    int startY = screenHeight - 130;
    int gap = screenWidth / 4; // Jarak antar barrier
    

    for (int i = 0; i < MAX_BARRIERS; i++) {
        barriers[i].x = startX + (i * gap);
        barriers[i].y = startY;
        barriers[i].health = 3;
    
    // Menggambar beberapa barrier
    for (int i = 0; i < MAX_BARRIERS; i++) {
        drawBarrier(barriers[i]);
         }  
    }

    checkAlienBulletCollision(barriers);
}       