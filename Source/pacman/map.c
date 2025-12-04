#include "../pacman/pacman.h"

uint8_t map[HEIGHT][WIDTH] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
  {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,  1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
	{1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,  1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1}, 
  {1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,  1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1},
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1}, 		
  {1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1},
	{1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1,  1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1}, 
  {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,  1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1, 2, 1, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 1, 2, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1, 2, 1, 2, 1, 1, 1, 0,  0, 1, 1, 1, 2, 1, 2, 1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 2, 1, 2, 1, 0, 0, 0,  0, 0, 0, 1, 2, 1, 2, 1, 1, 1, 1, 1},	
	{4, 0, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0,  0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 0, 4},
	{4, 0, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0,  0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 0, 4},
	{1, 1, 1, 1, 1, 2, 1, 2, 1, 0, 0, 0,  0, 0, 0, 1, 2, 1, 2, 1, 1, 1, 1, 1},		
	{1, 0, 0, 0, 1, 2, 1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1, 2, 1, 0, 0, 0, 1},	
	{1, 0, 0, 0, 1, 2, 1, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 1, 2, 1, 0, 0, 0, 1},	
	{1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1},	
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,  1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
	{1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,  1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1},
	{1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1}, 
	{1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1},
	{1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1,  1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1}, 	
  {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,  1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
	{1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,  0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1}, 
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

Direction allDirections[4] = {UP, DOWN, LEFT, RIGHT};

int equals(Coords a, Coords b) {
	return a.x == b.x && a.y == b.y;
}

Coords dirVector(Direction dir) {
	switch(dir) {
		case UP: return (Coords){0,-1};
		case DOWN: return (Coords){0,1};
		case LEFT: return (Coords){-1,0};
		case RIGHT: return (Coords){1, 0};
		case NOTHING: return (Coords){0, 0};
	}
}

// Choose a direction opposite to dir, but is aware of the walls.
// For example, if dir is UP, then DOWN would be chosen if tile below is not a wall.
Direction flipDirection(Coords c, Direction dir) {
	Direction reverse;
	Direction alt1;
	Direction alt2;
	if(dir == UP) {
		reverse = DOWN;
		alt1 = LEFT;
		alt2 = RIGHT;
	}
	if(dir == DOWN) {
		reverse = UP;
		alt1 = LEFT;
		alt2 = RIGHT;
	}
	if(dir == LEFT) {
		reverse = RIGHT;
		alt1 = UP;
		alt2 = DOWN;
	}
	if(dir == RIGHT) {
		reverse = LEFT;
		alt1 = UP;
		alt2 = DOWN;
	}
	
	// try the available directions
	if(tileAt(move(c, reverse)) != WALL) return reverse;
	if(tileAt(move(c, alt1)) != WALL) return alt1;
	if(tileAt(move(c, alt2)) != WALL) return alt2;
	
	return NOTHING;
}

TileType tileAt(Coords c) {
	return (TileType)map[c.y][c.x];
}

void setMap(Coords c, TileType type) {
	map[c.y][c.x] = type;
}

Coords move(Coords c, Direction dir) {
	Coords vec = dirVector(dir);
	return (Coords){c.x + vec.x, c.y + vec.y};
}