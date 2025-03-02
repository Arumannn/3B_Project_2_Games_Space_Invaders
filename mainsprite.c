#include <graphics.h>
#include "mainsprite.h"


// void MoveBullets(Player *player) {
//     int step = 10;
//     while(1){
//         cleardevice();



//     }
// }

void SpaceshipMove(Player *player, int y) {
    int step = 10;
    while(1){
        cleardevice();
        
        if (kbhit()) {
            char key = getch();
            if (key == 27) {
                break;
            }
            else if (key == 'a' && player->x_Player > 40) {
                player->x_Player -= step;
            } else if (key == 'd' && player->x_Player < getmaxy()- 40) {
                player->x_Player += step;
            }
            
       if (key == 32)
        {
            MoveBullets(player);
        }
        }
        

        // Badan utama
        line(player->x_Player, y, player->x_Player - 20, y + 40);
        line(player->x_Player, y, player->x_Player + 20, y + 40);
        line(player->x_Player - 20, y + 40, player->x_Player + 20, y + 40);
        
        // Sayap kiri
        line(player->x_Player - 20, y + 40, player->x_Player - 40, y + 60);
        line(player->x_Player - 40, y + 60, player->x_Player - 20, y + 60);
        
        // Sayap kanan
        line(player->x_Player + 20, y + 40, player->x_Player + 40, y + 60);
        line(player->x_Player + 40, y + 60, player->x_Player + 20, y + 60);
        
        // Mesin
        line(player->x_Player - 10, y + 40, player->x_Player - 10, y + 50);
        line(player->x_Player + 10, y + 40, player->x_Player + 10, y + 50);
        
        // Cahaya mesin
        setcolor(LIGHTBLUE);
        line(player->x_Player - 5, y + 50, player->x_Player, y + 60);
        line(player->x_Player + 5, y + 50, player->x_Player, y + 60);
        
    };
    setcolor(WHITE);
    
}