/*
 * modbus_responses.c
 *
 *  Created on: Feb 23, 2022
 *      Author: Dnyaneshwar
 */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_SCI5.h"
#include "modbus_variables.h"
#include "modbus_functions.h"
#include "global_defines.h"
#include "global_variables.h"


void respns1(void)
{
	int i,j,x,y;
	//unsigned char temp;

	i=0;
	if(regptr>MAXCOILS)regptr=MAXCOILS;			// Rohan ??????
	for(x=0,y=0;x<npoints;)
	{
		if(x<npoints)COILS[y].f_bit.f0 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)COILS[y].f_bit.f1 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)COILS[y].f_bit.f2 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)COILS[y].f_bit.f3 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)COILS[y].f_bit.f4 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)COILS[y].f_bit.f5 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)COILS[y].f_bit.f6 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)COILS[y].f_bit.f7 = Mod_Coils[(regptr&0xff)+(x++)];
		if(x<npoints)y++;
	}

	xbuff[i++]=slaveid;
	xbuff[i++]=function;
	xbuff[i++]=y+1;//bytecnt;

	for(j=0;j<(y+1);j++)
	{
		xbuff[i++]= COILS[j].f_byte;
	}

	calCRC(xbuff,i);				// Calculate CRC for the frame to be transmitted

	xbuff[i++] = crcval & 0xff;		// Attach CRC to the Frame
	xbuff[i++] = crcval>>8;

	xbytes=i;

	send();					// Send the frame over MODBUS RTU
}

/*The Function used to Read Holding Registers*/
void respns3(void)
{
	int i,j;
	unsigned char temp;

	bytecnt= npoints*2;

	i=0;

	xbuff[i++] = slaveid;		// Slave ID is extracted from received frame
	xbuff[i++] = function;		// Function is extracted from received frame
	xbuff[i++] = bytecnt;		// ByteCnt is extracted from received frame

	for(j=0;j<npoints;j++)
	{
	 temp = (records[(regptr&0xff)+j]>>8)&0xff;
	 xbuff[i++]=temp;
	 temp = records[(regptr&0xff)+j]&0xff;
	 xbuff[i++]=temp;
	 }

	calCRC(xbuff,i);			// Calculate CRC for the frame to be transmitted

	xbuff[i++] = crcval & 0xff;	// Attach CRC to the Frame
	xbuff[i++] = crcval>>8;

	xbytes=i;

	send();						// Send the frame over MODBUS RTU
}

void respns5(void)
{
	int i;

	if(npoints)
	{
		Mod_Coils[(regptr&0xff)] = 1;
	}
	else
	{
		Mod_Coils[(regptr&0xff)] = 0;
	}


	i=0;
	xbuff[i++]=slaveid;
	xbuff[i++]=function;
	xbuff[i++]=regptr>>8;
	xbuff[i++]=regptr&0xff;
	xbuff[i++]=npoints>>8;
	xbuff[i++]=npoints&0xff;

	calCRC(xbuff,i);

	xbuff[i++] = crcval & 0xff;
	xbuff[i++] = crcval>>8;

	xbytes=i;

	send();				// Send the frame of data over MODBUS RTU
}


/*The Function used to set single Holding register*/
void respns6(void)
{
	int i;

	records[(regptr&0xff)]=npoints;

	i=0;
	xbuff[i++]=slaveid;
	xbuff[i++]=function;
	xbuff[i++]=regptr>>8;
	xbuff[i++]=regptr&0xff;
	xbuff[i++]=npoints>>8;
	xbuff[i++]=npoints&0xff;

	calCRC(xbuff,i);

	xbuff[i++] = crcval & 0xff;
	xbuff[i++] = crcval>>8;

	xbytes=i;

	send();				// Send the frame of data over MODBUS RTU
}

/*The Function used to set multiple coils status*/
void respns0f(void)
{
	int i,j;
	//int temp;

	j=0;
	for(i=0;i<npoints;i++)
	{
	 	COILS[i+regptr].f_byte=rbuff[7+j++];
	}

 	i=0;
	xbuff[i++]=slaveid;
	xbuff[i++]=function;
	xbuff[i++]=regptr>>8;
	xbuff[i++]=regptr&0xff;
	xbuff[i++]=npoints>>8;
	xbuff[i++]=npoints&0xff;

	calCRC(xbuff,i);

	xbuff[i++] = crcval & 0xff;
	xbuff[i++] = crcval>>8;

	xbytes=i;

	send();
}

/*The Function used to set multiple register*/
void respns10(void)
{
	int i,j;
	int temp;

	j=0;
	for(i=0;i<npoints;i++)
	{
		temp=(int)(rbuff[7+j++])<<8;
		temp+=(rbuff[7+j++]);
	 	records[i+regptr]=temp;
	}

 	i=0;
	xbuff[i++]=slaveid;
	xbuff[i++]=function;
	xbuff[i++]=regptr>>8;
	xbuff[i++]=regptr&0xff;
	xbuff[i++]=npoints>>8;
	xbuff[i++]=npoints&0xff;

	calCRC(xbuff,i);		// Calculate the CRC for the Frame to be transmitted

	xbuff[i++] = crcval & 0xff;		// Attach CRC to the Frame
	xbuff[i++] = crcval>>8;

	xbytes=i;

	send();					// Send the Frame over MODBUS RTU
}
