////////////////////////////////////////////////////////////////////////////////
//
// File: AppKey.c
//
// Channel for input key
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "KnxTm.h"																// Header for the timer functions
#include "AppKey.h"																// Application input key


////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////

// Macros to get and set the key state without flag button pressed
#define GET_APP_KEY_STATE(pThis)			((pThis)->nState & KEY_ST_MASK)
#define GET_APP_KEY_EVENT(pThis)			((pThis)->nState & KEY_EV_MASK)
#define SET_APP_KEY_STATE(pThis, nNewState)	((pThis)->nState  = ((pThis)->nState & (~KEY_ST_MASK)) | \
																 (nNewState & KEY_ST_MASK))
#define SET_APP_KEY_EVENT(pThis, nNewEvent)	((pThis)->nState  = ((pThis)->nState & (~KEY_EV_MASK)) | \
																 (nNewEvent & KEY_EV_MASK))


////////////////////////////////////////////////////////////////////////////////
// Local members
////////////////////////////////////////////////////////////////////////////////

/// State of used keys
static KeyChannel_t m_pKeys[APP_KEY_COUNT];


////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

static void		AppKey_ProcessChannel(uint8_t nChannel);
static bool_t	AppKey_IsKeyPressed(uint8_t nChannel);


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/// This function is called to
/// initialize the key module.
///
/// @return TRUE if success
///
bool_t AppKey_Init(void)
{
	uint8_t nIndex;																// Index for loop

	for(nIndex = 0; nIndex < APP_KEY_COUNT; nIndex++)							// For any channel
	{
		m_pKeys[nIndex].nLastTime = KnxTm_GetTimeMs();							// Time stamp of last event
		m_pKeys[nIndex].nState = KEY_ST_OFF;									// State of key
	}

	return TRUE;																// Return success
}

/// This function returns and clears the key event.
///
/// The event number is the key number mapped
/// in AppKey.h.
///
/// @param[in] nKeyNo Key number
/// @return Event number, -1 = error
///
int8_t AppKey_GetKeyEvent(uint8_t nKeyNo)
{
	uint8_t nEvent;																// The key event

	if(nKeyNo >= APP_KEY_COUNT)													// If key no out of range
	{
		return -1;																// Return error
	}

	nEvent = GET_APP_KEY_EVENT(m_pKeys + nKeyNo);								// Extract the event
	SET_APP_KEY_EVENT(m_pKeys + nKeyNo, KEY_EV_NONE);							// Clear the event
	return nEvent;																// Return the event
}

/// This function checks the key state
/// and sets the key state.
///
void AppKey_Process(void)
{
	uint8_t nIndex;																// Index for loop

	for(nIndex = 0; nIndex < APP_KEY_COUNT; nIndex++)							// For any channel
	{
		AppKey_ProcessChannel(nIndex);											// Call state machine
	}
}

/// This function processes the state machine for keys.
/// This function has to be called periodically in main level.
///
/// @param[in] nChannel Current channel
///
static void AppKey_ProcessChannel(uint8_t nChannel)
{
	KeyChannel_t* pThis;														// Pointer to current channel

	pThis = &m_pKeys[nChannel];													// Get pointer to channel

	switch(GET_APP_KEY_STATE(pThis))											// Switch due to key state
	{
		case KEY_ST_OFF:														// State is off, key was up
			if(AppKey_IsKeyPressed(nChannel))									// If key actually is pressed
			{
				pThis->nLastTime = KnxTm_GetTimeMs();							// Get current time
				SET_APP_KEY_STATE(pThis, KEY_ST_NEW);							// Set new state
			}
			break;

		case KEY_ST_NEW:														// State is new pressed, key was down
			if(AppKey_IsKeyPressed(nChannel))									// If key still is pressed
			{
				if(KnxTm_GetDelayMs(pThis->nLastTime)
					> KEY_TIME_LONG)											// If delay is enough
				{
					SET_APP_KEY_STATE(pThis, KEY_ST_LONG);						// Set new state
					SET_APP_KEY_EVENT(pThis, KEY_EV_LONG);						// Set new state
				}
			}
			else																// If key is released
			{
				if(KnxTm_GetDelayMs(pThis->nLastTime)
					> KEY_TIME_SHORT)											// If time is enough
				{
					SET_APP_KEY_EVENT(pThis, KEY_EV_SHORT);						// Set new state
				}
				SET_APP_KEY_STATE(pThis, KEY_ST_OFF);							// Set new state
			}
			break;

		case KEY_ST_LONG:														// If state is on, key still pressed
			if(!AppKey_IsKeyPressed(nChannel))									// If key actually is pressed
			{
				SET_APP_KEY_STATE(pThis, KEY_ST_OFF);							// Set new state
				SET_APP_KEY_EVENT(pThis, KEY_EV_RELEASE);						// Release-Event-NEW
			}
			break;

		default:
			pThis->nState = KEY_ST_OFF;											// Set to state OFF
			break;
	}
}

/// This function is called to retrieve
/// the current state of a key.
///
/// @param[in] nChannel Current channel
/// @return TRUE if key is pressed,
///			FALSE if released and on error
///
bool_t AppKey_IsKeyPressed(uint8_t nChannel)
{
	switch(nChannel)															// Switch due to channel no.
	{
		//case 0:																	// Case: Channel #0
			//return IS_KEY_PRESSED_0;											// Return key state
//
//#if(APP_KEY_COUNT > 1)
		//case 1:																	// Case: Channel #1
			//return IS_KEY_PRESSED_1;											// Return key state
//#endif
//
//#if(APP_KEY_COUNT > 2)
		//case 2:																	// Case: Channel #2
			//return IS_KEY_PRESSED_2;											// Return key state
//#endif
//
//#if(APP_KEY_COUNT > 3)
		//case 3:																	// Case: Channel #3
			//return IS_KEY_PRESSED_3;											// Return key state
//#endif
//
//#if(APP_KEY_COUNT > 4)
		//case 4:																	// Case: Channel #4
			//return IS_KEY_PRESSED_4;											// Return key state
//#endif
//
//#if(APP_KEY_COUNT > 5)
		//case 5:																	// Case: Channel #5
			//return IS_KEY_PRESSED_5;											// Return key state
//#endif
//
		default:																// On any other value
			return FALSE;														// Return error
	}
}

// End of AppKey.c
