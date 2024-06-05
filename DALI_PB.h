/*
* DALI_PB.h
*
* Created: 24-04-2024 11:31:00 AM
*  Author: ADMIN
*/


#ifndef DALI_PB_H_
#define DALI_PB_H_

#include "StdDefs.h"
#include <stdint.h> // Include the stdint.h header file



 //extern Dali dali;



enum {
	HIGH=0,
	MID,
	LOW
	};



typedef struct{
	bool_t ASSIGNED;
	uint8_t	ACTUAL_DIM_LEVEL	;
	uint8_t	POWER_ON_LEVEL	;
	uint8_t	SYSTEM_FAILURE_LEVEL	;
	uint8_t	MIN_LEVEL	;
	uint8_t	MAX_LEVEL	;
	uint8_t	FADE_RATE	;
	uint8_t	FADE_TIME	;
	uint8_t	SHORT_ADDRESS	;
	uint8_t	SEARCH_ADDRESS[3]	;
	uint8_t	RANDOM_ADDRESS[3]	;
	bool_t GROUP_0;
	bool_t GROUP_1;
	bool_t GROUP_2;
	bool_t GROUP_3;
	bool_t GROUP_4;
	bool_t GROUP_5;
	bool_t GROUP_6;
	bool_t GROUP_7;
	bool_t GROUP_8;
	bool_t GROUP_9;
	bool_t GROUP_10;
	bool_t GROUP_11;
	bool_t GROUP_12;
	bool_t GROUP_13;
	bool_t GROUP_14;
	bool_t GROUP_15;
	uint8_t	GROUP_0_7	;
	uint8_t	GROUP_8_15	;
	uint8_t	SCENE[16]	;
	uint8_t	STATUS_INFORMATION	;
	uint8_t	VERSION_NUMBER	;
	uint8_t	DEVICE_TYPE	;
	uint8_t	PHYSICAL_MIN_LEVEL	;
}BALLAST;




union DALI_PARAMTERs{
	
	uint8_t DALI_PB_ARRAY[500];  //store status of AC
	BALLAST  LAMP_NO[64];		//store data of AC

};


extern union DALI_PARAMTERs DALI_PARAMTER	 ;


typedef struct
{
	uint16_t	TEMPORARY_x_COORDINATE	;
	uint16_t	REPORT_x_COORDINATE	;
	uint16_t	x_COORDINATE	;
	uint16_t	TEMPORARY_COLOUR_TEMPERATURE_Tc	;
	uint16_t	REPORT_COLOUR_TEMPERATURE_Tc	;
	uint16_t	COLOUR_TEMPERATURE_Tc	;
	uint16_t	COLOUR_TEMPERATURE_Tc_COOLEST	;
	uint16_t	COLOUR_TEMPERATURE_Tc_WARMEST	;
	uint16_t	COLOUR_TEMPERATURE_Tc_PHYSICAL_COOLEST	;
	uint16_t	COLOUR_TEMPERATURE_Tc_PHYSICAL_WARMEST	;
	uint16_t	TEMPORARY_PRIMARY_N_DIMLEVEL	;
	uint16_t	REPORT_PRIMARY_N_DIMLEVEL	;
	uint16_t	PRIMARY_N_DIMLEVEL	;
	uint16_t	x_COORDINATE_PRIMARY_N	;
	uint16_t	y_COORDINATE_PRIMARY_N	;
	uint16_t	TY_PRIMARY_N	;
	uint8_t		TEMPORARY_RED_DIMLEVEL	;
	uint8_t		REPORT_RED_DIMLEVEL	;
	uint8_t		RED_DIMLEVEL	;
	uint8_t		TEMPORARY_GREEN_DIMLEVEL	;
	uint8_t		REPORT_GREEN_DIMLEVEL	;
	uint8_t		GREEN_DIMLEVEL	;
	uint8_t		TEMPORARY_BLUE_DIMLEVEL	;
	uint8_t	REPORT_BLUE_DIMLEVEL	;
	uint8_t	BLUE_DIMLEVEL	;
	uint8_t	TEMPORARY_WHITE_DIMLEVEL	;
	uint8_t	REPORT_WHITE_DIMLEVEL	;
	uint8_t	WHITE_DIMLEVEL	;
	uint8_t	TEMPORARY_AMBER_DIMLEVEL	;
	uint8_t	REPORT_AMBER_DIMLEVEL	;
	uint8_t	AMBER_DIMLEVEL	;
	uint8_t	TEMPORARY_FREECOLOUR_DIMLEVEL	;
	uint8_t	REPORT_FREECOLOUR_DIMLEVEL	;
	uint8_t	FREECOLOUR_DIMLEVEL	;
	uint8_t	TEMPORARY_RGBWAF_CONTROL	;
	uint8_t	REPORT_RGBWAF_CONTROL	;
	uint8_t	RGBWAF_CONTROL	;
	uint8_t	ASSIGNED_COLOUR	;
	uint8_t	TEMPORARY_COLOUR_TYPE	;
	uint8_t	REPORT_COLOUR_TYPE	;
	uint8_t	SCENE_0_15_COLOUR_TYPE	;
	uint16_t	SCENE_0_15_COLOUR_VALUE	;
	uint8_t	POWER_ON_COLOUR_TYPE	;
	uint16_t	POWER_ON_COLOUR_VALUE	;
	uint8_t	SYSTEM_FAILURE_COLOUR_TYPE	;
	uint16_t	SYSTEM_FAILURE_COLOUR_VALUE	;
	uint8_t	GEAR_FEATURES_STATUS	;
	uint8_t	COLOUR_STATUS	;
	uint8_t	COLOUR_TYPE_FEATURES	;

	
}DT8_BALLAST;



	
extern	const	uint8_t MaxValue ;
extern	const	int  MinValue ;
extern	const	uint8_t  DimMaxValue 	;
extern	const	uint8_t ByteMax 	;

				//   extern "C"{
	
	
				   
				//	   }
					   
					   
					   
		//	extern	void	
					 
					


