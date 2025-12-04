#ifndef PACMAN_H
#define PACMAN_H

#include "./map.h"

#include "LPC17xx.h"
typedef enum {
    RUN = 0,
    CAUGHT =1
} PacmanState;

// Structure to hold Pac-Man's position, direction, and radius
typedef struct {
    Coords c;        // Pac-Man's current X and Y position
    Direction dir;   // Pac-Man's current direction
    int radius;      // Pac-Man's radius
		PacmanState state;
} PacMan;

typedef enum {
    HUNT = 0,
    RUNAWAY =1,
		EATEN =2
} GhostState;


typedef enum {
    PLAY = 0,
    PAUSE =1,
		WIN =2,
		LOOSE =3,
} GameStatus;



typedef struct {
    Coords c;        // Ghost's current X and Y position
    Direction dir;   // Ghost's current direction
	  GhostState state;// Ghost's current state
} Ghost;

typedef struct {
  PacMan pacman;
	Ghost ghost;
	int score;
	int lives;
	int time;
	int status;
	int numPills;
	int ghostSpeed;
	int lastThreshold;
} GameState;


typedef enum {
  RespawnPacman = 1,
	RespawnGhost = 1 << 1,
} RespawnRequest;

extern GameState gameState;

// Function prototypes
void resumeGame();
void movePacman(PacMan* pacman, int* score);

#endif // PACMAN_H