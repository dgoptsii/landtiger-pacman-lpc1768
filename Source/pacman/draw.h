#ifndef DRAW_H
#define DRAW_H

#include "./pacman.h"

typedef enum {
  RedrawPacman = 1,
	RedrawGhost = 1 << 1,
  RedrawWalls = 1 << 2,
  RedrawScoreAndLives = 1 << 3,
	RedrawTime = 1 << 4,
	GameOver = 1 << 5,
	GameWin = 1 << 6,
	GamePause = 1 << 7,
	GameResume = 1 << 8
} DrawRequest;

void requestDraw(DrawRequest flags);

void initGameScreen();

#endif // DRAW_H