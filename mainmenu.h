#ifndef MAINMENU_H
#define MAINMENU_H

#include <graphics.h>

// Fungsi untuk menampilkan menu utama
void showMainMenu();

// Fungsi untuk menampilkan teks di posisi tertentu
void drawText(const char *text, int x, int y, int color);

// Fungsi untuk menggambar tombol
void drawButton(const char *label, int x, int y, int width, int height, int color);

#endif
