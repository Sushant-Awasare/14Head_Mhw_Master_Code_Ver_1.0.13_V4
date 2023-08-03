/*
 * calibration.c
 *
 *  Created on: Feb 15, 2022
 *      Author: Dnyaneshwar
 */

#include "modbus_variables.h"
#include "global_variables.h"
#include "global_functions.h"
#include "global_defines.h"
#include "delay_functions.h"
#include "calibration_variables.h"
#include "calibration_functions.h"

void Calibration_Operation()
{
	unsigned char current_head;

	if(calib_button_flag==1 )
	{
		if(Selected_Heads)
		{
			current_head=(Selected_Heads>>Head_counter)&0X1;

			if(current_head)
			{
				Get_Weight();
				mS_Delay(5);
				Head_counter=Head_counter+1;
			}
			else
			{
				Head_counter++;
			}
		}

		if(MAX_HEADS<Head_counter)
		{
			Head_counter=1;
		}
	}

 if(Flag_AllHead == 0 ||  Flag_AllHead == 1)
	{
		Selected_Heads = 0;
		Check_Bucket_Selected();

		if((Coil_DumpBucketButton))
		{

			c_command=Selected_Heads<<1;
			c_command=c_command|0x0;
			c_command=c_command&0X3FFFF;
			Calibration_Operation_Can_Frame();
			calib_button_flag=1;
			Coil_DumpBucketButton=0;

			if(Flag_AllHead)
				Power_On_Run = 0;
		}
		if((Coil_TeraLoadCellButton))
		{
			c_command=Selected_Heads<<1;
			c_command=c_command|0x1;
			c_command=c_command & 0X3FFFF;
			Calibration_Operation_Can_Frame();
			Coil_TeraLoadCellButton=0;
		}
		if(Coil_CalibLoadCellButton)
		{
			calib_button_flag=1;
			c_command=Selected_Heads<<1;
			c_command=c_command|0x2;
			c_command=c_command & 0X3FFFF;
			Calibration_Operation_Can_Frame();
			Coil_CalibLoadCellButton=0;
		}
	}
}
