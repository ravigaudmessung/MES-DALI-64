////////////////////////////////////////////////////////////////////////////////
//
// File: AppBoard.c
//
// Hardware and port settings
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "AppBoard.h"															// General defines for port IO
#include <peripheral_clk_config.h>
#include "hal_gpio.h"
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>




//struct flash_descriptor FLASH_0;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

bool_t KNX_OK, KNX_OK_low = 0;
bool_t write_flash_flag = FALSE;

/// This function has to be called after startup to
/// initialize the port settings.
///


void AppBoard_PortsInit(void)
{

	init_mcu();

	PORT->Group[0].DIRSET.reg = PORTA_MODE_OUT_PINS;							// Set PortA pins as outputs
	PORT->Group[0].WRCONFIG.reg = PORT_WRCONFIG_WRPINCFG | PORT_WRCONFIG_PULLEN	// Write configuration as inputs with pullups
						| PORT_WRCONFIG_INEN | (PORTA_MODE_IN_PU_PINS & 0xFFFF);// for low 16 pins
	PORT->Group[0].WRCONFIG.reg = PORT_WRCONFIG_HWSEL | PORT_WRCONFIG_WRPINCFG	// Write configuration
						| PORT_WRCONFIG_PULLEN| PORT_WRCONFIG_INEN |			// as inputs with pullups
						(PORTA_MODE_IN_PU_PINS >> 16);							// for high 16 pins
	PORT->Group[0].OUTSET.reg = PORTA_MODE_IN_PU_PINS;							// Set psull-up on

	PORT->Group[1].DIRSET.reg = PORTB_MODE_OUT_PINS;							// Set PortB pins as outputs
	PORT->Group[1].WRCONFIG.reg = PORT_WRCONFIG_WRPINCFG | PORT_WRCONFIG_PULLEN	// Write config as inputs with pullups
						| PORT_WRCONFIG_INEN | (PORTB_MODE_IN_PU_PINS & 0xFFFF);// for low 16 pins
	PORT->Group[1].WRCONFIG.reg = PORT_WRCONFIG_HWSEL | PORT_WRCONFIG_WRPINCFG	// Write config
						| PORT_WRCONFIG_PULLEN| PORT_WRCONFIG_INEN | 			// as inputs with pullups
						(PORTB_MODE_IN_PU_PINS >> 16);							// for high 16 pins
	PORT->Group[1].OUTSET.reg = PORTB_MODE_IN_PU_PINS;							// Set pull-up on
	
	gpio_set_pin_direction(PB05, GPIO_DIRECTION_IN);				// configured to input
	gpio_set_pin_function(PB05, GPIO_PIN_FUNCTION_OFF);				// pin function off

}

// void FLASH_0_CLOCK_init(void)
// {
//   	_pm_enable_bus_clock(PM_BUS_APBB, NVMCTRL);
// }
// 
// void FLASH_0_init(void)
// {
// 	FLASH_0_CLOCK_init();
// 	flash_init(&FLASH_0, NVMCTRL);
// }


// 
// void systick_init(void)
// {
// 	SysTick->CTRL = 0;						// Disable systick
// 	SysTick->LOAD = 7999UL;					// Set Reload register
// 	NVIC_SetPriority(SysTick_IRQn,3);		// Set priority
// 	SysTick->VAL = 0;  //
// 	SysTick->CTRL = 0x00000007;
// 	NVIC_EnableIRQ(SysTick_IRQn);
// }
// 
// 
// void SysTick_Handler()
//{
// 	KNX_OK = gpio_get_pin_level(PB05);
// 	if (KNX_OK == FALSE)
// 	{
// 		KNX_OK_low++;
// 	}
// 
// 	if (KNX_OK_low == 5)
// 	{
// 		write_flash_flag = TRUE;
// 	}
// }




// End of AppBoard.c
