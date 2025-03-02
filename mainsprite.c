#include <graphics.h>
#include "mainsprite.h"

Bullet bullets[MAX_BULLETS];

void DrawSpaceShip(Player *player) { // Ubah nama jadi DrawSpaceShip
    int x = player->X_Player;
    int y = player->Y_Player;

    // Warna utama badan kapal
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    int body[] = {x, y, x - 20, y + 40, x + 20, y + 40, x, y};
    fillpoly(4, body);

    // Kokpit
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y + 10, 7, 10);

    // Sayap kiri
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int leftWing[] = {x - 20, y + 40, x - 40, y + 60, x - 20, y + 60, x - 20, y + 40};
    fillpoly(4, leftWing);

    // Sayap kanan
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int rightWing[] = {x + 20, y + 40, x + 40, y + 60, x + 20, y + 60, x + 20, y + 40};
    fillpoly(4, rightWing);

    // Mesin
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    bar(x - 10, y + 40, x + 10, y + 50);

    // Cahaya mesin (api thruster)
    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    int thruster[] = {x - 5, y + 50, x + 5, y + 50, x, y + 70, x - 5, y + 50};
    fillpoly(4, thruster);
}

void SpaceshipMove(Player *player) {
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000) && player->X_Player > 40) {
        player->X_Player -= 10;
    }
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) && player->X_Player < getmaxx() - 40) {
        player->X_Player += 10;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        ShootBullet(player);
    }
}

void SpaceShip(Player *player) {
    int buffer = imagesize(0, 0, getmaxx(), getmaxy());
    void *frameBuffer = malloc(buffer);

    while (1) {
        getimage(0, 0, getmaxx(), getmaxy(), frameBuffer);
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }
        cleardevice();
        SpaceshipMove(player);
        updateBullets();
        drawBullets();
        delay(30);
    }
}

void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

void ShootBullet(Player *player) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = player->X_Player;
            bullets[i].y = player->Y_Player - 10;
            bullets[i].active = 1;
            break;
        }
    }
}

void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].y -= 10;
            if (bullets[i].y < 0) {
                bullets[i].active = 0;
            }
        }
    }
}

void drawBullets() {
    setcolor(YELLOW);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            circle(bullets[i].x, bullets[i].y, 2);
            floodfill(bullets[i].x, bullets[i].y, YELLOW);
        }
    }
}