union ColorRGBs
{
	uint16_t arr[3];
	uint8_t RED;
	uint8_t GREEN;
	uint8_t BLUE;

} ;




extern	 struct sbytes sbyte;
extern	union ColorRGBs ColorRGB;



enum  AddrType  {
	AddrTypeShort       = 0,
	AddrTypeGroup       = 1,
	AddrTypeBroadcast   = 2
};


enum E_DALIV2AddrType {
	
	eDALIV2CommandPriorityHigh     = 0,
	eDALIV2CommandPriorityMiddle   = 1,
	eDALIV2CommandPriorityLow     = 2
	
};



enum E_DALIV2DimmingCurve
{
	eDALIV2DimmingCurveLogarithmic   = 0,
	eDALIV2DimmingCurveLinear        = 1
};

typedef struct{
	
	uint8_t	nOldAddress           ;
	uint8_t	nNewAddress            ;
	uint8_t	nRandomAddressHigh    ;
	uint8_t	nRandomAddressMiddle  ;
	uint8_t	nRandomAddressLow     ;
	uint32_t nErrors            ;
	
}E_DALIV2CurrentAddressingState;



typedef  struct{
	
	uint32_t		nErrors             ;
	bool_t			bPresent             ;
	uint8_t			nActualLevel         ;
	uint8_t			nPowerOnLevel        ;
	uint8_t			nSystemFailureLevel  ;
	uint8_t			nMinLevel           ;
	uint8_t			nMaxLevel            ;
	uint8_t			nFadeRate          ;
	uint8_t			nFadeTime            ;
	uint32_t		nRandomAddress      ;
	uint16_t		nGroups              ;
	uint8_t			nSceneLevels[16]        ;
	uint8_t			nStatus              ;
	uint8_t			nMajorVersion        ;
	uint8_t			nMinorVersion        ;
	uint8_t			nDeviceType          ;
	uint8_t			nPhysicalMinLevel    ;
	
}ST_DALIV2DeviceSettings;


typedef struct{
	
	bool_t	 bShortAddressValid;
	uint8_t nNewShortAddress;
	
}ST_DALIV2SwapShortAddressList;



enum {
	BROADCASR,
	ECG,
	GROUP
};

