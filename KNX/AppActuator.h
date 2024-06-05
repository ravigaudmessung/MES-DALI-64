////////////////////////////////////////////////////////////////////////////////
//
// File: AppActuator.h Header File
//
// Header File for Demo Application, actuator part
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPACTUATOR__H___INCLUDED
#define APPACTUATOR__H___INCLUDED

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings




//#include "IncFile1.h"
enum PB_Scene_Tag
{
	SCENE_1		=	11,
	SCENE_2,	
	SCENE_3,
	SCENE_4,
	SCENE_5,
	SCENE_6,
	SCENE_7,
	SCENE_8,
	SCENE_9,
	SCENE_10,
	SCENE_MAX				
	
};




#define  PB_MAX_LIMIT	240		

extern uint16_t Parameter_Byte_Index;
extern uint8_t Parameter_Byte_Data[PB_MAX_LIMIT];





extern unsigned char calculateChecksum(unsigned char *data, uint16_t length);




  
////////////////////////////////////////////////////////////////////////////////
// Public function prototypes
////////////////////////////////////////////////////////////////////////////////


extern void AppActuator_Init(void);
extern void AppActuator_Run(void);
extern 	   	void AppDali_HandleDatapointValueInd(uint16_t nDpId, uint8_t nDpState,uint8_t nDpLength, uint8_t* pData)   ;

extern void   AppActuator_HandleDatapointValueInd(uint16_t nDpId, uint8_t nDpState, uint8_t nDpLength, uint8_t* pData);
extern void   AppActuator_HandleGetParameterByteRes(uint16_t nIndex, uint8_t nByte);

#endif /* APPACTUATOR__H___INCLUDED */

// End of AppActuator.h
