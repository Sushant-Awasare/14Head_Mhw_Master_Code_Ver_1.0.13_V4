/***********************************************************************
*
s*  FILE        : New_Mhw_Master_Code_Ver_00.c
*  DATE        : 2022-02-03
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/

#include "includes.h"


void main(void)
{

	Initialize_Port_Pins();		// Initialize controller port pin to its default

	Initialize_Peripherals();	// Initialize and start the peripheral function used

	mS_Delay(1000);

	Initial_Values();

	mS_Delay(2000);

	open_dataflash();
	Delay_Loop(10);
	read_run_data();
	Delay_Loop(10);
	flash_read_data();
	read_bit_as_word(); //changes done on 31/07/2023

	while(1)
	{
		Clear_Watchdog();

		While_Loop_Time_flag = 1;

		/*if(test1>32767)
		{
			BucketWt[14] =  test1 - 65536 ;
		}
		else
			BucketWt[14] = test1;*/



		if(MHW_READY == 1)
			mhw_pin = 1;
		else
			mhw_pin = 0;

		getvals();			// Check for MODBUS received frame
		Copy_Records();		// Copy the records for bit level operation
		Init_Variables();

		HMI_Flash_Write_PE = ((HMI_Flash_Write_LCS ^ HMI_Flash_Write) & HMI_Flash_Write);
		HMI_Flash_Write_LCS = HMI_Flash_Write;
		if(HMI_Flash_Write_PE)
		{
			write_bit_as_word(); //changes done on 31/07/2023
			Delay_Loop(50);
			flash_write_data();
			flash_popup = 1;
			flash_popup_off_time=20000;
			//slave_flash_write_flash = 1;
			//Slave_Flash_Write();
		}
		if(COIL_ZEROING)
		{
			state = 100;
			Zeroing();
		}

		if(Coil_DryRunCycle == 1 && dry_runcycle_flag == 0)
		{
			dry_runcycle_flag = 1;
			DryRunCycle();
		}
		else if(Coil_DryRunCycle == 0 && dry_runcycle_flag == 1)
		{
			dry_runcycle_flag = 0;
			DryRunCycle();
		}

		if(Coil_AutoCorrection == 1 && auto_correction_flag == 0)
		{
			auto_correction_flag = 1;
			AutoCorrection();
		}
		else if(Coil_AutoCorrection == 0 && auto_correction_flag == 1)
		{
			auto_correction_flag = 0;
			AutoCorrection();
		}


		if((no_select_coil_flag == 0 && Coil_No_Select_Mode == 1) || ((Previous_no_select_count!=NO_SELECT_COUNT)&& Coil_No_Select_Mode == 1))
		{
			Previous_no_select_count=NO_SELECT_COUNT;
			Over_Under_Weight();
			no_select_coil_flag = 1;
		}
		else if(no_select_coil_flag == 1 && Coil_No_Select_Mode == 0)
		{
			no_select_coil_flag = 0;
		}
		if((Coil_Reset) && ((Coil_RunStartButton==0) || (Coil_RunButton == 0)))
		{
			Reset_Statistics();
		}

		if((Coil_Standard_Devi) && ((Coil_RunStartButton==0) || (Coil_RunButton == 0)))
		{
			Calculate_Standard_Deviation();
		}

		if(Coil_Stagger_Dump == 1)
		{
			stagger_flag = 1;
		}
		else
		{
			stagger_flag = 0;
		}

		if(Coil_Multi_Dump == 1 && MULTI_DUMP_DIV_COUNT!=0 && cycle_equal_time == 0)
		{
			multi_dump_flag = 1;
		}
		else if(Coil_Multi_Dump == 0 || MULTI_DUMP_DIV_COUNT==0)
		{
			multi_dump_flag       = 0;
			Multi_dump_display_wt = 0;
			Multi_Dump_Count      = 0;
		}

		Mhs_Start_Stop();

		Check_Bucket_Selected();

		Check_AllHeads_Firing();

		Check_AllHeads_Ontime();

		Check_mainhead_Firing_Ontime();

		check_new_firing();

		check_new_time();


		if(Coil_DumpTrigger == 1 && aux_open_flag == 0)
		{
			Aux_Open();
		}

		if(Coil_DumpTrigger == 0 && aux_open_flag == 1)
		{
			Aux_Close();

			if(MHW_READY == 0)
				aux_ready_flag = 1;
		}

		if(Coil_AuxBucketButton)
			run_with_aux_bucket_flag = 1;
		else
			run_with_aux_bucket_flag = 0;

		if(Coil_SingleCycleButton == 0)
			single_cycle_flag = 1;

		if((Coil_RunStartButton == 0) || (Coil_RunButton == 0) || (Sec_Counter>65000) || (Run_Dump_Counter>65000)||(wt_change == 1)||(speed_change == 1))
		{
			//if(MHW_READY)
				//aux_ready_flag = 1;

			if(MHW_READY == 0)
			{
				auxbucket_empty_flag = 0;
				Coil_CombiReady  = 0;
			}
			//MHW_READY = 0;

			ready_on_flag = 0;
			dump_feedback_count = 0;
			dump_bucket_count  = 0;

			if(MHW_READY == 0)
			{
				Flag_Check_Combi  = 0;
				hold_run_cycle_tmr = 0;
			}

			next_run_flag = 0;
		//	hold_run_cycle_tmr = 0;             // 21/12/22
		//	Coil_CombiReady  = 0;

			//if((Sec_Counter>65000) || (Run_Dump_Counter>65000)||(wt_change == 1)||(speed_change == 1))
			{
				Sec_Counter      = 0;
				Run_Dump_Counter = 0;
				Actual_Run_Speed = 0;

				Ms_Counter       = 0;
				records[20]      = 0;
				records[21]      = 0;
				records[22]      = 0;
				Total_Combi_head=0;Combi_Head_Counter=0;Average_Combi_Head=0;
				Total_Stable_head=0;Stable_Head_Counter=0;Average_Stable_Head=0;
				speed_change_flag = 0;
				speed_change = 0;
			}

		}

		if(Coil_ManualOprButton == 1 )
		{
//			manual_once_flag  = 1;
			Coil_SetParaADC     = 0;
			Coil_CalibOprButton = 0;
			Coil_RunStartButton = 0;
			Manual_Operation();
			Clear_Watchdog();
		}

		if(Coil_CalibOprButton)
		{
			Coil_SetParaADC      = 0;
			Coil_ManualOprButton = 0;
			Coil_RunStartButton  = 0;
			Calibration_Operation();
			Clear_Watchdog();
		}

		if((Coil_RunStartButton==1) && (Coil_RunButton == 1))
		{
			Run_Loop_Time_flag = 1;

			Coil_SetParaADC      = 0;
			Coil_ManualOprButton = 0;
			Coil_CalibOprButton  = 0;

			if(cycle_equal_time_flag == 0 && next_run_flag == 0)
			{
				Check_Bucket_Selected();
				Check_Bucket_Status();
			}

			Run_Operation();

			if(Run_Loop_Time_flag == 0)
			{
				Run_Loop_Time_buff[Run_Loop_Time_Count] = Run_Loop_Time;
				Run_Loop_Time = 0;
				Run_Loop_Time_Count++;

				if(Run_Loop_Time_Count >=100)
					Run_Loop_Time_Count=101;
			}


			Clear_Watchdog();
		}

		if(Coil_SetParaADC || Power_On_Para_Write) //changes done on 31/07/2023
		{
			Coil_ManualOprButton = 0;
			Coil_CalibOprButton  = 0;
			Coil_RunStartButton  = 0;
			Set_Para();
			Clear_Watchdog();
		}

		if(Coil_Set_Stepper_Pattern)
		{
			//Set_Stepper_Pattern();
		}

		if((Coil_RunStartButton==0) || (Coil_RunButton == 0) || (popup_reset == 1))
		{
			no_combi_popup_flag = 0;
			no_combi_popup_time = 0;
			no_combi_popup = 0;
			over_weight_popup = 0;
		}


		Coil_Increment_Firing     = 0;
		Coil_Decrement_Firing     = 0;
		Coil_MainDecrement_Firing = 0;
		Coil_MainIncrement_Firing = 0;

		While_Loop_Time_flag = 0;

		if(While_Loop_Time_flag == 0)
		{
			While_Loop_Time_buff[While_Loop_Time_Count] = While_Loop_Time;
			While_Loop_Time = 0;
			While_Loop_Time_Count++;

			if(While_Loop_Time_Count >=100)
				While_Loop_Time_Count=0;
		}
		Clear_Watchdog();

		if((Coil_CombiReady == 0) && (Coil_RunStartButton == 1) && (Coil_RunButton == 1) && no_combi_popup_flag == 0)
		{
			no_combi_popup_flag = 1;
			no_combi_popup_time = 20000;
		}
		else if (combination_ok_flag == 1 || under_over_combination_ok_flag == 1)
		{
			no_combi_popup_flag = 0;
			no_combi_popup = 0;
		}
	}
	
}


