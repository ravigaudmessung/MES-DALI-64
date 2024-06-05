////////////////////////////////////////////////////////////////////////////////
//
// File: App.c
//
// Demo Application for BAOS Development Board
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "App.h"																// Application related defines/interface
#include "AppActuator.h"														// Actuator related defines/interface
//#include "AppSensor.h"															// Sensor related defines/interface
#include "AppProgMode.h"														// ProgMode related defines/interface
//#include "AppKey.h"																// Application input key
#include "KnxBaos.h"															// Object server protocol to communicate with BAOS
#include "KnxTm.h"																// Timer functions
#include "KnxSer.h"																// Serial Communication
//#include "IncFile1.h"

#include "PB_Structure.h"

#include "AppBoard.h"



////////////////////////////////////////////////////////////////////////////////
// Types and Enums
////////////////////////////////////////////////////////////////////////////////

enum eState
{
	REQUEST_PROG_MODE,
	WAIT_FOR_PROG_MODE,
	REQUEST_DOWNLOAD_COUNTER,
	WAIT_FOR_DOWNLOAD_COUNTER,
	
	REQUEST_PARAMETERS,
	WAIT_FOR_PARAMETERS,


	RUNNING
};

////////////////////////////////////////////////////////////////////////////////
// Local members
////////////////////////////////////////////////////////////////////////////////

/// State machine
static enum eState m_eState;													// Current state of our state machine

/// ETS download counter
static uint16_t m_nDownloadCounter;												// ETS download counter (just an example)

/// Current connection state
static bool_t m_bConnected;														// Transport layer connection state (just an example)

uint16_t PB_REQ ;



////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////


/// Initialize the application at startup.
///
void App_Init(void)
{
	//AppKey_Init();																// Initialize our two push buttons
	AppActuator_Init();															// Initialize actuators
	//AppSensor_Init();															// Initialize sensors
	AppProgMode_Init();															// Initialize programming mode handler
	  AppDim_Init();
	m_eState = REQUEST_PROG_MODE;												// Initialize state machine
	m_nDownloadCounter = 0;														// Initialize ETS download counter
	m_bConnected = FALSE;														// Initialize connection state
}

/// Retrieve programming mode.
///
void App_RetrieveProgMode(void)
{
	KnxBaos_GetServerItem(ID_PROG_MODE, 1);
}

/// Retrieve download counter.
///
void App_RetrieveDownloadCounter(void)
{
	KnxBaos_GetServerItem(ID_DOWNLOAD_COUNTER, 1);
}

/// Get the download counter.
/// This function is not used in this demo code. It can be used as
/// an external call.
///
/// @return ETS download counter
///
uint16_t App_GetDownloadCounter(void)
{
	return m_nDownloadCounter;
}

/// Get the connection state.
/// This function is not used in this demo code. It can be used as
/// an external call.
///
/// @return Transport layer connection state
///
bool_t App_GetConnectionState(void)
{
	return m_bConnected;
}

/// Retrieve parameters bytes we need.
///
void App_RetrieveParameterBytes(void)
{
	KnxBaos_GetParameterByte(PB_FIRST, PB_MAX);
}

/// Main application entry which is executed in the main loop.
///
/// This handles first the state machine and
/// all Key inputs, communications to the BAOS board
/// and the dimming process for the LED.
///
void App_Main(void)
{
	static uint32_t nTimeOut;													// Time out time stamp for waiting for responses

	switch(m_eState)															// Handle state machine
	{
		case REQUEST_PROG_MODE:
		App_RetrieveProgMode();												// Get programming mode state
		nTimeOut = KnxTm_GetTimeMs() + 100;									// Set time out to 100 ms
		m_eState = WAIT_FOR_PROG_MODE;
		break;

		case REQUEST_DOWNLOAD_COUNTER:
		App_RetrieveDownloadCounter();										// Get ETS download counter
		nTimeOut = KnxTm_GetTimeMs() + 100;									// Set time out to 100 ms
		m_eState = WAIT_FOR_DOWNLOAD_COUNTER;
		break;
		
		case REQUEST_PARAMETERS:
		

		//KnxBaos_GetParameterByte(PB_REQ, 240);									// Get parameter byte from 1 to 240
		nTimeOut = KnxTm_GetTimeMs() + 100;									// Set time out to 100 ms
		m_eState = RUNNING;
		break;
		case RUNNING:
		
		  AppActuator_Run();
		//AppKey_Process();													// Handle key input
		//PB_READ();													// Call actuators main loop
		//AppSensor_Run();													// Call sensors main loop
		//AppProgMode_Run();													// Call programming mode handler
		break;

		case WAIT_FOR_PROG_MODE:
		case WAIT_FOR_DOWNLOAD_COUNTER:
		case WAIT_FOR_PARAMETERS:
		default:																// All WAIT states go here
		if(KnxTm_IsExpiredMs(nTimeOut))										// If a time out occurred
		{
			m_eState = REQUEST_PROG_MODE;									// start over to request programming mode
		}
		break;
	}
}




