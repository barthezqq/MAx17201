/** \file max17055.c ******************************************************
 *
 *             Project: MAX17205
 *            Filename: max17205.c
 *         Description: This module contains the Main application for the MAX5715 example program.
 *
 *    Revision History:
 *\n                    10 May,2018    Rev 00.10 TTS    Initial release.
 *\n                                      
 *\n                    		
 *
 *  --------------------------------------------------------------------
 *
 *  This code follows the following naming conventions:
 *
 *\n    char                    ch_pmod_value
 *\n    char (array)             s_pmod_string[16]
 *\n    float                  	 f_pmod_value
 *\n    int                    	 n_pmod_value
 *\n    int (array)             an_pmod_value[16]
 *\n    uint16_t                u_pmod_value
 *\n    uint16_t (array)             au_pmod_value[16]
 *\n    uint8_t                     	uch_pmod_value
 *\n    uint8_t (array)              auch_pmod_buffer[16]
 *\n    unsigned int     	un_pmod_value
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




#include "../MAX17201_IIC/max17201_I2C.h"
#include "../gui/gui.h"
#include "max17201.h"
#include "../malloc/malloc.h"
#include "../usart/usart.h"

 

uint8_t  RvBuffer[3];
uint16_t RvTemp;


uint8_t s_addr=Sl_Addr;//device address with W/R 0
uint8_t m_addr=Md_Addr;//SBS slave address


void WriteRegistor(uint16_t reg, uint16_t value)
{
    uint8_t buffer[2];
	uint8_t i=2;
	uint8_t t_reg=0;
    buffer[0]=(uint8_t)(value&0xff);
    buffer[1]=(uint8_t)(value>>8&0xff);
	t_reg=(uint8_t)(reg&0xff);
  if (reg>0xff)
	{ 
     I2C_MultiByteWrite(Md_Addr,buffer,t_reg,i);
	}
		else
	{
     I2C_MultiByteWrite(Sl_Addr,buffer,t_reg,i);
	}
}



uint16_t ReadRegistor (uint16_t reg)
{
	  RvBuffer[0]=0;
	  RvBuffer[1]=0;	 
	if (reg>0xff)
	{ 
		uint8_t t_reg=0;
		t_reg=(uint8_t)(reg&0xff);
		I2C_MultiByteRead(Md_Addr,RvBuffer,t_reg,2);
        RvTemp =(uint16_t)(((uint16_t)RvBuffer[1] << 8) | RvBuffer[0]);
    return RvTemp;
	}
	else
	{
		I2C_MultiByteRead(Sl_Addr,RvBuffer,reg,2);
        RvTemp =(uint16_t)(((uint16_t)RvBuffer[1] << 8) | RvBuffer[0]);
		return RvTemp;
	}
}



void WriteWordSBS(uint8_t reg, uint16_t value, uint8_t EnCrc)// To disable CRC set EnCrc=0
{
	if (EnCrc==0)
	{	
        uint8_t buffer[3];
        uint8_t i=3;
        buffer[0]=reg;
        buffer[1]=(uint8_t)(value&0xff);
        buffer[2]=(uint8_t)(value>>8&0xff);
        I2C_MultiByteWrite(m_addr,buffer, reg,i);	
		}
	else
		{
        uint8_t buffer[4];
        uint8_t i=4;
        buffer[0]= reg;
        buffer[1]=(uint8_t)(value&0xff);
        buffer[2]=(uint8_t)(value>>8&0xff);
        buffer[3]=EnCrc;
        I2C_MultiByteWrite(m_addr,buffer, reg,i);
		}
		
	
}

uint16_t ReadWordSBS(uint8_t reg, uint8_t PEC)//Set PEC=1 will return PEC
{
    SBS_RevWord ReadBuf;
    RvBuffer[0]=0;
	RvBuffer[1]=0;
	RvBuffer[2]=0;
	if (PEC==0)
		{
          I2C_MultiByteRead(m_addr,RvBuffer,reg,2);
          ReadBuf.Rev_Value=(uint16_t)(((uint16_t)RvBuffer[1] << 8) | RvBuffer[0]);
          ReadBuf.Rev_CRC=0;
          return ReadBuf.Rev_Value;
		}
	else
		{
           I2C_MultiByteRead(m_addr,RvBuffer,reg,3);
           ReadBuf.Rev_Value =(uint16_t)(((uint16_t)RvBuffer[2] << 16) | ((uint16_t)RvBuffer[1] << 8));
           ReadBuf.Rev_CRC=(uint16_t)RvBuffer[0];
           return ReadBuf.Rev_Value;
		}
}


int WriteVerifyReg( uint16_t reg, uint16_t value)
{
	int flag;
	int retries = 0;	 
	uint16_t read_value=0;
  do {
       WriteRegistor (reg, value);
       delay_ms(1); //wait
       read_value = ReadRegistor (reg) ;
	   retries++;
     }
 while ((value != read_value) && retries<4);
 if (value == read_value)
	 flag=0;
 else
	 flag=1;
 return flag;
	
}


/*Block write is only available for 0x0180~0x01ff or ox0000~0x01ff
  to load model is writing 0x0180~0x01df8
 */

