////////////////////////////////////////////////////////////////////////////////
//
// File: AppActuator.c
//
// Demo Application for BAOS Development Board, actuator part.
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "KnxBaos.h"
#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings
#include "hal_delay.h"
#include "App.h"																// Application related defines/interface
#include "AppBoard.h"															// General defines for port IO
#include "KnxBaos.h"			   												// Object server protocol to communicate with BAOS
//#include "AppDim.h"																// Functions for Led-dimming/switching
#include "AppActuator.h"
  #include "DPS_DALI.h"
#include "DALI_PB.h"

////////////////////////////////////////////////////////////////////////////////
// Local members
////////////////////////////////////////////////////////////////////////////////
void AppScene_Handle(uint16_t PB_Start);
// #include "IncFile1.h"
// /// Usage of data points #3, #4 and #5 (actuators)
static LightType_t m_nLightType = LT_DISABLED;
uint16_t Parameter_Byte_Index;

uint8_t Parameter_Byte_Data[PB_MAX_LIMIT];

uint8_t nValue;


/// Initialize the actuator at startup.
///
void AppActuator_Init(void)
{
	//AppLedPwm_Init();															// Set LED-configurations
	//AppDim_Init();																// Initialize dimming process
}

/// Actuator application entry which is executed in the main loop.
///
/// This handles the dimming process for the LED.
///
void AppActuator_Run(void)
{
	// 	if (PB_READ_COMPLETE==TRUE)
	// 	{
	//
	//
	// 	EXTFLASH_waitReady();
	// 	EXTFLASH_read(51, 2671, &AC_SELECT.AC_AC[0]);
	// 	EXTFLASH_waitReady();
	// 	EXTFLASH_read(2751, 752, &MACROS_CALL.macrosss[0]);
	// 	EXTFLASH_waitReady();
	// 	EXTFLASH_read(3612, 4000, &UNIVERSAL_IR_CODE_CALL.UNIVERSAL_IR_CODE_NO_PB[0]);
	// 	PB_READ_COMPLETE =FALSE;
	//
	// 	}
	for (uint8_t i = 0 ; i<=95 ; i ++)
	{
		AppDim_Process(i);
	}
	// Handle dimming LED
}



unsigned char calculateChecksum(unsigned char *data, uint16_t length) {
	unsigned char checksum = 0;
	for (size_t i = 0; i < length; i++) {
		checksum ^= data[i];
	}
	return checksum;
}




extern	void AppDali_HandleDatapointValueInd(uint16_t nDpId, uint8_t nDpState,uint8_t nDpLength, uint8_t* pData)   ;
 extern void	AppDali_Handle_DPS(uint16_t nDpId, uint8_t nDpState,uint8_t nDpLength, uint8_t* pData)	 ;

void AppActuator_HandleDatapointValueInd(uint16_t nDpId, uint8_t nDpState,uint8_t nDpLength, uint8_t* pData)
{
	

	
	switch(nDpId)
	{
		case RX_OBJECT:
		
		
			AppDali_Handle_DPS( nDpId,  nDpState, nDpLength,  pData);
		
		
// 		F1=KnxTm_GetTimeMs()+3000;
// 		
// 		F2=*pData;
// 		remaining = TOTAL_LENGTH - receivedCount;
// 		copySize = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
// 		
// 		// memset(destinationArray, 0, 1000);
// 		memcpy(destinationArray + receivedCount, pData, copySize);
// 		receivedCount += copySize;
// 		data_length= destinationArray[2] | destinationArray[3]<<8;
// 
// 		if (destinationArray[ 7+data_length + 1 ] == 0xCC)
// 		{
// 			end_frame= TRUE;
// 		}
// 		else end_frame = FALSE;
// 
// 		
// 		if (destinationArray[0]==0xEE && destinationArray[1]==0xFF && destinationArray[2]==0xEE)
// 		{
// 			//chip_erase();
// 			
// 			TT[0]= 0xEE;
// 			TT[1]= 0xFF;
// 			TT[2]= 0xEE;
// 			KnxBaos_SendValue(TX_OBJECT, DP_CMD_SET_SEND_VAL, 14, &TT[0]);
// 			TT[0]= 0x00;
// 			TT[1]= 0x00;
// 			TT[2]= 0x00;
// 		}
		
		
		break;
		
		
		
		
	}
	
	
	AppDali_HandleDatapointValueInd( nDpId,  nDpState, nDpLength,  pData);

}






uint16_t PARAMETER_READ_COUNT=0;
/// Handle the GetParameterByte.Res data.
///
/// A KNX telegram can hold more than one data. This functions gets called for
/// every single data in a telegram array.
///
/// @param[in] nIndex Current byte number (channel)
/// @param[in] nByte Current byte value from telegram
///
void AppActuator_HandleGetParameterByteRes(
uint16_t nIndex, uint8_t nByte)
{
	// 		EXTFLASH_waitReady();
	// 	EXTFLASH_write(PARAMETER_READ_COUNT,1 , nByte);
	//	uint8_t data[2];
	//			EXTFLASH_waitReady();
	//
	//		EXTFLASH_read(PARAMETER_READ_COUNT, 2, &data[0]
	Parameter_Byte_Data[PARAMETER_READ_COUNT] = nByte;
	
	PARAMETER_READ_COUNT++;
	
	// 		switch(nIndex)																// Parameter byte number
	// 		{
	// 			case PB_SWITCH_TYPE:														// Set data point #3, #4 and #5
	// 			m_nLightType = (LightType_t)nByte;
	// 			break;
	//
	// 			default:																// Ignore all other parameter bytes
	// 			break;
	// 		}

}
