/** \file main.c******************************************************
 *
 *             Project: LVD_Gui_MAX17205
 *            Filename: main.c         
 *            Description: main run of project LVD_Gui_MAX17205
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

/*****************************************************************************   
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA 
*****************************************************************************    */

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "public.h"
#include "../APP/MAX17201_IIC/MAX17201_I2C.h"
#include "../APP/MAX17201/MAX17201.h"
#include "systick.h"
#include "../APP/gui/gui.h"
#include "../APP/led/led.h"
#include "../APP/key/key.h"
#include "../APP/printf/printf.h"
#include "../APP/touch/touch.h"
#include "../APP/flash/flash.h"
#include "../APP/system/system.h"
#include "../APP/usart/usart.h"
#include "math.h"
#include "stdlib.h"


//state defination
#define PAGE_OPEN 0x00  //Opengning page
#define PAGE_SET  0x01  //Setting Page
#define PAGE_MAIN  0x02 // Main display page
#define PAGE_SET_TOUCH  0x03 //Adjust Display Page


int count=0; //indicate the number of data transferred
uint8_t  RxV; // RxN is the 8 bit buffer to load model
int sta_flag;
typedef struct
{
 float Soc;  
 float RemCap;
 float CellVolt;
 float Cur;
 float TimeFull;
 float TimeEmpty;
 float AvgCur;
 float TempOut;
}Output; //Out put values

void sign_output(float value,uint8_t *v_buffer)// buffer need to be more than 11 number
{
	int i;
  int temp=0;//integer part
	long temp2=0;//fractional part
	long tempr=0; //round off 
	for(i=0;i<10;i++)
	{
		v_buffer[i]=0;
	}
  if (value<0)
	{
	temp=-value;
	temp2=((-value)*1000)-(temp*1000);
	tempr=((-value)*10000);
	v_buffer[0]='-';
	}
	else 
	{
		temp=value;
		temp2=(value*1000)-(temp*1000);
		tempr=value*10000;
		v_buffer[0]=' ';
	}
		
	  v_buffer[1]=temp/10000+0x30;
			if(v_buffer[1]==0x30)
		{
		 v_buffer[1]=' ';
		}
		
	  v_buffer[2]=temp%10000/1000+0x30;
			if((v_buffer[2]==0x30)&&(v_buffer[1]==' '))
		{
		 v_buffer[2]=' ';
		}
		v_buffer[3]=temp%1000/100+0x30;
			if((v_buffer[3]==0x30)&&(v_buffer[2]==' '))
		{
		 v_buffer[3]=' ';
		}		
		v_buffer[4]=temp%100/10+0x30;
			if((v_buffer[4]==0x30)&&(v_buffer[3]==' '))
		{
		 v_buffer[4]=' ';
		}
		v_buffer[5]=temp%10+0x30;		
		v_buffer[6]='.';
		v_buffer[7]=temp2/100+0x30;
		v_buffer[8]=(temp2%100)/10+0x30;
		if (tempr%10>4)
		v_buffer[9]=temp2%10+1+0x30;
		else
	  v_buffer[9]=temp2%10+0x30;	
		v_buffer[10]='\0';

}


void unsgn_output(float value,uint8_t *v_buffer)
{
	int i;
  int temp= value; //integer part
	long temp2;//fractional part
	long tempr;//round off 
	temp2=(value*1000)-(temp*1000);
	tempr=value*10000;
	for(i=0;i<10;i++)
	{
		v_buffer[i]=0;
	}
	
	v_buffer[0]=temp/1000+0x30;
	if(v_buffer[0]==0x30)
	{
	 v_buffer[0]=' ';
	}
	
	v_buffer[1]=(temp%1000)/100+0x30;
		if((v_buffer[0]==' ')&&(v_buffer[1]==0x30))
		{
	   v_buffer[1]=' ';
		}
		
	v_buffer[2]=(temp%100)/10+0x30;
		if((v_buffer[0]==' ')&&(v_buffer[1]==' ')&&(v_buffer[2]==0x30))
		{
				v_buffer[2]=' ';
		}
	v_buffer[3]=temp%10+0x30;
	v_buffer[4]='.';
	v_buffer[5]=temp2/100+0x30;
	v_buffer[6]=(temp2%100)/10+0x30;
		if(tempr%10>4)
				v_buffer[7]=temp2%10+0x30+1;
	  else
			  v_buffer[7]=temp2%10+0x30;
	v_buffer[8]='\0';
}

