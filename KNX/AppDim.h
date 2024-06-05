////////////////////////////////////////////////////////////////////////////////
//
// File: AppDim.h
//
// Functions for dimming/switching
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPDIM__H___INCLUDED
#define APPDIM__H___INCLUDED


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
 #include "DALI_PB.h"

////////////////////////////////////////////////////////////////////////////////
// Defines and Enumerators
////////////////////////////////////////////////////////////////////////////////

#define DIM_MAX_VALUE		   0xFE												// Max. Brightness
#define DIM_MIN_VALUE		   0x00												// Min. Brightness

#define DIM_RAMP_TIME		   20												// Delay in ms for every dimming step

#define DIM_REL_STEP_CODE_MASK 0x07												// Bits expressing the step code
#define DIM_REL_DIRECTION_MASK 0x08												// Bits expressing the direction (brighter/darker)

#define DIM_REL_DIRECTION_UP   0x08												// Value for dimming up
#define DIM_REL_DIRECTION_DOWN 0x00												// Value for dimming down

/// Values for the step code from the KNX telegram.
enum
{
	DIM_REL_STOP	= 0x00,														// Dim stop
	DIM_REL_100		= 0x01,														// Dim rel. 100%
	DIM_REL_50		= 0x02,														// Dim rel.	 50%
	DIM_REL_25		= 0x03,														// Dim rel.	 25%
	DIM_REL_12		= 0x04,														// Dim rel.	 12%
	DIM_REL_6		= 0x05,														// Dim rel.	  6%
	DIM_REL_3		= 0x06,														// Dim rel.	  3%
	DIM_REL_1		= 0x07,														// Dim rel.	  1%
};


////////////////////////////////////////////////////////////////////////////////
// Public function prototypes
////
////////////////////////////////////////////////////////////////////////////

	 
extern uint16_t m_nCurrentBrightness[96];											// Actual brightness of LED
extern uint16_t m_nStopBrightness[96];												// Requested brightness
extern uint32_t m_nTimeStamp[96];													// Current time stamp for dimming process

 


extern void AppDim_Init(void);
extern void AppDim_Process(uint16_t address);

extern void AppDim_Switch(bool_t bValue);
extern void AppDim_DimUp(uint8_t nInterval,uint16_t address);
extern void AppDim_DimDown(uint8_t nInterval,uint16_t address);
extern void AppDim_DimStop(uint16_t address);
extern void AppDim_DimRelative(uint8_t nInterval,uint16_t address);
extern void AppDim_DimAbsolute(uint8_t nValue,uint16_t address);


#endif /* APPDIM__H___INCLUDED */

// End of AppDim.h
