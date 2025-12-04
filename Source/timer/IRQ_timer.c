/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include <stdio.h> /*for sprintf*/
#include "../pacman/pacman.h"
#include "../pacman/draw.h"
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


//GAME TIMER
void TIMER0_IRQHandler (void)
{

	gameState.time = gameState.time-1;

	disable_RIT();
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	if (gameState.time==0){
		gameState.status= LOOSE;
	}
	if(gameState.time==30 || gameState.time==10){
			gameState.ghostSpeed--;
		}

	if(	gameState.ghost.state==RUNAWAY){
			static int countSecondsHunt=0;
		countSecondsHunt++;
		if (countSecondsHunt==10){
			gameState.ghost.state=HUNT;
			countSecondsHunt=0;
		}
	}
	
	if(	gameState.ghost.state==EATEN){
			static int countSecondsEaten=0;
		countSecondsEaten++;
	//	gameState.ghostSpeed++;
		if (countSecondsEaten==3){
			respawn(RespawnGhost);
			countSecondsEaten=0;
		//	gameState.ghostSpeed--;
		}
	}
	
	enable_RIT();
	requestDraw(RedrawTime);

	LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
//MOVEMENT TIMER
void TIMER1_IRQHandler (void)
{
	static int move =0; 
	move++;
	movePacman(&gameState.pacman, &gameState.score);
//	
//	if (move>=gameState.ghostSpeed){
//		moveGhost(&gameState.ghost, &gameState.pacman);
//		move=0;
//	}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

// MUSIC

void TIMER2_IRQHandler (void)
{
//	static int sineticks=0;
//	/* DAC management */	
//	static int currentValue; 
//	currentValue = SinTable[sineticks];
//	currentValue -= 410;
//	currentValue /= 1;
//	currentValue += 410;
//	LPC_DAC->DACR = currentValue <<6;
//	sineticks++;
//	if(sineticks==45) sineticks=0;
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

// MUSIC
void TIMER3_IRQHandler (void)
{
	disable_timer(2);
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