//
// enum class ECG_DATA_POINT {
// 	ECG_ON_OFF =1	,
// 	ECG_ON_OFF_Status	,
// 	ECG_Relative_dimming_	,
// 	ECG_Absolute_Dimming	,
// 	ECG_Dimming_Status	,
// 	ECG_Lock_Unlock	,
// 	ECG_Alarm	,
// 	ECG_Scene	,
// 	ECG_Colour_temperature_4_Bit	,
// 	ECG_Colour_temperature_absolute	,
// 	ECG_Colour_temperature_status	,
// 	ECG_RGB_Colour	,
// 	ECG_RGB_Colour_Status
// };
//
// // 	enum class GROUP_DATA_POINT {
// // 		GROUP_ON_OFF 	,
// // 		GROUP_ON_OFF_Status	,
// // 		GROUP_Relative_dimming_	,
// // 		GROUP_Absolute_Dimming	,
// // 		GROUP_Dimming_Status	,
// // 		GROUP_Lock_Unlock	,
// // 		GROUP_Alarm	,
// // 		GROUP_Scene	,
// // 		GROUP_Colour_temperature_4_Bit	,
// // 		GROUP_Colour_temperature_absolute	,
// // 		GROUP_Colour_temperature_status	,
// // 		GROUP_RGB_Colour	,
// // 		GROUP_RGB_Colour_Status
// 	};
enum  BROADCAST_DATA_POINT {
	BROADCAST_ON_OFF =6 	,
		BROADCAST_Absolute_Dimming	,


};




enum
{
	SWITCH_OFF = 0,																// LED off
	SWITCH_ON  = 1																// LED on
};



#define DEFINE_DP(start, end,startvalue) \
start##_ON_OFF=startvalue, \
start##_ON_OFF_Status, \
start##_Relative_dimming, \
start##_Absolute_Dimming, \
start##_Dimming_Status, \
start##_Lock_Unlock, \
start##_Alarm, \
start##_Scene, \
start##_Colour_temperature_4_Bit, \
start##_Colour_temperature_absolute, \
start##_Colour_temperature_status, \
start##_RGB_Colour, \
start##_RGB_Colour_Status


typedef enum {
DEFINE_DP(GROUP_0,GROUP_0_END,21),
DEFINE_DP(GROUP_1,GROUP_1_END,36),
DEFINE_DP(GROUP_2,GROUP_2_END,51),
DEFINE_DP(GROUP_3,GROUP_3_END,66),
DEFINE_DP(GROUP_4,GROUP_4_END,81),
DEFINE_DP(GROUP_5,GROUP_5_END,96),
DEFINE_DP(GROUP_6,GROUP_6_END,111),
DEFINE_DP(GROUP_7,GROUP_7_END,126),
DEFINE_DP(GROUP_8,GROUP_8_END,141),
DEFINE_DP(GROUP_9,GROUP_9_END,156),
DEFINE_DP(GROUP_10,GROUP_10_END,171),
DEFINE_DP(GROUP_11,GROUP_11_END,186),
DEFINE_DP(GROUP_12,GROUP_12_END,201),
DEFINE_DP(GROUP_13,GROUP_13_END,216),
DEFINE_DP(GROUP_14,GROUP_14_END,231),
DEFINE_DP(GROUP_15,GROUP_15_END,246),


} ;



