#include "alien.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Variabel global
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
int BLOCK_SIZE;

// Variabel statis untuk window dan device context
static HWND hwnd;
static HDC hdc;

int main() {
    // Inisialisasi window
    WINDOW_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    WINDOW_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
    BLOCK_SIZE = WINDOW_HEIGHT / 40;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "SpaceInvaders";
    RegisterClass(&wc);

    hwnd = CreateWindow(
        "SpaceInvaders", "Space Alien Invaders",
        WS_POPUP | WS_VISIBLE,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    ShowWindow(hwnd, SW_MAXIMIZE);
    UpdateWindow(hwnd);
    hdc = GetDC(hwnd);

    // Inisialisasi aliens
    Alien aliens[MAX_ALIENS];
    int alienDir = 1;
    int gameOver = 0;
    srand(time(0));

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            int i = row * ALIEN_COLS + col;
            aliens[i].x = col * BLOCK_SIZE * 2 + WINDOW_WIDTH / 10;
            aliens[i].y = row * BLOCK_SIZE * 2 + WINDOW_HEIGHT / 10;
            aliens[i].active = 1;
        }
    }

    // Main loop
    while (!gameOver) {
        // Clear screen
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
        RECT rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);

        // Draw aliens
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active) {
                int x = aliens[i].x, y = aliens[i].y;

                // Badan utama (elips hijau)
                brush = CreateSolidBrush(RGB(0, 255, 0));
                SelectObject(hdc, brush);
                Ellipse(hdc, x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
                DeleteObject(brush);

                // Mata (elips putih besar)
                int eyeSize = BLOCK_SIZE / 2;
                int eyeX = x + BLOCK_SIZE / 4;
                int eyeY = y + BLOCK_SIZE / 4;
                brush = CreateSolidBrush(RGB(255, 255, 255));
                SelectObject(hdc, brush);
                Ellipse(hdc, eyeX, eyeY, eyeX + eyeSize, eyeY + eyeSize);
                DeleteObject(brush);

                // Pupil (bentuk plus hitam di tengah mata)
                int pupilSize = eyeSize / 2;
                int pupilX = eyeX + eyeSize / 4;
                int pupilY = eyeY + eyeSize / 4;
                brush = CreateSolidBrush(RGB(0, 0, 0));
                rect = (RECT){pupilX, pupilY + pupilSize / 4, pupilX + pupilSize, pupilY + pupilSize / 4 + pupilSize / 4};
                FillRect(hdc, &rect, brush);
                rect = (RECT){pupilX + pupilSize / 4, pupilY, pupilX + pupilSize / 4 + pupilSize / 4, pupilY + pupilSize};
                FillRect(hdc, &rect, brush);
                DeleteObject(brush);

                // Duri di sekeliling (8 arah)
                int spikeSize = BLOCK_SIZE / 4;
                int radius = BLOCK_SIZE / 2 + spikeSize / 2;
                brush = CreateSolidBrush(RGB(0, 255, 0));
                for (int j = 0; j < 8; j++) {
                    float angle = j * (3.14159265 / 4);
                    int spikeX = x + BLOCK_SIZE / 2 + cos(angle) * radius - spikeSize / 2;
                    int spikeY = y + BLOCK_SIZE / 2 + sin(angle) * radius - spikeSize / 2;
                    rect = (RECT){spikeX, spikeY, spikeX + spikeSize, spikeY + spikeSize};
                    FillRect(hdc, &rect, brush);
                }
                DeleteObject(brush);
            }
        }

        // Update aliens
        int moveDown = 0;
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active) {
                aliens[i].x += alienDir * BLOCK_SIZE / 2;
                if (aliens[i].x <= 0 || aliens[i].x >= WINDOW_WIDTH - BLOCK_SIZE) moveDown = 1;
                if (aliens[i].y >= WINDOW_HEIGHT - BLOCK_SIZE) gameOver = 1;
            }
        }
        if (moveDown) {
            alienDir = -alienDir;
            for (int i = 0; i < MAX_ALIENS; i++) {
                if (aliens[i].active) aliens[i].y += BLOCK_SIZE;
            }
        }

        // Cek semua alien mati (reset)
        int allDead = 1;
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (aliens[i].active) allDead = 0;
        }
        if (allDead) {
            for (int row = 0; row < ALIEN_ROWS; row++) {
                for (int col = 0; col < ALIEN_COLS; col++) {
                    int i = row * ALIEN_COLS + col;
                    aliens[i].x = col * BLOCK_SIZE * 2 + WINDOW_WIDTH / 10;
                    aliens[i].y = row * BLOCK_SIZE * 2 + WINDOW_HEIGHT / 10;
                    aliens[i].active = 1;
                }
            }
        }

        // Input (cuma tombol X buat keluar)
        if (GetAsyncKeyState('X') & 0x8000) gameOver = 1;

        // Update screen
        MSG msg = {0};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(50);
    }

    // Game over
    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    RECT rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    Sleep(2000);

    return 0;
}