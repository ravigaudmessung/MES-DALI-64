/*
* DALI_POU.c
*
* Created: 24-04-2024 02:27:04 PM
*  Author: ADMIN
*/

#include "DALI_PB.h"
#include "qqqDALI.h"
Dali dali;


extern "C"{
	#include "KnxTm.h"
	#include "AppDim.h"
	#include "hal_delay.h"
};

//DATAPOINT_struct DALI_1;

union DALI_PARAMTERs DALI_PARAMTER	 ;
struct Dpid_Handler {
	
	void (*handler)(uint8_t, uint8_t*,uint8_t,uint8_t);
};




const	uint8_t MaxValue = 127;
const	int  MinValue = -128;
const	uint8_t  DimMaxValue  = 254	;
const	uint8_t ByteMax = 255	;




//  void sets(uint8_t LEVEL,uint16_t address ){
//
// 	 dali.set_level(LEVEL,address);
//  }

struct  Dpid_Handler handler_dali[] = {
	{		D_ON_OFF	},
	{		D_ON_OFF_Status	},
	{		D_Relative_dimming	},
	{		D_Absolute_Dimming	},
	{		D_Dimming_Status	},
	{		D_Lock_Unlock		},
	{		D_ECG1_Alarm	},
	{		D_Scene	},
	{		D_Colour_temperature__4_Bit	},
	{		D_Colour_temperature_absolute	},
	{		D_Colour_temperature_status	},
	{		D_RGB_Colour	},
	{		D_RGB_Colour_Status	}
	
};

double scaleValue(double value, double min_old, double max_old, double min_new, double max_new) {
	return min_new + ((value - min_old) / (max_old - min_old)) * (max_new - min_new);
}


double min_old = 0.0;
double max_old = 100.0;
double min_new = 1000.0;
double max_new = 20000.0;

// 		  DALI_PARAMTER.LAMP_NO[0].SHORT_ADDRESS =0;
//
// 		 DALI_PARAMTER.LAMP_NO[1].SHORT_ADDRESS =1;


void D_ON_OFF(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType)
{
	
	if (AddrType==AddrTypeShort)  {	  ADDRESS = DALI_PARAMTER.LAMP_NO[ADDRESS].SHORT_ADDRESS;}
	else if (AddrType==AddrTypeGroup){ADDRESS=ADDRESS+64;}
	else if (AddrType==AddrTypeBroadcast){ADDRESS=127;}

	if(nDpLength == 1)													// We expect 1 byte data length
	{
		if(*pData == SWITCH_ON)											// Switch on
		{
			if (DALI_PARAMTER.LAMP_NO[ADDRESS].ACTUAL_DIM_LEVEL == 0)
			{
				
				dali.set_level(254 ,ADDRESS)  ;
				DALI_PARAMTER.LAMP_NO[ADDRESS].ACTUAL_DIM_LEVEL =254;
				m_nCurrentBrightness[ADDRESS]=254;
				m_nStopBrightness[ADDRESS] =254;
				//	dali.set_ON(ADDRESS);
			}
			else
			{
				
				dali.set_level(DALI_PARAMTER.LAMP_NO[ADDRESS].ACTUAL_DIM_LEVEL ,ADDRESS)  ;
				DALI_PARAMTER.LAMP_NO[ADDRESS].ACTUAL_DIM_LEVEL =254;
				m_nCurrentBrightness[ADDRESS]=254;
				m_nStopBrightness[ADDRESS] =254;
				
			}
		}
		else if(*pData == SWITCH_OFF)									// Switch off
		{
			dali.set_level(0 ,ADDRESS)  ;
			m_nCurrentBrightness[ADDRESS]=0;
			m_nStopBrightness[ADDRESS] =0;
			//dali.set_OFF(ADDRESS);
			DALI_PARAMTER.LAMP_NO[ADDRESS].ACTUAL_DIM_LEVEL =0;
		}
		
	}

}

