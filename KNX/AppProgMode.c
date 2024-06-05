////////////////////////////////////////////////////////////////////////////////
//
// File: AppProgMode.c
//
// Demo Application for BAOS Development Board, programming mode handler.
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "AppKey.h"																// Application input key
#include "App.h"																// Application related defines/interface
#include "AppProgMode.h"														// ProgMode related defines/interface
#include "KnxBaos.h"															// Object server protocol to communicate with BAOS


////////////////////////////////////////////////////////////////////////////////
// Local members
////////////////////////////////////////////////////////////////////////////////

/// Current state of Learn LED
static bool_t m_bProgMode;


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////


/// Initialize the sensor at startup.
///
void AppProgMode_Init(void)
{
	m_bProgMode = FALSE;														// Initialize programming mode (off)
}

/// Programming mode handler entry which is executed in the main loop.
///
/// This handles the key input and communications to the BAOS board.
///
void AppProgMode_Run(void)
{
	uint8_t nEvent;																// Key event (buttons)
	uint8_t nValue;																// Generic value for communication via BAOS protocol


	/*
	 * Key S3 (Prog. Key)
	 * (see AppKey.h for key mapping)
	 *
	 * If the Learning Key of the Development Board is pressed,
	 * check the last state of the programming mode from the BAOS
	 * board and toggle it (SWITCH_ON/OFF).
	 */

	nEvent = AppKey_GetKeyEvent(0);												// Get event for key S3

	switch(nEvent)
	{
		case KEY_EV_LONG:														// Key long pressed
			break;

		case KEY_EV_SHORT:														// Key short pressed
			nValue = AppProgMode_IsProgMode() ? 0 : 1;
			KnxBaos_SetServerItem(ID_PROG_MODE, 1, &nValue);					// Set new programming mode state
			AppProgMode_SetProgMode(nValue == 1 ? TRUE : FALSE);                // Turn LED on or off
			break;

		case KEY_EV_RELEASE:													// Key released after long press
			break;

		case KEY_EV_NONE:														// State of key not changed
			break;

		default:
			DBG_SUSPEND;														// Should never happen
			break;
	}
}

/// Handle the GetServerItem.Res and ServerItem.Ind data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] nSiId Current item ID from telegram
/// @param[in] nSiLength Length of current byte data from telegram
/// @param[in] pData Pointer to byte data from telegram
///
void AppProgMode_HandleServerItemResAndInd(
	uint16_t nSiId,	uint8_t nSiLength, uint8_t* pData)
{
	switch(nSiId)
	{
		case ID_PROG_MODE:														// Is it programming mode?
			if(*pData == 1)														// Is programming mode active?
			{
				m_bProgMode = TRUE;												// Set programming mode
				//SET_PRG_LED;													// Turn learning LED on
			}
			else																// No, programming mode is off
			{
				m_bProgMode = FALSE;											// Clear programming mode
				//CLR_PRG_LED;													// Turn learning LED off
			}
			break;
	}

}

/// Is the programming mode active?
///
/// @return TRUE if yes
///
bool_t AppProgMode_IsProgMode(void)
{
	return m_bProgMode;															// return current programming mode
}

/// Set programming mode
///
/// @param[in] bProgMode TRUE: Activate programming mode
///
void AppProgMode_SetProgMode(bool_t bProgMode)
{
	m_bProgMode = bProgMode;													// return current programming mode

	if(m_bProgMode)																// Is programming mode active?
	{
		//SET_PRG_LED;															// Turn learning LED on
	}
	else																		// No, programming mode is off
	{
		//CLR_PRG_LED;															// Turn learning LED off
	}
}

// End of AppProgMode.c
