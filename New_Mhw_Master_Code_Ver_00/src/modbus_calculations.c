/*
 * modbus_calculations.c
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


/*The Function used to calculate the CRC of the Serial frame need to transmitted*/
void calCRC(unsigned char *bfptr, unsigned char nbytes)
{
	unsigned int itemp;
	char i;
	crcval=0xffff;
	do
	{
		itemp=crcval&0xff;
		itemp^=*bfptr;
		itemp=itemp&0x00ff;
		crcval&=0xff00;
		crcval|=itemp;

		for(i=0;i<8;i++)
		{
			if(crcval&0x01)
			{
				crcval=crcval>>1;
				crcval^=0xA001;
			}
		else
		 crcval=crcval>>1;
		}
		nbytes--;
		bfptr++;
	}
	while(nbytes!=0);
}

/*The Function used to convert ASCII to Hex*/
unsigned char asc2hx(signed char ch)
{
	ch=(ch-0x30);
	if(ch>=10)
	{
		ch=ch-7;
	}
	return(ch);
}

/*The Function used to covert Hex to ASCII*/
unsigned char hx2asc(signed char ch)
{
	ch&=0x0f;
	if((ch>= 0)&&(ch<10))
	 ch=ch+'0';
	else if((ch>=10)&&(ch<16))
	      ch = ch+'A'-10;
	return(ch);
}

/*The Function used to extract the data from the received frame*/
void xtrbuff(void)
{
	unsigned int i,temp;
	i=0;

	slaveid=rbuff[i++];				// Extract the slave id from the received frame

	if(slaveid==SLAVE.f_byte)		// Match Slave ID
	{
		function= rbuff[i++];		// Extract function code
		temp=(int)(rbuff[i++])<<8;
		temp+=(rbuff[i++]);
		regptr=temp;				// Extract register ID
		temp=(int)(rbuff[i++])<<8;
		temp+=(rbuff[i++]);
		npoints=temp;				// Extract number of Register

		if(function!=6)
		{
			if(npoints>NPNTS)
			npoints=NPNTS;
		}
		if(regptr>MAXREGS)
		 regptr=MAXREGS;
	}
}
