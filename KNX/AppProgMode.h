////////////////////////////////////////////////////////////////////////////////
//
// File: AppProgMode.h Header File
//
// Header File for Demo Application, programming mode handler
//
// (c) 2002-2016 WEINZIERL ENGINEERING GmbH
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPPROGMODE__H___INCLUDED
#define APPPROGMODE__H___INCLUDED

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "config.h"																// Global configuration
#include "StdDefs.h"															// General defines and project settings


////////////////////////////////////////////////////////////////////////////////
// Public function prototypes
////////////////////////////////////////////////////////////////////////////////

extern void   AppProgMode_Init(void);
extern void   AppProgMode_Run(void);

extern bool_t AppProgMode_IsProgMode(void);
extern void   AppProgMode_SetProgMode(bool_t bProgMode);
extern void   AppProgMode_HandleServerItemResAndInd(uint16_t nItemId, uint8_t nItemDataLength, uint8_t* pData);


#endif /* APPPROGMODE__H___INCLUDED */

// End of AppProgMode.h
