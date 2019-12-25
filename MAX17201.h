/** \file max17201.h ******************************************************
 *
 *             Project: MAX17201
 *            Filename: MAX17201.h
 *         Description: Header file for the Main application module MAX17201 example program.
 *
 *    
 *\n                                        
 *\n                    		
 *
 *  --------------------------------------------------------------------
 *
 *  This code follows the following naming conventions:
 *
 *\n    char                    ch_pmod_value
 *\n    char (array)            s_pmod_string[16]
 *\n    float                  	f_pmod_value
 *\n    int                    	n_pmod_value
 *\n    int (array)             an_pmod_value[16]
 *\n    uint16_t                u_pmod_value
 *\n    uint16_t (array)        au_pmod_value[16]
 *\n    uint8_t                 uch_pmod_value
 *\n    uint8_t (array)         auch_pmod_buffer[16]
 *\n    unsigned int          	un_pmod_value
 *\n    int *                   pun_pmod_value
 *
 *  ------------------------------------------------------------------------- */
/*
 * Copyright (C) 2012 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED PRODUCTS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated Products
 * shall not be used except as stated in the Maxim Integrated Products
 * Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products retains all ownership rights.
 *
 ***************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _H
#define _H
#include "stm32f10x.h"
//#include "../iic/iic.h"
//#include "../IIC/I2C.h"

/** 
  * @brief  Algorithm Output Registers
  */

/** 
  * @brief  Algorithm Output Registers [Address]
  */

#define AvCap                       ((uint16_t)0x1F)
#define AvSOC                       ((uint16_t)0x0E)
#define RepCap                      ((uint16_t)0x05)
#define RepSOC                      ((uint16_t)0x06)
#define Temp                        ((uint16_t)0x08)
#define FullCap                     ((uint16_t)0x10)
#define FullCapRep                  ((uint16_t)0x35)
#define FullCapNom                  ((uint16_t)0x23)
#define TTE                         ((uint16_t)0x11)
#define TTF                         ((uint16_t)0x20)
#define Age                         ((uint16_t)0x07)
#define Cycles                      ((uint16_t)0x17)
#define TimerH                      ((uint16_t)0xBE)
#define AgeForecast                 ((uint16_t)0xB9)
#define RCell                       ((uint16_t)0x14)
#define VRipple                     ((uint16_t)0xBC)


/** 
  * @ModelGauge m5 Learned information [Address]
  */

#define FullCapNom                  ((uint16_t)0x23)//??
#define Cycles					          	((uint16_t)0x17)//??
#define TimerH					 				    ((uint16_t)0xBE)//??

#define nQRTable00									((uint16_t)0x01A0)//??
#define nQRTable01									((uint16_t)0x01A1)//??
#define nQRTable02									((uint16_t)0x01A2)//??
#define nQRTable03									((uint16_t)0x01A3)//??

#define IAvgEmpty                   ((uint8_t)0x36)//??
#define RComp0				 							((uint16_t)0x38)//??
#define TempCo				 				    	((uint16_t)0x39)//??
#define FullCapRep									((uint16_t)0x35)//??


/** 
  * @ModelGauge m5 Algorithm Configuration [Address]
  */

#define nRippleCfg                  ((uint16_t)0x01B1)//??
#define nConvgCfg                   ((uint16_t)0x01B7)//??
#define nAgeFcCfg                   ((uint16_t)0x01D2)//??
#define nLearnCfg                   ((uint16_t)0x019F)//??
#define nRelaxCfg                   ((uint16_t)0x01B6)//??
#define nMiscCfg                    ((uint16_t)0x01B2)//??

#define AtRate                      ((uint16_t)0x0004)//??

#define nPackCfg                    ((uint16_t)0x01B5)//??
#define nRSense                     ((uint16_t)0x01CF)//No Nonvolatile





/** 
  * @ModelGauge m5 Analog input [Address]
  */

#define VCell                      ((uint16_t)0x0009)//??
#define Current                    ((uint16_t)0x000A)//??
#define AvgVCell                   ((uint16_t)0x0019)//??
#define AvgCurrent                 ((uint16_t)0x000B)//??

#define AvgTemp1                   ((uint16_t)0x0037)//0x0137
#define AvgTemp2                   ((uint16_t)0x0039)//0x0139
#define AvgIntTemp                 ((uint16_t)0x0038)//0x0138


/** 
  * @ModelGauge m5 Application Specific [Address]
  */

#define nDesignVoltage             ((uint16_t)0x01D3)//??
#define nDesignCap                 ((uint16_t)0x01B3)//??
#define nIChgTerm                  ((uint16_t)0x019C)//??

