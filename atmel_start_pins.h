/*
* Code generated from Atmel Start.
*
* This file will be overwritten when reconfiguring your Atmel Start project.
* Please copy examples or other code you want to keep to a separate file
* to avoid losing it when reconfiguring.
*/
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD20 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7


#define EEPROM_WP GPIO(GPIO_PORTB, 08)
#define NEXT_PIN GPIO(GPIO_PORTA, 07)
#define SELECT_PIN GPIO(GPIO_PORTA, 06)
#define PREV_PIN GPIO(GPIO_PORTA, 05)
#define DC GPIO(GPIO_PORTA, 16)
#define MISO GPIO(GPIO_PORTA, 17)
#define ERD GPIO(GPIO_PORTA, 18)
#define CLCK GPIO(GPIO_PORTA, 19)
#define WDI GPIO(GPIO_PORTB, 16)
#define DB7 GPIO(GPIO_PORTB, 17)
#define MOSI GPIO(GPIO_PORTA, 20)
#define DB4 GPIO(GPIO_PORTA, 23)
#define DB5 GPIO(GPIO_PORTA, 24)
#define DB6 GPIO(GPIO_PORTA, 25)
#define RST GPIO(GPIO_PORTB, 22)
#define CS GPIO(GPIO_PORTB, 23)

#define DALI_RX GPIO(GPIO_PORTB, 02)
#define DALI_TX GPIO(GPIO_PORTB, 03)





#define GET_DALI_RX			 ((PORT->Group[1].IN.reg & BIT02)  == 1)
#define SET_LOW_DALI_TX		  {PORT->Group[1].OUTCLR.reg = BIT03;}
#define SET_HIGH_DALI_TX		  {PORT->Group[1].OUTSET.reg = BIT03;}

#define SET_LOW_TEST		  {PORT->Group[1].OUTCLR.reg = BIT03;}
#define SET_HIGH_TEST		  {PORT->Group[1].OUTSET.reg = BIT03;}
#define GET_TEST		 ((PORT->Group[1].IN.reg & BIT02)  == 1)



// #define TEST_PIN GPIO(GPIO_PORTA, 23)
// #define TEST_PIN_2 GPIO(GPIO_PORTA, 02)

// #define PA24 GPIO(GPIO_PORTA, 24)	   //TX
// #define PA25 GPIO(GPIO_PORTA, 25) //RX
#endif // ATMEL_START_PINS_H_INCLUDED
