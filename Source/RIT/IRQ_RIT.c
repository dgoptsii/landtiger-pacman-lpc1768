/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../pacman/pacman.h"

#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include <stdio.h> // For sprintf function
#include "../pacman/draw.h"
#include "../music/music.h"
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int pressed=0;
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1


//SHORTENING UNDERTALE: TOO MANY REPETITIONS
//NOTE song[] = 
//{
//	// 1
//	{d3, time_semicroma},
//	{d3, time_semicroma},
//	{d4, time_croma},
//	{a3, time_croma},
//	{pause, time_semicroma},
//	{a3b, time_semicroma},
//	{pause, time_semicroma},
//	{g3, time_croma},
//	{f3, time_semicroma*2},
//	{d3, time_semicroma},
//	{f3, time_semicroma},
//	{g3, time_semicroma},
//	// 2
//	{c3, time_semicroma},
//	{c3, time_semicroma},
//	{d4, time_croma},
//	{a3, time_croma},
//	{pause, time_semicroma},
//	{a3b, time_semicroma},
//	{pause, time_semicroma},
//	{g3, time_croma},
//	{f3, time_semicroma*2},
//	{d3, time_semicroma},
//	{f3, time_semicroma},
//	{g3, time_semicroma},
//	// 3
//	{c3b, time_semicroma},
//	{c3b, time_semicroma},
//	{d4, time_croma},
//	{a3, time_croma},
//	{pause, time_semicroma},
//	{a3b, time_semicroma},
//	{pause, time_semicroma},
//	{g3, time_croma},
//	{f3, time_semicroma*2},
//	{d3, time_semicroma},
//	{f3, time_semicroma},
//	{g3, time_semicroma},
//	// 4
//	{a2b, time_semicroma},
//	{a2b, time_semicroma},
//	{d4, time_croma},
//	{a3, time_croma},
//	{pause, time_semicroma},
//	{a3b, time_semicroma},
//	{pause, time_semicroma},
//	{g3, time_croma},
//	{f3, time_semicroma*2},
//	{d3, time_semicroma},
//	{f3, time_semicroma},
//	{g3, time_semicroma},
//	// 5
//	
//};
NOTE song[] = 
{
    // New sequence
    {g3, time_semicroma}, // G
    {a3, time_semicroma}, // A
    {c4, time_semicroma}, // C
    {a3, time_semicroma}, // A
    {e4, time_croma}, // E
    {e4, time_croma}, // E
    {d4, time_croma}, // D
		
    {g3, time_semicroma}, // G
    {a3, time_semicroma}, // A
    {c4, time_semicroma}, // C
    {a3, time_semicroma}, // A
    {d4, time_croma}, // G
    {d4, time_croma}, // B
    {c4, time_croma}, // C
		
    {b3, time_semicroma}, // B
    {a3, time_croma}, // A
		
    {g3, time_semicroma}, // G
    {a3, time_semicroma}, // A
    {c4, time_semicroma}, // C
    {a3, time_semicroma}, // A
    {c4, time_croma}, // C
    {d4, time_semicroma}, // D
    {b3, time_semicroma}, // B
    {a3, time_semicroma}, // A
    {g3, time_croma}, // G
    {g3, time_semicroma}, // G
    {d4, time_croma}, // D
    {c4, time_croma}, // C
};

void RIT_IRQHandler (void)
{		
	//movePacman(&gameState.pacman, &gameState.score);
	static int down=0;
	static int up=0;
	static int right=0;
	static int left=0;
	static int position=0;	
	static int moveP=0; 
	static int moveG=0; 
	
	if (gameState.status == LOOSE){
		disable_timer(0);
		disable_timer(1);
		disable_RIT();
		requestDraw(GameOver);
	} else if(gameState.status == WIN){
		disable_timer(0);
		disable_timer(1);
		disable_RIT();
		requestDraw(GameWin);
	} 


///////////////////////////////////////////////////
	static int currentNote = 0;
	static int ticks = 0;
	if(!isNotePlaying())
	{
		++ticks;
		if(ticks == UPTICKS)
		{
			ticks = 0;
			playNote(song[currentNote++]);
		}
	}	if(currentNote == (sizeof(song) / sizeof(song[0])))
	{
		currentNote =0 ;
	}
///////////////////////////////////////////////////
	
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick UP pressed */
		
		right++;
		switch(right){
			case 1:
				gameState.pacman.dir = RIGHT;
				break;
			default:
				break;
		}
	}
	else{
			right=0;
	}
	
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick UP pressed */
	
		left++;
		switch(left){
			case 1:
				gameState.pacman.dir = LEFT;
				break;
			default:
				break;
		}
	}
	else{
			left=0;
	}
	
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick UP pressed */
		down++;
		switch(down){
			case 1:
				gameState.pacman.dir = DOWN;
				break;
			default:
				break;
		}
	}
	else{
			down=0;
	}
	
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		//pacman.dir = UP;
		up++;
		switch(up){
			case 1:
			gameState.pacman.dir = UP;
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}

	
	/* button management */
	if(pressed>=1){ 
	
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* KEY1 pressed */
			//	pause++;
			switch(pressed){				
				case 2:				/* pay attention here: please see slides 19_ to understand value 2 */
				if (gameState.status==PLAY){
					gameState.status=PAUSE;
					disable_RIT();
					disable_timer(0);
					disable_timer(1);
					//disable_timer(2);
					requestDraw(GamePause);
					init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
					enable_RIT();
				}else if (gameState.status==PAUSE){
					disable_RIT();
					gameState.status=PLAY;
					requestDraw(GameResume);
					init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
					enable_RIT();
					enable_timer(0);
					enable_timer(1);
					//enable_timer(2);
				}

					break;
				default:
					break;
			}
			pressed++;
		}
		else {	/* button released */
			pressed=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}	else{
			if(pressed==1)
				pressed++;
	}
	

	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