void Interval_Timer_Isr(void)	//0.5ms Timer isr
{

	if((ready_on_flag == 1) && (dump_feedback_count == dump_bucket_count))
	{
		ready_on_flag = 0;
		dump_feedback_count = 0;
		dump_bucket_count = 0;

		if(Coil_AuxBucketButton)
			MHW_READY = 1;
	}

	if ((no_combi_popup_flag == 1) && (no_combi_popup_time>0))
		no_combi_popup_time --;

	if (no_combi_popup_flag == 1 && no_combi_popup_time == 0)
	{
		no_combi_popup = 1;
		no_combi_popup_flag = 0;
	}

	if (flash_popup_off_time>0)
		flash_popup_off_time--;

	if(flash_popup_off_time==0)
	{
		flash_popup = 0;
		flash_popup_off_time = 0;
	}

	if (flash_write_time>0)
	{
		flash_write_time--;
	}

	if(flash_write_time==0)
	{
		flash_write_done = 0;
		flash_write_done_flag = 0;
		flash_popup_off_time = 0;
		slave_flash_write_flash = 0;
	}

	if(While_Loop_Time_flag)
		While_Loop_Time++;

	if(Run_Loop_Time_flag)
			Run_Loop_Time++;

	if(combi_time_flag == 1)
			comi++;

	if(mhs_on_flag)
		mhs_start_counter++;

	if(mhs_off_flag)
		mhs_stop_counter++;

	if(Trigger_Data_History_Time>0)
		Trigger_Data_History_Time--;

	if(Trigger_Data_History_Time == 0)
		Trigger_For_Data_History = 0;

	if(send_reacharge_frame_flag == 1 && Recharge_Frame_Time <= RECHARGE_TIME)
		Recharge_Frame_Time++;

	if((cycle_equal_time>0) && (Coil_RunStartButton == 1) &&  (Coil_RunButton==1)&&(hold_run_cycle_tmr==0)&&(stagger_wait_time==0))
		cycle_equal_time--;

	if(cycle_equal_time == 0 )//&& aux_ready_time == 0 )
	{
		cycle_equal_time_flag=0;
		//ready_on_flag = 0;
		records[25] = 0;
		records[26] = 0;
		records[27] = 0;
		records[28] = 0;
		records[29] = 0;
		records[30] = 0;
		records[31] = 0;
		records[32] = 0;
		records[33] = 0;
		records[34] = 0;



		/*if(Coil_AuxBucketButton == 0)
			Flag_Check_Combi  = 0;*/

		/*if(Coil_AuxBucketButton == 0)
		{
			Coil_CombiReady = 0;
			MHW_READY   = 0;
			//ready_on_flag = 0;
		}*/

		//if(Coil_AuxBucketButton == 0)
			//MHW_READY   = 0;

		DUMP_CONFIRMATION = 0;
		OVER_UNDER_WT     = 0;

	}


	if((Coil_RunButton)&&(Coil_RunStartButton) && Ms_Counter == 2000 && Sec_Counter<65000)
	{
		Ms_Counter = 0;
		Sec_Counter++;

	}

	if((Coil_RunButton)&&(Coil_RunStartButton)&&(Ms_Counter<2000))
	{
		Ms_Counter++;
	}

	if(rxflag!=0)
	{
		if(rxtimer>0)
			rxtimer--;
		if(rxtimer==0)
		{
			rxflag = 0;
			stxflg = 0;
			xtrflg = 1;
		}
	}

	if(msflag==0)
	{
		 msflag = 1;
	}
	else if(msflag)
	{
		if(send_time>0)
			send_time--;

		if(Manual_Cycle_Time>0)
			Manual_Cycle_Time--;

		if(mS_Cnt>0)
			mS_Cnt--;

		if(ready_time>0)
			ready_time--;

		/*if(ready_time == 0 && ready_on_flag == 1)
		{
			ready_on_flag = 0;
			MHW_READY = 1;
		}*/


		msflag = 0;
	}

}

