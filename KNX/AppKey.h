////////////////////////////////////////////////////////////////////////////////
//
// File: AppKey.h
//
// Channel for input key
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPKEY__H___INCLUDED
#define APPKEY__H___INCLUDED


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "AppBoard.h"															// General defines for port IO


////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////

// Declare here the application keys
//
// Here we can map all key we like to use.
//

// Check contact
#define APP_KEY_COUNT		3													// Count of application keys
#define IS_KEY_PRESSED_0	GET_KEY_S3											// Port pin for channel 0
#define IS_KEY_PRESSED_1	GET_KEY_S4											// Port pin for channel 1
#define IS_KEY_PRESSED_2	GET_KEY_S5											// Port pin for channel 2
#define IS_KEY_PRESSED_3	GET_KEY_S6											// Port pin for channel 3
#define IS_KEY_PRESSED_4	GET_KEY_S7											// Port pin for channel 4

// States of key
#define	KEY_ST_OFF			0x00												// Key is released
#define	KEY_ST_NEW			0x10												// Key is just pressed
#define	KEY_ST_SHORT		0x30												// Key is pressed short
#define	KEY_ST_LONG			0x40												// Key is pressed long
#define	KEY_ST_MASK			0xF0												// Key state mask

// Events of key
#define	KEY_EV_NONE			0x00												// No new event on key
#define	KEY_EV_SHORT		0x01												// Event: Key is pressed short
#define	KEY_EV_LONG			0x02												// Event: Key is pressed long
#define KEY_EV_RELEASE		0x03												// Event: Key is released
#define	KEY_EV_MASK			0x0F												// Event state mask

// Key timing
#define	KEY_TIME_SHORT		60													// De-bounce time before pressed short
#define	KEY_TIME_LONG		300													// Time before pressed long


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////

/// This structure is used to define a key channel
typedef struct KeyChannel_tag
{
	uint32_t	nLastTime;														// Time stamp of last event
	uint8_t		nState;															// State of key
} KeyChannel_t;


////////////////////////////////////////////////////////////////////////////////
// Public function prototypes
////////////////////////////////////////////////////////////////////////////////

extern bool_t	AppKey_Init(void);
extern int8_t	AppKey_GetKeyEvent(uint8_t nKeyNo);
extern void		AppKey_Process(void);


#endif /* APPKEY__H___INCLUDED */

// End of AppKey.h