typedef enum {
	DEFINE_DP(ECG_0,ECG_0_END,282),
	DEFINE_DP(ECG_1,ECG_1_END,297),
	DEFINE_DP(ECG_2,ECG_2_END,312),
	DEFINE_DP(ECG_3,ECG_3_END,327),
	DEFINE_DP(ECG_4,ECG_4_END,342),
	DEFINE_DP(ECG_5,ECG_5_END,357),
	DEFINE_DP(ECG_6,ECG_6_END,372),
	DEFINE_DP(ECG_7,ECG_7_END,387),
	DEFINE_DP(ECG_8,ECG_8_END,402),
	DEFINE_DP(ECG_9,ECG_9_END,417),
	DEFINE_DP(ECG_10,ECG_10_END,432),
	DEFINE_DP(ECG_11,ECG_11_END,447),
	DEFINE_DP(ECG_12,ECG_12_END,462),
	DEFINE_DP(ECG_13,ECG_13_END,477),
	DEFINE_DP(ECG_14,ECG_14_END,492),
	DEFINE_DP(ECG_15,ECG_15_END,507),
	DEFINE_DP(ECG_16,ECG_16_END,522),
	DEFINE_DP(ECG_17,ECG_17_END,537),
	DEFINE_DP(ECG_18,ECG_18_END,552),
	DEFINE_DP(ECG_19,ECG_19_END,567),
	DEFINE_DP(ECG_20,ECG_20_END,582),
	DEFINE_DP(ECG_21,ECG_21_END,597),
	DEFINE_DP(ECG_22,ECG_22_END,612),
	DEFINE_DP(ECG_23,ECG_23_END,627),
	DEFINE_DP(ECG_24,ECG_24_END,642),
	DEFINE_DP(ECG_25,ECG_25_END,657),
	DEFINE_DP(ECG_26,ECG_26_END,672),
	DEFINE_DP(ECG_27,ECG_27_END,687),
	DEFINE_DP(ECG_28,ECG_28_END,702),
	DEFINE_DP(ECG_29,ECG_29_END,717),
	DEFINE_DP(ECG_30,ECG_30_END,732),
	DEFINE_DP(ECG_31,ECG_31_END,747),
	DEFINE_DP(ECG_32,ECG_32_END,762),
	DEFINE_DP(ECG_33,ECG_33_END,777),
	DEFINE_DP(ECG_34,ECG_34_END,792),
	DEFINE_DP(ECG_35,ECG_35_END,807),
	DEFINE_DP(ECG_36,ECG_36_END,822),
	DEFINE_DP(ECG_37,ECG_37_END,837),
	DEFINE_DP(ECG_38,ECG_38_END,852),
	DEFINE_DP(ECG_39,ECG_39_END,867),
	DEFINE_DP(ECG_40,ECG_40_END,882),
	DEFINE_DP(ECG_41,ECG_41_END,897),
	DEFINE_DP(ECG_42,ECG_42_END,912),
	DEFINE_DP(ECG_43,ECG_43_END,927),
	DEFINE_DP(ECG_44,ECG_44_END,942),
	DEFINE_DP(ECG_45,ECG_45_END,957),
	DEFINE_DP(ECG_46,ECG_46_END,972),
	DEFINE_DP(ECG_47,ECG_47_END,987),
	DEFINE_DP(ECG_48,ECG_48_END,1002),
	DEFINE_DP(ECG_49,ECG_49_END,1017),
	DEFINE_DP(ECG_50,ECG_50_END,1032),
	DEFINE_DP(ECG_51,ECG_51_END,1047),
	DEFINE_DP(ECG_52,ECG_52_END,1062),
	DEFINE_DP(ECG_53,ECG_53_END,1077),
	DEFINE_DP(ECG_54,ECG_54_END,1092),
	DEFINE_DP(ECG_55,ECG_55_END,1107),
	DEFINE_DP(ECG_56,ECG_56_END,1122),
	DEFINE_DP(ECG_57,ECG_57_END,1137),
	DEFINE_DP(ECG_58,ECG_58_END,1152),
	DEFINE_DP(ECG_59,ECG_59_END,1167),
	DEFINE_DP(ECG_60,ECG_60_END,1182),
	DEFINE_DP(ECG_61,ECG_61_END,1197),
	DEFINE_DP(ECG_62,ECG_62_END,1212),
	DEFINE_DP(ECG_63,ECG_63_END,1227),



	
} ;

// 		Group Group_DP;
// 		ECG ECG_DP;

	
	//extern void		 	 sets(uint8_t LEVEL,uint16_t address );
		 	
	 

extern void D_ON_OFF(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_ON_OFF_Status(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Relative_dimming(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Absolute_Dimming(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Dimming_Status(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Lock_Unlock(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_ECG1_Alarm(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Scene(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Colour_temperature__4_Bit(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Colour_temperature_absolute(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_Colour_temperature_status(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_RGB_Colour(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);
extern void D_RGB_Colour_Status(uint8_t nDpLength , uint8_t* pData,uint8_t ADDRESS,uint8_t AddrType);

#endif /* DALI_PB_H_ */