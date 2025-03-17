/* gameover.h */
#ifndef GAMEOVER_H
#define GAMEOVER_H

#define MAX_NAME_LENGTH 20
#define MAX_ENTRIES 10

void showGameOver(int score);
void saveToLeaderboard(const char *name, int score);
void displayLeaderboard();

#endif /* GAMEOVER_H */