/// Handle reset indication.
///
/// BAOS has been reset (could be due to a change of the parameters via ETS).
///
void App_HandleResetIndication(void)
{
	m_eState = REQUEST_PROG_MODE;												// Initialize state machine
}

/// Handle the connect/disconnect indication.
///
/// This indication is sent asynchronously if the connection state has been
/// changed. This happens also while an ETS download.
///
/// @param[in] bConnected TRUE if connected, else FALSE
///
void App_HandleConnectInd(bool_t bConnected)
{
	m_bConnected = bConnected;													// Store current connection state
}

/// Handle the GetServerItem.Res data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] nSiId Current item ID from telegram
/// @param[in] nSiLength Length of current byte data from telegram
/// @param[in] pData Pointer to byte data from telegram
///
void App_HandleGetServerItemRes(
uint16_t nSiId,	uint8_t nSiLength, uint8_t* pData)
{
	if(m_eState != RUNNING)														// Are we waiting for any server items?
	{
		switch(nSiId)
		{
			case ID_PROG_MODE:													// Is it programming mode?
			AppProgMode_HandleServerItemResAndInd(nSiId, nSiLength, pData);	// Call actuator handler
			m_eState = REQUEST_DOWNLOAD_COUNTER;							// Go to next state
			break;

			case ID_DOWNLOAD_COUNTER:											// Is it download counter?
			m_nDownloadCounter = BUILD_WORD(pData[0], pData[1]);			// Store download counter
			m_eState = REQUEST_PARAMETERS;									// Go to next state
			PB_REQ =1;
// 			EXTFLASH_erase(PB_REQ,1023 );
// 			EXTFLASH_erase(1024,1024 );
// 			EXTFLASH_erase(2048,1024 );
// 			EXTFLASH_erase(3072,1024 );
// 			EXTFLASH_erase(4096,1024 );

			PARAMETER_READ_COUNT=0;;

			break;
		}
	}
}

/// Handle the SetServerItem.Res data.
///
void App_HandleSetServerItemRes(void)
{
	// Implement this if you want to evaluate the response
	// of a SetServerItem.Req (BAOS_SET_SRV_ITEM_REQ)
}

/// Handle the GetDatapointDescription.Res data.
///
/// A BAOS telegram can hold more than one data. This functions is called
/// for every single value (data point) in a telegram array.
///
/// @param[in] nCurrentDatapoint Current data point
/// @param[in] nDpValueLength Current length from telegram
/// @param[in] nDpConfigFlags Current configuration flags from telegram
///
void App_HandleGetDatapointDescriptionRes(
uint16_t nCurrentDatapoint, uint8_t nDpValueLength, uint8_t nDpConfigFlags)
{
	// Implement this if you want to evaluate the response
	// of a GetDatapointDescription.Req (BAOS_GET_DP_DESCR_REQ)
}

/// Handle the GetDescriptionString.Res data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] strDpDescription Pointer to current string from telegram
/// @param[in] nDpDescriptionLength Length of current string from telegram
///
void App_HandleGetDescriptionStringRes(
uint8_t* strDpDescription, uint16_t nDpDescriptionLength)
{
	// Implement this if you want to evaluate the response
	// of a GetDescriptionString.Req (BAOS_GET_DESCR_STR_REQ)
}