void Initialize_Port_Pins()
{
	R_Config_PORT_Create();

	MHW_READY = 0;
	MHS_ON_OFF = 0;
	DUMP_CONFIRMATION = 0;
	OVER_UNDER_WT = 0;
	RLY5 = 0;
	RLY6 = 0;
}

void Initialize_Peripherals()
{
	R_Config_CMT0_Create();                    // Create Compare Match timer 0
	R_Config_CMT0_Start();                    // Start Compare Match Timer 0
	Init_Sci_Peripheral();	                 // Initialize and start the peripheral functions used
	Initialize_CAN_Communication();         // Initialize CAN Communication
}

void Initial_Values()
{
	  Selected_Heads = 0;
	  Head_counter   = 1;
	  Power_On_Para_Write = 1; //changes done on 31/07/2023
}


void Clear_Watchdog()
{
	R_Config_WDT_Restart();
}


void Reset_Statistics()
{
	Inbetween_less_counter_1 = 0;
	records[179] = Inbetween_less_counter_1;

	Inbetween_less_counter = 0;
	records[180] = Inbetween_less_counter;

	Inbetween_more_counter = 0;
	records[181] = Inbetween_more_counter;

	more_counter = 0;
	records[182] = more_counter;

	more_counter_1 = 0;
	records[183] = more_counter_1;

	more_counter_2 = 0;
	records[184] = more_counter_2;

	more_counter_3 = 0;
	records[185] = more_counter_3;

	more_counter_4 = 0;
	records[186] = more_counter_4;

	more_counter_5 = 0;
	records[187] = more_counter_5;

	more_counter_6 = 0;
	records[188] = more_counter_6;

	Disp_Avg_weight        = 0;
	records[176] = Disp_Avg_weight;

	Min_Wt                 = 0;
	records[174] = Min_Wt;

	Max_Wt                 = 0;
	records[175] = Max_Wt;

	Run_Dump_Counter = 0;
	Sec_Counter      = 0;

	for(dev_l=0;dev_l<MAX_DEVIATION_COUNT;dev_l++)
	{
		Avg_Dev_weights[dev_l] = 0;
		Dev_weights[dev_l]     = 0;
	}

	sum_devi_wt  = 0;
	total_dev_wt = 0;
	dev_counter  = 0;
	dev_i        = 0;

	standard_deviation     = 0;
	records[177] =  standard_deviation;

	total_dump_in_day = 0;
	records[178] = total_dump_in_day;
}