void AppDim_Process(uint16_t address)
{
	if(KnxTm_GetDelayMs(m_nTimeStamp[address]) > DIM_RAMP_TIME)							// If it is time for next step
	{
		if(m_nCurrentBrightness[address] != m_nStopBrightness[address])							// If we have to do dimming
		{

			if(m_nCurrentBrightness[address] < m_nStopBrightness[address])						// If we should dim up
			{
				m_nCurrentBrightness[address]++;
				if (m_nCurrentBrightness[address]>=253){	m_nCurrentBrightness[address] =254;}
				else	m_nCurrentBrightness[address]++;
				if (m_nCurrentBrightness[address]>=253){	m_nCurrentBrightness[address] =254;}
				else	m_nCurrentBrightness[address]++;											// Increase brightness (0..255)
				if (m_nCurrentBrightness[address]>=253){	m_nCurrentBrightness[address] =254;}
			}
			else																// Else we should dim down
			{
				
				m_nCurrentBrightness[address]--;
				
				
				
				if (m_nCurrentBrightness[address]<=1){	  m_nCurrentBrightness[address]	=0;}
				
				else	m_nCurrentBrightness[address]--;
				if (m_nCurrentBrightness[address]<=1){	  m_nCurrentBrightness[address]	=0;}
				else	m_nCurrentBrightness[address]--;											// Decrease brightness (0..255)
				if (m_nCurrentBrightness[address]<=1){	  m_nCurrentBrightness[address]	=0;}
			}
			
			if (m_nCurrentBrightness[address]>=254){	m_nCurrentBrightness[address] =254;}
			if (m_nCurrentBrightness[address]<=1){	  m_nCurrentBrightness[address]	=0;}
			
			//   double result = call_C_f(&instance_of_C, 2);
			if(address<=79){
				dali.set_level( m_nCurrentBrightness[address],address);
			}
			if(address>=80){
				
				double scaled_value = scaleValue(m_nCurrentBrightness[address], min_old, max_old, min_new, max_new);

				dali.setTC( address-16,1000000/scaled_value);
				dali.set_level( m_nCurrentBrightness[address-16],address-16);
				delay_ms(200);
			}
			

			//AppLedPwm_SetValue(m_nCurrentBrightness);							// Set new brightness value
		}



		
		m_nTimeStamp[address] = KnxTm_GetTimeMs();										// Set new time stamp
	}
}



void D_ON_OFF_Status(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){



}

void D_Relative_dimming(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){
	
	if (AddrType==AddrTypeShort)  {	  ADDRESS = DALI_PARAMTER.LAMP_NO[ADDRESS].SHORT_ADDRESS;}
	else if (AddrType==AddrTypeGroup){ADDRESS=ADDRESS+64;}
	else if (AddrType==AddrTypeBroadcast){ADDRESS=127;}
	
	if(nDpLength == 1)												// We expect 1 byte data length
	{
		AppDim_DimRelative(*pData, ADDRESS)	;
	}
	
}
void D_Absolute_Dimming(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){
	
	if (AddrType==AddrTypeShort)  {	  ADDRESS = DALI_PARAMTER.LAMP_NO[ADDRESS].SHORT_ADDRESS;}
	else if (AddrType==AddrTypeGroup){ADDRESS=ADDRESS+64;}
	else if (AddrType==AddrTypeBroadcast){ADDRESS=127;}
	
	if(nDpLength == 1)												// We expect 1 byte data length
	{
		if (*pData < ByteMax)
		{
			dali.set_level(*pData ,ADDRESS);
		}
		else	 dali.set_level(DimMaxValue ,ADDRESS);
		
	}

}


void D_Dimming_Status(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){



}
void D_Lock_Unlock(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){



}


void D_ECG1_Alarm(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){





}

int findNumberIndex(uint8_t arr[], int size, uint8_t* num) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == *num) {
			return i; // Return the index if number is found
		}
	}
	return -1; // Return -1 if number is not found
}


void D_Scene(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){
	
	if (AddrType=AddrTypeShort)  {	  ADDRESS = DALI_PARAMTER.LAMP_NO[ADDRESS].SHORT_ADDRESS;}
	else if (AddrType=AddrTypeGroup){ADDRESS+64;}
	else if (AddrType=AddrTypeBroadcast){ADDRESS=127;}
	
	if(nDpLength == 1)												// We expect 1 byte data length
	{
		
		int index = findNumberIndex(DALI_PARAMTER.LAMP_NO[ADDRESS].SCENE, 16, pData);  //PB

		if (index != -1) {
			
			dali.goto_scene(ADDRESS,index);
			
		}
	}
	
	
}

void D_Colour_temperature__4_Bit(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){
	
	
	 	if (AddrType==AddrTypeShort)  {	  ADDRESS = DALI_PARAMTER.LAMP_NO[ADDRESS].SHORT_ADDRESS;}
	 	else if (AddrType==AddrTypeGroup){ADDRESS=ADDRESS+64;}
	 	else if (AddrType==AddrTypeBroadcast){ADDRESS=127;}
	 	
	 	if(nDpLength == 1)												// We expect 1 byte data length
	 	{
		 	AppDim_DimRelative(*pData, ADDRESS+16)	;
	 	}


}

