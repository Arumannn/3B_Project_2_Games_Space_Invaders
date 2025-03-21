#include <graphics.h>
#include <conio.h>
#include "barrier.h"
#include "alien.h"
#include "ufo.h"
#include "mainsprite.h"

#define MAX_BARRIERS 4


extern AlienBullet alienBullets[MAX_ALIEN_BULLETS];
extern Bullet bullets_player[MAX_BULLETS];

// Fungsi untuk menggambar barrier
void drawBarrier(Barrier b) {
    if (b.health > 0) {
        // Menentukan warna berdasarkan health
        int color = WHITE;
        if (b.health > 15 && b.health <= 30 ) color = GREEN;
        else if (b.health > 5 && b.health <=15) color = YELLOW;
        else if (b.health > 1 && b.health <= 5) color = RED;
        
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

void checkAlienBulletCollision(Barrier barriers[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets_player[i].active) {
            int bulletLeft = bullets_player[i].x;
            int bulletTop = bullets_player[i].y;
            int bulletRight = bullets_player[i].x + 10;
            int bulletBottom = bullets_player[i].y + 10;

            for (int j = 0; j < MAX_BARRIERS; j++) {
                if(barriers[j].health > 0){
                int BarrierLeft = barriers[j].x;
                int BarrierRight = barriers[j].x + 80;
                int BarrierTop = barriers[j].y - 5;
                int BarrierBottom = barriers[j].y + 25;
    
                    if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                        bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                        barriers[j].health--;
                        bullets_player[i].active = 0;
                        drawBarrier(barriers[j]);
                    }
                }
            }
        }
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            int bulletLeft = alienBullets[i].x;
            int bulletRight = alienBullets[i].x + BLOCK_SIZE / 2;
            int bulletTop = alienBullets[i].y;
            int bulletBottom = alienBullets[i].y + BLOCK_SIZE;
            
            for (int j = 0; j < MAX_BARRIERS; j++) {
                if(barriers[j].health > 0){
                int BarrierLeft = barriers[j].x;
                int BarrierRight = barriers[j].x + 80;
                int BarrierTop = barriers[j].y - 5;
                int BarrierBottom = barriers[j].y + 25;
    
                    if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                        bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                        barriers[j].health--;
                        alienBullets[i].active = 0;
                        drawBarrier(barriers[j]);
                    }
                }
            }
        }
            
    for (int i = 0; i < MAX_UFO_BULLETS; i++) {
        if (ufoBullets[i].active) {
            int bulletLeft = ufoBullets[i].x - 3;
            int bulletRight = ufoBullets[i].x + 3;
            int bulletTop = ufoBullets[i].y - 3;
            int bulletBottom = ufoBullets[i].y + 3;
                
                for (int j = 0; j < MAX_BARRIERS; j++) {
                    if(barriers[j].health > 0){
                    int BarrierLeft = barriers[j].x;
                    int BarrierRight = barriers[j].x + 80;
                    int BarrierTop = barriers[j].y - 5;
                    int BarrierBottom = barriers[j].y + 25;
        
                    if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                        bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                        barriers[j].health--;
                        ufoBullets[i].active = 0;
                        drawBarrier(barriers[j]);
                    }
                }
            }
        }
    }
}
}
}

void initBarriers(Barrier barriers[]) {
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    
    int startX = (screenWidth / 4) - 110;
    int startY = screenHeight - 170;
    int gap = screenWidth / 5;

    for (int i = 0; i < MAX_BARRIERS; i++) {
        barriers[i].x = startX + (i * gap);
        barriers[i].y = startY;
        barriers[i].health = 30;
    }
}