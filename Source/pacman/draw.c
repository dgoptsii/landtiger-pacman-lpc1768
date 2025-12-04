#include "./draw.h"
#include <stdio.h> 
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"

// maps map coordinates to pixel coordinates
Coords mapToPixel(Coords c) {
	return (Coords){c.x * SQUARE_SIZE, c.y * SQUARE_SIZE + OFFSET_Y};
}

// Draw a square with center (x, y) and side length S
void drawSquare(int x, int y, int S) {
        // Draw the four sides of the square
    // Top side (x, y) is the top-left corner
    LCD_DrawLine(x, y, x + S, y, Blue); // Blue color
    // Bottom side
    LCD_DrawLine(x, y + S, x + S, y + S, Blue);
    // Left side
    LCD_DrawLine(x, y, x, y + S, Blue);
    // Right side
    LCD_DrawLine(x + S, y, x + S, y + S, Blue);
}

void drawPill(int x, int y, int S, int R) {
    // Calculate the center of the square
    int centerX = x + S / 2;
    int centerY = y + S / 2;

    int i, j;
    // Draw a circle (pill shape) around (centerX, centerY) with radius R
    for (i = -R; i <= R; i++) {
        for (j = -R; j <= R; j++) {
            if (i * i + j * j <= R * R) { // Check if the point lies within the circle
                int drawX = centerX + i;
                int drawY = centerY + j;

                // Ensure the drawn coordinates are within bounds
                if (drawX >= x && drawX < x + S && drawY >= y && drawY < y + S) {
                    // Call LCD_DrawLine to draw the pixel for the pill shape
                    LCD_DrawLine(drawX, drawY, drawX, drawY, White); // Yellow color
                }
            }
        }
    }
}

void updateScoreAndLives() {
	char score_in_char[5] = "";
	sprintf(score_in_char,"%4d", gameState.score);
	GUI_Text(45, 0, (uint8_t *) score_in_char, White, Blue);
	
	
	if (gameState.score / 1000 > gameState.lastThreshold) { // Check if we passed a new 1000-point threshold
		gameState.lives++;  // Award 1 extra life
		gameState.lastThreshold = gameState.score / 1000; // Update last awarded threshold
				
		
		char lives_in_char[5] = "";
		sprintf(lives_in_char,"%4d", gameState.lives);
		GUI_Text(45, 20, (uint8_t *) lives_in_char, White, Blue);
  }
	if (gameState.pacman.state==CAUGHT){
		char lives_in_char[5] = "";
		sprintf(lives_in_char,"%4d", gameState.lives);
		GUI_Text(45, 20, (uint8_t *) lives_in_char, White, Blue);
	}
}

void updateTime(){
	char time_in_char[5] = "";
	sprintf(time_in_char,"%4d", gameState.time);
	GUI_Text(145, 0, (uint8_t *) time_in_char, White, Blue);
}


void fillRectangle(Coords A, Coords B, Coords C, Coords D) { 
	int16_t x, y;
	for ( y = A.y; y <= C.y; y++) { 
	for ( x = A.x; x <= C.x; x++) { 
	LCD_DrawLine(x, y, x, y, Black); 
	} 
	}
}

void drawGameOver(){
	GUI_Text(85, 160, (uint8_t *)"GAME OVER", White, Blue);
}

void drawGameWin(){
	GUI_Text(85, 160, (uint8_t *)"YOU WON!", White, Blue);
}

void drawGamePause(){
	GUI_Text(100, 160, (uint8_t *)"PAUSE", White, Blue);
}

void drawGameResume(){
    Coords A = {100, 160};
    Coords B = {140, 160};
    Coords C = {140, 180};
    Coords D = {100, 180};
    fillRectangle(A, B, C, D);
}

void drawPacMan(PacMan* pacman) {
    // Calculate the center of the square
	  Coords pacmanPos = mapToPixel(pacman->c);
    int centerX = pacmanPos.x + SQUARE_SIZE / 2;
    int centerY = pacmanPos.y + SQUARE_SIZE / 2;

    int i, j;
    // Draw a circle (pill shape) around (centerX, centerY) with radius R
    for (i = -pacman->radius; i <= pacman->radius; i++) {
        for (j = -pacman->radius; j <= pacman->radius; j++) {
            if (i * i + j * j <= pacman->radius * pacman->radius) { // Check if the point lies within the circle
                int drawX = centerX + i;
                int drawY = centerY + j;

                // Ensure the drawn coordinates are within bounds
                if (drawX >= pacmanPos.x && drawX < pacmanPos.x + SQUARE_SIZE && drawY >= pacmanPos.y && drawY < pacmanPos.y + SQUARE_SIZE) {
                    // Call LCD_DrawLine to draw the pixel for the pill shape
                    LCD_DrawLine(drawX, drawY, drawX, drawY, Yellow); // Yellow color
                }
            }
        }
    }
}

