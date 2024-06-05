/**
 * \file
 *
 * \brief HPL initialization related functionality implementation.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <hpl_gpio.h>
#include <hpl_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_config.h>

/* Referenced GCLKs (out of 0~7), should be initialized firstly
 * - GCLK 3 for DFLL48M
 */
#define _GCLK_INIT_1ST 0x00000008
/* Not referenced GCLKs, initialized last */
#define _GCLK_INIT_LAST 0x000000F7

/**
 * \brief Initialize the hardware abstraction layer
 */
void _init_chip(void)
{
	hri_nvmctrl_set_CTRLB_RWS_bf(NVMCTRL, CONF_NVM_WAIT_STATE);

	_pm_init();
	 	initializeSystemFor48MHz();

	//_sysctrl_init_sources();
// #if _GCLK_INIT_1ST
// 	_gclk_init_generators_by_fref(_GCLK_INIT_1ST);
// #endif
// 	_sysctrl_init_referenced_generators();
// 	_gclk_init_generators_by_fref(_GCLK_INIT_LAST);
}

  void initializeSystemFor48MHz()
{
    // Change the timing of the NVM access 
    NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val; // 1 wait state for operating at 2.7-3.3V at 48MHz.

    // Enable the bus clock for the clock system.
    PM->APBAMASK.bit.GCLK_ = true;


SYSCTRL->XOSC.reg = SYSCTRL_XOSC_STARTUP(0xC) | SYSCTRL_XOSC_AMPGC |		// StartUp=0.25s, auto gain
					SYSCTRL_XOSC_XTALEN		  | SYSCTRL_XOSC_ENABLE;		// Crystal and oscillator enable

    // Initialise the DFLL to run in closed-loop mode at 48MHz
    // 1. Make a software reset of the clock system.
    GCLK->CTRL.bit.SWRST = true;
    while (GCLK->CTRL.bit.SWRST && GCLK->STATUS.bit.SYNCBUSY) {};
    // 2. Make sure the OCM8M keeps running.
    SYSCTRL->XOSC.bit.ONDEMAND = 0;
    // 3. Set the division factor to 64, which reduces the 1MHz source to 15.625kHz
    GCLK->GENDIV.reg =
        GCLK_GENDIV_ID(3) | // Select generator 3
        GCLK_GENDIV_DIV(150); // Set the division factor to 64
    // 4. Create generic clock generator 3 for the 15KHz signal of the DFLL
    GCLK->GENCTRL.reg =
        GCLK_GENCTRL_ID(3) | // Select generator 3
        GCLK_GENCTRL_SRC_XOSC | // Select source OSC8M
        GCLK_GENCTRL_GENEN; // Enable this generic clock generator
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
    // 5. Configure DFLL with the
    GCLK->CLKCTRL.reg =
        GCLK_CLKCTRL_ID_DFLL48M | // Target is DFLL48M
        GCLK_CLKCTRL_GEN(3) | // Select generator 3 as source.
        GCLK_CLKCTRL_CLKEN; // Enable the DFLL48M
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
    // 6. Workaround to be able to configure the DFLL.
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = false;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 7. Change the multiplication factor. 
  //  SYSCTRL->DFLLMUL.bit.MUL = 0; // 48MHz / (1MHz / 64)
    //SYSCTRL->DFLLMUL.bit.CSTEP = 0; // Coarse step = 1
    //SYSCTRL->DFLLMUL.bit.FSTEP = 0; // Fine step = 1
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 8. Start closed-loop mode
//     SYSCTRL->DFLLCTRL.reg |=
//         SYSCTRL_DFLLCTRL_MODE | // 1 = Closed loop mode.
//         SYSCTRL_DFLLCTRL_QLDIS; // 1 = Disable quick lock.
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 9. Clear the lock flags.
    SYSCTRL->INTFLAG.bit.DFLLLCKC = 1;
    SYSCTRL->INTFLAG.bit.DFLLLCKF = 1;
    SYSCTRL->INTFLAG.bit.DFLLRDY = 1;
    // 10. Enable the DFLL
    SYSCTRL->DFLLCTRL.bit.ENABLE = true;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 11. Wait for the fine and coarse locks.
  //  while (!SYSCTRL->INTFLAG.bit.DFLLLCKC && !SYSCTRL->INTFLAG.bit.DFLLLCKF) {};
    // 12. Wait until the DFLL is ready.
    while (!SYSCTRL->INTFLAG.bit.DFLLRDY) {};  

    // Switch the main clock speed.
    // 1. Set the divisor of generic clock 0 to 0
    GCLK->GENDIV.reg =
        GCLK_GENDIV_ID(0) | // Select generator 0
        GCLK_GENDIV_DIV(0);
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization       
    // 2. Switch generic clock 0 to the DFLL
    GCLK->GENCTRL.reg =
        GCLK_GENCTRL_ID(0) | // Select generator 0
        GCLK_GENCTRL_SRC_DFLL48M | // Select source DFLL
        GCLK_GENCTRL_IDC | // Set improved duty cycle 50/50
        GCLK_GENCTRL_GENEN; // Enable this generic clock generator
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
		
		
		
				 GCLK->GENDIV.reg =
				 GCLK_GENDIV_ID(1) | // Select generator 3
				 GCLK_GENDIV_DIV(2); // Set the division factor to 64
				 // 4. Create generic clock generator 3 for the 15KHz signal of the DFLL
				 GCLK->GENCTRL.reg =
				 GCLK_GENCTRL_ID(1) | // Select generator 3
				 GCLK_GENCTRL_SRC_XOSC| // Select source OSC8M
				 GCLK_GENCTRL_GENEN; // Enable this generic clock generator
				 while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
				 // 5. Configure DFLL with the
// 				 GCLK->CLKCTRL.reg =
// 				 GCLK_CLKCTRL_ID_TC0_TC1| // Target is DFLL48M
// 				 GCLK_CLKCTRL_GEN(1) | // Select generator 3 as source.
// 				 GCLK_CLKCTRL_CLKEN; // Enable the DFLL48M
// 				 while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
			 
		 // 5. Configure DFLL with the
// 		 GCLK->CLKCTRL.reg =
// 		 GCLK_CLKCTRL_ID_DFLL48M | // Target is DFLL48M
// 		 GCLK_CLKCTRL_GEN(3) | // Select generator 3 as source.
// 		 GCLK_CLKCTRL_CLKEN; // Enable the DFLL48M
// 		 while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
		
		 GCLK->GENDIV.reg =
		 GCLK_GENDIV_ID(2) | // Select generator 3
		 GCLK_GENDIV_DIV(3); // Set the division factor to 64
		 // 4. Create generic clock generator 3 for the 15KHz signal of the DFLL
		 GCLK->GENCTRL.reg =
		 GCLK_GENCTRL_ID(2) | // Select generator 3
		 GCLK_GENCTRL_SRC_DFLL48M| // Select source OSC8M
		 GCLK_GENCTRL_GENEN; // Enable this generic clock generator
		 while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
			 
			 
			 
			 
		
}

