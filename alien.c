#include "alien.h"
#include "mainsprite.h"
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
            aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 5; // Spawn point diturunkan
            aliens[i].active = 1;
        }
    }

    srand(time(0));
}

void drawAliens(Alien aliens[]) {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int x = aliens[i].x, y = aliens[i].y;
            int row = i / ALIEN_COLS; // Menentukan baris berdasarkan indeks

            // Pilih desain alien berdasarkan baris
            if (row == 0 || row == 1) { // Baris 1 dan 2: Jenis 1 (kembali ke desain awal)
                // Badan utama (elips hijau)
                setcolor(LIGHTGREEN);
                setfillstyle(SOLID_FILL, LIGHTGREEN);
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
            else if (row == 2 || row == 3) { // Baris 3 dan 4: Jenis 2 (berdasarkan gambar biru muda)
                // Badan utama (oval LightBlue)
                setcolor(LIGHTBLUE);
                setfillstyle(SOLID_FILL, LIGHTBLUE);
                ellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, 0, 360, BLOCK_SIZE / 3, BLOCK_SIZE / 2);

                // Mata (elips putih besar dengan pupil hitam)
                int eyeSize = BLOCK_SIZE / 2;
                int eyeX = x + BLOCK_SIZE / 2;
                int eyeY = y + BLOCK_SIZE / 2;
                setcolor(WHITE);
                setfillstyle(SOLID_FILL, WHITE);
                fillellipse(eyeX, eyeY, eyeSize / 2, eyeSize / 2);

                // Pupil (lingkaran hitam)
                int pupilSize = eyeSize / 3;
                int pupilX = eyeX;
                int pupilY = eyeY;
                setcolor(BLACK);
                setfillstyle(SOLID_FILL, BLACK);
                fillellipse(pupilX, pupilY, pupilSize, pupilSize);

                // Detail bawah (kaki atau elemen kecil)
                setcolor(LIGHTBLUE);
                line(x + BLOCK_SIZE / 4, y + BLOCK_SIZE, x + BLOCK_SIZE / 4, y + 6 * BLOCK_SIZE / 5);
                line(x + 3 * BLOCK_SIZE / 4, y + BLOCK_SIZE, x + 3 * BLOCK_SIZE / 4, y + 6 * BLOCK_SIZE / 5);
            }
            else if (row == 4 || row == 5) { // Baris 5 dan 6: Jenis 3 (berdasarkan gambar merah)
                // Badan utama (elips LightRed)
                setcolor(LIGHTRED);
                setfillstyle(SOLID_FILL, LIGHTRED);
                fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLOCK_SIZE / 2);

                // Mata (elips putih besar dengan pupil hitam)
                int eyeSize = BLOCK_SIZE / 2;
                int eyeX = x + BLOCK_SIZE / 2;
                int eyeY = y + BLOCK_SIZE / 2;
                setcolor(WHITE);
                setfillstyle(SOLID_FILL, WHITE);
                fillellipse(eyeX, eyeY, eyeSize / 2, eyeSize / 2);

                // Pupil (lingkaran hitam)
                int pupilSize = eyeSize / 3;
                int pupilX = eyeX;
                int pupilY = eyeY;
                setcolor(BLACK);
                setfillstyle(SOLID_FILL, BLACK);
                fillellipse(pupilX, pupilY, pupilSize, pupilSize);

                // Tanduk di atas
                setcolor(LIGHTRED);
                line(x + BLOCK_SIZE / 3, y, x + BLOCK_SIZE / 4, y - BLOCK_SIZE / 3);
                line(x + 2 * BLOCK_SIZE / 3, y, x + 3 * BLOCK_SIZE / 4, y - BLOCK_SIZE / 3);
                setfillstyle(SOLID_FILL, LIGHTRED);
                fillellipse(x + BLOCK_SIZE / 4, y - BLOCK_SIZE / 3, 2, 2);
                fillellipse(x + 3 * BLOCK_SIZE / 4, y - BLOCK_SIZE / 3, 2, 2);

                // Titik di bawah (mimik mulut)
                setcolor(LIGHTRED);
                setfillstyle(SOLID_FILL, LIGHTRED);
                fillellipse(x + BLOCK_SIZE / 2 - BLOCK_SIZE / 6, y + BLOCK_SIZE, 2, 2);
                fillellipse(x + BLOCK_SIZE / 2, y + BLOCK_SIZE, 2, 2);
                fillellipse(x + BLOCK_SIZE / 2 + BLOCK_SIZE / 6, y + BLOCK_SIZE, 2, 2);
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
                }
            }
        }
    }

    // Jika harus turun, semua alien turun satu baris
    if (moveDown) {
        *alienDir *= -1; // Balik arah
        for (int i = 0; i < MAX_ALIENS; i++) {
            aliens[i].y += BLOCK_SIZE / 2; // Turunkan posisi alien
        }
    }
}