void start_display()  //Startup display
{
    GUI_Show12ASCII(10,10,"Maxim Integrated",GREEN,BLACK);
	GUI_Show12ASCII(10,30,"K_UP: Adjust",YELLOW,BLACK);
	GUI_Show12ASCII(10,50,"K_Down: Enter Model Gauge Setting Menu",YELLOW,BLACK);	
	GUI_Show12ASCII(10,90,"K_LEFT:Skip to Main Display",YELLOW,BLACK);	
	
}
void input_error_display()  //Startup display
{
//	TFT_ClearScreen(BLACK);
    GUI_Show12ASCII(10,10,"Maxim Integrated",GREEN,BLACK);
	GUI_Show12ASCII(10,30,"The input value is invalide",RED,BLACK);
	GUI_Show12ASCII(10,50,"Please try again",RED,BLACK);
}
void setting_display()  //mode select display
{
//	TFT_ClearScreen(BLACK);
//  GUI_Show12ASCII(10,10,"Maxim Integrated",GREEN,BLACK);

	/* Show two touch buttons
	   if change the location of the buttons,
	   please also change the touch location in m5_stting()
	*/
	GUI_Box(20, 30, 290, 120, BLUE);//shape of Button1(xStart,yStart,xEnd,yEnd,color)
    GUI_Show12ASCII(50,70,"EZ Mode",WHITE,BLUE);//text inside Button1
	GUI_Box(20, 140, 290, 230, YELLOW);//shape of Button2(xStart,yStart,xEnd,yEnd,color)
    GUI_Show12ASCII(50,180,"Use the Existing Model",BLACK,YELLOW);//text inside Button2
	GUI_Box(20, 250, 290, 340, GREEN); //(xStart,yStart,xEnd,yEnd,color) 
	GUI_Show12ASCII(50,290,"Load New Model",BLACK,GREEN);//text inside Button3
	GUI_Box(20, 360, 290, 450, CYAN); //(xStart,yStart,xEnd,yEnd,color) 
	GUI_Show12ASCII(50,400,"Back",BLACK,CYAN);//text inside Button4
}
void main_display(uint16_t x, uint16_t y) //main display
{	//mode select display

	TFT_ClearScreen(BLACK);
	GUI_Show12ASCII(x,10,"ModelGauge Demo--MAX17205!",YELLOW,BLACK);
	GUI_Show12ASCII(x,27,"I2C Interface: PB10--SCL PB11--SDA ",YELLOW,BLACK);
	GUI_Show12ASCII(x,y,"Reported State of Charge(%):",YELLOW,BLACK);
	GUI_Show12ASCII(x,y+17*1,"Remaining Capacity(mAh):",YELLOW,BLACK);
	GUI_Show12ASCII(x,y+17*2,"Cell Votage(V):",YELLOW,BLACK);
	GUI_Show12ASCII(x,y+17*3,"Time to Full(hr):",YELLOW,BLACK);
	GUI_Show12ASCII(x,y+17*4,"Time to Empty(hr):",YELLOW,BLACK);
	GUI_Show12ASCII(x,y+17*5,"Pack Current(mA)",YELLOW,BLACK);
    GUI_Show12ASCII(x,y+17*6,"Pack Avg Current(mA)",YELLOW,BLACK);
	GUI_Show12ASCII(x,y+17*7,"Temprature(C)",YELLOW,BLACK);
	delay_ms(100);
}
int adjust_OK()  //Touch screen adjust OK
{
	int flag=2;
	return flag;
}





void show_rem_nv_times(uint16_t rem_times, uint8_t *v_buffer)
{
	uint8_t i,j,times_dec;
	uint8_t  h_value;//upper 8bit of 0x1ed
//	double   logic_value;
	j=0x80;
	times_dec=0;
		for(i=0;i<10;i++)
	{
		v_buffer[i]=0;
	}
	h_value=(rem_times&0xff00)>>8;
	while ((h_value&j)==0)
	{
		j=j>>1;
		times_dec++;
	}

	v_buffer[0]=times_dec+0x30;
	v_buffer[1]='\0';
}