void deletePacman(Coords c) {
    int i, j;
    // Calculate the center of the square
	  Coords pacmanPos = mapToPixel(c);
	  int radius = gameState.pacman.radius;
    int centerX = pacmanPos.x + SQUARE_SIZE / 2;
    int centerY = pacmanPos.y + SQUARE_SIZE / 2;

    // Draw a circle (pill shape) around (centerX, centerY) with radius R
    for (i = -radius; i <= radius; i++) {
        for (j = -radius; j <= radius; j++) {
            if (i * i + j * j <= radius * radius) { // Check if the point lies within the circle
                int drawX = centerX + i;
                int drawY = centerY + j;

                // Ensure the drawn coordinates are within bounds
                if (drawX >= pacmanPos.x && drawX < pacmanPos.x + SQUARE_SIZE && drawY >= pacmanPos.y && drawY < pacmanPos.y + SQUARE_SIZE) {
                    // Call LCD_DrawLine to draw the pixel for the pill shape
                    LCD_DrawLine(drawX, drawY, drawX, drawY, Black); // Yellow color
                }
            }
        }
    }
			
		 int neighbors[4][2] = {
        {c.x, c.y - 1}, // Up
        {c.x, c.y + 1}, // Down
        {c.x - 1, c.y}, // Left
        {c.x + 1, c.y}  // Right
    };
				 
		  for ( i = 0; i < 4; i++) {
        int neighborX = neighbors[i][0];
        int neighborY = neighbors[i][1];
            if (neighborX >=0&& neighborX<=23&&neighborY >=0&& neighborY<=23&& map[neighborY][neighborX] == 1 ) {
                int drawX = neighborX * SQUARE_SIZE;
                int drawY = neighborY * SQUARE_SIZE + OFFSET_Y;
                drawSquare(drawX, drawY, SQUARE_SIZE);
        }
    }
}

// Function to draw walls, pills, and handle their consecutive placement
void drawWalls(uint8_t map[HEIGHT][WIDTH]) {

    int y, x;
    int currentX = 0;  // To track the current X position for drawing squares/pills
    int currentY = OFFSET_Y;  // To track the current Y position for drawing squares/pills
	int count =0;
   // srand(time(NULL)); 
	init_timer(2, 0xBEBC20);
	enable_timer(2);
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            // If it's a wall (1), draw a square
            if (map[y][x] == WALL) {
                drawSquare(currentX, currentY, SQUARE_SIZE);
                currentX += SQUARE_SIZE; // Move horizontally by the side of the square
            }
            // If it's a pill (2), draw a pill
						else if (map[y][x] == PILL) {
							gameState.numPills++;
                // Check if we should place a power pill instead of a regular pill
                if (count < MAX_POWER_PILLS && ((LPC_TIM2->TC&0b11111111)%100) <=3 ) {
                    // Draw a power pill
                    drawPill(currentX, currentY, SQUARE_SIZE, POWER_PILL_RADIUS);
                    count++;  // Increment the count of power pills placed
										map[y][x] = POWER_PILL;
                } else {
                    // Draw a regular pill
                    drawPill(currentX, currentY, SQUARE_SIZE, PILL_RADIUS);
                }
                currentX += SQUARE_SIZE; // Move horizontally by the side of the square
            } 
						else if (map[y][x] == EMPTY||map[y][x] == TELEPORT) {
                currentX += SQUARE_SIZE; // Move horizontally by the side of the square
							
            }
            // Move to the next row when the end of the current row is reached
            if (x == WIDTH - 1) {
                currentX = 0; // Reset X position for the next row
                currentY += SQUARE_SIZE; // Move vertically by the side of the square
            }
        }
    }
		disable_timer(2);
		
}


void drawGhost(Ghost *ghost) {
	  Coords ghostPos = mapToPixel(ghost->c);
    int centerX = ghostPos.x + SQUARE_SIZE / 2;
    int centerY = ghostPos.y + SQUARE_SIZE / 2;

    int i, j;
    for (i = -SQUARE_SIZE / 2; i <= SQUARE_SIZE / 2; i++) {
        for (j = -SQUARE_SIZE / 2; j <= SQUARE_SIZE / 2; j++) {
            if (i * i + j * j <= (SQUARE_SIZE / 2) * (SQUARE_SIZE / 2)) {
                int drawX = centerX + i;
                int drawY = centerY + j;

                if (drawX >= ghostPos.x && drawX < ghostPos.x + SQUARE_SIZE && drawY >= ghostPos.y && drawY < ghostPos.y + SQUARE_SIZE) {
                  if (gameState.ghost.state==HUNT){  
									LCD_DrawLine(drawX, drawY, drawX, drawY, Red);
									}else{
										LCD_DrawLine(drawX, drawY, drawX, drawY, Blue2);
									}
                }
            }
        }
    }
}