void Calculate_Standard_Deviation()
{
	sum_devi_wt  = 0;
	total_dev_wt = 0;

	if(dev_counter>=MAX_DEVIATION_COUNT)
	{
		for(dev_k=0;dev_k<MAX_DEVIATION_COUNT;dev_k++)
		{
			sum_devi_wt += Dev_weights[dev_k];
		}
		avg_devi_wt = sum_devi_wt/MAX_DEVIATION_COUNT;

		for(dev_l=0;dev_l<MAX_DEVIATION_COUNT;dev_l++)
		{
			Avg_Dev_weights[dev_l] = avg_devi_wt - Dev_weights[dev_l];
		}

		for(dev_l=0;dev_l<MAX_DEVIATION_COUNT;dev_l++)
		{
			dev_wt = Avg_Dev_weights[dev_l];
			total_dev_wt += dev_wt*dev_wt;
		}

		mean_dev_wt = total_dev_wt/MAX_DEVIATION_COUNT-1;

		standard_deviation = sqrt(mean_dev_wt);
		records[177] = standard_deviation;
	}
	else
	{
		for(dev_k=0;dev_k<dev_counter;dev_k++)
		{
			sum_devi_wt += Dev_weights[dev_k];
		}

		avg_devi_wt = sum_devi_wt/dev_counter;

		for(dev_l=0;dev_l<dev_counter;dev_l++)
		{
			if(avg_devi_wt>Dev_weights[dev_l])
				Avg_Dev_weights[dev_l] = avg_devi_wt - Dev_weights[dev_l];
			else
				Avg_Dev_weights[dev_l] = Dev_weights[dev_l] - avg_devi_wt;
		}

		for(dev_l=0;dev_l<dev_counter;dev_l++)
		{
			dev_wt = Avg_Dev_weights[dev_l];
			total_dev_wt += dev_wt*dev_wt;
		}

		mean_dev_wt = total_dev_wt/dev_counter-1;

		standard_deviation = sqrt(mean_dev_wt);
		records[177] = standard_deviation;

		}

}


