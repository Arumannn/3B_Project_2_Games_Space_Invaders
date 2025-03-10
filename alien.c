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
            if (row == 0 || row == 1) {
                // Baris 0 dan 1: Spawn dari kiri
                aliens[i].x = col * BLOCK_SIZE * 2 + getmaxx() / 10;
                aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
                aliens[i].active = 1;
            } else {
                // Baris 2-5: Spawn dari kanan
                aliens[i].x = getmaxx() - (col + 1) * BLOCK_SIZE * 2 - getmaxx() / 10;
                aliens[i].y = row * BLOCK_SIZE * 2 + getmaxy() / 5;
                aliens[i].active = 1;
            }
        }
    }

    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        alienBullets[i].active = 0;
    }

    for (int i = 0; i < MAX_ALIENS; i++) {
        alienExplosions[i].active = 0;
        alienExplosions[i].lifetime = 0;
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

void updateAliens(Alien aliens[], int *alienDirFirst, int *alienDirRest) {
    int moveDownFirst = 0; // Untuk baris 0-1
    int moveDownRest = 0;  // Untuk baris 2-5
    static int frameCounter = 0;
    frameCounter++;

    // Update peluru alien
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++) {
        if (alienBullets[i].active) {
            alienBullets[i].y += BLOCK_SIZE / 2;
            if (alienBullets[i].y > getmaxy()) {
                alienBullets[i].active = 0;
            }
        }
    }

    // Update pergerakan alien berdasarkan baris
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            int row = i / ALIEN_COLS;

            if (row == 0 || row == 1) {
                // Baris 0 dan 1: Bergerak ke kanan dari kiri
                aliens[i].x += *alienDirFirst * BLOCK_SIZE / 2;
                if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                    moveDownFirst = 1;
                }
            }
            else if (row == 2 || row == 3) {
                // Baris 2 dan 3: Bergerak ke kiri dari kanan
                aliens[i].x += *alienDirRest * BLOCK_SIZE / 2;
                if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                    moveDownRest = 1;
                }
            }
            else if (row == 4 || row == 5) {
                // Baris 4 dan 5: Bergerak ke kiri dari kanan dengan gelombang
                aliens[i].x += *alienDirRest * BLOCK_SIZE / 2;
                aliens[i].y += (int)(sin(frameCounter * 0.1) * 5); // Gelombang kecil
                if (aliens[i].x <= 0 || aliens[i].x >= getmaxx() - BLOCK_SIZE) {
                    moveDownRest = 1;
                }
            }

            // Logika alien menembak
            if (rand() % 5000 < 10) {
                for (int j = 0; j < MAX_ALIEN_BULLETS; j++) {
                    if (!alienBullets[j].active) {
                        alienBullets[j].x = aliens[i].x + BLOCK_SIZE / 4;
                        alienBullets[j].y = aliens[i].y + BLOCK_SIZE;
                        alienBullets[j].active = 1;
                        break;
                    }
                }
            }
        }
    }

    // Turun untuk baris 0-1
    if (moveDownFirst) {
        *alienDirFirst *= -1;
        for (int i = 0; i < MAX_ALIENS; i++) {
            int row = i / ALIEN_COLS;
            if (row == 0 || row == 1) {
                aliens[i].y += BLOCK_SIZE * 2;
            }
        }
    }

    // Turun untuk baris 2-5
    if (moveDownRest) {
        *alienDirRest *= -1;
        for (int i = 0; i < MAX_ALIENS; i++) {
            int row = i / ALIEN_COLS;
            if (row >= 2) {
                aliens[i].y += BLOCK_SIZE * 2;
            }
        }
    }
}

void checkAlienCollisions(Alien aliens[], Bullet bullets[], int bulletCount) {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].active) {
            for (int j = 0; j < bulletCount; j++) {
                if (bullets[j].active) {
                    // Perluas area tabrakan untuk akurasi
                    int alienLeft = aliens[i].x;
                    int alienRight = aliens[i].x + BLOCK_SIZE;
                    int alienTop = aliens[i].y;
                    int alienBottom = aliens[i].y + BLOCK_SIZE;

                    int bulletLeft = bullets[j].x - BLOCK_SIZE / 4;
                    int bulletRight = bullets[j].x + BLOCK_SIZE / 4;
                    int bulletTop = bullets[j].y;
                    int bulletBottom = bullets[j].y + BLOCK_SIZE;

                    // Cek tabrakan dengan rentang yang lebih jelas
                    if (bulletRight > alienLeft && bulletLeft < alienRight &&
                        bulletBottom > alienTop && bulletTop < alienBottom) {
                        // Tambahkan ledakan
                        for (int k = 0; k < MAX_ALIENS; k++) {
                            if (!alienExplosions[k].active) {
                                alienExplosions[k].x = aliens[i].x + BLOCK_SIZE / 2;
                                alienExplosions[k].y = aliens[i].y + BLOCK_SIZE / 2;
                                alienExplosions[k].active = 1;
                                alienExplosions[k].lifetime = 5;
                                break;
                            }
                        }
                        // Nonaktifkan alien dan peluru
                        aliens[i].active = 0;
                        bullets[j].active = 0;
                        break; // Keluar dari loop peluru setelah tabrakan
                    }
                }
            }
        }
    }

    // Perbarui lifetime ledakan
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (alienExplosions[i].active) {
            alienExplosions[i].lifetime--;
            if (alienExplosions[i].lifetime <= 0) {
                alienExplosions[i].active = 0;
            }
        }
    }
}

void drawAlienExplosions() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (alienExplosions[i].active) {
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            fillellipse(alienExplosions[i].x, alienExplosions[i].y, BLOCK_SIZE, BLOCK_SIZE); // Lingkaran kuning
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            fillellipse(alienExplosions[i].x, alienExplosions[i].y, BLOCK_SIZE / 2, BLOCK_SIZE / 2); // Lingkaran merah
        }
    }
}
