/*
 * Button.c
 *
 * Created: 10/9/2023 4:14:48 PM
 *  Author: M Osama
 */ 


#include "Button.h"

void Button_vInit (unsigned char port , unsigned char pin){
	DIO_pin_dierction(port,pin,0);	
}

unsigned char Button_u8read(unsigned char port,unsigned char pin){
	unsigned char retval = 0;
	retval = DIO_pin_read(port,pin);
	return retval;	
}	