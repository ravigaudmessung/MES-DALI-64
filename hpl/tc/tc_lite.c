
/**
 * \file
 *
 * \brief TC related functionality implementation.
 *
 * Copyright (c) 2017 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
		  #include "atmel_start_pins.h"
#include "tc_lite.h"

/**
 * \brief Initialize TC interface
 */
int8_t TIMER_0_init()
{

	hri_tc_wait_for_sync(TC2);
	if (hri_tc_get_CTRLA_reg(TC2, TC_CTRLA_ENABLE)) {
		hri_tc_clear_CTRLA_ENABLE_bit(TC2);
		hri_tc_wait_for_sync(TC2);
	}
	hri_tc_write_CTRLA_reg(TC2, TC_CTRLA_SWRST);
	hri_tc_wait_for_sync(TC2);

	hri_tc_write_CTRLA_reg(TC2,
	                       0 << TC_CTRLA_PRESCSYNC_Pos       /* Prescaler and Counter Synchronization: 0 */
	                           | 0 << TC_CTRLA_RUNSTDBY_Pos  /* Run in Standby: disabled */
	                           | 0 << TC_CTRLA_PRESCALER_Pos /* Setting: 6 */
	                           | 2 << TC_CTRLA_WAVEGEN_Pos   /* Waveform Generation Operation: 2 */
	                           | 0x1 << TC_CTRLA_MODE_Pos);  /* Operating Mode: 0x1 */

// 	 hri_tc_write_CTRLB_reg(TC2,0 << TC_CTRLBSET_CMD_Pos /* Command: 0 */
// 			 | 0 << TC_CTRLBSET_ONESHOT_Pos /* One-Shot: disabled */
// 			 | 0 << TC_CTRLBSET_DIR_Pos); /* Counter Direction: disabled */

	// hri_tc_write_CTRLC_reg(TC2,0 << TC_CTRLC_CPTEN1_Pos /* Capture Channel 1 Enable: disabled */
	//		 | 0 << TC_CTRLC_CPTEN0_Pos); /* Capture Channel 0 Enable: disabled */

	// hri_tc_write_READREQ_reg(TC2,0 << TC_READREQ_RREQ_Pos /*  Read Request: disabled */
	//		 | 0 << TC_READREQ_RCONT_Pos /*  Read Continuously: disabled */
	//		 | 0x0); /* Address: 0x0 */

	// hri_tc_write_DBGCTRL_reg(TC2,0); /* Run in debug: 0 */

 hri_tccount8_write_CC_reg(TC2, 0 ,0xFF); /* Compare/Capture Value: 0x0 */

	// hri_tccount8_write_CC_reg(TC2, 1 ,0x0); /* Compare/Capture Value: 0x0 */

	// hri_tccount8_write_COUNT_reg(TC2,0x0); /* Counter Value: 0x0 */

	// hri_tc_write_PER_reg(TC2,0x0); /*  Period Value: 0x0 */

	// hri_tc_write_EVCTRL_reg(TC2,0 << TC_EVCTRL_MCEO0_Pos /* Match or Capture Channel 0 Event Output Enable: disabled
	// */
	//		 | 0 << TC_EVCTRL_MCEO1_Pos /* Match or Capture Channel 1 Event Output Enable: disabled */
	//		 | 0 << TC_EVCTRL_OVFEO_Pos /* Overflow/Underflow Event Output Enable: disabled */
	//		 | 0 << TC_EVCTRL_TCEI_Pos /* TC Event Input: disabled */
	//		 | 0 << TC_EVCTRL_TCINV_Pos /* TC Inverted Event Input: disabled */
	//		 | 0); /* Event Action: 0 */

	  hri_tc_write_INTEN_reg(TC2,1 << TC_INTENSET_MC0_Pos /* Match or Capture Channel 0 Interrupt Enable: disabled */
	  |  1<< TC_INTENSET_MC1_Pos /* Match or Capture Channel 1 Interrupt Enable: disabled */
	  | 0 << TC_INTENSET_SYNCRDY_Pos /* Synchronization Ready Interrupt Enable: disabled */
	  | 0 << TC_INTENSET_ERR_Pos /* Error Interrupt Enable: disabled */
	  | 0 << TC_INTENSET_OVF_Pos); /* Overflow Interrupt enable: disabled */

	hri_tc_write_CTRLA_ENABLE_bit(TC2, 1 << TC_CTRLA_ENABLE_Pos); /* Enable: enabled */

NVIC_SetPriority(TC2_IRQn,0);
NVIC_EnableIRQ(TC2_IRQn);
	return 0;
}



#define PFI_TM_SET_REG_SYS(nTicks)	{ TC2->COUNT8.CC[0].reg = nTicks; }		// Access to set register (time stamp of system timer interrupt)
#define PFI_TM_GET_REG_SYS			(TC2->COUNT8.CC[0].reg)					// Access to get register (time stamp of system timer interrupt)

// Macros to get compare register for receive timer interrupts
#define PFI_TM_GET_REG_REC			(TC2->COUNT8.CC[1].reg)					// Access to get register (time stamp of receive timer interrupt)

// Macro to clear system timer interrupt
#define PFI_TM_CLEAR_MS_INT			{ TC2->COUNT8.INTFLAG.reg = TC_INTFLAG_MC((1 << 0)); }// Clear interrupt flag


// 
//  void TC1_Handler(void)
//  {
//  																// Delta between interrupt- and current time [ticks]
//  					//gpio_toggle_pin_level(TEST_PIN_2)	;
// 				  					gpio_toggle_pin_level(TEST_PIN)	;
// 				
//  	if(TC1->COUNT8.INTFLAG.bit.MC0 && TC1->COUNT8.INTENSET.bit.MC0)			// If system timer interrupt pending and enabled
//  	{
//  		PFI_TM_CLEAR_MS_INT;													// Clear interrupt flag
//  
//  
//  
//  // 		nDelta  = PFI_TM_TICKS;													// Set current time [ticks]
// //  		nDelta -= PFI_TM_GET_REG_SYS;											// Subtract interrupt time -> Delta [ticks]
// //  		if(nDelta < (PFI_TM_TICKS_PER_MS / 2))									// If delta smaller than half of a period
// //  		{
// //  			PFI_TM_SET_REG_SYS(PFI_TM_GET_REG_SYS + PFI_TM_TICKS_PER_MS);		// Set time stamp [ticks] for next interrupt (1 ms relative to interrupt time)
// //  		}
// //  		else																	// If delta bigger than half of a period
// //  		{
// //  			PFI_TM_SET_REG_SYS(PFI_TM_TICKS + PFI_TM_TICKS_PER_MS);				// Set time stamp [ticks] for next interrupt (1 ms relative to current time)
// //  		}
// 
//  		
//  	}
//  }
