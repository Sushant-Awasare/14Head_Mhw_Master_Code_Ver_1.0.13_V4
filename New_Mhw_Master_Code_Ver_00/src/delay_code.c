/*
 * delay.c
 *
 *  Created on: Feb 11, 2022
 *      Author: Dnyaneshwar
 */



#include "delay_functions.h"
#include "global_functions.h"
#include "global_variables.h"


#define DELAY_VAR
#include "delay_variables.h"



void mS_Delay(unsigned int mS)
{
	mS_Cnt = mS;

	while(mS_Cnt)
	{
		Clear_Watchdog();
	}
	return;

}

void Delay_Loop(unsigned long int Cntr)
{
	unsigned long int i,j;
	for(i=0; i<=Cntr; i++)
	 for(j = 0; j <= 100; j++);
}