void nv_load()
{
    int th_flag=0;
    uint16_t  rem_times_value;
	uint8_t out_buffer[2];
	
	TFT_ClearScreen(BLACK);
	rem_times_value=Max17205_RemainNV_Times();
	show_rem_nv_times(rem_times_value,out_buffer);
	GUI_Box(20, 110, 290, 220, YELLOW);//shape of Button1(xStart,yStart,xEnd,yEnd,color)
    GUI_Show12ASCII(50,160,"Load Model to NV",BLACK,YELLOW);//text inside Button1
	GUI_Show12ASCII(50,177,"Times Remained",BLACK,YELLOW);
	GUI_Show12ASCII(250,177,out_buffer,YELLOW,BLACK);
	GUI_Box(20, 240, 290, 340, GREEN); //(xStart,yStart,xEnd,yEnd,color) 
	GUI_Show12ASCII(50,280,"Run without Load to NV ",BLACK,GREEN);//text inside Button2


  while (th_flag==0)
	{	
		if(TOUCH_Scan() == 0)
			{		
				th_flag=2;	
			}
			
			if(th_flag==2)
			{
				if(TouchData.lcdx>20&&TouchData.lcdx<290&&TouchData.lcdy>110&&TouchData.lcdy<220)	
				{
					TFT_ClearScreen(BLACK);
					GUI_Show12ASCII(10,300,"Loading...",YELLOW,BLACK);
					GUI_Show12ASCII(10,317,"Please wait 10 seconds",YELLOW,BLACK);
					Max17205_Load_NV();
					rem_times_value=Max17205_RemainNV_Times();
					show_rem_nv_times(rem_times_value,out_buffer);
					GUI_Show12ASCII(100,334,"Times Remained",YELLOW,BLACK);
					GUI_Show12ASCII(30,334,out_buffer,YELLOW,BLACK);
					delay_ms(1000);
				}
				if(TouchData.lcdx>20&&TouchData.lcdx<290&&TouchData.lcdy>240&&TouchData.lcdy<340)	
				{	
					TFT_ClearScreen(BLACK);	//extend funtion
				}	
			}
		}			
}

void ez_mode_set()
{
	int retry;
	EZ_setting ez_cfg;
	TFT_ClearScreen(BLACK);
	retry=4;
	GUI_Show12ASCII(10,200,"Loading...",YELLOW,BLACK);

	ez_cfg.CellNo=0x0005; //5cells
	ez_cfg.DesCap=1500; //mAh
	ez_cfg.PckCf=(Pck_High_Cell_N|ez_cfg.CellNo);
	ez_cfg.Rsen=10;//milliohom
	ez_cfg.Vemp=3.3;//V
	
	do{
		retry=Max17205_EZ_Mode(ez_cfg);
	}
	while(retry!=0);
	delay_ms(100);
	Max17205_Reset_Chip();
  GUI_Show12ASCII(10,250,"Done!",YELLOW,BLACK);
}

