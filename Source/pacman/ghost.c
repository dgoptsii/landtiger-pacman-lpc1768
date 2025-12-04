#include "../pacman/pacman.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include <time.h> 
#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#include "queue.h"
#include "draw.h"

CircularQueue q = {};
	
Coords NOT_VISITED = (Coords){255, 255};

// 0 = not visited
// any other number = id of the parent
Coords parents[HEIGHT][WIDTH] = {};
	
Coords *parentOf(Coords c) {
	return &parents[c.y][c.x];
}
	
int coordsToId(Coords c) {
	return c.y * WIDTH + c.x;
}

void clearParents() {
	int r, c;
	for(r = 0; r<HEIGHT; r++) {
		for(c = 0; c<WIDTH; c++) {
			parents[c][r] = NOT_VISITED;
		}
	}
}

Direction shortestPath(Coords start, Coords goal) {
	
	// Clear data structures
  clearQueue(&q);
	clearParents();
	
	if (equals(start, goal)){
		return NOTHING;
	}
	
	// BFS
	enqueue(&q, (QueueElement)start);
	int ghostId = coordsToId(start);
	
	while(!isEmpty(&q)) {
		Coords current;
		dequeue(&q, &current);
		
		if(equals(current, goal)) {
			// reconstruct path
			Coords parent = *parentOf(current);
			Coords lastParent = parent;
			while(!equals(parent, start)) {
				// go to parent's parent
				lastParent = parent;
				parent = *parentOf(parent);
			}
			
			if(lastParent.x > start.x) return RIGHT;
			if(lastParent.x < start.x) return LEFT;
			if(lastParent.y > start.y) return DOWN;
			if(lastParent.y < start.y) return UP;
			return NOTHING;
		}
		
		// didnt find pacman, add unvisited neighbors
		int i;
		for(i = 0; i < sizeof(allDirections)/sizeof(allDirections[0]);i++){
			Direction dir = allDirections[i];
			Coords neighbor = move(current, dir);
			Coords *parent = parentOf(neighbor);
			if(equals(*parent, NOT_VISITED) && tileAt(neighbor) != WALL && tileAt(neighbor) != TELEPORT) {
				enqueue(&q, (QueueElement)neighbor);
				*parent = current;
			}
		}
	}
	
	return NOTHING;
}


// Function to calculate the Euclidean distance between two points
double calculateDistance(Coords point1, Coords point2) {
    return sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
}

// Function to find the corner with the longest distance from a point
Coords findLongestDistancePoint(Coords point) {
    // Define the 4 corners of the rectangle
    Coords corners[4] = {
        {1, 1},
        {22, 22},
        {1, 22},
        {22, 1}
    };

    double maxDistance = 0.0;
    Coords farthestCorner = corners[0];
		int i;
    // Iterate through the corners to find the farthest corner
    for ( i = 0; i < 4; i++) {
        double distance = calculateDistance(point, corners[i]);
        if (distance > maxDistance) {
            maxDistance = distance;
            farthestCorner = corners[i];
        }
    }

    return farthestCorner;
}


void checkGhostCatchPacman(Coords ghost, Coords pacman) {
    // Check if the ghost is next to Pacman
    if ((abs(ghost.x - pacman.x) == 1 && ghost.y == pacman.y) || // Left or right
        (abs(ghost.y - pacman.y) == 1 && ghost.x == pacman.x))   // Up or down
    {
        gameState.lives--; // Update game status
				gameState.pacman.state=CAUGHT;
				requestDraw(RedrawScoreAndLives);
    }
}



void moveGhost(Ghost *ghost, PacMan *pacman) {
	if(ghost->state == EATEN){
		return;
	}
	
	if(ghost->state == RUNAWAY){
		// Run away from pacman
		Direction dir = shortestPath(ghost->c, findLongestDistancePoint(pacman->c));
		ghost->dir =  dir;
	} else if(ghost->state == HUNT){
		// Go towards pacman
		Direction dir = shortestPath(ghost->c, pacman->c);
		ghost->dir = dir;
	}
	
	ghost->c = move(ghost->c, ghost->dir);
	if (ghost->dir !=NOTHING){
	requestDraw(RedrawGhost);
	}
	
	if(ghost->state == HUNT){
		checkGhostCatchPacman(ghost->c, pacman->c);
	}
	if (gameState.lives==0){
		gameState.status=LOOSE;
	}
	if (gameState.pacman.state==CAUGHT){
	respawn(RespawnGhost | RespawnPacman);
		gameState.pacman.state=RUN;
	}

}
