/*
 * MyCalculator.c
 *
 * Created: 11/24/2023 5:23:28 PM
 *  Author: M Osama
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"
#include "KEYPAD.h"
#include "Button.h"

//macros
#define SIZE               10
// functions
void ClearForRest(void);
void ConvertToFloat(void);
void DoOperation(void);
void ActiveInterrupt(void);

// variables

	unsigned char FirstNumber[SIZE] =  {NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED};
	unsigned char SecondNumber[SIZE] = {NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED,NOTPRESSED};
	unsigned char ClearCounter = 0;
	unsigned char operator = NOTPRESSED;
	unsigned char nextoperator = NOTPRESSED;
	unsigned char equal = NOTPRESSED;
	unsigned char counter1 = 0;
	unsigned char counter2 = 0;
	unsigned char FirstNumberFlag = 0;
	unsigned char SecondNumberFlag = 0;
	unsigned char FirstNumberFloatFlag = 0;
	unsigned char SecondNumberFloatFlag = 0;
	unsigned char nextoperationFlag = 0;
	unsigned char NumberOneSign = 0;
	unsigned char NumberTwoSign = 0;
	unsigned char ResultSign = 0;
	float NumberOne,NumberTwo = 0;
	float Result ;
	
int main(void)
{
Rsetlabel:	
	// initialization
	LCD_vInit();
	keypad_vInit();
    while(1)
    {
		/*Take Numbers*/
			if (counter1 < SIZE || equal != '=' || operator != '+' || operator != '-' || 
			 operator != '*' || operator != '/' || operator != '^' || operator != '&' ||
			 operator != '|' || operator != '$' || operator != '>' || operator != '<' || operator != '%' || operator != '~'){
				label1:
				FirstNumberFlag = 1;
				SecondNumberFlag = 0;
				do
				{
					FirstNumber[counter1] = keypad_u8check_press();
				} while (FirstNumber[counter1] == NOTPRESSED);
				if (FirstNumber[counter1] == '+' ||
					FirstNumber[counter1] == '-' ||
					FirstNumber[counter1] == '/' ||
					FirstNumber[counter1] == '*' ||
					FirstNumber[counter1] == '^' ||
					FirstNumber[counter1] == '&' ||
					FirstNumber[counter1] == '|' ||
					FirstNumber[counter1] == '$' ||
					FirstNumber[counter1] == '>' ||
					FirstNumber[counter1] == '<' ||
					FirstNumber[counter1] == '%' ||
					FirstNumber[counter1] == '~')
				{
					if (FirstNumber[counter1] == '-' && counter1 == 0)
					{
						NumberOneSign = 1;
						FirstNumber[counter1] = NOTPRESSED;
						LCD_vSend_char('-');
						goto label1;
						
					}
					else if ( (FirstNumber[counter1] == '+' ||
							   FirstNumber[counter1] == '/' ||
							   FirstNumber[counter1] == '*' ||
							   FirstNumber[counter1] == '^' ||
							   FirstNumber[counter1] == '&' ||
							   FirstNumber[counter1] == '|' ||
							   FirstNumber[counter1] == '$' ||
							   FirstNumber[counter1] == '>' ||
							   FirstNumber[counter1] == '<' ) && counter1 == 0)
					{
						LCD_clearscreen();
						ClearForRest();
						goto Rsetlabel;		
					}
					else if (FirstNumber[counter1] == '&')
					{
						operator = FirstNumber[counter1];
						FirstNumber[counter1] = NOTPRESSED;
						LCD_vSend_string("AND");
						goto label2;
					}
					else if (FirstNumber[counter1] == '|')
					{
						operator = FirstNumber[counter1];
						FirstNumber[counter1] = NOTPRESSED;
						LCD_vSend_string("OR");
						goto label2;
					}
					else if (FirstNumber[counter1] == '$')
					{
						operator = FirstNumber[counter1];
						FirstNumber[counter1] = NOTPRESSED;
						LCD_vSend_string("XOR");
						goto label2;
					}
					else if (FirstNumber[counter1] == '<')
					{
						operator = FirstNumber[counter1];
						FirstNumber[counter1] = NOTPRESSED;
						LCD_vSend_string("<<");
						goto label2;
					}
					else if (FirstNumber[counter1] == '>')
					{
						operator = FirstNumber[counter1];
						FirstNumber[counter1] = NOTPRESSED;
						LCD_vSend_string(">>");
						goto label2;
					}
					else if (FirstNumber[counter1] == '~')
					{
						if (counter1 == 0)
						{
							operator = FirstNumber[counter1];
							FirstNumber[counter1] = NOTPRESSED;
							LCD_vSend_string("NOT");
							goto label2;	
						}
						else
						{
							LCD_clearscreen();
							ClearForRest();
							goto Rsetlabel;	
						}	
					}
					else
					{
						operator = FirstNumber[counter1];
						FirstNumber[counter1] = NOTPRESSED;
						LCD_vSend_char(operator);
						goto label2;	
					}					
				}
				else if (FirstNumber[counter1] == 'C')
				{
					LCD_clearscreen();
					ClearForRest();
					goto Rsetlabel;
					
				}
				else if (FirstNumber[counter1] == '.')
				{
					FirstNumberFloatFlag = 1;
					LCD_vSend_char(FirstNumber[counter1]);
					counter1++;
					goto label1;
				}
				else
				{
					LCD_vSend_char(FirstNumber[counter1]);
					counter1++;
					goto label1;
				}
			}
			else{
				counter1 = 0;
			}
			
			if (counter2 < SIZE){
				label2:
				FirstNumberFlag = 0;
				SecondNumberFlag = 1;
				do
				{
					SecondNumber[counter2] = keypad_u8check_press();
				} while (SecondNumber[counter2] == NOTPRESSED);
				if (SecondNumber[counter2] == '=')
				{
					equal = SecondNumber[counter2];
					SecondNumber[counter2] = NOTPRESSED;
					LCD_vSend_char(equal);
					
					ConvertToFloat();
					DoOperation();
					do
					{
						nextoperator = keypad_u8check_press();
					} while (nextoperator == NOTPRESSED);
					if (nextoperator == '+' ||
						nextoperator == '-' ||
						nextoperator == '/' ||
						nextoperator == '*' ||
						nextoperator == '^' ||
						nextoperator == '&' ||
						nextoperator == '|' ||
						nextoperator == '$' ||
						nextoperator == '>' ||
						nextoperator == '<' ||
						nextoperator == '%')
					{
						nextoperationFlag = 1;
						ClearForRest();
						LCD_clearscreen();
						if (ResultSign == 1)
						{
							LCD_vSend_char('-');
						}
						else{}
						LCD_vSend_float(Result);
						if (nextoperator == '&')
						{
							LCD_vSend_string("AND");
							NumberOne = Result;
							NumberOneSign = ResultSign;
							operator = nextoperator;
							goto label2;	
						}
						else if (nextoperator == '|')
						{
							LCD_vSend_string("OR");
							NumberOne = Result;
							NumberOneSign = ResultSign;
							operator = nextoperator;
							goto label2;
						}
						else if (nextoperator == '$')
						{
							LCD_vSend_string("XOR");
							NumberOne = Result;
							NumberOneSign = ResultSign;
							operator = nextoperator;
							goto label2;
						}
						else if (nextoperator == '<')
						{
							LCD_vSend_string("<<");
							NumberOne = Result;
							NumberOneSign = ResultSign;
							operator = nextoperator;
							goto label2;
						}
						else if (nextoperator == '>')
						{
							LCD_vSend_string(">>");
							NumberOne = Result;
							NumberOneSign = ResultSign;
							operator = nextoperator;
							goto label2;
						}
						else
						{
							LCD_vSend_char(nextoperator);
							NumberOne = Result;
							NumberOneSign = ResultSign;
							operator = nextoperator;
							goto label2;	
						}			
					}
					else
					{
						nextoperationFlag = 0;
						LCD_clearscreen();
						ClearForRest();
						goto Rsetlabel;
					}
				}
				else if (SecondNumber[counter2] == 'C')
				{
					LCD_clearscreen();
					ClearForRest();
					goto Rsetlabel;
				}
				else if (SecondNumber[counter2] == '-' && counter2 == 0)
				{
					NumberTwoSign = 1;
					SecondNumber[counter2] = NOTPRESSED;
					LCD_vSend_char('-');
					goto label2;	
				}
				else if (SecondNumber[counter2] == '.')
				{
					SecondNumberFloatFlag = 1;
					LCD_vSend_char(SecondNumber[counter2]);
					counter2++;
					goto label2;
				}
				else
				{
					LCD_vSend_char(SecondNumber[counter2]);
					counter2++;
					goto label2;
				}
			}
			else{
				counter2 = 0;
			}	
    }
}