void m5_load()
{
	int i=0;
	int j=0; // init dummy number, to record how many times of transfer  
	int result=0;
	int retries=0;	
  uint8_t pn_buffer[192];
	
//	pn_buffer={0x00,0x21,0x01,0x84,0x02,0x3C,0x0D,0x01,0x1C,0x02,0x1B,0xEA,0x0E,0xE0,0x09,0xDB,0x08,0xD2,0x06,0xD0,0x07,0xB6,0x06,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA7,0xEB,0xA8,0x57,0xB1,0x9D,0xB9,0x2B,0xBB,0x6E,0xBC,0x4C,0xBF,0xDC,0xC1,0x56,0xC3,0xB2,0xC8,0x9D,0xCB,0xB7,0xD1,0xEF,0x03,0x33,0x00,0x00,0xA0,0x5F,0x26,0x02,0x41,0x00,0x1D,0x00,0x0B,0x00,0x08,0x00,0x00,0x00,0x23,0x00,0x20,0x3B,0x2D,0x48,0x00,0x00,0x23,0x00,0x00,0x00,0x80,0x7F,0x00,0xFF,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x38,0x70,0x23,0x00,0x00,0x00,0x0C,0x01,0x00,0x00,0x22,0x41,0xC1,0xB0,0x20,0x06,0xFE,0x0A,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5F,0x05,0x00,0x00,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE8,0x00,0x00,0x00,0x00,0xD5,0xE3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	count=0;
	//uint8_t* pn = (uint8_t*)malloc(100 * sizeof(uint8_t)); 
	GUI_Show12ASCII(10,100,"Please transfer the data via UART",YELLOW,BLACK);
	usart_init();//usart init && enable
	
  while(count<192)
	{	
    if((j-count)!=0) //check if data is new
		{
		  GUI_Show12ASCII(10,150,"Loading...",YELLOW,BLACK);
			pn_buffer[i]=RxV;
			while(RxV!=pn_buffer[i])
			{
				pn_buffer[i]=RxV;
			}
			i++;
			j=count;
			}
		}
	USART_Cmd(USART1, DISABLE);		   /*After transfer turn off uart*/
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//Disable interrupt of Usart
	delay_ms(200);	
	//WriteModel(pn_buffer);
    result=WriteVerifyModel(pn_buffer);
	delay_ms(1000);
  if (result==0)
	{
	TFT_ClearScreen(BLACK);	
  GUI_Show12ASCII(10,150,"Done!",GREEN,BLACK);		
	}
	else 
	{
		do 
		{
			result=WriteVerifyModel(pn_buffer);
			retries++;
		}
		while((result!=0)&&(retries<4));
			if(result==0)
					{
	          TFT_ClearScreen(BLACK);	
            GUI_Show12ASCII(10,150,"Done!",GREEN,BLACK);		
        	}
				else
					{
	          TFT_ClearScreen(BLACK);	
            GUI_Show12ASCII(10,150,"Load failed try again",GREEN,BLACK);		
        	}
		}
			
  Max17205_Reset_Chip();	
	delay_ms(100);
}



/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