void Check_Bucket_Selected()
{
	if((Coil_AllHeadsButton == 1)&&(Flag_AllHead == 0))
	{
		Flag_AllHead      = 1;

		Coil_Head1Button  = 1;
		Coil_Head2Button  = 1;
		Coil_Head3Button  = 1;
		Coil_Head4Button  = 1;
		Coil_Head5Button  = 1;
		Coil_Head6Button  = 1;
		Coil_Head7Button  = 1;
		Coil_Head8Button  = 1;
		Coil_Head9Button  = 1;
		Coil_Head10Button = 1;
		//Coil_Head11Button = 1;
		//Coil_Head12Button = 1;
		//Coil_Head13Button = 1;
		//Coil_Head14Button = 1;
		Coil_Head15Button = 1;
	}

	if((Coil_AllHeadsButton == 0)&&(Flag_AllHead == 1))
	{
		Flag_AllHead      = 0;

		Coil_Head1Button  = 0;
		Coil_Head2Button  = 0;
		Coil_Head3Button  = 0;
		Coil_Head4Button  = 0;
		Coil_Head5Button  = 0;
		Coil_Head6Button  = 0;
		Coil_Head7Button  = 0;
		Coil_Head8Button  = 0;
		Coil_Head9Button  = 0;
		Coil_Head10Button = 0;
		//Coil_Head11Button = 0;
		//Coil_Head12Button = 0;
		//Coil_Head13Button = 0;
		//Coil_Head14Button = 0;
		Coil_Head15Button = 0;
	}

	Selected_Heads = 0;

	if(Coil_Head15Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head15Button == 0)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}

	if(Coil_Head14Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head14Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head13Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head13Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head12Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head12Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head11Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head11Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head10Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head10Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head9Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head9Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head8Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head8Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head7Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head7Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head6Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head6Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head5Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head5Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head4Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head4Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head3Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head3Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head2Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head2Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

	if(Coil_Head1Button == 1)
	{
		Selected_Heads=Selected_Heads|0x1;
		Selected_Heads<<=1;
	}
	else if(Coil_Head1Button == 0)
	{
		Selected_Heads=Selected_Heads|0x0;
		Selected_Heads<<=1;
	}

}