//   void initializeSystemFor48MHz()
//   {
// 	  // Change the timing of the NVM access
// 	  NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val; // 1 wait state for operating at 2.7-3.3V at 48MHz.
// 
// 	  // Enable the bus clock for the clock system.
// 	  PM->APBAMASK.bit.GCLK_ = true;
// 
// 	  // Initialise the DFLL to run in closed-loop mode at 48MHz
// 	  // 1. Make a software reset of the clock system.
// 	  GCLK->CTRL.bit.SWRST = true;
// 	  while (GCLK->CTRL.bit.SWRST && GCLK->STATUS.bit.SYNCBUSY) {};
// 	  // 2. Make sure the OCM8M keeps running.
// 	  SYSCTRL->OSC8M.bit.ONDEMAND = 0;
// 	  // 3. Set the division factor to 64, which reduces the 1MHz source to 15.625kHz
// 	  GCLK->GENDIV.reg =
// 	  GCLK_GENDIV_ID(3) | // Select generator 3
// 	  GCLK_GENDIV_DIV(64); // Set the division factor to 64
// 	  // 4. Create generic clock generator 3 for the 15KHz signal of the DFLL
// 	  GCLK->GENCTRL.reg =
// 	  GCLK_GENCTRL_ID(3) | // Select generator 3
// 	  GCLK_GENCTRL_SRC_OSC8M | // Select source OSC8M
// 	  GCLK_GENCTRL_GENEN; // Enable this generic clock generator
// 	  while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
// 	  // 5. Configure DFLL with the
// 	  GCLK->CLKCTRL.reg =
// 	  GCLK_CLKCTRL_ID_DFLL48M | // Target is DFLL48M
// 	  GCLK_CLKCTRL_GEN(3) | // Select generator 3 as source.
// 	  GCLK_CLKCTRL_CLKEN; // Enable the DFLL48M
// 	  while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
// 	  // 6. Workaround to be able to configure the DFLL.
// 	  SYSCTRL->DFLLCTRL.bit.ONDEMAND = false;
// 	  while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
// 	  // 7. Change the multiplication factor.
// 	  SYSCTRL->DFLLMUL.bit.MUL = 3072; // 48MHz / (1MHz / 64)
// 	  SYSCTRL->DFLLMUL.bit.CSTEP = 1; // Coarse step = 1
// 	  SYSCTRL->DFLLMUL.bit.FSTEP = 1; // Fine step = 1
// 	  while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
// 	  // 8. Start closed-loop mode
// 	  SYSCTRL->DFLLCTRL.reg |=
// 	  SYSCTRL_DFLLCTRL_MODE | // 1 = Closed loop mode.
// 	  SYSCTRL_DFLLCTRL_QLDIS; // 1 = Disable quick lock.
// 	  while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
// 	  // 9. Clear the lock flags.
// 	  SYSCTRL->INTFLAG.bit.DFLLLCKC = 1;
// 	  SYSCTRL->INTFLAG.bit.DFLLLCKF = 1;
// 	  SYSCTRL->INTFLAG.bit.DFLLRDY = 1;
// 	  // 10. Enable the DFLL
// 	  SYSCTRL->DFLLCTRL.bit.ENABLE = true;
// 	  while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
// 	  // 11. Wait for the fine and coarse locks.
// 	  while (!SYSCTRL->INTFLAG.bit.DFLLLCKC && !SYSCTRL->INTFLAG.bit.DFLLLCKF) {};
// 	  // 12. Wait until the DFLL is ready.
// 	  while (!SYSCTRL->INTFLAG.bit.DFLLRDY) {};
// 
// 	  // Switch the main clock speed.
// 	  // 1. Set the divisor of generic clock 0 to 0
// 	  GCLK->GENDIV.reg =
// 	  GCLK_GENDIV_ID(0) | // Select generator 0
// 	  GCLK_GENDIV_DIV(0);
// 	  while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
// 	  // 2. Switch generic clock 0 to the DFLL
// 	  GCLK->GENCTRL.reg =
// 	  GCLK_GENCTRL_ID(0) | // Select generator 0
// 	  GCLK_GENCTRL_SRC_DFLL48M | // Select source DFLL
// 	  GCLK_GENCTRL_IDC | // Set improved duty cycle 50/50
// 	  GCLK_GENCTRL_GENEN; // Enable this generic clock generator
// 	  while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
//  // void initializeSystemFor48MHzs()
// {
	

