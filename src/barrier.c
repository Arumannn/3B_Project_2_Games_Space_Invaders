#include <graphics.h>
#include <conio.h>
#include "barrier.h"

// Fungsi untuk menggambar barrier
void drawBarrier(int x, int y) {
    setfillstyle(SOLID_FILL, GREEN);
    bar(x, y, x + 40, y + 20); // Barrier utama
    bar(x + 10, y - 10, x + 30, y); // Bagian atas barrier
}

void barBarrier(){
    // Menggunakan getmaxx dan getmaxy untuk layar fullscreen
    int screenWidth = getmaxx();
    int screenHeight =  getmaxy();
    
    // Posisi barrier berdasarkan ukuran layar
    int startX = screenWidth / 5;
    int startY = screenHeight - 130;
    int gap = screenWidth / 5; // Jarak antar barrier
    
    // Menggambar beberapa barrier
    for (int i = 0; i < 4; i++) {
        drawBarrier(startX + (i * gap), startY);
    }
}