#define Status                     ((uint16_t)0x0000)//??

/** 
  * @ModelGauge m5 Algorithm Model Registers [Address]
  */

#define nXTable0                  ((uint16_t)0x0180)//??
#define nXTable1                  ((uint16_t)0x0181)//??
#define nXTable2                  ((uint16_t)0x0182)//??
#define nXTable3                  ((uint16_t)0x0183)//??
#define nXTable4                  ((uint16_t)0x0184)//??
#define nXTable5                  ((uint16_t)0x0185)//??
#define nXTable6                  ((uint16_t)0x0186)//??
#define nXTable7                  ((uint16_t)0x0187)//??
#define nXTable8                  ((uint16_t)0x0188)//??
#define nXTable9                  ((uint16_t)0x0189)//??
#define nXTable10                 ((uint16_t)0x018A)//??
#define nXTable11                 ((uint16_t)0x018B)//??

#define nOCVTable0                ((uint16_t)0x0190)//??
#define nOCVTable1                ((uint16_t)0x0191)//??
#define nOCVTable2                ((uint16_t)0x0192)//??
#define nOCVTable3                ((uint16_t)0x0193)//??
#define nOCVTable4                ((uint16_t)0x0194)//??
#define nOCVTable5                ((uint16_t)0x0195)//??
#define nOCVTable6                ((uint16_t)0x0196)//??
#define nOCVTable7                ((uint16_t)0x0197)//??
#define nOCVTable8                ((uint16_t)0x0198)//??
#define nOCVTable9                ((uint16_t)0x0199)//??
#define nOCVTable10               ((uint16_t)0x019A)//??
#define nOCVTable11               ((uint16_t)0x019B)//??

#define nQRTable00                ((uint16_t)0x01A0)//??
#define nQRTable10                ((uint16_t)0x01A1)//??
#define nQRTable20                ((uint16_t)0x01A2)//??
#define nQRTable30                ((uint16_t)0x01A3)//??

#define nFullSOCThr               ((uint16_t)0x01C6)//??
#define nVEmpty                   ((uint16_t)0x019E)//??
#define nDesignCap                ((uint16_t)0x01B3)//??
#define nRFastVShdn               ((uint16_t)0x01D5)//??
#define nIChgTerm                 ((uint16_t)0x019C)//??
#define nRComp0                   ((uint16_t)0x01A6)//??
#define nTempCo                   ((uint16_t)0x01A7)//??
#define nIAvgEmpty                ((uint16_t)0x01A8)//??
#define nIAvgEmpty                ((uint16_t)0x01A8)//??

/** 
  * @[Value] ModelGauge m5 package configuration 
  */

#define En_CxEn                  ((uint16_t)0x0100)//Enable Cellx voltage mesurement
#define En_BtEn                  ((uint16_t)0x0200)//Enable the voltage measure of BATT Channel,only if the CxEn=0 and ChEn=0
#define En_ChEn                  ((uint16_t)0x0400)//Enable the voltage measure of Cell1 Cell2 and VBATT pins

#define Sl_Addr                   ((uint8_t)0x6C)
#define Md_Addr                   ((uint8_t)0x16)
#define Md_Top                    ((uint8_t)0x80)
/*----------------------------------------------------------
nPackCfg Setting--1
The next four pin setting shows as follows:
FGT A2EN  A1EN TDEN => FUEL Gauge Input/Temp Register
0000  => Internal/DieTemp
1010  => AIN1/Temp1
0100  => AIN2/Temp2
0101  => AIN2/Temp2
0011  => Inter/Die
1011  => AIN1/Temp1
1110  => AIN1/Temp1
0110  => Temp1and Temp2
1111  =>AIN1/Temp1
0111  =>Temp1 and Temp2 

Other configurations are invalid
---------------------------------------------------------------  */
#define Pck_En_TdEn                  ((uint16_t)0x0800)//Die temp enable
#define Pck_En_A1En                  ((uint16_t)0x1000)//AIN1 Enable
#define Pck_En_A2En                  ((uint16_t)0x2000)//AIN2 Enable
#define Pck_En_FGT                   ((uint16_t)0x8000)//Fuel Gauge temprature input selet in combination with others

#define CommStat        	  		 		 ((uint16_t)0x0061)//??