void Check_Bucket_Status()
{

	//if(BucketWt[0] >= 0 && Coil_Head1Button == 1  && wb1_nothoming_flag==0  && Head1_Stable == 1 && skip_for_combination_1 == 0 && skip_for_recharge_1 == 0)
	if(Coil_Head1Button == 1  && wb1_nothoming_flag==0  && Head1_Stable == 1 && skip_for_combination_1 == 0 && skip_for_recharge_1 == 0)
	{
		bucket_no[0] = 1;
	}
	else
	{
		bucket_no[0] =  0;
	//	records[25]  =  1;
	}

	//if(BucketWt[1] >= 0 && Coil_Head2Button == 1 && wb2_nothoming_flag==0  && Head2_Stable == 1 && skip_for_combination_2 == 0 && skip_for_recharge_2 == 0 )
	if(Coil_Head2Button == 1 && wb2_nothoming_flag==0  && Head2_Stable == 1 && skip_for_combination_2 == 0 && skip_for_recharge_2 == 0 )
	{
		bucket_no[1] = 1;
	}
	else
	{
		bucket_no[1] =  0;
	//	records[26]  =  1;
	}

	//if(BucketWt[2] >= 0 &&  Coil_Head3Button == 1 && wb3_nothoming_flag==0  && Head3_Stable == 1 && skip_for_combination_3 == 0 && skip_for_recharge_3 == 0)
	if(Coil_Head3Button == 1 && wb3_nothoming_flag==0  && Head3_Stable == 1 && skip_for_combination_3 == 0 && skip_for_recharge_3 == 0)
	{
		bucket_no[2] = 1;
	}
	else
	{
		bucket_no[2] =  0;
	//	records[27]  =  1;
	}

	//if(BucketWt[3] >= 0 &&  Coil_Head4Button == 1 && wb4_nothoming_flag==0  && Head4_Stable == 1 && skip_for_combination_4 == 0 && skip_for_recharge_4 == 0)
	if(Coil_Head4Button == 1 && wb4_nothoming_flag==0  && Head4_Stable == 1 && skip_for_combination_4 == 0 && skip_for_recharge_4 == 0)
	{
		bucket_no[3] = 1;
	}
	else
	{
		bucket_no[3] =  0;
	//	records[28]  =  1;
	}

	//if(BucketWt[4] >= 0 &&  Coil_Head5Button == 1 && wb5_nothoming_flag==0 && Head5_Stable == 1 && skip_for_combination_5 == 0 && skip_for_recharge_5 == 0)
	if(Coil_Head5Button == 1 && wb5_nothoming_flag==0 && Head5_Stable == 1 && skip_for_combination_5 == 0 && skip_for_recharge_5 == 0)
	{
		bucket_no[4] = 1;
	}
	else
	{
		bucket_no[4] =  0;
	//	records[29]  =  1;
	}
	//if(BucketWt[5] >= 0 &&  Coil_Head6Button == 1 && wb6_nothoming_flag==0  && Head6_Stable == 1 && skip_for_combination_6 == 0 && skip_for_recharge_6 == 0)
	if(Coil_Head6Button == 1 && wb6_nothoming_flag==0  && Head6_Stable == 1 && skip_for_combination_6 == 0 && skip_for_recharge_6 == 0)
	{
		bucket_no[5] = 1;
	}
	else
	{
		bucket_no[5] =  0;
	//	records[30]  =  1;
	}

	//if(BucketWt[6] >= 0 &&  Coil_Head7Button == 1 && wb7_nothoming_flag==0  && Head7_Stable == 1 && skip_for_combination_7 == 0 && skip_for_recharge_7 == 0)
	if(Coil_Head7Button == 1 && wb7_nothoming_flag==0  && Head7_Stable == 1 && skip_for_combination_7 == 0 && skip_for_recharge_7 == 0)
	{
		bucket_no[6] = 1;
	}
	else
	{
		bucket_no[6] =  0;
	//	records[31]  =  1;
	}

	//if( BucketWt[7] >= 0 &&  Coil_Head8Button == 1 && wb8_nothoming_flag==0  && Head8_Stable == 1 && skip_for_combination_8 == 0 && skip_for_recharge_8 == 0)
	if(Coil_Head8Button == 1 && wb8_nothoming_flag==0  && Head8_Stable == 1 && skip_for_combination_8 == 0 && skip_for_recharge_8 == 0)
	{
		bucket_no[7] = 1;
	}
	else
	{
		bucket_no[7] =  0;
	//	records[32]  =  1;
	}

	//if( BucketWt[8] >= 0 &&  Coil_Head9Button == 1 && wb9_nothoming_flag==0  && Head9_Stable == 1 && skip_for_combination_9 == 0 && skip_for_recharge_9 == 0)
	if(Coil_Head9Button == 1 && wb9_nothoming_flag==0  && Head9_Stable == 1 && skip_for_combination_9 == 0 && skip_for_recharge_9 == 0)
	{
		bucket_no[8] = 1;
	}
	else
	{
		bucket_no[8] =  0;
	//	records[33]  =  1;
	}

	//if(BucketWt[9] >= 0 &&  Coil_Head10Button == 1 && wb10_nothoming_flag==0  && Head10_Stable == 1 && skip_for_combination_10 == 0 && skip_for_recharge_10 == 0)
	if(Coil_Head10Button == 1 && wb10_nothoming_flag==0  && Head10_Stable == 1 && skip_for_combination_10 == 0 && skip_for_recharge_10 == 0)
	{
		bucket_no[9] = 1;
	}
	else
	{
		bucket_no[9] =  0;
	//	records[34]  =  1;
	}

	//if(BucketWt[10] >= 0 && Coil_Head11Button == 1 && wb11_nothoming_flag==0  && Head11_Stable == 1 && skip_for_combination_11 == 0 && skip_for_recharge_11 == 0)
	if(Coil_Head11Button == 1 && wb11_nothoming_flag==0  && Head11_Stable == 1 && skip_for_combination_11 == 0 && skip_for_recharge_11 == 0)
	{
		bucket_no[10] = 1;
	}
	else
	{
		bucket_no[10] =  0;
	//	records[35]  =  1;
	}

	//if(BucketWt[11] >= 0 &&  Coil_Head12Button == 1 && wb12_nothoming_flag==0  && Head12_Stable == 1 && skip_for_combination_12 == 0 && skip_for_recharge_12 == 0)
	if(Coil_Head12Button == 1 && wb12_nothoming_flag==0  && Head12_Stable == 1 && skip_for_combination_12 == 0 && skip_for_recharge_12 == 0)
	{
		bucket_no[11] = 1;
	}
	else
	{
		bucket_no[11] =  0;
	//	records[36]   =  1;
	}

	//if(BucketWt[12] >= 0 &&  Coil_Head13Button == 1 && wb13_nothoming_flag==0  && Head13_Stable == 1 && skip_for_combination_13 == 0 && skip_for_recharge_13 == 0)
	if(Coil_Head13Button == 1 && wb13_nothoming_flag==0  && Head13_Stable == 1 && skip_for_combination_13 == 0 && skip_for_recharge_13 == 0)
	{
		bucket_no[12] = 1;
	}
	else
	{
		bucket_no[12] =  0;
		//records[37]   =  1;
	}

	//if(BucketWt[13] >= 0 &&  Coil_Head14Button == 1 && wb14_nothoming_flag==0  && Head14_Stable == 1 && skip_for_combination_14 == 0 && skip_for_recharge_14 == 0)
	if(Coil_Head14Button == 1 && wb14_nothoming_flag==0  && Head14_Stable == 1 && skip_for_combination_14 == 0 && skip_for_recharge_14 == 0)
	{
		bucket_no[13] = 1;
	}
	else
	{
		bucket_no[13] =  0;
	//	records[38]   =  1;
	}
}