/// Handle the GetDatapointValue.Res data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] nDpId Current data point ID from telegram
/// @param[in] nDpState Current data point state from telegram
/// @param[in] nDpLength Current data point length from telegram
/// @param[in] pData Pointer to byte data from telegram
///
void App_HandleGetDatapointValueRes(
uint16_t nDpId, uint8_t nDpState,
uint8_t nDpLength, uint8_t* pData)
{
	// Implement this if you want to evaluate the response
	// of a GetDatapointValue.Req (BAOS_GET_DP_VALUE_REQ)
}

/// Handle the DatapointValue.Ind data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] nDpId Current data point ID from telegram
/// @param[in] nDpState Current data point state from telegram
/// @param[in] nDpLength Current data point length from telegram
/// @param[in] pData Pointer to byte data from telegram
///
void App_HandleDatapointValueInd(
uint16_t nDpId, uint8_t nDpState,
uint8_t nDpLength, uint8_t* pData)
{
	
	if (nDpId >= 1 && nDpId <= 1000)
	{
		AppActuator_HandleDatapointValueInd(nDpId, nDpState, nDpLength, pData);		// Call actuators call back
	}
	
}

/// Handle the ServerItem.Ind data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] nSiId Current server item ID from telegram
/// @param[in] nSiLength Current server item length from telegram
/// @param[in] pData Pointer to byte data from telegram
///
void App_HandleServerItemInd(
uint16_t nSiId, uint8_t nSiLength, uint8_t* pData)
{
	AppProgMode_HandleServerItemResAndInd(nSiId, nSiLength, pData);				// Call programming mode handler
}

/// Handle the SetDatapointValue.Res data.
///
void App_HandleSetDatapointValueRes(void)
{
	// Implement this if you want to evaluate the response
	// of a SetDatapointValue.Req (BAOS_SET_DP_VALUE_REQ)
}

/// Handle the GetParameterByte.Res data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] nIndex Current byte number (channel)
/// @param[in] nByte Current byte value from telegram
///



//---------------------------------- Read Parameter byte------------------------------------//

void App_HandleGetParameterByteRes(uint16_t nIndex, uint8_t nByte)
{
	AppActuator_HandleGetParameterByteRes(nIndex, nByte);						// Call actuator handler

		   	m_eState = RUNNING;	
	if((nIndex == PB_MAX) &&	( PB_REQ < 7960)	)						// If we have got the last parameter
	{
	//	INDC_ON;
		
		//EXTFLASH_write(PB_REQ,240 , Parameter_Byte_Data);
		PARAMETER_READ_COUNT=0;
		PB_REQ+=PB_MAX;
		m_eState = REQUEST_PARAMETERS;											// Go to next state
		
	}
	// 	else if (PB_REQ >= 4320)
	// 	{
	// 		uint8_t dass=1;
	// 		dass=5;
	// 	}

	else if((nIndex == PB_MAX) && (PB_REQ > 7960))						// If we have got the last parameter
	{
		
		


		//----------------------------- Read Parameter byte----------------------------------------------//


		//---------------------------------Read Parameter byte after power off(Recover_Data_code)--------------------------//

		uint8_t Empty_buff[10] = {0};


		if (Empty_buff[9]==0)
		{
			
			for (int i = 0 ;i<10;i++){
				Empty_buff[i] = 255;
			}




		}

		m_eState = RUNNING;														// Go to next state

	}


}

/// Handle error code.
///
/// @param[in] nErrorCode KNX BAOS ObjectServer error code
///
void App_HandleError(uint8_t nErrorCode)
{
	//	0 = No error (this function gets never called with that)
	//	1 = Internal error
	//	2 = No item found
	//	3 = Buffer is too small
	//	4 = Item is not writable
	//	5 = Service is not supported
	//	6 = Bad service parameter
	//	7 = Wrong data point ID
	//	8 = Bad data point command
	//	9 = Bad length of the data point value
	// 10 = Message inconsistent
	// 11 = Object server is busy

	DBG_SUSPEND;																// Stop here for debugging
}

// End of App.c