/*----------------------------------------------------------
nPackCfg Setting --2
Channel Enable 
---------------------------------------------------------------  */
#define Pck_2S_4S_NoBal                ((uint16_t)0x0200)//BtEN on enable voltage measurements of the VBATT pin
#define Pck_2S_4S_Bal                  ((uint16_t)0x0400)//ChEn Set to 1 to enable voltage measurements of the CELL1, CELL2, and VBATT pins. 
#define Pck_4S_More                    ((uint16_t)0x0100)//CELLx Channel Enable. Set to 1 to enable voltage measurements of the CELLx pin.


/*----------------------------------------------------------
nPackCfg Setting --3
Channel Enable 
Balancing Threshold = 1.25mV*2^BALCFG
---------------------------------------------------------------  */
#define  Pck_BALCFG_OFF                ((uint16_t)0x0000)// cell balancing is disabled.
#define  Pck_BALCFG_25                 ((uint16_t)0x0020)//          
#define  Pck_BALCFG_50                 ((uint16_t)0x0040)//  
#define  Pck_BALCFG_100                ((uint16_t)0x0060)// 
#define  Pck_BALCFG_200                ((uint16_t)0x0080)//
#define  Pck_BALCFG_400                ((uint16_t)0x00A0)//
#define  Pck_BALCFG_800                ((uint16_t)0x00C0)//
#define  Pck_BALCFG_1600               ((uint16_t)0x00E0)//

/*----------------------------------------------------------
 nPackCfg Setting --4
 number of cells in range 1~15(0x0~0xF)

 EZ mode settings
---------------------------------------------------------------  */
#define  Pck_N_Cells      		         ((uint16_t)0x0005)// 5Cells

/*----------------------------------------------------------
 nPackCfg Setting --5 
 Quick setting 
 skip the steps above
 there's four quick settings optional
 Name with an "N" needs to combine with Pck_N_Cells
---------------------------------------------------------------  */
#define  Pck_Q_1S                      ((uint16_t)0x1C01)// 1S Cell Pack
#define  Pck_Q_2_4_ChnM_N              ((uint16_t)0x3A00)// 2S–4S Cell Pack w/o Channel Measurements 
#define  Pck_High_Cell_N               ((uint16_t)0x3900)//High-Cell Count Pack
#define  Pck_2S_ChnM                    ((uint16_t)0x3C62)//2S Cell Pack with Channel Measurements
#define  Pck_3S_ChnM                    ((uint16_t)0x3C63)//3S Cell Pack with Channel Measurements 

typedef struct 
{
  uint8_t Cell_Num;           /*LSB 4bits of PackCgf to set number of cells  */
  
  uint8_t Bal_Cgf;           /*Bit 7~ bit5 of PackCgf to set Balance threhold, default value is "0"--balancing disabled*/
  
  uint16_t PackCon;            /* Other configuration of Package*/
  
}Pack_InitStruct;


/** 
  * @[Value] Smart Battery Compliant Operation
  */
  
#define En_TdEn                  ((uint16_t)0x0800)//Die temp enable

  
typedef struct
{
  uint16_t Rev_Value;                      /*!< Value received */

  uint8_t  Rev_CRC;                       /*!< CRC received*/

}SBS_RevWord;

typedef struct
{
  uint8_t  Size;
  
  uint8_t  Data0;                      /*!< Value received */
  
  uint8_t  Data1;                      /*!< Value received */
  
  uint8_t  Data2;                      /*!< Value received */

  uint8_t  Data3;                      /*!< Value received */

  uint8_t  Rev_CRC;                       /*!< CRC received*/

}SBS_RevBlock;

typedef struct
{
	uint16_t PckCf; //configuration 
	int      Rsen;  //value of Rsense milliohm
	float    Vemp;  //(V) Empty voltage per cell
	int      DesCap;// cell size mAh
	uint16_t CellNo;// cell number
}EZ_setting;



/* Public */
void WriteRegistor(uint16_t reg, uint16_t value);
uint16_t ReadRegistor (uint16_t reg);
void WriteWordSBS(uint8_t reg, uint16_t value, uint8_t EnCrc);
uint16_t ReadWordSBS(uint8_t reg, uint8_t EnCrc); 
int  WriteVerifyReg( uint16_t reg, uint16_t value);
//uint8_t m_load(uint8_t name, uint8_t value, int con,uint8_t flag);
void WriteModel(uint8_t* pbuffer);
void Package_init(Pack_InitStruct Pack);
void max17205_init_chip(float ChargeVolt);
int WriteVerifyModel(uint8_t* pbuffer);
void Max17205_Reset_Chip(void);
void Max17205_Load_NV(void);
int Max17205_EZ_Mode(EZ_setting cfg);
uint16_t Max17205_RemainNV_Times(void);
#endif
