#ifndef ALIEN_H
#define ALIEN_H

#include <windows.h>

// Konstanta
#define ALIEN_ROWS 5
#define ALIEN_COLS 10
#define MAX_ALIENS (ALIEN_ROWS * ALIEN_COLS)

// Struktur untuk alien
typedef struct {
    int x, y;
    int active;
} Alien;

// Variabel global
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int BLOCK_SIZE;

#endif // GRAPHICS_H