int main()			  
{	
	uint16_t value;
	uint8_t dat[11];// buffer need to be more than 11 number

	Output output_value;
	float Rsense;
	int v_temp;
//	float f_temp;
	uint16_t rem_times_value;
	uint16_t rwx,rwy;
	uint16_t t_soc=0;
	
  int result=0;
	int retries=0;	
	/* model buffer*/
	uint8_t mbuffer[192]= {0x00,0x21,0x01,0x84,0x02,0x3C,0x0D,0x01,0x1C,0x02,0x1B,0xEA,0x0E,0xE0,0x09,0xDB,0x08,0xD2,0x06,0xD0,0x07,0xB6,0x06,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA7,0xEB,0xA8,0x57,0xB1,0x9D,0xB9,0x2B,0xBB,0x6E,0xBC,0x4C,0xBF,0xDC,0xC1,0x56,0xC3,0xB2,0xC8,0x9D,0xCB,0xB7,0xD1,0xEF,0x03,0x33,0x00,0x00,0xA0,0x5F,0x26,0x02,0x41,0x00,0x1D,0x00,0x0B,0x00,0x08,0x00,0x00,0x00,0x23,0x00,0x20,0x3B,0x2D,0x48,0x00,0x00,0x23,0x00,0x00,0x00,0x80,0x7F,0x00,0xFF,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x38,0x70,0x23,0x00,0x00,0x00,0x0C,0x01,0x00,0x00,0x22,0x41,0xC1,0xB0,0x20,0x06,0xFE,0x0A,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5F,0x05,0x00,0x00,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE8,0x00,0x00,0x00,0x00,0xD5,0xE3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	rwx=10;
	rwy=130;
//	rwx_value=230;

	
    sta_flag=PAGE_OPEN; //init state 
    I2C_MAX17205_Init(); 

//	I2C_MAX17201_Init();
	TFT_Init();	 //TFT screen init
	LED_Init();	  //LED init
	KEY_Config();
	FLASH_Init(); 
	TOUCH_Init();

  SYSTICK_Delay1ms(200);
	TFT_ClearScreen(BLACK);



	GUI_Show12ASCII(10,100,"GUI Start",YELLOW,BLACK);
  SYSTICK_Delay1ms(200);


  LoadModel(mbuffer);	
	delay_ms(200);
	Max17205_Reset_Cnf();
  delay_ms(100);

 TFT_ClearScreen(BLACK); //clear screen for following functions

 main_display(rwx,rwy);	
 rem_times_value=Max17205_RemainNV_Times();
 show_rem_nv_times(rem_times_value,dat);
 GUI_Show12ASCII(rwx,rwy+17*8,"Times Remained",YELLOW,BLACK);
 GUI_Show12ASCII(230,rwy+17*8,dat,YELLOW,BLACK);
 
 while(1)
	{
		  value=ReadRegistor(nRSense);
          Rsense=(float)value;
			if (Rsense==0)
			{
				TFT_ClearScreen(BLACK);
				GUI_Show12ASCII(230,150,"Configuration Fault--Rsense Error",RED,BLACK);
				GUI_Show12ASCII(230,150,"Press Reset or Power on again to reload ",RED,BLACK);
				while(1)
				{

				}
			}
		
		 // State of Charge
		  value=ReadRegistor(RepSOC);
	      output_value.Soc= value/256.000;
		  unsgn_output(output_value.Soc,dat);
          GUI_Show12ASCII(230,rwy,dat,YELLOW,BLACK);		
		
		 //  Remaining Capacity
		  value=ReadRegistor(RepCap);
		  output_value.RemCap=value*500.000/Rsense;
	      unsgn_output(output_value.RemCap,dat);
          GUI_Show12ASCII(230,rwy+17,dat,YELLOW,BLACK);
			
    //  Cell voltage		
		  value=ReadRegistor(VCell);
          output_value.CellVolt=value*0.078125*0.001;
   		  unsgn_output(output_value.CellVolt,dat);
          GUI_Show12ASCII(230,rwy+17*2,dat,YELLOW,BLACK);		
		
    //  Time to Full 			
		  value=ReadRegistor(TTF);
		  output_value.TimeFull=value*5.625/3600;
		  unsgn_output(output_value.TimeFull,dat);   
		  GUI_Show12ASCII(230,rwy+17*3,dat,YELLOW,BLACK);		
		
		//  Time to Empty
		  value=ReadRegistor(TTE);
		  output_value.TimeEmpty=value*5.625/3600;
			unsgn_output(output_value.TimeEmpty,dat);  
		  GUI_Show12ASCII(230,rwy+17*4,dat,YELLOW,BLACK);

		//  Pack Current
			value=ReadRegistor(Current);
		   if (value&0x8000)
			 {
				 v_temp=0xffff-value+1;
	     //  f_temp=temp*(156.25/Rsense)*-1.000;
				// output_value.Cur=f_temp;
				 output_value.Cur=v_temp*(156.25/Rsense)*-1.000;
			 }
			 else 
			 {
				 output_value.Cur= value*(156.25/Rsense);
			 }
			 sign_output(output_value.Cur,dat);  
		   GUI_Show12ASCII(230-16,rwy+17*5,dat,YELLOW,BLACK);

		  //  Pack Anverage Current			 
			 value=ReadRegistor(AvgCurrent);
		   if (value&0x8000)
			 {
				 v_temp=0xffff-value+1;
	     //  f_temp=temp*(156.25/Rsense)*-1.000;
				// output_value.Cur=f_temp;
				 output_value.AvgCur=v_temp*(156.25/Rsense)*-1.000;
			 }
			 else 
			 {
				 output_value.AvgCur= value*(156.25/Rsense);
			 }
			 sign_output(output_value.AvgCur,dat);  
		   GUI_Show12ASCII(230-16,rwy+17*6,dat,YELLOW,BLACK);
			 
			//  Temprature 
			value=ReadRegistor(Temp);
		   if (value&0x8000)
			 {
				 v_temp=0xffff-value+1;
				 output_value.TempOut=v_temp*0.004-1.000;
			 }
			 else 
			 {
				 output_value.TempOut= value*0.004;
			 }
			 sign_output(output_value.TempOut,dat);  
		     GUI_Show12ASCII(230-16,rwy+17*7,dat,YELLOW,BLACK);
			
			
		  v_temp =abs(t_soc-(int)output_value.Soc);
		  if(v_temp>=1)
		  //if(abs(t_soc-(int)output_value.Soc)>1) //hard fault occured
			{
				t_soc=output_value.Soc;
				
			}
            GUI_Battery(0,310,TFT_XMAX,420,t_soc);//(xStart,yStart,xEnd,yEnd,percentage)
			delay_ms(500);


   }
}
