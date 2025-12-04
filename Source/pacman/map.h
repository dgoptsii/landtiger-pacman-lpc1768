#ifndef MAP_H
#define MAP_H

#include <stdint.h>

// Direction enum for Pac-Man movement
typedef enum {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
	  NOTHING
} Direction;

extern Direction allDirections[4];

typedef enum {
    EMPTY = 0,
    WALL,
    PILL,
    POWER_PILL,
    TELEPORT
} TileType;

typedef struct {
    int16_t x;
    int16_t y;
} Coords;

#define HEIGHT 25
#define WIDTH 24

#define OFFSET_Y 50

#define SQUARE_SIZE 10
#define PILL_RADIUS 1
#define POWER_PILL_RADIUS 5

#define POWER_PILL_CHANCE 80   
#define MAX_POWER_PILLS 6

extern uint8_t map[HEIGHT][WIDTH];

int equals(Coords a, Coords b);

Coords dirVector(Direction dir);

Coords move(Coords c, Direction dir);

TileType tileAt(Coords c);

void setMap(Coords c, TileType type);

Direction flipDirection(Coords c, Direction dir);

#endif // MAP_H