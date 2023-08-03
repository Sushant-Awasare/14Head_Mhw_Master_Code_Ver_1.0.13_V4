/*
 * modbus_records.c
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


/*The function used to clear all the records*/
void Delete_ALL_Records(void)
{
	unsigned int i;
	for(i=0;i<MAXREGS;i++)
	 records[i]=10;
}


void Copy_Records(void)
{
	unsigned char index;
	for(index=0; index<MAXREGS;index++)
	{
 		records1[index].f_word = records[index];
	}
}
