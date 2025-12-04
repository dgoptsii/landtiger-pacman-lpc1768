#include "../pacman/pacman.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include <time.h> 
#include <stdio.h> 
#include <math.h>
#include "draw.h"

//#define SQUARE_SIZE 10
//#define PILL_RADIUS 1
//#define POWER_PILL_RADIUS 5

//#define POWER_PILL_CHANCE 80   
//#define MAX_POWER_PILLS 6

GameState gameState = {
	.time= 60,
	.score = 0,
	.lives = 1,
	.lastThreshold = 0,
	.status = PLAY,
	.numPills =0,
	.ghostSpeed =2,
	.pacman = {
		.c = { 12, 22 },
	  .dir = DOWN,
	  .radius = POWER_PILL_RADIUS,
		.state = RUN
	},
	.ghost = {
	  .c = { 12, 12},
	  .dir = UP,
	  .state = HUNT
	}

};

void checkPacmanAteGhost(Coords ghost, Coords pacman) {
    // Check if the ghost is next to Pacman
    if ((ghost.x==pacman.x)&& (ghost.y == pacman.y)) 
    {
        gameState.score+=100; // Update game status
				gameState.ghost.state=EATEN;
				requestDraw(RedrawScoreAndLives);
    }
}



void movePacman(PacMan* pacman, int* score){
		
		Coords oldPos = pacman->c;
	  int mapX = pacman->c.x;
    int mapY = pacman->c.y;

    // Handle PacMan's movement based on direction
		Coords nextPos = move(pacman->c, pacman->dir);
   
    // Check if PacMan can move to the next square (empty or pill)
	  TileType nextTile = tileAt(nextPos);
    if (nextTile != WALL ) {
			// Update PacMan's position
			
			pacman->c = nextPos;
			requestDraw(RedrawPacman);
		}
	 	TileType currentTile = tileAt(oldPos);
		 if(currentTile == TELEPORT){
			if (nextPos.x < 0){
				pacman->c.x = WIDTH - 1;
			} else if (nextPos.x > WIDTH - 1){
				pacman->c.x = 0;
			}
			pacman->c.y = nextPos.y;
			requestDraw(RedrawPacman);
		}

		if (currentTile == PILL) {
			gameState.numPills--;
			setMap(oldPos, EMPTY);
			*score += 10;  // Regular pill
			requestDraw(RedrawScoreAndLives);
		} else if (currentTile == POWER_PILL) {
			gameState.numPills--;
			setMap(oldPos, EMPTY);
			*score += 50;  // Power pill;
			requestDraw(RedrawScoreAndLives);
			gameState.ghost.state=RUNAWAY;
		}
		if (gameState.numPills ==0){
			gameState.status = WIN;
		}
		
		
		if (gameState.ghost.state == RUNAWAY){
		checkPacmanAteGhost(gameState.ghost.c, gameState.pacman.c);
	}
	
}

void resumeGame(){
	requestDraw(RedrawPacman | RedrawWalls);
}

void respawn(RespawnRequest flags){
	if(flags & RespawnPacman) {
		gameState.pacman.c.x = 12;
		gameState.pacman.c.y = 22;
		gameState.pacman.dir = DOWN;
		
	}
		if(flags & RespawnGhost) {
		gameState.ghost.c.x = 12;
		gameState.ghost.c.y = 12;
	  gameState.ghost.dir = UP;
	  gameState.ghost.state = HUNT;
}
}



