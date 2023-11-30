/*
 * Button.h
 *
 * Created: 10/9/2023 4:15:02 PM
 *  Author: M Osama
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include "DIO.h"

void Button_vInit (unsigned char port , unsigned char pin);

unsigned char Button_u8read(unsigned char port,unsigned char pin);

#endif /* BUTTON_H_ */