void Check_AllHeads_Firing()
{
	if((Coil_AllHead_Firing == 1)&&(Flag_AllHead_firing == 0))
	{
		Flag_AllHead_firing = 1;


		Coil_Firing_Angle_1   = 1;
		Coil_Firing_Angle_2   = 1;
		Coil_Firing_Angle_3   = 1;
		Coil_Firing_Angle_4   = 1;
		Coil_Firing_Angle_5   = 1;
		Coil_Firing_Angle_6   = 1;
		Coil_Firing_Angle_7   = 1;
		Coil_Firing_Angle_8   = 1;
		Coil_Firing_Angle_9   = 1;
		Coil_Firing_Angle_10  = 1;
		Coil_Firing_Angle_11  = 1;
		Coil_Firing_Angle_12  = 1;
		Coil_Firing_Angle_13  = 1;
		Coil_Firing_Angle_14  = 1;
	}

	if((Coil_AllHead_Firing == 0)&&(Flag_AllHead_firing == 1))
	{
		Flag_AllHead_firing   = 0;
		Coil_Firing_Angle_1   = 0;
		Coil_Firing_Angle_2   = 0;
		Coil_Firing_Angle_3   = 0;
		Coil_Firing_Angle_4   = 0;
		Coil_Firing_Angle_5   = 0;
		Coil_Firing_Angle_6   = 0;
		Coil_Firing_Angle_7   = 0;
		Coil_Firing_Angle_8   = 0;
		Coil_Firing_Angle_9   = 0;
		Coil_Firing_Angle_10  = 0;
		Coil_Firing_Angle_11  = 0;
		Coil_Firing_Angle_12  = 0;
		Coil_Firing_Angle_13  = 0;
		Coil_Firing_Angle_14  = 0;
	}
}

void Check_AllHeads_Ontime()
{
	if((Coil_AllHead_OnTime == 1)&&(Flag_AllHead_ontime == 0))
	{
		Flag_AllHead_ontime = 1;
		Coil_On_Time_1   = 1;
		Coil_On_Time_2   = 1;
		Coil_On_Time_3   = 1;
		Coil_On_Time_4   = 1;
		Coil_On_Time_5   = 1;
		Coil_On_Time_6   = 1;
		Coil_On_Time_7   = 1;
		Coil_On_Time_8   = 1;
		Coil_On_Time_9   = 1;
		Coil_On_Time_10  = 1;
		Coil_On_Time_11  = 1;
		Coil_On_Time_12  = 1;
		Coil_On_Time_13  = 1;
		Coil_On_Time_14  = 1;
	}

	if((Coil_AllHead_OnTime == 0)&&(Flag_AllHead_ontime == 1))
	{
		Flag_AllHead_ontime = 0;

		Coil_On_Time_1   = 0;
		Coil_On_Time_2   = 0;
		Coil_On_Time_3   = 0;
		Coil_On_Time_4   = 0;
		Coil_On_Time_5   = 0;
		Coil_On_Time_6   = 0;
		Coil_On_Time_7   = 0;
		Coil_On_Time_8   = 0;
		Coil_On_Time_9   = 0;
		Coil_On_Time_10  = 0;
		Coil_On_Time_11  = 0;
		Coil_On_Time_12  = 0;
		Coil_On_Time_13  = 0;
		Coil_On_Time_14  = 0;
	}
}

void Check_mainhead_Firing_Ontime()
{
	if((Coil_MainHead_Firing==1)&&(Mainhead_Firing_Flag==0))
	{
		Mainhead_Firing_Flag=1;
	}
	else if((Coil_MainHead_Firing==0)&&(Mainhead_Firing_Flag==1))
	{
		Mainhead_Firing_Flag = 0;
	}

	if((Coil_MainHead_OnTime==1)&&(Mainhead_Ontime_Flag==0))
	{
		Mainhead_Ontime_Flag = 1;
	}
	else if((Coil_MainHead_OnTime==0)&&(Mainhead_Ontime_Flag==1))
	{
		Mainhead_Ontime_Flag = 0;
	}
}

