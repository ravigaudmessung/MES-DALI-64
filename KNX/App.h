  ////////////////////////////////////////////////////////////////////////////////
//
// File: App.h Header File
//
// Header File for Demo Application
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APP__H___INCLUDED
#define APP__H___INCLUDED

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "PB_Structure.h"

////////////////////////////////////////////////////////////////////////////////
// Defines and Enumerators
////////////////////////////////////////////////////////////////////////////////

/// Data point assignment
///
/// Data points used by this application:
///
/// DP#1: Output for binary Switch
/// DP#2: Output for relative dimming or shutter move (see parameter byte #1)
/// DP#3: Input switch for LED (see also parameter byte #2)
/// DP#4: Input dimmer for LED
/// DP#5: Input absolute dimmer value for LED
///
enum DpUsage_tag
{
	DP_UNUSED					  =	 0,											// DP#0 is never used
	 
		RX_OBJECT	= 8,											// DP#1
		TX_OBJECT	=9,											// DP#2
	
	

	DP_MAX						  										// All remaining data points are not used
};










/// Parameter byte assignment
///
/// Parameter bytes used by this application:
///
/// PB#1: Configuration for DP#1 and #2
/// PB#2: Configuration for DP#3, #4 and #5
///
extern uint16_t PARAMETER_READ_COUNT ;

enum PbUsage_tag
{
	PB_UNUSED					  =	 0,											// PB#0 is never used
	PB_FIRST					  =	 1,											// First used parameter byte
	PB_SWITCH_TYPE				  =	 1,											// PB#1									// PB#2
	PB_SHUTTER_TYPE				  =  2,
	
	
	PB_MAX						  = 240,												// All remaining parameter bytes are not used
	

};



/// Parameter byte #1 configuration.
/// Usage of data points #1 and #2 (sensors)
typedef enum
{
	ST_DISABLED = 0,															// Data points are disabled
	ST_SWITCH	= 1,															// Used as binary switch
	ST_DIMMER	= 2,															// Used as dimmer
	ST_SHUTTER	= 3,																// Used as shutter control
	
} SwitchType_t;

/// Parameter byte #2 configuration.
/// Usage of data points #3, #4 and #5 (actuators)
typedef enum
{
	LT_DISABLED = 0,															// Data points are disabled
	LT_SWITCH	= 1,															// LED switch
	LT_DIMMER	= 2,																// LED dimmer
	
} LightType_t;



/// Values for the KNX telegram controlling LED switching on/off



////////////////////////////////////////////////////////////////////////////////
// Public function prototypes
////////////////////////////////////////////////////////////////////////////////

extern void     App_Init(void);
extern void     App_RetrieveProgMode(void);
extern void     App_RetrieveDownloadCounter(void);
extern uint16_t App_GetDownloadCounter(void);
extern bool_t   App_GetConnectionState(void);
extern void     App_RetrieveParameterBytes(void);
extern void     App_Main(void);
extern bool_t PB_READ_COMPLETE ;

extern void App_HandleResetIndication(void);
extern void App_HandleConnectInd(bool_t bConnected);
extern void App_HandleGetServerItemRes(uint16_t nItemId, uint8_t nItemDataLength, uint8_t* pData);
extern void App_HandleSetServerItemRes(void);
extern void App_HandleGetDatapointDescriptionRes(uint16_t nCurrentDatapoint, uint8_t nDpValueLength, uint8_t nDpConfigFlags);
extern void App_HandleGetDescriptionStringRes(uint8_t* strDpDescription, uint16_t nDpDescriptionLength);
extern void App_HandleGetDatapointValueRes(uint16_t nDpId, uint8_t nDpState, uint8_t nDpLength, uint8_t* pData);
extern void App_HandleDatapointValueInd(uint16_t nDpId, uint8_t nDpState, uint8_t nDpLength, uint8_t* pData);
extern void App_HandleServerItemInd(uint16_t nSiId, uint8_t nSiLength, uint8_t* pData);
extern void App_HandleSetDatapointValueRes(void);
extern void App_HandleGetParameterByteRes(uint16_t nIndex, uint8_t nByte);
extern void App_HandleError(uint8_t nErrorCode);
extern void PB_READ();

#endif /* APP__H___INCLUDED */

// End of App.h