//     // Change the timing of the NVM access 
//     NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val; // 1 wait state for operating at 2.7-3.3V at 48MHz.
// 
//     // Enable the bus clock for the clock system.
//     PM->APBAMASK.bit.GCLK_ = true;
// 
//     // Initialise the DFLL to run in closed-loop mode at 48MHz
//     // 1. Make a software reset of the clock system.
//     GCLK->CTRL.bit.SWRST = true;
//     while (GCLK->CTRL.bit.SWRST && GCLK->STATUS.bit.SYNCBUSY) {};
//     // 2. Make sure the OCM8M keeps running.
//    // SYSCTRL->OSC8M.bit.ONDEMAND = 0;
//     // 3. Set the division factor to 64, which reduces the 1MHz source to 15.625kHz
//     GCLK->GENDIV.reg =
//         GCLK_GENDIV_ID(3) | // Select generator 3
//         GCLK_GENDIV_DIV(64); // Set the division factor to 64
//     // 4. Create generic clock generator 3 for the 15KHz signal of the DFLL
//     GCLK->GENCTRL.reg =
//         GCLK_GENCTRL_ID(3) | // Select generator 3
//         GCLK_GENCTRL_SRC_OSC8M| // Select source OSC8M
//         GCLK_GENCTRL_GENEN; // Enable this generic clock generator
//     while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
//     // 5. Configure DFLL with the
//     GCLK->CLKCTRL.reg =
//         GCLK_CLKCTRL_ID_DFLL48M | // Target is DFLL48M
//         GCLK_CLKCTRL_GEN(3) | // Select generator 3 as source.
//         GCLK_CLKCTRL_CLKEN; // Enable the DFLL48M
//     while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
//     // 6. Workaround to be able to configure the DFLL.
//     SYSCTRL->DFLLCTRL.bit.ONDEMAND = false;
//     while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
//     // 7. Change the multiplication factor. 
//     SYSCTRL->DFLLMUL.bit.MUL = 3072; // 48MHz / (1MHz / 64)		 /3072
//     SYSCTRL->DFLLMUL.bit.CSTEP = 1; // Coarse step = 1
//     SYSCTRL->DFLLMUL.bit.FSTEP = 1; // Fine step = 1
//     while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
//     // 8. Start closed-loop mode
//    // SYSCTRL->DFLLCTRL.reg |=
//     //    SYSCTRL_DFLLCTRL_MODE | // 1 = Closed loop mode.
//      //   SYSCTRL_DFLLCTRL_QLDIS; // 1 = Disable quick lock.
//     while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
//     // 9. Clear the lock flags.
//     SYSCTRL->INTFLAG.bit.DFLLLCKC = 1;
//     SYSCTRL->INTFLAG.bit.DFLLLCKF = 1;
//     SYSCTRL->INTFLAG.bit.DFLLRDY = 1;
//     // 10. Enable the DFLL
//     SYSCTRL->DFLLCTRL.bit.ENABLE = true;
//     while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
//     // 11. Wait for the fine and coarse locks.
// 	while (!SYSCTRL->INTFLAG.bit.DFLLLCKC && !SYSCTRL->INTFLAG.bit.DFLLLCKF) {};
//     // 12. Wait until the DFLL is ready.
//     while (!SYSCTRL->INTFLAG.bit.DFLLRDY) {};  
// 
//     // Switch the main clock speed.
//     // 1. Set the divisor of generic clock 0 to 0
//     GCLK->GENDIV.reg =
//         GCLK_GENDIV_ID(0) | // Select generator 0
//         GCLK_GENDIV_DIV(0);
//     while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization       
//     // 2. Switch generic clock 0 to the DFLL
//     GCLK->GENCTRL.reg =
//         GCLK_GENCTRL_ID(0) | // Select generator 0
//         GCLK_GENCTRL_SRC_DFLL48M | // Select source DFLL
//         GCLK_GENCTRL_IDC | // Set improved duty cycle 50/50
//         GCLK_GENCTRL_GENEN; // Enable this generic clock generator
//     while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
// 
// 
// 
// 
// // GCLK->GENDIV.reg =
// GCLK_GENDIV_ID(2) | // Select generator 3
// GCLK_GENDIV_DIV(6); // Set the division factor to 64

// 
//  //	GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC) 				// Enable clock
 	//| GCLK_GENCTRL_ID(1);
// 
// 	GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC(2) 				// Enable clock
// 	| GCLK_GENCTRL_ID(2);
	
// 	
//