typedef union tc
{
	uint16_t temp_16bit;
	uint8_t temp_8bit[2];
	
} tc;

tc TC ;
void D_Colour_temperature_absolute(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){
	
	if (AddrType=AddrTypeShort)  {	  ADDRESS = DALI_PARAMTER.LAMP_NO[ADDRESS].SHORT_ADDRESS;}
	else if (AddrType=AddrTypeGroup){ADDRESS=ADDRESS+64;}
	else if (AddrType=AddrTypeBroadcast){ADDRESS=127;}
	

	
	if (nDpLength==2)
	{
		TC.temp_8bit[1]=*pData;
		pData++;
		TC.temp_8bit[0]=*pData;

		dali.setTC(ADDRESS,1000000/TC.temp_16bit);
		dali.set_level(100,ADDRESS);
		
	}
}


void D_Colour_temperature_status(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){
	
	
}



void D_RGB_Colour(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType){
	
	
	if (AddrType=AddrTypeShort)  {	  ADDRESS = DALI_PARAMTER.LAMP_NO[ADDRESS].SHORT_ADDRESS;}
	else if (AddrType=AddrTypeGroup){ADDRESS+64;}
	else if (AddrType=AddrTypeBroadcast){ADDRESS=127;}
	
	
	if (nDpLength==1)
	{
		ColorRGB.RED=*pData;
		pData++;
		ColorRGB.BLUE=*pData;
		pData++;
		ColorRGB.GREEN=*pData;

		dali.setRGB(ADDRESS,ColorRGB);
		
	}
}

void D_RGB_Colour_Status(uint8_t, uint8_t*,uint8_t,uint8_t){



}




extern ColorRGBs ColorRGB ;


extern "C"{
	
	void AppDali_HandleDatapointValueInd(uint16_t nDpId, uint8_t nDpState,uint8_t nDpLength, uint8_t* pData)   ;
	
	//   void AppDali_Handle_DPS(uint16_t nDpId, uint8_t nDpState,uint8_t nDpLength, uint8_t* pData);

}



void AppDali_HandleDatapointValueInd(uint16_t nDpId, uint8_t nDpState,uint8_t nDpLength, uint8_t* pData)
{
	
	//GRPUOP_NOS[1]::GROUP_Absolute_Dimming;
	
	int groupIndex = (nDpId - 1) / 100;

	groupIndex  =ECG_0_ON_OFF;
	if (nDpId >=ECG_0_ON_OFF && nDpId <= ECG_63_RGB_Colour_Status)
	{
		nDpId=nDpId-282;
		uint8_t LAMP_NO,LAMP_DP;
		
		LAMP_NO =  nDpId/15	<=63 ? 	nDpId/15 : 0 ;
		LAMP_DP =  nDpId%15	<=15 ? nDpId % 15: 0 ;
		
		handler_dali[LAMP_DP].handler(nDpLength, pData,LAMP_NO,AddrTypeShort);
		
	}
	else if (nDpId >=GROUP_0_ON_OFF && nDpId <=  GROUP_15_RGB_Colour_Status)
	{
		nDpId=nDpId-21;
		uint8_t GROUP_NO,GROUP_DP;
		GROUP_NO =  (nDpId) /15	<=16 ? 	nDpId/15 : 0 ;
		GROUP_DP =  (nDpId)%15	<=15 ? 	nDpId%15 : 0 ;
		handler_dali[GROUP_DP].handler(nDpLength, pData,GROUP_NO,AddrTypeGroup);
		
	}
	else if (nDpId >= BROADCAST_ON_OFF && nDpId <=  BROADCAST_Absolute_Dimming)
	{
		
		uint8_t	BROADCAST_DP;
		
		if (nDpId ==BROADCAST_ON_OFF )
		
		{
			BROADCAST_DP =0;
		}
		
		if (nDpId ==BROADCAST_Absolute_Dimming )
		
		{
			BROADCAST_DP =3;
		}
		
		//BROADCAST_DP =  (nDpId-5)/2	<=2 ? 	nDpId/2 : 0 ;
		handler_dali[BROADCAST_DP].handler(nDpLength, pData,255,AddrTypeBroadcast)	 ;
		
	}

}

//void Dpid_handle_ECG(uint8_t ECG_COMMAND,uint8_t nDpLength,uint8_t* pData,uint8_t ECG_NO) ;



