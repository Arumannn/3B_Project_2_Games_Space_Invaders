#include <graphics.h>
#include "mainsprite.h"
#include "ufo.h"
#include "score.h"
#include "alien.h"
#include <stdlib.h>
#include <time.h>

// Inisialisasi UFO
float ufoX = 100.0, ufoY = 100.0;
float ufoSpeed = 10.0;
int ufoDirection = 1;
int ufoActive = 1;  
int ufoHealth = 20;  // **Pastikan ada nilai awal**
const int ufoMaxHealth = 20;

// Inisialisasi peluru UFO
AlienBullet ufoBullets[MAX_ALIEN_BULLETS];
int ufoBurstCount = 0;
int ufoShootCooldown = 0;

// **Fungsi untuk menembakkan fireball**
void drawExplosion(int x, int y); 
void shootUFOBullet() {
    if (!ufoActive || ufoShootCooldown > 0) {
        ufoShootCooldown--;
        return;
    }

    if (ufoBurstCount >= 3) {  
        ufoBurstCount = 0;
        ufoShootCooldown = 20; // Jeda antar burst
        return;
    }
        
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (!ufoBullets[i].active) {
            // **Acak posisi tembakan sedikit ke kiri/kanan**
            int offsetX = (rand() % 21) - 10;  // -10 hingga 10
            
            ufoBullets[i].x = ufoX + offsetX;
            ufoBullets[i].y = ufoY + 20;
            ufoBullets[i].active = 1;

            // **Acak kecepatan peluru**
            ufoBullets[i].speed = (rand() % 4) + 8; // Kecepatan antara 8 hingga 12
            
            // **Acak arah peluru (miring ke kiri, lurus, atau miring ke kanan)**
            int direction = rand() % 3 - 1;  // -1 (kiri), 0 (lurus), 1 (kanan)
            ufoBullets[i].dx = direction * 2; // Pergerakan horizontal (-2, 0, 2)

            ufoBurstCount++;
            ufoShootCooldown = 5; // Jeda antar peluru dalam satu burst
            break;
        }
    }
}

// **Update pergerakan peluru**
#define FIREBALL_SPEED 12  

void updateUFOBullets() {
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (ufoBullets[i].active) {
            ufoBullets[i].y += ufoBullets[i].speed;  // **Gunakan speed acak**
            ufoBullets[i].x += ufoBullets[i].dx;     // **Gunakan arah acak**

            // **Pastikan peluru tidak keluar dari layar**
            if (ufoBullets[i].y > getmaxy() || ufoBullets[i].x < 0 || ufoBullets[i].x > getmaxx()) {
                ufoBullets[i].active = 0;
            }
        }
    }
}

// **Menggambar Fireball UFO**
void drawUFOBullets() {
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (ufoBullets[i].active) {
            int bx = ufoBullets[i].x;
            int by = ufoBullets[i].y;

            // Efek luar (Merah)
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            fillellipse(bx, by, 6, 6);

            // Efek tengah (Oranye)
            setcolor(14);
            setfillstyle(SOLID_FILL, 14);
            fillellipse(bx, by, 4, 4);

            // Efek dalam (Kuning)
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            fillellipse(bx, by, 2, 2);
        }
    }
}

// **Logika UFO (Pastikan Menembak Fireball)**
void UFO(Alien aliens[]) {
    if (!ufoActive) return;

    ufoX += ufoDirection * ufoSpeed;
    if (ufoX > getmaxx() - 60 || ufoX < 60) ufoDirection *= -1;
    
    drawUFO((int)ufoX, (int)ufoY);
    
    // **Panggil fungsi untuk menembak fireball**
    shootUFOBullet();
    updateUFOBullets();
    drawUFOBullets();

    for (int j = 0; j < MAX_BULLETS; j++) {
        if (bullets_player[j].active &&
            bullets_player[j].x > ufoX - 45 &&
            bullets_player[j].x < ufoX + 45 &&
            bullets_player[j].y > ufoY - 40 && 
            bullets_player[j].y < ufoY + 20) {
            
            bullets_player[j].active = 0;
            if (ufoHealth > 0) {  
                ufoHealth--;
            }

            if (ufoHealth <= 0) {
                drawExplosion((int)ufoX, (int)ufoY);
                ufoActive = 0;
                addUFOScore();
            }
        }
    }
}

// **Inisialisasi HP UFO secara benar**
void initUFO() {
    srand(time(NULL));  
    ufoHealth = 15 + rand() % 6;  // **HP sekarang pasti ada nilainya**
}

// **Efek Ledakan**
void drawExplosion(int x, int y) {
    for (int radius = 10; radius <= 70; radius += 7) {  // **Ukuran ledakan lebih besar**
        // **Efek lingkaran luar (Api merah besar)**
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(x, y, radius, radius);

        // **Efek lingkaran tengah (Oranye)**
        setcolor(14);
        setfillstyle(SOLID_FILL, 14);
        fillellipse(x, y, radius - 7, radius - 7);

        // **Efek lingkaran dalam (Kuning)**
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x, y, radius - 14, radius - 14);

        // **Efek cahaya pusat (Putih)**
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(x, y, radius - 21, radius - 21);

        delay (5);
    }
}


// **Menggambar UFO (HP Bar Sekarang Berfungsi)**
void drawUFO(int x, int y) {
    if (!ufoActive) return;
    y -= 40;

    // **Body utama UFO**
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, 60, 25);

    setcolor(LIGHTRED);
    setfillstyle(SOLID_FILL, LIGHTRED);
    fillellipse(x, y - 10, 50, 15);

    // **Jendela UFO**
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y - 20, 40, 15);
    setcolor(LIGHTCYAN);
    fillellipse(x - 15, y - 25, 8, 8);

    // **Lampu kecil di bawah UFO**
    for (int i = -40; i <= 40; i += 20) {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, DARKGRAY);
        fillellipse(x + i, y + 15, 6, 6);
        
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x + i, y + 15, 4, 4);
    }

    // **Kaki UFO**
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, 3);
    line(x - 45, y + 20, x - 55, y + 45);
    line(x + 45, y + 20, x + 55, y + 45);
    rectangle(x - 55, y + 45, x - 50, y + 50);
    rectangle(x + 50, y + 45, x + 55, y + 50);
    floodfill(x - 52, y + 47, DARKGRAY);
    floodfill(x + 52, y + 47, DARKGRAY);

    // **Tambahkan HP Bar UFO**
    int barWidth = 50;
    int barHeight = 5;
    int barX = x - barWidth / 2;
    int barY = y - 50;
    
    setcolor(WHITE);
    rectangle(barX, barY, barX + barWidth, barY + barHeight);
    
    int hpWidth = (ufoHealth * barWidth) / ufoMaxHealth;
    if (hpWidth > 0) {  // **Pastikan HP Bar tidak menggambar negatif**
        setfillstyle(SOLID_FILL, GREEN);
        bar(barX, barY, barX + hpWidth, barY + barHeight);
    }
}