#include <graphics.h>
#include <conio.h>
#include "barrier.h"

// Fungsi untuk menggambar barrier
void drawBarrier(int x, int y) {
    setfillstyle(SOLID_FILL, GREEN);
        
    // Badan utama barrier
    bar(x, y, x + 50, y + 30);
        
    // Membuat lengkungan dengan blok bertingkat
    bar(x + 5, y - 5, x + 45, y);
    bar(x + 10, y - 10, x + 40, y - 5);
    bar(x + 15, y - 15, x + 35, y - 10);
    bar(x + 20, y - 20, x + 30, y - 15);
        
    // Potongan di tengah bawah
    setfillstyle(SOLID_FILL, BLACK);
    bar(x + 15, y + 20, x + 35, y + 30);
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