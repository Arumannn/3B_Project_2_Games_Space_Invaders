#include "alien.h"
#include "mainsprite.h"
#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>



// Variabel global untuk BLOCK_SIZE
int BLOCK_SIZE; // Definisi variabel global

void initAliens(Alien aliens[]) {
    BLOCK_SIZE = getmaxy() / 40; // Set nilai BLOCK_SIZE

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            int i = row * ALIEN_COLS + col;
            aliens[i].x = col * BLOCK_SIZE * 2 + getmaxx() / 10;
            aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 10;
            aliens[i].active = 1;
        }
    }

    srand(time(0));
}

void drawAliens(Alien aliens[]) {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int x = aliens[i].x, y = aliens[i].y;

            // Badan utama (elips hijau)
            setcolor(GREEN);
            setfillstyle(SOLID_FILL, GREEN);
            fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLOCK_SIZE / 2);

            // Mata (elips putih besar)
            int eyeSize = BLOCK_SIZE / 2;
            int eyeX = x + BLOCK_SIZE / 4;
            int eyeY = y + BLOCK_SIZE / 4;
            setcolor(WHITE);
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(eyeX + eyeSize / 2, eyeY + eyeSize / 2, eyeSize / 2, eyeSize / 2);

            // Pupil (bentuk plus hitam di tengah mata)
            int pupilSize = eyeSize / 2;
            int pupilX = eyeX + eyeSize / 4;
            int pupilY = eyeY + eyeSize / 4;
            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            bar(pupilX, pupilY + pupilSize / 4, pupilX + pupilSize, pupilY + pupilSize / 4 + pupilSize / 4);
            bar(pupilX + pupilSize / 4, pupilY, pupilX + pupilSize / 4 + pupilSize / 4, pupilY + pupilSize);

            // Duri di sekeliling (8 arah)
            int spikeSize = BLOCK_SIZE / 4;
            int radius = BLOCK_SIZE / 2 + spikeSize / 2;
            setcolor(GREEN);
            setfillstyle(SOLID_FILL, GREEN);
            for (int j = 0; j < 8; j++) {
                float angle = j * (3.14159265 / 4);
                int spikeX = x + BLOCK_SIZE / 2 + cos(angle) * radius - spikeSize / 2;
                int spikeY = y + BLOCK_SIZE / 2 + sin(angle) * radius - spikeSize / 2;
                bar(spikeX, spikeY, spikeX + spikeSize, spikeY + spikeSize);
            }
        }
    }
}

void updateAliens(Alien aliens[], int *alienDir) {
    int moveDown = 0;
    
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            // Pergerakan alien
            aliens[i].x += *alienDir * BLOCK_SIZE / 2;
            
            // Jika mencapai batas layar, turun ke bawah
            if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                moveDown = 1;
            }
            
            // Jika alien mencapai dasar layar, game over
            if (aliens[i].y >= getmaxy() - BLOCK_SIZE) return;

            // **Cek tabrakan dengan peluru**
            for (int j = 0; j < MAX_BULLETS; j++) {
                if (bullets_player[j].active &&
                    bullets_player[j].x > aliens[i].x &&
                    bullets_player[j].x < aliens[i].x + BLOCK_SIZE &&
                    bullets_player[j].y > aliens[i].y &&
                    bullets_player[j].y < aliens[i].y + BLOCK_SIZE) {
                    
                    // Hapus alien yang terkena peluru
                    aliens[i].active = 0;
                    
                    // Nonaktifkan peluru yang mengenai alien
                    bullets_player[j].active = 0;
                    
                    // Sistem skor
                    addAlienScore();
                }
            }
        }
    }

    // Jika harus turun, semua alien turun satu baris
    if (moveDown) {
        *alienDir *= -10; // Balik arah
        for (int i = 0; i < MAX_ALIENS; i++) {
            aliens[i].y += BLOCK_SIZE / 2; // Turunkan posisi alien
        }
    }
}
