/*
 * modbus_code.c
 *
 *  Created on: Feb 9, 2022
 *      Author: Dnyaneshwar
 */

//#define VAR_DECLS

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_SCI5.h"
#include "modbus_variables.h"
#include "modbus_functions.h"
#include "global_defines.h"
#include "global_variables.h"
#include "delay_functions.h"



/*The Function used to send the Frame of Data Over Serial Communication, Transmit the data using Tx ISR*/
void send(void)
{
	DIRN = 1;			// Direction pin of the MODBUS communcation
	Delay_Loop(250);	// 0.5 msec
	rbytes = 0;
	R_Config_SCI5_Serial_Send((uint8_t *)xbuff,(uint16_t)xbytes);
}


/*The Function used to process the frame received serial over MODBUS*/
void getvals(void)
{
	unsigned int rcvdchsum;

	if(xtrflg==1)
	{
		xtrflg=0;

		SLAVE.f_byte = FIXSLAVEID;

		calCRC(rbuff,rbytes-2);

		rcvdchsum=(unsigned int)rbuff[rbytes-2] + (unsigned int)rbuff[rbytes-1] * 256;

		if(rcvdchsum==crcval)
		{
			chsumflg=0;
			xtrbuff();			// Extract the data from serial frame
			decode();			// Decode the extracted data and send response
		}
	}

}




/*The function used to decode the Function type of received frame*/
void decode(void)
{
	if(slaveid==SLAVE.f_byte)
	{
		if(function==0x03)
		 respns3();					// Read Holding Registers
		else if (function==0x06)
		 respns6();					// Preset Single Register
		else if (function==0x10)
		 respns10();				// Preset Multiple Registers
		else if (function==0x01)
		 respns1();					// Read Coil status
		else if (function==0x05)
		 respns5();					// Write single coil
	}
}

/*The Function used to read the Coils status*/




/*The Function is interrupt service routine of MODBUS receive*/
void rxisr(uint8_t rx_data)
{
	if(stxflg==0)
	{
	 stxflg=1;
	 rbytes=0;
	}
	if(rbytes<MAXBYTES)
	{
		rbuff[rbytes]=rx_data;
		rbytes++;
	}
	rxflag=1;
	rxtimer=4;

}






void Init_Sci_Peripheral(void)
{
	R_Config_SCI5_Start();		// Used to Start the Serial Communication used as MODBUS Communication

}


void Init_Variables()
{
	Recharge_Divisor = records[90];
	Dryrun_weight_Division = records[91];
	Dryrun_weight_addition = records[92];


	if((records[103] < MAX_SPEED) && (records[103] != 0))
	{
		RunCycleTime = MAX_RUNCYCLE_TIME/records[103];
		Target_Speed = records[103];
		New_Target_Speed = Target_Speed;

		if(speed_change_flag == 0)
		{
			Previous_Speed = Target_Speed;
			speed_change_flag = 1;
		}
		else
		{
			if(Previous_Speed != Target_Speed)
				speed_change = 1;
		}
	}
	else
	{
		RunCycleTime = FIX_RUNCYCLE_TIME;
	}



	if(records[100]<=MAX_TARGET_WEIGHT)
	{
		TargetWt = records[100];

		NewTargetWt = TargetWt;

		if(wt_change_flag == 0)
		{
			if(TargetWt != 0 )
			{
				previous_weight = TargetWt;
				wt_change_flag = 1;
			}
		}
		else
		{
			if(previous_weight != TargetWt)
			{
				wt_change = 1;
			}
		}

	}
	else
	{
		TargetWt = FIX_TARGET_WEIGHT;
	}

	if(Multi_Dump_Count == 0)
		MULTI_DUMP_DIV_COUNT = records[170];

		NegErrorWt = records[102];
		PosErrorWt = records[101];

		under_over_NegErrorWt = NegErrorWt + 100;
		under_over_PosErrorWt = PosErrorWt + 1000;

		Mainhead_Firing =  records[104];

		All_head_firing = records[105];

		All_head_lf_ondelay = records[106];
		All_head_lf_ontime = records[107];


		All_head_fb_ondelay = records[108];
		All_head_fb_ontime = records[109];

		All_head_wb_ondelay = records[110];
		All_head_wb_ontime = records[111];

		All_head_cb_ondelay = records[112];
		All_head_cb_ontime = records[113];

		All_head_stepper_pattern = records[114];
		Discharge_ontime = records[115];
		MAX_STAGGER_TIME = records[116];
		NO_SELECT_COUNT = records[118];

		MHW_READY_TIME = records[119];

		ADC_GAIN = records[120];
		ADC_SPEED = records[121];
		ADC_SAMPLE = records[122];
		ADC_DIFFERENCE_COUNT = records[123];
		ADC_STABLE_COUNT = records[124];
		AUTO_ZERO_INTERVAL = records[125];
		WEIGHING_RANGE = records[126];
		COMPENSATION = records[127];
		STABLE_TIME = records[128];

		Tare_Count[1]  = records[130];
		Tare_Count[2]  = records[132];
		Tare_Count[3]  = records[134];
		Tare_Count[4]  = records[136];
		Tare_Count[5]  = records[138];
		Tare_Count[6]  = records[140];
		Tare_Count[7]  = records[142];
		Tare_Count[8]  = records[144];
		Tare_Count[9]  = records[146];
		Tare_Count[10] = records[148];
		Tare_Count[11]  = records[230];
		Tare_Count[12]  = records[231];
		Tare_Count[13]  = records[232];
		Tare_Count[14] = records[233];

		Calib_Count[1]  = records[131];
		Calib_Count[2]  = records[133];
		Calib_Count[3]  = records[135];
		Calib_Count[4]  = records[137];
		Calib_Count[5]  = records[139];
		Calib_Count[6]  = records[141];
		Calib_Count[7]  = records[143];
		Calib_Count[8]  = records[145];
		Calib_Count[9]  = records[147];
		Calib_Count[10] = records[149];
		Calib_Count[11]  = records[210];
		Calib_Count[12]  = records[211];
		Calib_Count[13]  = records[212];
		Calib_Count[14] = records[213];


		Calib_Slope[1]  = records[150];
		Calib_Slope[2]  = records[151];
		Calib_Slope[3]  = records[152];
		Calib_Slope[4]  = records[153];
		Calib_Slope[5]  = records[154];
		Calib_Slope[6]  = records[155];
		Calib_Slope[7]  = records[156];
		Calib_Slope[8]  = records[157];
		Calib_Slope[9]  = records[158];
		Calib_Slope[10] = records[159];
		Calib_Slope[11]  = records[220];
		Calib_Slope[12]  = records[221];
		Calib_Slope[13]  = records[222];
		Calib_Slope[14] = records[223];

	if(multi_dump_flag==1 && Multi_Dump_Count == 0)
	{
		Multi_Dump_Target_Wt = NewTargetWt;
		Multi_Dump_Wt =  Multi_Dump_Target_Wt/MULTI_DUMP_DIV_COUNT;
	}

		records[189] = NewTargetWt-10; //474
		records[190] = NewTargetWt-5; //474.5
		records[191] = NewTargetWt;		//475
		records[192] = NewTargetWt+5;	//475.5
		records[193] = NewTargetWt+10;	//476
		records[194] = NewTargetWt+15;	//476.5
		records[195] = NewTargetWt+20;	//477
		records[196] = NewTargetWt+30;	//478
		records[197] = NewTargetWt+40;	//479
		records[198] = NewTargetWt+50;	//480



}