void deleteGhost(Coords c) {
    int i, j;
    // Calculate the center of the square
	  Coords ghostPos = mapToPixel(c);
	  int radius = gameState.pacman.radius;
    int centerX = ghostPos.x + SQUARE_SIZE / 2;
    int centerY = ghostPos.y + SQUARE_SIZE / 2;

    // Draw a circle (pill shape) around (centerX, centerY) with radius R
    for (i = -radius; i <= radius; i++) {
        for (j = -radius; j <= radius; j++) {
            if (i * i + j * j <= radius * radius) { // Check if the point lies within the circle
                int drawX = centerX + i;
                int drawY = centerY + j;

                // Ensure the drawn coordinates are within bounds
                if (drawX >= ghostPos.x && drawX < ghostPos.x + SQUARE_SIZE && drawY >= ghostPos.y && drawY < ghostPos.y + SQUARE_SIZE) {
                    // Call LCD_DrawLine to draw the pixel for the pill shape
                    LCD_DrawLine(drawX, drawY, drawX, drawY, Black); 
                }
								
							
            }
        }
    }
		
			int drawX = c.x * SQUARE_SIZE;
      int drawY = c.y * SQUARE_SIZE + OFFSET_Y;
			if (map[c.y][c.x] == PILL){
				
				drawPill(drawX, drawY, SQUARE_SIZE, PILL_RADIUS);
			}else if (map[c.y][c.x]  == POWER_PILL){
				drawPill(drawX, drawY, SQUARE_SIZE, POWER_PILL_RADIUS);
			}
			
		 int neighbors[4][2] = {
        {c.x, c.y - 1}, // Up
        {c.x, c.y + 1}, // Down
        {c.x - 1, c.y}, // Left
        {c.x + 1, c.y}  // Right
    };
				 
		  for ( i = 0; i < 4; i++) {
        int neighborX = neighbors[i][0];
        int neighborY = neighbors[i][1];
            if (neighborX >=0&& neighborX<=23&&neighborY >=0&& neighborY<=23 ) {
							 drawX = neighborX * SQUARE_SIZE;
               drawY = neighborY * SQUARE_SIZE + OFFSET_Y;
							if  (map[neighborY][neighborX] == WALL){
                drawSquare(drawX, drawY, SQUARE_SIZE);
							} 
        }
    }
}


// Drawing bookkeeping
typedef struct {
  Coords ghostOldPos;
	Coords pacmanOldPos;
} DrawData;

DrawData drawData = (DrawData){
	.ghostOldPos = {0,0},   // gameState.ghost.c
	.pacmanOldPos = {0,0}, // gameState.pacman.c
};

void requestDraw(DrawRequest flags) {
	if(flags & RedrawPacman) {
		deletePacman(drawData.pacmanOldPos);
		drawPacMan(&gameState.pacman);
		drawData.pacmanOldPos = gameState.pacman.c;
	}
	
	if(flags & RedrawGhost) {
		deleteGhost(drawData.ghostOldPos);
		drawGhost(&gameState.ghost);
		drawData.ghostOldPos = gameState.ghost.c;
	}
	
	if(flags & RedrawScoreAndLives) {
		updateScoreAndLives();
	}
	if(flags & RedrawTime) {
		updateTime();
	}
	if(flags & GameOver) {
		drawGameOver();
	}
	
	if(flags & GameWin) {
		drawGameWin();
	}
	if(flags & GamePause) {
		drawGamePause();
	}
		if(flags & GameResume) {
		drawGameResume();
	}
}

void initGameScreen(){
	GUI_Text(0, 0, (uint8_t *) "Score ", White, Blue);
	char score_in_char[5] = "";
	sprintf(score_in_char,"%4d", gameState.score);
	GUI_Text(45, 0, (uint8_t *) score_in_char, White, Blue);
	drawWalls(map);
	
	
	GUI_Text(0, 20, (uint8_t *) "Lives ", White, Blue);
	char lives_in_char[5] = "";
	sprintf(lives_in_char,"%4d", gameState.lives);
	GUI_Text(45, 20, (uint8_t *) lives_in_char, White, Blue);
	
	
	
	GUI_Text(100, 0, (uint8_t *) "Time ", White, Blue);
	char time_in_char[5] = "";
	sprintf(time_in_char,"%4d", gameState.time);
	GUI_Text(145, 0, (uint8_t *) time_in_char, White, Blue);
}
