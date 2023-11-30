/*
 * KEYPAD.c
 *
 * Created: 11/4/2023 8:22:26 PM
 *  Author: M Osama
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>
#include "KEYPAD.h"


/*Hardware array*/
/*
char arr[4][4] = {{'1','2','3','+'},
				  {'4','5','6','-'},
				  {'7','8','9','*'},
				  {'C','0','=','/'}};
		
*/

/*Simulation array*/	
 
char arr[4][7] = {{'7','8','9','/','.','$','~'},
				  {'4','5','6','*','^','>',' '},
				  {'1','2','3','-','&','<',' '},
				  {'C','0','=','+','|','%',' '}};


void keypad_vInit(){
	/* rows pins as output*/
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN0,DIO_OUTPUT);
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN1,DIO_OUTPUT);
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN2,DIO_OUTPUT);
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN3,DIO_OUTPUT);
	/*columns pin as input*/
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN4,DIO_INPUT);
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN5,DIO_INPUT);
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN6,DIO_INPUT);
	DIO_pin_dierction(KEYPAD_PORT,DIO_PIN7,DIO_INPUT);
	DIO_pin_dierction(DIO_PORTD,DIO_PIN0,DIO_INPUT);
	DIO_pin_dierction(DIO_PORTD,DIO_PIN1,DIO_INPUT);
	DIO_pin_dierction(DIO_PORTD,DIO_PIN2,DIO_INPUT);
	/*active internal pull up*/
	DIO_connectPullUP(KEYPAD_PORT,DIO_PIN4,PULLUP_CONNECT);
	DIO_connectPullUP(KEYPAD_PORT,DIO_PIN5,PULLUP_CONNECT);
	DIO_connectPullUP(KEYPAD_PORT,DIO_PIN6,PULLUP_CONNECT);
	DIO_connectPullUP(KEYPAD_PORT,DIO_PIN7,PULLUP_CONNECT);
	DIO_connectPullUP(DIO_PORTD,DIO_PIN0,PULLUP_CONNECT);
	DIO_connectPullUP(DIO_PORTD,DIO_PIN1,PULLUP_CONNECT);
	DIO_connectPullUP(DIO_PORTD,DIO_PIN2,PULLUP_CONNECT);
		
}

char keypad_u8check_press(){
	char rows,columns,x;
	char retval = NOTPRESSED;
	for (rows = 0 ; rows < 4 ; rows++)
	{
		DIO_pin_write(KEYPAD_PORT,DIO_PIN0,1);	
		DIO_pin_write(KEYPAD_PORT,DIO_PIN1,1);
		DIO_pin_write(KEYPAD_PORT,DIO_PIN2,1);
		DIO_pin_write(KEYPAD_PORT,DIO_PIN3,1);
		DIO_pin_write(KEYPAD_PORT,rows,0);
		
		for (columns = 0 ; columns < 7 ; columns++)
		{
			if (columns < 4)
			{
				x = DIO_pin_read(KEYPAD_PORT,(columns+4));
			}
			else
			{
				x = DIO_pin_read(DIO_PORTD,(columns-4));
			}
			if (x == 0)
			{	
				retval = arr[rows][columns];
				/* Handel long press*/
				while (x == 0)
				{
					if (columns < 4)
					{
						x = DIO_pin_read(KEYPAD_PORT,(columns+4));
					}
					else
					{
						x = DIO_pin_read(DIO_PORTD,(columns-4));
					}	
				}
				break;
			}
		}
		if (x == 0)
		{
			break;
		}
	}
	
	return retval;
	
}