void LoadModel(uint8_t* pbuffer)
{
	uint16_t reg=0x180; //load model from 0x0180h~0x01DF
	uint16_t value;
	uint16_t read_value;
	int i=0;
	for(i=0;i<191;i=i+2)
	{
		if ((reg==0x1bc)||(reg==0x1bd)||(reg==0x1be)||(reg==0x1bf))//RomID is not reprogramable
		{
			reg++;
		}
		else
		{
		value=(uint16_t)((pbuffer[i]<<8)&0xff00)+(uint16_t)pbuffer[i+1];
		WriteRegistor (reg, value);
    delay_ms(1); //wait
    //read_value = ReadRegistor (reg);
		reg++;
	  }

	}
}


void WriteModel(uint8_t* pbuffer)
{
	uint8_t reg=0x80;
  I2C_MultiByteWrite(Md_Addr,pbuffer,reg,96);//load model from 0x0180h~0x01DF
}

//void Package_init(Pack_InitStruct Pack)
//{
//  uint16_t Pck_Temp=0x0000;
//     
//	 if (2<=Pack.Cell_Num<=4)
//	 	{
//	 	 Pck_Temp=(uint16_t)(Pck_Temp|En_BtEn);
//	 	}
//	 else if (Pack.Cell_Num>4)
//	 	Pck_Temp=(uint16_t)(Pck_Temp|En_CxEn);// 2S~4S setting Vs more than 4S setting
//	 else 
//	 	//printf ("Warnning! Cell Number out of range");

//	 Pck_Temp=(uint16_t)(Pck_Temp|(Pack.Cell_Num & 0x0f));//input cell number
//	 Pck_Temp=(uint16_t)(Pck_Temp|((Pack.Bal_Cgf<<1)&0x0f)<<4);//input Balance setting
//	 Pck_Temp|=Pack.PackCon;
//	 WriteVerifyReg(nPackCfg,Pck_Temp);
//	 
//		
//}
void Max17205_Reset_Chip()
{
	WriteRegistor(0x060, 0x000f);//POR IC
	delay_ms(200);
	WriteRegistor(0x0bb, 0x0001);//Reset firmware
	delay_ms(200);
}

void Max17205_Reset_Cnf()
{
	WriteRegistor(0x0bb, 0x0001);//Reset firmware
	delay_ms(200);
}

void Max17205_Load_NV()
{
	uint16_t temp=0;
	uint8_t finish=0;
	temp=ReadRegistor(Status);
	if ((temp&0x0002)==0x0002)
	{
		temp=temp&0xffffd;
		WriteRegistor(Status,temp);//clear POR 
	}
	else{}
		
	while(finish==0)
	{
			if((ReadRegistor(CommStat)&0x0002)==0)
			{
				do
					{
					temp=ReadRegistor(CommStat);
					temp=temp&0xfffb;// keep the status & Clear CommStat.NVError bit
					WriteRegistor(CommStat, temp);	//Clear CommStat.NVError bit
					WriteRegistor(0x060, 0xe904);	//initiate a block copy
					delay_ms(9900);
				}
				while((ReadRegistor(CommStat)&0x0004)==0x0004);
			  while((ReadRegistor(CommStat)&0x0002)==0x0002);
				finish=1;
			}
			else
			{
				finish=0;
			}
		}
	WriteRegistor(0x060, 0x000f);//POR IC
    delay_ms(100000);//tPOR=100ms

//	while((ReadRegistor(Status)&0x0002)==0x0002) //wait untile POR detected
//		{
//			delay_ms(1);
//		}
	temp=ReadRegistor(Status);
	temp=temp&0xffffd;
    WriteRegistor(Status,temp);//clear POR 
	WriteRegistor(0x0bb, 0x0001);//Reset firmware
	delay_ms(10000000);//tPOR=100ms
	
}

uint16_t Max17205_RemainNV_Times() //remaining configuration memory writes
{
    uint16_t rem_times=0;
	WriteRegistor(0x060, 0xE2FA);//Recall remaining configuration memory writes
    delay_ms(100000);//tRECALL=5ms *2
	rem_times=ReadRegistor (0x1ED);
	
 return rem_times;	
	 
}

		
		
int Max17205_EZ_Mode(EZ_setting cfg)
{
	uint16_t temp_cap,temp_cnf, temp_sens, temp_vem,temp;
	int retries;
	temp_cap=(uint16_t)(cfg.DesCap/2);
	temp_cnf=cfg.PckCf;
	temp_sens=(uint16_t)(cfg.Rsen*100);
	temp =ReadRegistor(nVEmpty); //read out the VE &VR
	temp=temp&0xf8;//protec VR
	temp_vem=((uint16_t)(cfg.Vemp*100)<<7)|temp;
	
	retries=WriteVerifyReg(nPackCfg,temp_cnf);
	retries=retries+WriteVerifyReg(nRSense,temp_sens);
	retries=retries+WriteVerifyReg(nVEmpty,temp_vem);
	retries=retries+WriteVerifyReg(nDesignCap,temp_cap);
	delay_ms(100);
  return retries;
}
	
	 	

