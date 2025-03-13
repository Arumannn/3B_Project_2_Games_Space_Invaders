#include <graphics.h>
#include <conio.h>
#include "barrier.h"

// Fungsi untuk menggambar barrier
void drawBarrier(int x, int y) {
    setfillstyle(SOLID_FILL, GREEN);
        
    // Badan utama barrier
    bar(x, y - 5, x + 80, y + 25);
        
    // Membuat lengkungan dengan blok bertingkat lebih besar
    bar(x + 5, y - 10, x + 75, y - 5);
    bar(x + 10, y - 15, x + 70, y - 10);
    bar(x + 15, y - 20, x + 65, y - 15);
  
        
    // Potongan di tengah bawah lebih besar
    setfillstyle(SOLID_FILL, BLACK);
    bar(x + 15, y + 15, x + 65, y + 25);
    bar(x + 20, y + 10, x + 60, y + 20);
    bar(x + 25, y + 5, x + 55, y + 15);

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