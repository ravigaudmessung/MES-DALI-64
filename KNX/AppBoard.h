/////////////////////////////////////////////////////////////////////////////////
//
// File: AppBoard.h
//
// Hardware and port setting
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPBOARD__H___INCLUDED
#define APPBOARD__H___INCLUDED


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "hal_gpio.h"
//#include <hal_flash.h>

//extern struct flash_descriptor FLASH_0;
////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////

// Enable and disable interrupt system
#define ENABLE_INT						{__enable_irq();	}					// Enable interrupt processing
#define DISABLE_INT						{__disable_irq();	}					// Disable interrupt processing

// Defines for key readout

#define GET_KEY_S3			//	((PORT->Group[0].IN.reg & BIT25)  == 0)			// TRUE, if key is pressed
#define GET_KEY_S4			//	((PORT->Group[0].IN.reg & BIT23)  == 0)			// TRUE, if key is pressed
#define GET_KEY_S5			//	((PORT->Group[0].IN.reg & BIT22)  == 0)			// TRUE, if key is pressed
#define GET_KEY_S6			//	((PORT->Group[0].IN.reg & BIT21)  == 0)			// TRUE, if key is pressed
#define GET_KEY_S7			//	((PORT->Group[0].IN.reg & BIT20)  == 0)			// TRUE, if key is pressed

#define PA00 GPIO(GPIO_PORTA, 0)
#define LED0 GPIO(GPIO_PORTA, 14)


// Set all output lines:
#define PORTA_MODE_OUT_PINS					( BIT27 | BIT23 | BIT22 | BIT21| BIT20| BIT19 | BIT18 | BIT11 | BIT10 | BIT09 | BIT08| BIT07 | BIT06 | BIT05 | BIT04 | BIT03 | BIT02 | BIT01 | BIT00 )

// Set all input lines with pull-up:
#define PORTA_MODE_IN_PU_PINS				(BIT24 | BIT25)

// Set all output lines:
#define PORTB_MODE_OUT_PINS					(BIT00 | BIT04 | BIT05 | BIT06 | BIT07 | BIT08 | BIT09 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15 | BIT16 | BIT17)

// Set all input lines with pull-up:
#define PORTB_MODE_IN_PU_PINS				( BIT01 | BIT22 | BIT23)


#define WDI									GPIO(GPIO_PORTA, 27)
#define PB05								GPIO(GPIO_PORTB, 05)



////////////////////////////////////////////////////////////////////////////////
// Public function prototypes
////////////////////////////////////////////////////////////////////////////////
extern bool_t write_flash_flag;
extern void AppBoard_PortsInit(void);
extern void FLASH_0_init(void);
#endif /* APPBOARD__H___INCLUDED */

// End of AppBoard.h
