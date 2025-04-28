#include <graphics.h>
#include <conio.h>
#include "bullet.h"
#include "barrier.h"
#include "alien.h"
#include "ufo.h"
#include "mainsprite.h"

#define MAX_BARRIERS 4


extern Bullet alienBullets[MAX_ALIEN_BULLETS];
extern BulletNode *playerBullets;

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
        setfillstyle(SOLID_FILL, BLUE);
        bar(b.x + 15, b.y + 15, b.x + 65, b.y + 25);
        bar(b.x + 20, b.y + 10, b.x + 60, b.y + 20);
        bar(b.x + 25, b.y + 5, b.x + 55, b.y + 15);
    }
}

void checkAlienBulletCollision(Barrier barriers[]) {
    BulletNode *current = playerBullets;
while (current != NULL) {
    if (current->bullet.active) {
        int bulletLeft = current->bullet.x;
        int bulletTop = current->bullet.y;
        int bulletRight = current->bullet.x + 10;
        int bulletBottom = current->bullet.y + 10;

        for (int j = 0; j < MAX_BARRIERS; j++) {
            if (barriers[j].health > 0) {
                int BarrierLeft = barriers[j].x;
                int BarrierRight = barriers[j].x + 80;
                int BarrierTop = barriers[j].y - 5;
                int BarrierBottom = barriers[j].y + 25;

                if (bulletRight > BarrierLeft && bulletLeft < BarrierRight &&
                    bulletBottom > BarrierTop && bulletTop < BarrierBottom) {
                    barriers[j].health--;
                    current->bullet.active = 0;
                    drawBarrier(barriers[j]);
                }
            }
        }
    }
    current = current->next;    
    }
}

void initBarriers(Barrier barriers[]) {
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    
    int startX = (screenWidth / 4) - 110;
    int startY = screenHeight - 210;
    int gap = screenWidth / 5;

    for (int i = 0; i < MAX_BARRIERS; i++) {
        barriers[i].x = startX + (i * gap);
        barriers[i].y = startY;
        barriers[i].health = 30;
    }
}