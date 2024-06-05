////////////////////////////////////////////////////////////////////////////////
//
// File: AppDim.c
//
// Functions for dimming/switching
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "AppDim.h"																// Functions for dimming/switching
#include "KnxTm.h"
 #include "AppDim.h"
#include "DALI_PB.h"

//  	 extern  "C" {
// 	 	 void sets(uint8_t LEVEL,uint16_t address );
//  	 };
// Timer functions
////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////

/// Brightness values for every KNX request
static const uint16_t m_pnStepTable[] =
{
	DIM_MAX_VALUE,		 // dummy (not used)
	DIM_MAX_VALUE,		 // 100% = 1/1
	DIM_MAX_VALUE >> 1,	 //	 50% = 1/2
	DIM_MAX_VALUE >> 2,	 //	 25% = 1/4
	DIM_MAX_VALUE >> 3,	 //	 12% = 1/8
	DIM_MAX_VALUE >> 4,	 //	  6% = 1/16
	DIM_MAX_VALUE >> 5,	 //	  3% = 1/32
	DIM_MAX_VALUE >> 6	 //	  1% = 1/64
};


////////////////////////////////////////////////////////////////////////////////
// Local members
////////////////////////////////////////////////////////////////////////////////
//
//   extern "C"{
// 	  //		void sets(uint8_t LEVEL,uint16_t address );
//
// 	  };
//#include "qqqDALI.h"


//  double call_C_f(struct C* p, int i);
//  void ccc(struct C* p, int i)
//  {
// 	 double d = call_C_f(p,i);
// 	 /* ... */
//  }





 uint16_t m_nCurrentBrightness[96];											// Actual brightness of LED
 uint16_t m_nStopBrightness[96];												// Requested brightness
 uint32_t m_nTimeStamp[96];													// Current time stamp for dimming process

//
// ////////////////////////////////////////////////////////////////////////////////
// // Functions
// ////////////////////////////////////////////////////////////////////////////////
//
// /// Initializes the application part for dimming.
// ///
void AppDim_Init(void)
{
	for (int i=0 ;i<=95;i++)
	{
			
			m_nCurrentBrightness[i] = DIM_MIN_VALUE;										// LED is off
			m_nStopBrightness[i] = DIM_MIN_VALUE;
			m_nTimeStamp[i] = KnxTm_GetTimeMs();
	}
	
}
//
// /// Main process for dimming.
// /// Called frequently by the main loop.
// ///

//  struct C instance_of_C;


	 extern double setss( uint8_t ll, uint16_t ss );





//
// /// Switch LED on or off.
// ///
// /// @param[in] bValue TRUE: switch LED to maximum brightness,
// /// FALSE: switch it off
// ///
// void AppDim_Switch(bool_t bValue)
// {
// 	if(bValue)																	// Select between Led-On and Led-Off
// 	{
// 		m_nCurrentBrightness = DIM_MAX_VALUE;									// Turn LED on (max. brightness)
// 		m_nStopBrightness	 = DIM_MAX_VALUE;									// Turn LED on (max. brightness)
// 	}
// 	else
// 	{
// 		m_nCurrentBrightness = DIM_MIN_VALUE;									// Turn LED off
// 		m_nStopBrightness	 = DIM_MIN_VALUE;									// Turn LED off
// 	}
// 	AppLedPwm_SetValue(m_nCurrentBrightness);									// Set new PWM duty cycle
// }
//
// /// Dim LED up, down or stop it.
// ///
// /// @param[in] nKnxData relative dimming code from telegram
// ///
void AppDim_DimRelative(uint8_t nKnxData,uint16_t address)
{
	uint8_t nStepCode;
	uint8_t nDirection;
	uint8_t nInterval;

	nStepCode  = nKnxData & DIM_REL_STEP_CODE_MASK;								// Extract step code
	nDirection = nKnxData & DIM_REL_DIRECTION_MASK;								// Extract direction bit (brighter/darker)
	nInterval  = m_pnStepTable[nStepCode];										// Map to interval

	if(nStepCode == DIM_REL_STOP)												// If it is a stop command
	{
		AppDim_DimStop(address);														// Stop dimming
	}
	else if(nDirection == DIM_REL_DIRECTION_UP)									// Else if dim up command
	{
		AppDim_DimUp(nInterval,address);												// Dim up
	}
	else																		// Else it is a dim down command
	{
		AppDim_DimDown(nInterval,address);												// Dim down
	}
}

/// Perform a relative dimming process.
///
/// @param[in] nInterval 0..255
///
void AppDim_DimUp(uint8_t nInterval,uint16_t address)
{
	m_nStopBrightness[address] = m_nCurrentBrightness[address] + nInterval;						// Calculate new value

	if(m_nStopBrightness[address] > DIM_MAX_VALUE)										// If requested brightness too high
	{
		m_nStopBrightness[address] = DIM_MAX_VALUE;										// Set to maximum (255)
	}
}

/// Perform a relative dimming process.
///
/// @param[in] nInterval 0..255
///
void AppDim_DimDown(uint8_t nInterval ,uint16_t address)
{
	m_nStopBrightness[address] = m_nCurrentBrightness[address] - nInterval;						// Calculate new value

	if(m_nStopBrightness[address] > DIM_MAX_VALUE)										// If underflow occurred
	{
		m_nStopBrightness[address] = DIM_MIN_VALUE;										// Set to zero
	}
}


/// Stop a running dimming process.
///
void AppDim_DimStop(uint16_t address)														// If dim-break has received
{
	m_nStopBrightness[address] = m_nCurrentBrightness[address];									// Keep brightness at current level
}


/// Perform a dimming process to an absolute brightness.
///
/// @param[in] nValue New value for LED (0 - 255)
///
// void AppDim_DimAbsolute(uint8_t nValue)
// {
// 	m_nStopBrightness = nValue;													// set requested value
// }
//
//
// // End of AppDim.c
//