void Mhs_Start_Stop()
{
		if(LEVEL_SENSOR == 0 && mhs_on_flag == 0 && mhs_start_counter == 0 )
		{
			mhs_on_flag       = 1;
			mhs_start_counter = 0;
		}

		if(mhs_start_counter >= 200 && LEVEL_SENSOR == 0 )
		{
			mhs_on_flag = 0;
			MHS_ON_OFF  = 1;
		}

		if(mhs_start_counter >= 200 && LEVEL_SENSOR == 1 && mhs_off_flag == 0 )
		{
			mhs_off_flag = 1;

		}

		if(mhs_stop_counter>=200)
		{
			MHS_ON_OFF        = 0;
			mhs_on_flag       = 0;
			mhs_off_flag      = 0;
			mhs_start_counter = 0;
			mhs_stop_counter  = 0;
		}
}

void write_bit_as_word()
{
	BAW_1.Bit_Val.Bit0 = Coil_Head1Button; //Head 1
	BAW_1.Bit_Val.Bit1 = Coil_Head2Button; //Head 2
	BAW_1.Bit_Val.Bit2 = Coil_Head3Button; //Head 3
	BAW_1.Bit_Val.Bit3 = Coil_Head4Button; //Head 4
	BAW_1.Bit_Val.Bit4 = Coil_Head5Button; //Head 5
	BAW_1.Bit_Val.Bit5 = Coil_Head6Button; //Head 6
	BAW_1.Bit_Val.Bit6 = Coil_Head7Button; //Head 7
	BAW_1.Bit_Val.Bit7 = Coil_Head8Button; //Head 8
	BAW_1.Bit_Val.Bit8 = Coil_Head9Button; //Head 9
	BAW_1.Bit_Val.Bit9 = Coil_Head10Button; //Head 10
	BAW_1.Bit_Val.Bit10 = Coil_Head15Button; //Auxillary Mode
	BAW_1.Bit_Val.Bit11 = Coil_AllHeadsButton; //All Head Mode
	gs_test_w_buff[109] = BAW_1.Word_Val; //All Head Modes
}

void read_bit_as_word()
{
	BAW_1.Word_Val = gs_test_r_buff[109]; //All Head Modes
	Coil_Head1Button = BAW_1.Bit_Val.Bit0; //Head 1
	Coil_Head2Button = BAW_1.Bit_Val.Bit1; //Head 2
	Coil_Head3Button = BAW_1.Bit_Val.Bit2; //Head 3
	Coil_Head4Button = BAW_1.Bit_Val.Bit3; //Head 4
	Coil_Head5Button = BAW_1.Bit_Val.Bit4; //Head 5
	Coil_Head6Button = BAW_1.Bit_Val.Bit5; //Head 6
	Coil_Head7Button = BAW_1.Bit_Val.Bit6; //Head 7
	Coil_Head8Button = BAW_1.Bit_Val.Bit7; //Head 8
	Coil_Head9Button = BAW_1.Bit_Val.Bit8; //Head 9
	Coil_Head10Button = BAW_1.Bit_Val.Bit9; //Head 10
	Coil_Head15Button = BAW_1.Bit_Val.Bit10; //Auxillary Mode
	Coil_AllHeadsButton = BAW_1.Bit_Val.Bit11; //All Head Mode
}

/*void Set_Stepper_Pattern()
{
	   TMP_COUNT = 0;
	    lp_ctr0=0;
	    lp_ctr1=1;
	    for(;;)
	    {


	        Target_Wt_ULimit = Target_Wt+ Wt_Band ;
	        Target_Wt_LLimit = Target_Wt -Wt_Band;
	        aa = 0;
	        Last_found_wt =0;
	        for(lp_ctr0 = 1;lp_ctr0 <= 0x03fff ;lp_ctr0++)
	            {
	               tmp_ptr0 =1;
	               tmp_wt =0;
	               for(lp_ctr1 =0;lp_ctr1 <14;lp_ctr1++)
	                 {
	                   if ((lp_ctr0 & tmp_ptr0) !=0)
	                   {
	                     tmp_wt = tmp_wt + Headi[lp_ctr1].Wt_Count;

	                    }
	                    //if(tmp_wt > Target_Wt_ULimit)
	                   // {
	                     //break;
	                    //    ;
	                   // }
	                    tmp_ptr0 = tmp_ptr0 << 1 ;

	                 }


	                if ((labs(tmp_wt - Target_Wt)) <= Wt_Band)
	                {
	                  if((labs(Target_Wt - tmp_wt)) < (labs(Target_Wt - Last_found_wt)))
	                   {
	                    aa= lp_ctr0;
	                    tmp_ptr0 =0;
	                    Last_found_wt = tmp_wt;

	                   }

	                }

	            }
	    TMP_COUNT =TMP_COUNT+1;
	    combi_time_flag = 0;
	    }
}*/