void ConvertToFloat(void){
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;
	unsigned char l = 0;
	float factor1 = 10;
	float factor2 = 10;
	if (nextoperationFlag == 0)
	{
		NumberOne = (FirstNumber[0] - 48) ;
		if (FirstNumberFloatFlag == 1)
		{
			for (i = 1 ; i < SIZE ; i++)
			{
				if (FirstNumber[i] == '.')
				{
					break;
				}
				else
				{
					NumberOne = (NumberOne*10) + (FirstNumber[i] - 48);
				}
			}
			for (k = i+1 ; k < SIZE ; k++)
			{
				if (FirstNumber[k] == NOTPRESSED)
				{
					break;
				}
				else
				{
					NumberOne = NumberOne + ((FirstNumber[k] - 48.0)/factor1);
					factor1 *=10;
				}
			}
			FirstNumberFloatFlag = 0;
		}
		else
		{
			for (i = 1 ; i < SIZE ; i++)
			{
				if (FirstNumber[i] == NOTPRESSED)
				{
					break;
				}
				else
				{
					NumberOne = (NumberOne*10) + (FirstNumber[i] - 48);
				}
			}	
		}	
	}
	else{}
	NumberTwo = (SecondNumber[0] - 48) ;
	if (SecondNumberFloatFlag == 1)
	{
		for (j = 1 ; j < SIZE ; j++)
		{
			if (SecondNumber[j] == '.')
			{
				break;
			}
			else
			{
				NumberTwo = (NumberTwo*10) + (SecondNumber[j] - 48);
			}
		}
		for (l = j+1 ; l < SIZE ; l++)
		{
			if (SecondNumber[l] == NOTPRESSED)
			{
				break;
			}
			else
			{
				NumberTwo = NumberTwo + ((SecondNumber[l] - 48.0)/factor2);
				factor2 *=10;
			}
			
		}
		SecondNumberFloatFlag = 0;
	}
	else
	{
		for (j = 1 ; j < SIZE ; j++)
		{
			if (SecondNumber[j] == NOTPRESSED)
			{
				break;
			}
			else
			{
				NumberTwo = (NumberTwo*10) + (SecondNumber[j] - 48);
			}
		}	
	}
	
	
	if (NumberOneSign == 1)
	{
		NumberOne = -NumberOne;
	}
	else{}
	
	if (NumberTwoSign == 1)
	{
		NumberTwo = -NumberTwo;
	}
	else{}
	
}

void DoOperation(void){
	float Power = 0;
	int  BitWiseOne = 0;
	int  BitWiseTwo = 0;
	int  BitWiseResult = 0;
	switch(operator){
		case '+' :
			Result = NumberOne + NumberTwo;
			if (Result < 0)
			{
				ResultSign = 1;
				LCD_vSend_char('-');
				Result = -Result;
				LCD_vSend_float(Result);
			}
			else
			{
				ResultSign = 0;
				LCD_vSend_float(Result);
			}
			break;
		case '-' :
			Result = NumberOne - NumberTwo;
			if (Result < 0)
			{
				ResultSign = 1;
				LCD_vSend_char('-');
				Result = -Result;
				LCD_vSend_float(Result);
			}
			else
			{
				ResultSign = 0;
				LCD_vSend_float(Result);
			}
			break;
		case '*' :
			Result = NumberOne * NumberTwo;
			if (Result < 0)
			{
				ResultSign = 1;
				LCD_vSend_char('-');
				Result = -Result;
				LCD_vSend_float(Result);
			}
			else
			{
				ResultSign = 0;
				LCD_vSend_float(Result);
			}
			break;
		case '/' :
			Result = ((float)NumberOne) / ((float)NumberTwo);
			if (Result < 0)
			{
				ResultSign = 1;
				LCD_vSend_char('-');
				Result = -Result;
				LCD_vSend_float(Result);
			}
			else
			{
				ResultSign = 0;
				LCD_vSend_float(Result);
			}
			break;
		case '%' :
			Result = ((int)NumberOne) % ((int)NumberTwo);
			if (Result < 0)
			{
				ResultSign = 1;
				LCD_vSend_char('-');
				Result = -Result;
				LCD_vSend_float(Result);
			}
			else
			{
				ResultSign = 0;
				LCD_vSend_float(Result);
			}
			break;
		case '^' :
			Result = NumberOne;
			Power = NumberTwo-1;
			while (Power != 0)
			{
				Result = Result*NumberOne;
				Power--;	
			}
			if (Result < 0)
			{
				ResultSign = 1;
				LCD_vSend_char('-');
				Result = -Result;
				LCD_vSend_float(Result);
			}
			else
			{
				ResultSign = 0;
				LCD_vSend_float(Result);
			}
			break;
		case '&' :
			BitWiseOne = NumberOne;
			BitWiseTwo = NumberTwo;
			Result = BitWiseOne & BitWiseTwo;
			LCD_vSend_float(Result);
			break;
		case '|' :
			BitWiseOne = NumberOne;
			BitWiseTwo = NumberTwo;
			Result = BitWiseOne | BitWiseTwo;
			LCD_vSend_float(Result);
			break;
		case '$' :
			BitWiseOne = NumberOne;
			BitWiseTwo = NumberTwo;
			Result = BitWiseOne ^ BitWiseTwo;
			LCD_vSend_float(Result);
			break;
		case '>' :
			BitWiseOne = NumberOne;
			BitWiseTwo = NumberTwo;
			Result = BitWiseOne >> BitWiseTwo;
			LCD_vSend_float(Result);
			break;
		case '<' :
			BitWiseOne = NumberOne;
			BitWiseTwo = NumberTwo;
			Result = BitWiseOne << BitWiseTwo;
			LCD_vSend_float(Result);
			break;
		case '~' :
			BitWiseOne = NumberOne;
			BitWiseTwo = NumberTwo;
			Result =  ~BitWiseTwo;
			if (Result < 0)
			{
				ResultSign = 1;
				LCD_vSend_char('-');
				Result = -Result;
				LCD_vSend_float(Result);
			}
			else
			{
				ResultSign = 0;
				LCD_vSend_float(Result);
			}
			break;
		default:  break;
	}		
}

void ClearForRest(void)
{
	for (ClearCounter = 0 ; ClearCounter < SIZE ; ClearCounter++){
		FirstNumber[ClearCounter] = NOTPRESSED;
		SecondNumber[ClearCounter] = NOTPRESSED;
	}
	if (nextoperationFlag == 1)
	{
		NumberTwoSign = 0;	
	}
	else
	{
		NumberOneSign = 0;
		NumberTwoSign = 0;
		ResultSign = 0;	
	}
	NumberOne = 0;
	NumberTwo = 0;
	counter1 = 0;
	counter2 = 0;
	FirstNumberFloatFlag = 0;
	SecondNumberFloatFlag = 0;
}





