/*

 * runcycle_code.c
 *
 *  Created on: Feb 17, 2022
 *      Author: Dnyaneshwar
 */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "modbus_variables.h"

#include "global_variables.h"
#include "global_functions.h"
#include "global_defines.h"

#include "runcycle_variables.h"
#include "runcycle_functions.h"

#include "manual_functions.h"

#include "delay_functions.h"

void Run_Operation(void)
{

	Clear_Watchdog();

	unsigned char run_current_head;

	if(Power_On_Run == 0)
	{
		Drain();
		//Power_On_Run = 1;
	}
	//else if(cycle_equal_time_flag == 0 && Power_On_Run == 1 && next_run_flag == 0 && auxbucket_empty_flag == 0 && MHW_READY == 0)
	else if(cycle_equal_time_flag == 0 && Power_On_Run == 1 && ready_on_flag == 0 && next_run_flag == 0)
	{
		mS_Delay(500);
		combination_weight = 0;
		combination_weight_flag = 0;
	//	auxbucket_empty_flag = 1;
		Head_counter = 1;
		cycle_equal_time      = (RunCycleTime*2);
		cycle_equal_time_flag = 1;
		get_weight_counter    = 0;
		combi_cycle_flag      = 0;
		weight_flag           = 1;
		error_wt_flag         = 0;
		aux_frame_once        = 0;
		WT1=WT2=WT3=WT4=WT5=WT6=WT7=WT8=0;
		BandErrorWt           = 0;
		ErrorWt               = 0;
		Weight_Selected       = Selected_Heads;
		BucketWt[0] = BucketWt[1] = BucketWt[2] = BucketWt[3] = BucketWt[4] = BucketWt[5] = BucketWt[6] = 0;
		BucketWt[7] = BucketWt[8] = BucketWt[9] = BucketWt[10] = BucketWt[11] = BucketWt[12] = BucketWt[13] = 0;
		//BucketWt[7] = BucketWt[8] = BucketWt[9] = 0;
	}

	if((Weight_Selected)&&(get_weight_counter!=MAX_GET_WEIGHT_COUNTER)&&(weight_flag==1) && next_run_flag == 0 && ready_on_flag == 0)
	{
		    Clear_Watchdog();

			run_current_head=(Weight_Selected>>Head_counter)&0X1;

			if(run_current_head)
			{
				Clear_Watchdog();
				Run_Get_weight();
				mS_Delay(5);
				Head_counter = Head_counter+1;
				get_weight_counter++;
			}
			else
			{
				Head_counter = Head_counter+1;
				get_weight_counter++;
			}

			if((Head_counter>MAX_HEADS))
			{
				Head_counter = 1;
			}
	}

	if(get_weight_counter == MAX_GET_WEIGHT_COUNTER && combi_cycle_flag == 0 && next_run_flag == 0 )
	{

		//Trigger_For_Data_History = 1;
		//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

		Check_Bucket_Status();

		weight_flag = 0;

		Head[0]    = 0;
		Head[1]    = 0;
		Head[2]    = 0;
		Head[3]    = 0;
		Head[4]    = 0;
		Head[5]    = 0;
		Head[6]    = 0;
		Head[7]    = 0;
		Head[8]    = 0;
		Head[9]    = 0;

		Stable_Head_Counter = 0;

		if(Coil_No_Select_Mode == 1)
		{
			if(head1_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 1;
			}
			if(head2_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 2;
			}
			if(head3_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 3;
			}
			if(head4_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 4;
			}
			if(head5_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 5;
			}
			if(head6_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 6;
			}
			if(head7_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 7;
			}
			if(head8_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 8;
			}
			if(head9_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 9;
			}
			if(head10_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 10;
			}
			if(head11_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 11;
			}
			if(head12_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 12;
			}
			if(head13_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 13;
			}
			if(head14_not_in_combi_flag == 1 )
			{
				not_in_combi_count++;
				head_number = 14;
			}
		}

		if(Head1_Stable==1 && skip_for_combination_1 == 0 && skip_for_recharge_1 == 0)
			Stable_Head_Counter++;
		if(Head2_Stable==1 && skip_for_combination_2 == 0 && skip_for_recharge_2 == 0)
			Stable_Head_Counter++;
		if(Head3_Stable==1 && skip_for_combination_3 == 0 && skip_for_recharge_3 == 0)
			Stable_Head_Counter++;
		if(Head4_Stable==1 && skip_for_combination_4 == 0 && skip_for_recharge_4 == 0)
			Stable_Head_Counter++;
		if(Head5_Stable==1 && skip_for_combination_5 == 0 && skip_for_recharge_5 == 0)
			Stable_Head_Counter++;
		if(Head6_Stable==1 && skip_for_combination_6 == 0 && skip_for_recharge_6 == 0)
			Stable_Head_Counter++;
		if(Head7_Stable==1 && skip_for_combination_7 == 0 && skip_for_recharge_7 == 0)
			Stable_Head_Counter++;
		if(Head8_Stable==1 && skip_for_combination_8 == 0 && skip_for_recharge_8 == 0)
			Stable_Head_Counter++;
		if(Head9_Stable==1 && skip_for_combination_9 == 0 && skip_for_recharge_9 == 0)
			Stable_Head_Counter++;
		if(Head10_Stable==1 && skip_for_combination_10 == 0 && skip_for_recharge_10 == 0)
			Stable_Head_Counter++;
		if(Head11_Stable==1 && skip_for_combination_11 == 0 && skip_for_recharge_11 == 0)
			Stable_Head_Counter++;
		if(Head12_Stable==1 && skip_for_combination_12 == 0 && skip_for_recharge_12 == 0)
			Stable_Head_Counter++;
		if(Head13_Stable==1 && skip_for_combination_13 == 0 && skip_for_recharge_13 == 0)
			Stable_Head_Counter++;
		if(Head14_Stable==1 && skip_for_combination_14 == 0 && skip_for_recharge_14 == 0)
			Stable_Head_Counter++;

		//mS_Delay(50);

		records[21] = Stable_Head_Counter;

		Check_Combination();

		if (combination_ok_flag == 0 && Coil_Under_Over == 1 && Stable_Head_Counter == 10 )
		{
			under_over_combination_ok_flag = 0;
			WT1=WT2=WT3=WT4=WT5=WT6=WT7=WT8=0;
			BandErrorWt           = 0;
			ErrorWt               = 0;
			combination_weight_flag  = 0;
			combination_weight = 0;
			PosErrorWt = under_over_PosErrorWt;
			NegErrorWt = under_over_NegErrorWt;
			Check_Combination();
			OVER_UNDER_WT = 1;
			over_weight_popup = 1;
		}
		else if(combination_ok_flag == 0 && Coil_No_Select_Mode == 1 && not_in_combi_count != 0 && Stable_Head_Counter == 10)
		{
			WT1=WT2=WT3=WT4=WT5=WT6=WT7=WT8=0;
			combination_weight_flag  = 0;
			combination_weight = 0;
			BandErrorWt           = 0;
			ErrorWt               = 0;

			/*if(not_in_combi_count == 1 && (bucket_no[head_number-1]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				Trigger_For_Data_History = 1;
				Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = bucket_no[head_number-1];
				Head[0] = head_number;
			}*/

			if((head1_not_in_combi_flag == 1)&&(BucketWt[0]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[0];
				Head[0] = 1;
			}
			else if((head2_not_in_combi_flag == 1)&&(BucketWt[1]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[1];
				Head[0] = 2;
			}
			else if((head3_not_in_combi_flag == 1)&&(BucketWt[2]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[2];
				Head[0] = 3;
			}
			else if((head4_not_in_combi_flag == 1)&&(BucketWt[3]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[3];
				Head[0] = 4;
			}
			else if((head5_not_in_combi_flag == 1)&&(BucketWt[4]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[4];
				Head[0] = 5;
			}
			else if((head6_not_in_combi_flag == 1)&&(BucketWt[5]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[5];
				Head[0] = 6;
			}
			else if((head7_not_in_combi_flag == 1)&&(BucketWt[6]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[6];
				Head[0] = 7;
			}
			else if((head8_not_in_combi_flag == 1)&&(BucketWt[7]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//+- = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[7];
				Head[0] = 8;
			}
			else if((head9_not_in_combi_flag == 1)&&(BucketWt[8]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[8];
				Head[0] = 9;
			}
			else if((head10_not_in_combi_flag == 1)&&(BucketWt[9]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[9];
				Head[0] = 10;
			}
			else if((head11_not_in_combi_flag == 1)&&(BucketWt[10]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[10];
				Head[0] = 11;
			}
			else if((head12_not_in_combi_flag == 1)&&(BucketWt[11]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[11];
				Head[0] = 12;
			}
			else if((head13_not_in_combi_flag == 1)&&(BucketWt[12]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[12];
				Head[0] = 13;
			}
			else if((head14_not_in_combi_flag == 1)&&(BucketWt[13]>=NewTargetWt))
			{
				combination_ok_flag = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = BucketWt[13];
				Head[0] = 14;
			}
			else if(combination_ok_flag == 0)
			{
				if(head1_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 1;
					No_Select_Combination();
				}
				if(head2_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 2;
					No_Select_Combination();
				}
				if(head3_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 3;
					No_Select_Combination();
				}
				if(head4_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 4;
				}
				if(head5_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 5;
					No_Select_Combination();
				}
				if(head6_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 6;
					No_Select_Combination();
				}
				if(head7_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 7;
				}
				if(head8_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 8;
					No_Select_Combination();
				}
				if(head9_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 9;
					No_Select_Combination();
				}
				if(head10_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 10;
					No_Select_Combination();
				}
				if(head11_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 11;
					No_Select_Combination();
				}
				if(head12_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 12;
					No_Select_Combination();
				}
				if(head13_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 13;
					No_Select_Combination();
				}
				if(head14_not_in_combi_flag == 1 )
				{
					not_in_combi_count++;
					head_number = 14;
					No_Select_Combination();
				}
			}

	}

		if(combination_ok_flag == 1 || under_over_combination_ok_flag == 1 )
		{
	 		Clear_Watchdog();
	 	//	auxbucket_empty_flag = 1;

			if(skip_for_combination_1 == 1)
				skip_for_combination_1 = 0;
			if(skip_for_combination_2 == 1)
				skip_for_combination_2 = 0;
			if(skip_for_combination_3 == 1)
				skip_for_combination_3 = 0;
			if(skip_for_combination_4 == 1)
				skip_for_combination_4 = 0;
			if(skip_for_combination_5 == 1)
				skip_for_combination_5 = 0;
			if(skip_for_combination_6 == 1)
				skip_for_combination_6 = 0;
			if(skip_for_combination_7 == 1)
				skip_for_combination_7 = 0;
			if(skip_for_combination_8 == 1)
				skip_for_combination_8 = 0;
			if(skip_for_combination_9 == 1)
				skip_for_combination_9 = 0;
			if(skip_for_combination_10 == 1)
				skip_for_combination_10 = 0;
			if(skip_for_combination_11 == 1)
				skip_for_combination_11 = 0;
			if(skip_for_combination_12 == 1)
				skip_for_combination_12 = 0;
			if(skip_for_combination_13 == 1)
				skip_for_combination_13 = 0;
			if(skip_for_combination_14 == 1)
				skip_for_combination_14 = 0;

			if(skip_for_recharge_1 == 1)
				skip_for_recharge_1 = 0;
			if(skip_for_recharge_2 == 1)
				skip_for_recharge_2 = 0;
			if(skip_for_recharge_3 == 1)
				skip_for_recharge_3 = 0;
			if(skip_for_recharge_4 == 1)
				skip_for_recharge_4 = 0;
			if(skip_for_recharge_5 == 1)
				skip_for_recharge_5 = 0;
			if(skip_for_recharge_6 == 1)
				skip_for_recharge_6 = 0;
			if(skip_for_recharge_7 == 1)
				skip_for_recharge_7 = 0;
			if(skip_for_recharge_8 == 1)
				skip_for_recharge_8 = 0;
			if(skip_for_recharge_9 == 1)
				skip_for_recharge_9 = 0;
			if(skip_for_recharge_10 == 1)
				skip_for_recharge_10 = 0;
			if(skip_for_recharge_11 == 1)
				skip_for_recharge_11 = 0;
			if(skip_for_recharge_12 == 1)
				skip_for_recharge_12 = 0;
			if(skip_for_recharge_13 == 1)
				skip_for_recharge_13 = 0;
			if(skip_for_recharge_14 == 1)
				skip_for_recharge_14 = 0;

			Clear_Watchdog();

			for(jidx=0;jidx<8;jidx++)
			{
				for(iidx=15; iidx>0; iidx--)
				{
					if(Head[jidx]==iidx)
					{
						Heads_In_Combination();
					}
					/*else
					{
						if(Coil_No_Select_Mode == 1)
						{
							Dump_Cycle_To_Reject();
						}
					}*/
				}
			}
			Clear_Watchdog();

		//	mS_Delay(5);

			for(iidx=0;iidx<14;iidx++)
			{
				Recharge_Head();
			}

	 		if(Multi_Dump_Count==0 && multi_dump_flag == 1 && aux_at_homing == 1)
	 			MHW_READY = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

	 	//	else if(multi_dump_flag == 1 && Multi_Dump_Count == (MULTI_DUMP_DIV_COUNT-1))
	 	//		MHW_READY = 1;

	 		if(multi_dump_flag == 0 && aux_at_homing == 1 && Coil_AuxBucketButton==0)
	 			MHW_READY = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

	 		records[17] = DisplayWt;

	 		records[15] = DisplayWt;

	 		Trigger_For_Data_History = 1;					//28/07/2023
			Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;  //28/07/2023


	 		if(multi_dump_flag == 0)
				Avg_weight += DisplayWt;
			else if(Multi_Dump_Count<MULTI_DUMP_DIV_COUNT)
			{
				multi_dump_total_wt += DisplayWt;
			}

			if(Multi_Dump_Count==(MULTI_DUMP_DIV_COUNT-1))
			{
				Avg_weight += multi_dump_total_wt;
			//	multi_dump_total_wt = 0;
			}


			if(multi_dump_flag == 0)
			{
				Calculate_Weights_Counter();
			}
			else if(Multi_Dump_Count==(MULTI_DUMP_DIV_COUNT-1))
			{
				Calculate_Multidump_Weights_Counter();
			}

			if(Run_Dump_Counter == 0 || wt_change == 1 || Min_Wt == 0 || Max_Wt == 0 )
			{
				Clear_Watchdog();
				if(multi_dump_flag == 0)
				{
					Min_Wt = DisplayWt;
					Max_Wt = DisplayWt;

					records[174] = Min_Wt;
					records[175] = Max_Wt;
					//Avg_weight = 0;
				}
				else if(Multi_Dump_Count==(MULTI_DUMP_DIV_COUNT-1))
				{
					Min_Wt = multi_dump_total_wt;
					Max_Wt = multi_dump_total_wt;

					records[174] = multi_dump_total_wt;
					records[175] = multi_dump_total_wt;
					//Avg_weight = 0;
				}
			}
			else
			{
				Clear_Watchdog();
				if(multi_dump_flag == 0)
				{
					if(DisplayWt<Min_Wt)
						Min_Wt = DisplayWt;
					else if(DisplayWt>Max_Wt)
						Max_Wt = DisplayWt;

					records[174] = Min_Wt;
					records[175] = Max_Wt;
				}
				else if(Multi_Dump_Count==(MULTI_DUMP_DIV_COUNT-1))
				{
					if(multi_dump_total_wt<Min_Wt)
						Min_Wt = multi_dump_total_wt;
					else if(multi_dump_total_wt>Max_Wt)
						Max_Wt = multi_dump_total_wt;

					records[174] = Min_Wt;
					records[175] = Max_Wt;
				}
			}


	 		if(Run_Dump_Counter == 0 || wt_change == 1 || Min_Wt == 0 || Max_Wt == 0 )
			//if(Run_Dump_Counter == 0 || wt_change == 1)// || Min_Wt == 0 || Max_Wt == 0 )
			{
	 			Clear_Watchdog();

	 			if(multi_dump_flag == 0)
	 				Disp_Avg_weight = DisplayWt;
	 			else if((multi_dump_flag == 1) && (Multi_Dump_Count==(MULTI_DUMP_DIV_COUNT-1)))
	 				Disp_Avg_weight = multi_dump_total_wt;

	 		//	if(wt_change == 1 && Run_Dump_Counter!= 0)
	 				Avg_weight = 0;

	 			Run_Dump_Counter = 0;
	 			Sec_Counter      = 0;
	 			Clear_Watchdog();
	 			for(dev_l=0;dev_l<MAX_DEVIATION_COUNT;dev_l++)
				{
					Avg_Dev_weights[dev_l] = 0;
					Dev_weights[dev_l]     = 0;
				}

	 			sum_devi_wt  = 0;
				total_dev_wt = 0;
				dev_counter  = 0;
				dev_i        = 0;
	 			wt_change      = 0;
	 			wt_change_flag = 0;
	 		}
	 		else
	 		{
	 			if(multi_dump_flag == 0)
	 				Disp_Avg_weight = Avg_weight/(Run_Dump_Counter);
	 			else if(Multi_Dump_Count==(MULTI_DUMP_DIV_COUNT-1))
	 				Disp_Avg_weight = Avg_weight/(Run_Dump_Counter);
	 		}

	 		records[176] = Disp_Avg_weight;

	 		if((multi_dump_flag == 0)||(Multi_Dump_Count==(MULTI_DUMP_DIV_COUNT-1)))
	 		{
				if(dev_counter<MAX_DEVIATION_COUNT)
				{
					if(multi_dump_flag == 0)
						Dev_weights[dev_i] = DisplayWt;
					else
						Dev_weights[dev_i] = multi_dump_total_wt;

					dev_i++;
					dev_counter++;
				}
				else
				{
					Clear_Watchdog();

					for(dev_j=0;dev_j<MAX_DEVIATION_COUNT;dev_j++)
					{
						if(dev_j == (MAX_DEVIATION_COUNT-1))
						{
							if(multi_dump_flag == 0)
								Dev_weights[dev_j] = DisplayWt;
							else
								Dev_weights[dev_j] = multi_dump_total_wt;
						}
						else
							Dev_weights[dev_j] = Dev_weights[dev_j+1];
					}

				}
	 		}

	 		if(multi_dump_flag == 1)
	 			Multi_dump_display_wt += DisplayWt;

	 		Show_Head_Status();

	 		Clear_Watchdog();

			for(avg=0;avg<8;avg++)
			{
				if(Head[avg] != 0)
					Combi_Head_Counter++;
			}

			if(stagger_flag == 1)
			{
				stagger_count     = Combi_Head_Counter;
				stagger_time      = MAX_STAGGER_TIME/stagger_count;
				stagger_wait_time = 0;
			}

		/*	if(Head1_Stable==1)
				Stable_Head_Counter++;
			if(Head2_Stable==1)
				Stable_Head_Counter++;
			if(Head3_Stable==1)
				Stable_Head_Counter++;
			if(Head4_Stable==1)
				Stable_Head_Counter++;
			if(Head5_Stable==1)
				Stable_Head_Counter++;
			if(Head6_Stable==1)
				Stable_Head_Counter++;
			if(Head7_Stable==1)
				Stable_Head_Counter++;
			if(Head8_Stable==1)
				Stable_Head_Counter++;
			if(Head9_Stable==1)
				Stable_Head_Counter++;
			if(Head10_Stable==1)
				Stable_Head_Counter++;
			if(Head11_Stable==1)
				Stable_Head_Counter++;
			if(Head12_Stable==1)
				Stable_Head_Counter++;
			if(Head13_Stable==1)
				Stable_Head_Counter++;
			if(Head14_Stable==1)
				Stable_Head_Counter++;*/

			next_run_flag = 1;

			if(Coil_AuxBucketButton)
				next_combination_ready_flag = 1;

			if(Coil_AuxBucketButton)
				Calculate_Speed();

			Flag_Check_Combi = 1;


			if(Coil_AuxBucketButton == 0)
			{
				MHW_READY = 1;
				Coil_CombiReady = 1;
				//Trigger_For_Data_History = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
			}

			if(Coil_AuxBucketButton == 0)
				hold_run_cycle_tmr = 1;

			//if(previous_dump_flag == 0)
				//previous_dump_flag = 1;

			//Create_Auxdump_Recharge_Frame();

			//mS_Delay(5);

			//ready_on_flag = 1;
			//ready_time = All_head_wb_ondelay + All_head_wb_ontime;
			abort_recharge_frame_flag = 1;


		}
	 	else
	 	{
			Clear_Watchdog();

		//	auxbucket_empty_flag = 0;

		//	Coil_CombiReady	 = 0;

			if(skip_for_combination_1 == 1)
				skip_for_combination_1 = 0;
			if(skip_for_combination_2 == 1)
				skip_for_combination_2 = 0;
			if(skip_for_combination_3 == 1)
				skip_for_combination_3 = 0;
			if(skip_for_combination_4 == 1)
				skip_for_combination_4 = 0;
			if(skip_for_combination_5 == 1)
				skip_for_combination_5 = 0;
			if(skip_for_combination_6 == 1)
				skip_for_combination_6 = 0;
			if(skip_for_combination_7 == 1)
				skip_for_combination_7 = 0;
			if(skip_for_combination_8 == 1)
				skip_for_combination_8 = 0;
			if(skip_for_combination_9 == 1)
				skip_for_combination_9 = 0;
			if(skip_for_combination_10 == 1)
				skip_for_combination_10 = 0;
			if(skip_for_combination_11 == 1)
				skip_for_combination_11 = 0;
			if(skip_for_combination_12 == 1)
				skip_for_combination_12 = 0;
			if(skip_for_combination_13 == 1)
				skip_for_combination_13 = 0;
			if(skip_for_combination_14 == 1)
				skip_for_combination_14 = 0;

			if(skip_for_recharge_1 == 1)
				skip_for_recharge_1 = 0;
			if(skip_for_recharge_2 == 1)
				skip_for_recharge_2 = 0;
			if(skip_for_recharge_3 == 1)
				skip_for_recharge_3 = 0;
			if(skip_for_recharge_4 == 1)
				skip_for_recharge_4 = 0;
			if(skip_for_recharge_5 == 1)
				skip_for_recharge_5 = 0;
			if(skip_for_recharge_6 == 1)
				skip_for_recharge_6 = 0;
			if(skip_for_recharge_7 == 1)
				skip_for_recharge_7 = 0;
			if(skip_for_recharge_8 == 1)
				skip_for_recharge_8 = 0;
			if(skip_for_recharge_9 == 1)
				skip_for_recharge_9 = 0;
			if(skip_for_recharge_10 == 1)
				skip_for_recharge_10 = 0;
			if(skip_for_recharge_11 == 1)
				skip_for_recharge_11 = 0;
			if(skip_for_recharge_12 == 1)
				skip_for_recharge_12 = 0;
			if(skip_for_recharge_13 == 1)
				skip_for_recharge_13 = 0;
			if(skip_for_recharge_14 == 1)
				skip_for_recharge_14 = 0;


			for(iidx=0; iidx<14; iidx++)
			{
				BucWeight  = BucketWt[iidx];

				if(multi_dump_flag == 0)
				{
					Recharge_Head();
				}
				else if(multi_dump_flag == 1)
				{
					Recharge_Head_In_MultiDump();
				}
			}

			Clear_Watchdog();

			next_run_flag = 1;
			send_reacharge_frame_flag = 1;
			Recharge_Frame_Time = 0;

			if(Coil_AuxBucketButton==0)
			{
				Send_Recharge_Frame();
			}


	 	}

	 	combi_cycle_flag=1;
	}

	//if((Coil_RunButton)&&(DUMP_TRIGGER==0) && (combi_cycle_flag==1)&&(multi_dump_flag == 0)&&(Coil_AuxBucketButton==0) && (stagger_flag == 0))
	if((Coil_RunButton)&&(combi_cycle_flag==1)&&(multi_dump_flag == 0)&&(Coil_AuxBucketButton==0) && (stagger_flag == 0)&&(auxbucket_empty_flag == 0) && (Flag_Check_Combi == 1)&&(DUMP_TRIGGER==0))
	{
			//Clear_Watchdog();

			hold_run_cycle_tmr = 0;
			Create_Dump_Recharge_Frame();
			Coil_CombiReady = 0;
			previous_dump_flag = 0;
			//mS_Delay(5);
			MHW_READY = 0;
			wt_feedback_flag = 1;
			//Trigger_For_Data_History = 1;					28/07/2023
			//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

	}
	else if((Coil_RunButton)&&(DUMP_TRIGGER==0||(Multi_Dump_Count>=1 &&  Multi_Dump_Count <(MULTI_DUMP_DIV_COUNT))) && (combi_cycle_flag==1)&&(multi_dump_flag == 1)&& (stagger_flag == 0))
	{
		//	Clear_Watchdog();Clear_Watchdog();Clear_Watchdog();Clear_Watchdog();

			hold_run_cycle_tmr = 0;

			Create_Multidump_Recharge_Frame();
	}
	else if((Coil_RunButton)&& (combi_cycle_flag==1)&&(Coil_AuxBucketButton==1)&& (auxbucket_empty_flag == 0)&&(Flag_Check_Combi==1)&& (stagger_flag == 0)&& (aux_command_flag == 0))
	{
		//	Clear_Watchdog();Clear_Watchdog();Clear_Watchdog();Clear_Watchdog();
		//	hold_run_cycle_tmr = 0;
		//	aux_ready_flag = 1;
		//	aux_ready_time = MHW_READY_TIME;
		//	Create_Auxdump_Recharge_Frame();
	}
	else if((Coil_RunButton)&&(DUMP_TRIGGER==0) && (combi_cycle_flag==1)&&(multi_dump_flag == 0)&&(Coil_AuxBucketButton==0) && (stagger_flag == 1))
	{
			Clear_Watchdog();

			hold_run_cycle_tmr = 0;

			Staggerdump_Recharge_Frame();
	}


	if(previous_dump_flag == 0)
		previous_dump_flag = 1;


	if(Coil_AuxBucketButton == 1 && next_combination_ready_flag == 1 && auxbucket_empty_flag == 0 && previous_dump_flag == 1)
	{
		next_combination_ready_flag = 0;
		auxbucket_empty_flag = 1;
		Create_Auxdump_Recharge_Frame();
		ready_time  = MHW_READY_TIME;
		ready_on_flag = 1;

		/*if(MHW_READY == 0)
			MHW_READY      = 1;*/

		//mS_Delay(5);
	}

	//if((send_reacharge_frame_flag == 1) && (Recharge_Frame_Time == RECHARGE_TIME))
	if((send_reacharge_frame_flag == 1) && (cycle_equal_time == 0) && (next_combination_ready_flag == 0)&&(next_run_flag == 1)&&(Coil_AuxBucketButton == 1))
	{
		Send_Recharge_Frame();
		mS_Delay(5);

		if(MHW_READY == 0)
			auxbucket_empty_flag = 0;
	}

	if(recharge_frame_sent_flag == 1)
	{
		Reset_Run_Parameter();
	}

	if(MHW_READY == 1 && DUMP_TRIGGER == 0 && Coil_AuxBucketButton == 1 && auxbucket_empty_flag == 1 )
	{
		MHW_READY    = 0;
		Aux_Bucket_Dump();
		Coil_CombiReady = 0;
		//previous_dump_flag = 0;
		//Trigger_For_Data_History = 1;
		//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

		mS_Delay(5);
	}

	/*if(aux_frame_sent_flag == 1)
	{
		Coil_CombiReady = 0;
		Flag_Check_Combi = 1;
		aux_frame_sent_flag = 0;
	}

	if(Flag_Check_Combi == 1 && auxbucket_empty_flag == 0 && next_run_flag == 1 )
	{
		mS_Delay(5);
		Create_Auxdump_Recharge_Frame();
		mS_Delay(5);
	}*/

	if(dump_frame_sent_flag == 1)
	{
		Reset_Run_Parameter();
		mS_Delay(5);
	}

}


void Check_Combination(void)
{
	Clear_Watchdog();

	for(combi_i = 1;combi_i<MAX_HEADS;combi_i++)
	{
		if( BucketWt[combi_i-1] > 0)
		{
			all_combi_wt += BucketWt[combi_i-1];
		}
	}

	if(((all_combi_wt>=NewTargetWt)&&(all_combi_wt<=(NewTargetWt+PosErrorWt))) || ((all_combi_wt<=NewTargetWt)&&(all_combi_wt>=(NewTargetWt-NegErrorWt)))||(all_combi_wt==NewTargetWt))
	{
		combination_weight = all_combi_wt;
		combination_ok_flag = 1;
		//Trigger_For_Data_History = 1;
		//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

		DisplayWt = all_combi_wt;

			Head[0] = 1;
			Head[1] = 1;
			Head[2] = 1;
			Head[3] = 1;
			Head[4] = 1;
			Head[5] = 1;
			Head[6] = 1;
			Head[7] = 1;
			Head[8] = 1;
			Head[9] = 1;
			Head[10] = 1;
			Head[11] = 1;
			Head[12] = 1;
			Head[13] = 1;


	}


	for(p=1;p<=MAX_HEADS;p++)
	{
		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}

		if(multi_dump_flag == 0)
		{
			if(NewTargetWt>WT1)
			{
				ErrorWt = NewTargetWt - WT1;
			}
			else
			{
				ErrorWt = WT1 - NewTargetWt;
			}
		}
		else if(multi_dump_flag == 1)
		{
			if(Multi_Dump_Wt>WT1)
			{
				ErrorWt = Multi_Dump_Wt - WT1;
			}

			else
			{
				ErrorWt = WT1 - Multi_Dump_Wt;
			}

		}

		if(multi_dump_flag == 0)
		{
			if(((WT1>=NewTargetWt)&&(WT1<=(NewTargetWt+PosErrorWt))) || ((WT1<=NewTargetWt)&&(WT1>=(NewTargetWt-NegErrorWt)))||(WT1==NewTargetWt))
			{
				//if((ErrorWt<BandErrorWt) || error_wt_flag == 0)
				if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT1<combination_weight))
				{
					combination_weight_flag  = 1;
					combination_weight = WT1;
				//{
					error_wt_flag = 1;
					BandErrorWt = ErrorWt;
					//auxbucket_empty_flag = 1;
					combination_ok_flag = 1;
					under_over_combination_ok_flag = 1;
				//	Flag_Check_Combi = 1;
					//Trigger_For_Data_History = 1;
					//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

					DisplayWt = WT1;

					if(p1 == 0)
						Head[0] = p;
					if(q1 == 0)
						Head[1] = 0;
					if(r1 == 0)
						Head[2] = 0;
					if(s1 == 0)
						Head[3] = 0;
					if(t1 == 0)
						Head[4] = 0;
					if(u1 == 0)
						Head[5] = 0;
					if(v1 == 0)
						Head[6] = 0;
					if(w1 == 0)
						Head[7] = 0;
				}
			}
			else
			{
				if(error_wt_flag == 0)
				{
					BandErrorWt = ErrorWt;
					error_wt_flag = 1;
				}
				else if(ErrorWt<BandErrorWt)
				{
					BandErrorWt = ErrorWt;
				}
			}
		}
		else if(multi_dump_flag == 1)
		{
			if(((WT1>=Multi_Dump_Wt)&&(WT1<=(Multi_Dump_Wt+PosErrorWt))) || ((WT1<=Multi_Dump_Wt)&&(WT1>=(Multi_Dump_Wt-NegErrorWt)))||(WT1==Multi_Dump_Wt))
			{
				if((ErrorWt<BandErrorWt) || error_wt_flag == 0)
				{
					error_wt_flag = 1;
					BandErrorWt = ErrorWt;
					combination_ok_flag = 1;
					under_over_combination_ok_flag = 1;
					//Flag_Check_Combi = 1;
					//Trigger_For_Data_History = 1;
					//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

					DisplayWt = WT1;

					Head[0] = p;
					Head[1] = 0;
					Head[2] = 0;
					Head[3] = 0;
					Head[4] = 0;
					Head[5] = 0;
					Head[6] = 0;
					Head[7] = 0;
				}
			}
			else
			{
				if(error_wt_flag == 0)
				{
					BandErrorWt = ErrorWt;
					error_wt_flag = 1;
				}
				else if(ErrorWt<BandErrorWt)
				{
					BandErrorWt = ErrorWt;
				}
			}

		}
	}

	Clear_Watchdog();

	for(p=1;p<=MAX_HEADS;p++)
	{
		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}

		for(q=p+1;q<=MAX_HEADS;q++)
		{
			if(bucket_no[q-1]==1)
			{
				WT2 = WT1+BucketWt[q-1];
				q1 = 0;
			}
			else
				q1 = 1;


			if(multi_dump_flag == 0)
			{
				if(NewTargetWt>WT2)
				{
					ErrorWt = NewTargetWt - WT2;
				}
				else
				{
					ErrorWt = WT2 - NewTargetWt;
				}
		    }
			else if(multi_dump_flag == 1)
			 {
				if(Multi_Dump_Wt>WT2)
				{
					ErrorWt = Multi_Dump_Wt - WT2;
				}

				else
				{
					ErrorWt = WT2 - Multi_Dump_Wt;
				}

			 }



			if(multi_dump_flag == 0)
			{

				if(((WT2>=NewTargetWt)&&(WT2<=(NewTargetWt+PosErrorWt))) || ((WT2<=NewTargetWt)&&(WT2>=(NewTargetWt-NegErrorWt)))||(WT2==NewTargetWt))
				{
					 if(ErrorWt<BandErrorWt)
					 {
						BandErrorWt = ErrorWt;
					 }

					 if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT2<combination_weight))
						{
							combination_weight_flag  = 1;
							combination_weight = WT2;
						combination_ok_flag = 1;
						under_over_combination_ok_flag = 1;
					//	auxbucket_empty_flag = 1;
				//		Flag_Check_Combi = 1;
						//Trigger_For_Data_History = 1;
						//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
						DisplayWt = WT2;


						if(p1 == 0)
							Head[0] = p;
						if(q1 == 0)
							Head[1] = q;
						if(r1 == 0)
							Head[2] = 0;
						if(s1 == 0)
							Head[3] = 0;
						if(t1 == 0)
							Head[4] = 0;
						if(u1 == 0)
							Head[5] = 0;
						if(v1 == 0)
							Head[6] = 0;
						if(w1 == 0)
							Head[7] = 0;
					}
				}
				else if(ErrorWt<BandErrorWt)
				{
					BandErrorWt = ErrorWt;
				}
			}
			else if(multi_dump_flag == 1)
			{
				if(((WT2>=Multi_Dump_Wt)&&(WT2<=(Multi_Dump_Wt+PosErrorWt))) || ((WT2<=Multi_Dump_Wt)&&(WT2>=(Multi_Dump_Wt-NegErrorWt)))||(WT2==Multi_Dump_Wt))
				{
					 if(ErrorWt<BandErrorWt)
					 {
						BandErrorWt = ErrorWt;
						combination_ok_flag = 1;
						under_over_combination_ok_flag = 1;
					//	Flag_Check_Combi = 1;
						//Trigger_For_Data_History = 1;
						//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
						DisplayWt = WT2;

						Head[0] = p;
						Head[1] = q;
						Head[2] = 0;
						Head[3] = 0;
						Head[4] = 0;
						Head[5] = 0;
						Head[6] = 0;
						Head[7] = 0;
					}
				}
				else if(ErrorWt<BandErrorWt)
				{
					BandErrorWt = ErrorWt;
				}
			}

		}

	}

	Clear_Watchdog();

	for(p=1;p<=MAX_HEADS;p++)
	{
		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}

		for(q=p+1;q<=MAX_HEADS;q++)
		{
			if(bucket_no[q-1]==1)
			{
				WT2 = WT1+BucketWt[q-1];
				q1 = 0;
			}
			else
			{
				WT2 = 0;
				q1 = 1;
			}

			for(r=q+1;r<=MAX_HEADS;r++)
			{
				if(bucket_no[r-1]==1)
				{
					WT3 = WT2+BucketWt[r-1];
					r1 = 0;
				}
				else
					r1 = 1;


				if(multi_dump_flag == 0)
				{
					if(NewTargetWt>WT3)
					{
						ErrorWt = NewTargetWt - WT3;
					}
					else
					{
						ErrorWt = WT3 - NewTargetWt;
					}
				}
				else if(multi_dump_flag == 1)
				 {
					if(Multi_Dump_Wt>WT3)
					{
						ErrorWt = Multi_Dump_Wt - WT3;
					}

					else
					{
						ErrorWt = WT3 - Multi_Dump_Wt;
					}
				 }

				if(multi_dump_flag == 0)
				{
					if(((WT3>=NewTargetWt)&&(WT3<=(NewTargetWt+PosErrorWt))) || ((WT3<=NewTargetWt)&&(WT3>=(NewTargetWt-NegErrorWt)))||(WT3==NewTargetWt))
					{

						if(ErrorWt<BandErrorWt)
						{

							BandErrorWt = ErrorWt;
						}

						if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT3<combination_weight))
						{
							combination_weight_flag  = 1;
							combination_weight = WT3;
						//	auxbucket_empty_flag = 1;
							combination_ok_flag = 1;
							under_over_combination_ok_flag = 1;
						//	Flag_Check_Combi = 1;
							//Trigger_For_Data_History = 1;
							//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
							DisplayWt = WT3;

							if(p1 == 0)
								Head[0] = p;
							if(q1 == 0)
								Head[1] = q;
							if(r1 == 0)
								Head[2] = r;
							if(s1 == 0)
								Head[3] = 0;
							if(t1 == 0)
								Head[4] = 0;
							if(u1 == 0)
								Head[5] = 0;
							if(v1 == 0)
								Head[6] = 0;
							if(w1 == 0)
								Head[7] = 0;
						}
					}
					else if(ErrorWt<BandErrorWt)
					{
						BandErrorWt = ErrorWt;
					}
				}
				else if(multi_dump_flag == 1)
				{
					if(((WT3>=Multi_Dump_Wt)&&(WT3<=(Multi_Dump_Wt+PosErrorWt))) || ((WT3<=Multi_Dump_Wt)&&(WT3>=(Multi_Dump_Wt-NegErrorWt)))||(WT3==Multi_Dump_Wt))
					{

						if(ErrorWt<BandErrorWt)
						{

							BandErrorWt = ErrorWt;

							combination_ok_flag = 1;
							under_over_combination_ok_flag = 1;
					//		Flag_Check_Combi = 1;
							//Trigger_For_Data_History = 1;
							//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
							DisplayWt = WT3;

							Head[0] = p;
							Head[1] = q;
							Head[2] = r;
							Head[3] = 0;
							Head[4] = 0;
							Head[5] = 0;
							Head[6] = 0;
							Head[7] = 0;
						}
					}
					else if(ErrorWt<BandErrorWt)
					{
						BandErrorWt = ErrorWt;
					}
				}

				}
		}

	}

	Clear_Watchdog();

	for(p=1;p<=MAX_HEADS;p++)
	{
		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}

		for(q=p+1;q<=MAX_HEADS;q++)
		{
			if(bucket_no[q-1]==1)
			{
				WT2 = WT1+BucketWt[q-1];
				q1 = 0;
			}
			else
			{
				WT2 = 0;
				q1 = 1;
			}

			for(r=q+1;r<=MAX_HEADS;r++)
			{
				if(bucket_no[r-1]==1)
				{
					WT3 = WT2+BucketWt[r-1];
					r1 = 0;
				}
				else
				{
					WT3 = 0;
					r1 = 1;
				}

				for(s=r+1;s<=MAX_HEADS;s++)
				{
					if(bucket_no[s-1]==1)
					{
						WT4 = WT3+BucketWt[s-1];
						s1 = 0;
					}
					else
						s1 = 1;

					if(multi_dump_flag == 0)
					{
						if(NewTargetWt>WT4)
						{
							ErrorWt = NewTargetWt - WT4;
						}
						else
						{
							ErrorWt = WT4 - NewTargetWt;
						}
					}
					else if(multi_dump_flag == 1)
					 {
						if(Multi_Dump_Wt>WT4)
						{
							ErrorWt = Multi_Dump_Wt - WT4;
						}

						else
						{
							ErrorWt = WT4 - Multi_Dump_Wt;
						}

					 }

					if(multi_dump_flag == 0)
					{
						if(((WT4>=NewTargetWt)&&(WT4<=(NewTargetWt+PosErrorWt))) || ((WT4<=NewTargetWt)&&(WT4>=(NewTargetWt-NegErrorWt)))||(WT4==NewTargetWt))
						{
							if(ErrorWt<BandErrorWt)
							 {
								BandErrorWt = ErrorWt;
							 }

							if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT4<combination_weight))
							{
								combination_weight_flag  = 1;
								combination_weight = WT4;
							//	auxbucket_empty_flag = 1;
								combination_ok_flag = 1;
								under_over_combination_ok_flag = 1;
							//	Flag_Check_Combi = 1;
								//Trigger_For_Data_History = 1;
								//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
								DisplayWt = WT4;


								if(p1 == 0)
									Head[0] = p;
								if(q1 == 0)
									Head[1] = q;
								if(r1 == 0)
									Head[2] = r;
								if(s1 == 0)
									Head[3] = s;
								if(t1 == 0)
									Head[4] = 0;
								if(u1 == 0)
									Head[5] = 0;
								if(v1 == 0)
									Head[6] = 0;
								if(w1 == 0)
									Head[7] = 0;
							}
						}
						else if(ErrorWt<BandErrorWt)
						{
							BandErrorWt = ErrorWt;
						}
					}
					else if(multi_dump_flag == 1)
					{
						if(((WT4>=Multi_Dump_Wt)&&(WT4<=(Multi_Dump_Wt+PosErrorWt))) || ((WT4<=Multi_Dump_Wt)&&(WT4>=(Multi_Dump_Wt-NegErrorWt)))||(WT4==Multi_Dump_Wt))
						{
							if(ErrorWt<BandErrorWt)
							 {
								BandErrorWt = ErrorWt;
								combination_ok_flag = 1;
								under_over_combination_ok_flag = 1;
							//	Flag_Check_Combi = 1;
								//Trigger_For_Data_History = 1;
								//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
								DisplayWt = WT4;

								Head[0] = p;
								Head[1] = q;
								Head[2] = r;
								Head[3] = s;
								Head[4] = 0;
								Head[5] = 0;
								Head[6] = 0;
								Head[7] = 0;
							}
						}
						else if(ErrorWt<BandErrorWt)
						{
							BandErrorWt = ErrorWt;
						}
					}

					}
			}
		}
	}

	Clear_Watchdog();
	for(p=1;p<=MAX_HEADS;p++)
	{

		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}

		for(q=p+1;q<=MAX_HEADS;q++)
		{
			if(bucket_no[q-1]==1)
			{
				WT2 = WT1+BucketWt[q-1];
				q1 = 0;
			}
			else
			{
				WT2 = 0;
				q1 = 1;
			}
			for(r=q+1;r<=MAX_HEADS;r++)
			{
				if(bucket_no[r-1]==1)
				{
					WT3 = WT2+BucketWt[r-1];
					r1 = 0;
				}
				else
				{
					WT3 = 0;
					r1 = 1;
				}

				for(s=r+1;s<=MAX_HEADS;s++)
				{
					if(bucket_no[s-1]==1)
					{
						WT4 = WT3+BucketWt[s-1];
						s1 = 0;
					}
					else
					{
						WT4 = 0;
						s1 = 1;
					}

					for(t=s+1;t<=MAX_HEADS;t++)
					{
						if(bucket_no[t-1]==1)
						{
							WT5 = WT4+BucketWt[t-1];
							t1 = 0;
						}
						else
							t1 = 1;

						if(multi_dump_flag == 0)
						{
							if(NewTargetWt>WT5)
							{
								ErrorWt = NewTargetWt - WT5;
							}
							else
							{
								ErrorWt = WT5 - NewTargetWt;
							}
					    }
						else if(multi_dump_flag == 1)
						 {
							if(Multi_Dump_Wt>WT5)
							{
								ErrorWt = Multi_Dump_Wt - WT5;
							}

							else
							{
								ErrorWt = WT5 - Multi_Dump_Wt;
							}

						 }

						if(multi_dump_flag == 0)
						{
						if(((WT5>=NewTargetWt)&&(WT5<=(NewTargetWt+PosErrorWt))) || ((WT5<=NewTargetWt)&&(WT5>=(NewTargetWt-NegErrorWt)))||(WT5==NewTargetWt))
						{
							if(ErrorWt<BandErrorWt)
							{
									BandErrorWt = ErrorWt;
							}

							if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT5<combination_weight))
							{
								combination_weight_flag  = 1;
								combination_weight = WT5;
								//	auxbucket_empty_flag = 1;
									combination_ok_flag = 1;
									under_over_combination_ok_flag = 1;
								//	Flag_Check_Combi = 1;
									//Trigger_For_Data_History = 1;
									//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
									DisplayWt = WT5;

									if(p1 == 0)
										Head[0] = p;
									if(q1 == 0)
										Head[1] = q;
									if(r1 == 0)
										Head[2] = r;
									if(s1 == 0)
										Head[3] = s;
									if(t1 == 0)
										Head[4] = t;
									if(u1 == 0)
										Head[5] = 0;
									if(v1 == 0)
										Head[6] = 0;
									if(w1 == 0)
										Head[7] = 0;
								}
							}
							else if(ErrorWt<BandErrorWt)
							{
								BandErrorWt = ErrorWt;
							}
						}
						else if(multi_dump_flag == 1)
						{
							if(((WT5>=Multi_Dump_Wt)&&(WT5<=(Multi_Dump_Wt+PosErrorWt))) || ((WT5<=Multi_Dump_Wt)&&(WT5>=(Multi_Dump_Wt-NegErrorWt)))||(WT5==Multi_Dump_Wt))
							{
								if(ErrorWt<BandErrorWt)
								{
										BandErrorWt = ErrorWt;
										combination_ok_flag = 1;
										under_over_combination_ok_flag = 1;
								//		Flag_Check_Combi = 1;
										//Trigger_For_Data_History = 1;
										//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
										DisplayWt = WT5;

										Head[0] = p;
										Head[1] = q;
										Head[2] = r;
										Head[3] = s;
										Head[4] = t;
										Head[5] = 0;
										Head[6] = 0;
										Head[7] = 0;
									}
								}
								else if(ErrorWt<BandErrorWt)
								{
									BandErrorWt = ErrorWt;
								}
						}
					}
				}
			}
		}

	}

	Clear_Watchdog();
	for(p=1;p<=MAX_HEADS;p++)
	{
		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}

		for(q=p+1;q<=MAX_HEADS;q++)
		{
			if(bucket_no[q-1]==1)
			{
				WT2 = WT1+BucketWt[q-1];
				q1 = 0;
			}
			else
			{
				WT2 = 0;
				q1 = 1;
			}

			for(r=q+1;r<=MAX_HEADS;r++)
			{
				if(bucket_no[r-1]==1)
				{
					WT3 = WT2+BucketWt[r-1];
					r1 = 0;
				}
				else
				{
					r1 = 1;
					WT3 = 0;
				}


				for(s=r+1;s<=MAX_HEADS;s++)
				{
					if(bucket_no[s-1]==1)
					{
						WT4 = WT3+BucketWt[s-1];
						s1 = 0;
					}
					else
					{
						WT4 = 0;
						s1 = 1;
					}

					for(t=s+1;t<=MAX_HEADS;t++)
					{
						if(bucket_no[t-1]==1)
						{
							WT5 = WT4+BucketWt[t-1];
							t1 = 0;
						}
						else
						{
							WT5 = 0;
							t1 = 1;
						}

						for(u=t+1;u<=MAX_HEADS;u++)
						{
							if(bucket_no[u-1]==1)
							{
								WT6 = WT5+BucketWt[u-1];
								u1 = 0;
							}
							else
								u1 = 1;

							if(multi_dump_flag == 0)
							{
								if(NewTargetWt>WT6)
								{
									ErrorWt = NewTargetWt - WT6;
								}
								else
								{
									ErrorWt = WT6 - NewTargetWt;
								}
						   }
							else if(multi_dump_flag == 1)
							 {
								if(Multi_Dump_Wt>WT6)
								{
									ErrorWt = Multi_Dump_Wt - WT6;
								}

								else
								{
									ErrorWt = WT6 - Multi_Dump_Wt;
								}

							 }

							if(multi_dump_flag == 0)
							{
							if(((WT6>=NewTargetWt)&&(WT6<=(NewTargetWt+PosErrorWt))) || ((WT6<=NewTargetWt)&&(WT6>=(NewTargetWt-NegErrorWt)))||(WT6==NewTargetWt))
							{

									if(ErrorWt<BandErrorWt)
									{
										BandErrorWt = ErrorWt;
									}

									if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT6<combination_weight))
									{
										combination_weight_flag  = 1;
										combination_weight = WT6;
									//	auxbucket_empty_flag = 1;
										combination_ok_flag = 1;
										under_over_combination_ok_flag = 1;
									//	Flag_Check_Combi = 1;
										//Trigger_For_Data_History = 1;
										//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
										DisplayWt = WT6;

										if(p1 == 0)
											Head[0] = p;
										if(q1 == 0)
											Head[1] = q;
										if(r1 == 0)
											Head[2] = r;
										if(s1 == 0)
											Head[3] = s;
										if(t1 == 0)
											Head[4] = t;
										if(u1 == 0)
											Head[5] = u;
										if(v1 == 0)
											Head[6] = 0;
										if(w1 == 0)
											Head[7] = 0;
									}
								}
								else if(ErrorWt<BandErrorWt)
								{
									BandErrorWt = ErrorWt;
								}
							}
							else if(multi_dump_flag == 1)
							{
								if(((WT6>=Multi_Dump_Wt)&&(WT6<=(Multi_Dump_Wt+PosErrorWt))) || ((WT6<=Multi_Dump_Wt)&&(WT6>=(Multi_Dump_Wt-NegErrorWt)))||(WT6==Multi_Dump_Wt))
								{

										if(ErrorWt<BandErrorWt)
										{
											BandErrorWt = ErrorWt;
											combination_ok_flag = 1;
											under_over_combination_ok_flag = 1;
										//	Flag_Check_Combi = 1;
											//Trigger_For_Data_History = 1;
											//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
											DisplayWt = WT6;

											Head[0] = p;
											Head[1] = q;
											Head[2] = r;
											Head[3] = s;
											Head[4] = t;
											Head[5] = u;
											Head[6] = 0;
											Head[7] = 0;
										}
									}
									else if(ErrorWt<BandErrorWt)
									{
										BandErrorWt = ErrorWt;
									}
							}
						}
					}
				}
			}
		}

	}

	Clear_Watchdog();
	for(p=1;p<=MAX_HEADS;p++)
	{
		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}

		for(q=p+1;q<=MAX_HEADS;q++)
		{
			if(bucket_no[q-1]==1)
			{
				WT2 = WT1+BucketWt[q-1];
				q1 = 0;
			}
			else
			{
				WT2 = 0;
				q1 = 1;
			}

			for(r=q+1;r<=MAX_HEADS;r++)
			{
				if(bucket_no[r-1]==1)
				{
					WT3 = WT2+BucketWt[r-1];
					r1 = 0;
				}
				else
				{
					WT3 = 0;
					r1 = 1;
				}

				for(s=r+1;s<=MAX_HEADS;s++)
				{
					if(bucket_no[s-1]==1)
					{
						WT4 = WT3+BucketWt[s-1];
						s1 = 0;
					}
					else
					{
						WT4 = 0;
						s1 = 1;
					}

					for(t=s+1;t<=MAX_HEADS;t++)
					{
						if(bucket_no[t-1]==1)
						{
							WT5 = WT4+BucketWt[t-1];
							t1 = 0;
						}
						else
						{
							WT5 = 0;
							t1 = 1;
						}

						for(u=t+1;u<=MAX_HEADS;u++)
						{
							if(bucket_no[u-1]==1)
							{
								WT6 = WT5+BucketWt[u-1];
								u1 = 0;
							}
							else
							{
								WT6 = 0;
								u1 = 1;
							}

							for(v=u+1;v<=MAX_HEADS;v++)
							{
								if(bucket_no[v-1]==1)
								{
									WT7 = WT6+BucketWt[v-1];
									v1 = 0;
								}
								else
									 v1 = 1;

								if(multi_dump_flag == 0)
								{
									if(NewTargetWt>WT7)
									{
										ErrorWt = NewTargetWt - WT7;
									}
									else
									{
										ErrorWt = WT7 - NewTargetWt;
									}
								}
								else if(multi_dump_flag == 1)
								 {
									if(Multi_Dump_Wt>WT7)
									{
										ErrorWt = Multi_Dump_Wt - WT7;
									}
									else
									{
										ErrorWt = WT7 - Multi_Dump_Wt;
									}

								 }
								if(multi_dump_flag == 0)
								{
									if(((WT7>=NewTargetWt)&&(WT7<=(NewTargetWt+PosErrorWt))) || ((WT7<=NewTargetWt)&&(WT7>=(NewTargetWt-NegErrorWt)))||(WT7==NewTargetWt))
									{

										if(ErrorWt<BandErrorWt)
										{
											BandErrorWt = ErrorWt;
										}

										if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT7<combination_weight))
										{
											combination_weight_flag  = 1;
											combination_weight = WT7;
										//	auxbucket_empty_flag = 1;
											combination_ok_flag = 1;
											under_over_combination_ok_flag = 1;
									//		Flag_Check_Combi = 1;
											//Trigger_For_Data_History = 1;
											//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
											DisplayWt = WT7;

											if(p1 == 0)
												Head[0] = p;
											if(q1 == 0)
												Head[1] = q;
											if(r1 == 0)
												Head[2] = r;
											if(s1 == 0)
												Head[3] = s;
											if(t1 == 0)
												Head[4] = t;
											if(u1 == 0)
												Head[5] = u;
											if(v1 == 0)
												Head[6] = v;
											if(w1 == 0)
												Head[7] = 0;
										}
									}
									else if(ErrorWt<BandErrorWt)
									{
										BandErrorWt = ErrorWt;
									}
								}
								else if(multi_dump_flag == 1)
								{
									if(((WT7>=Multi_Dump_Wt)&&(WT7<=(Multi_Dump_Wt+PosErrorWt))) || ((WT7<=Multi_Dump_Wt)&&(WT7>=(Multi_Dump_Wt-NegErrorWt)))||(WT7==Multi_Dump_Wt))
									{

										if(ErrorWt<BandErrorWt)
										{
											BandErrorWt = ErrorWt;
											combination_ok_flag = 1;
											under_over_combination_ok_flag = 1;
										//	Flag_Check_Combi = 1;
											//Trigger_For_Data_History = 1;
											//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
											DisplayWt = WT7;

											Head[0] = p;
											Head[1] = q;
											Head[2] = r;
											Head[3] = s;
											Head[4] = t;
											Head[5] = u;
											Head[6] = v;
											Head[7] = 0;
										}
									}
									else if(ErrorWt<BandErrorWt)
									{
										BandErrorWt = ErrorWt;
									}
								}
							}
						}
					}
				}
			}
		}

}


	Clear_Watchdog();
	for(p=1;p<=MAX_HEADS;p++)
	{
		if(bucket_no[p-1]==1)
		{
			WT1 = BucketWt[p-1];
			p1 = 0;
		}
		else
		{
			WT1 = 0;
			p1 = 1;
		}
		for(q=p+1;q<=MAX_HEADS;q++)
		{
			if(bucket_no[q-1]==1)
			{
				WT2 = WT1+BucketWt[q-1];
				q1 = 0;
			}
			else
			{
				WT2 = 0;
				q1 = 1;
			}
			for(r=q+1;r<=MAX_HEADS;r++)
			{
				if(bucket_no[r-1]==1)
				{
					WT3 = WT2+BucketWt[r-1];
					r1 = 0;
				}
				else
				{
					WT3 = 0;
					r1 = 1;
				}
				for(s=r+1;s<=MAX_HEADS;s++)
				{
					if(bucket_no[s-1]==1)
					{
						WT4 = WT3+BucketWt[s-1];
						s1 = 0;
					}
					else
					{
						WT4 = 0;
						s1 = 1;
					}

					for(t=s+1;t<=MAX_HEADS;t++)
					{
						if(bucket_no[t-1]==1)
						{
							WT5 = WT4+BucketWt[t-1];
							t1 = 0;
						}
						else
						{
							WT5 = 0;
							t1 = 1;
						}
						for(u=t+1;u<=MAX_HEADS;u++)
						{
							if(bucket_no[u-1]==1)
							{
								WT6 = WT5+BucketWt[u-1];
								u1 = 0;
							}
							else
							{
								WT6 = 0;
								u1 = 1;
							}
							for(v=u+1;v<=MAX_HEADS;v++)
							{
								if(bucket_no[v-1]==1)
								{
									WT7 = WT6+BucketWt[v-1];
									v1 = 0;
								}
								else
								{
									WT7 = 0;
									v1 = 1;
								}
								for(w=v+1;w<=MAX_HEADS;w++)
								{
									if(bucket_no[w-1]==1)
									{
										WT8 = WT7+BucketWt[w-1];
										w1 = 0;
									}
									else
										w1 = 1;

									if(multi_dump_flag == 0)
									{
										if(NewTargetWt>WT8)
										{
											ErrorWt = NewTargetWt - WT8;
										}
										else
										{
											ErrorWt = WT8 - NewTargetWt;
										}
								    }
									else if(multi_dump_flag == 1)
									{
										if(Multi_Dump_Wt>WT8)
										{
											ErrorWt = Multi_Dump_Wt - WT8;
										}
										else
										{
											ErrorWt = WT8 - Multi_Dump_Wt;
										}

									}
									if(multi_dump_flag == 0)
									{
										if(((WT8>=NewTargetWt)&&(WT8<=(NewTargetWt+PosErrorWt))) || ((WT8<=NewTargetWt)&&(WT8>=(NewTargetWt-NegErrorWt)))||(WT8==NewTargetWt))
										{

											if(ErrorWt<BandErrorWt)
											{
												BandErrorWt = ErrorWt;
											}

											if(combination_weight_flag == 0 || (combination_weight_flag == 1 && WT8<combination_weight))
											{
												combination_weight_flag  = 1;
												combination_weight = WT8;
											//	auxbucket_empty_flag = 1;
												combination_ok_flag = 1;
												under_over_combination_ok_flag = 1;
										//		Flag_Check_Combi = 1;
												//Trigger_For_Data_History = 1;
												//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

												DisplayWt = WT8;

												if(p1 == 0)
													Head[0] = p;
												if(q1 == 0)
													Head[1] = q;
												if(r1 == 0)
													Head[2] = r;
												if(s1 == 0)
													Head[3] = s;
												if(t1 == 0)
													Head[4] = t;
												if(u1 == 0)
													Head[5] = u;
												if(v1 == 0)
													Head[6] = v;
												if(w1 == 0)
													Head[7] = w;
											}
										}
										else if(ErrorWt<BandErrorWt)
										{
											BandErrorWt = ErrorWt;
										}
									}
									else if(multi_dump_flag == 1)
									{
										if(((WT8>=Multi_Dump_Wt)&&(WT8<=(Multi_Dump_Wt+PosErrorWt))) || ((WT8<=Multi_Dump_Wt)&&(WT8>=(Multi_Dump_Wt-NegErrorWt)))||(WT8==Multi_Dump_Wt))
										{

											if(ErrorWt<BandErrorWt)
											{
												BandErrorWt = ErrorWt;
												combination_ok_flag = 1;
												under_over_combination_ok_flag = 1;
										//		Flag_Check_Combi = 1;
												//Trigger_For_Data_History = 1;
												//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

												DisplayWt = WT8;

												Head[0] = p;
												Head[1] = q;
												Head[2] = r;
												Head[3] = s;
												Head[4] = t;
												Head[5] = u;
												Head[6] = v;
												Head[7] = w;
											}
										}
										else if(ErrorWt<BandErrorWt)
										{
											BandErrorWt = ErrorWt;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		Clear_Watchdog();
	}

}

void Calculate_Weights_Counter()
{
	if((DisplayWt<=NewTargetWt-5) && (DisplayWt>=(NewTargetWt-10))) //474 to 474.5 189
	{
		Inbetween_less_counter_1++;
		records[179] = Inbetween_less_counter_1;
	}
	if((DisplayWt<=NewTargetWt) && (DisplayWt>=(NewTargetWt-5))) //474.5 to 475 190
	{
		Inbetween_less_counter++;
		records[180] = Inbetween_less_counter;
	}
	else if((DisplayWt>NewTargetWt) && (DisplayWt<=(NewTargetWt+5))) //475 to 475.5 191
	{
		more_counter++;
		records[182] = more_counter;
	}
	else if((DisplayWt>(NewTargetWt+5)) && (DisplayWt<=(NewTargetWt+10))) //475.5 to 476 192
	{
		more_counter_1++;
		records[183] = more_counter_1;
	}
	else if((DisplayWt>(NewTargetWt+10)) && (DisplayWt<=(NewTargetWt+15))) //476 to 476.5 193
	{
		more_counter_2++;
		records[184] = more_counter_2;
	}
	else if((DisplayWt>(NewTargetWt+15)) && (DisplayWt<=(NewTargetWt+20))) //476.5 to 477 194
	{
		more_counter_3++;
		records[185] = more_counter_3;
	}

	else if((DisplayWt>(NewTargetWt+20)) && (DisplayWt<=(NewTargetWt+30))) //477 to 478 195
	{
		more_counter_4++;
		records[186] = more_counter_4;
	}

	else if((DisplayWt>(NewTargetWt+30)) && (DisplayWt<=(NewTargetWt+40))) //478 to 479 196
	{
		more_counter_5++;
		records[187] = more_counter_5;
	}

	else if((DisplayWt>(NewTargetWt+40)) && (DisplayWt<=(NewTargetWt+50))) //479 to 480 197
	{
		more_counter_6++;
		records[188] = more_counter_6;
	}
	else if((DisplayWt>(NewTargetWt+50)) && (DisplayWt<=(NewTargetWt+60))) //480 to 481 198
	{
		Inbetween_more_counter++;
		records[181] = Inbetween_more_counter;
	}
}

void Calculate_Multidump_Weights_Counter()
{
	if((multi_dump_total_wt<=NewTargetWt) && (multi_dump_total_wt>=(NewTargetWt-5)))
	{
		Inbetween_less_counter++;
		records[179] = Inbetween_less_counter;
	}
	else if(multi_dump_total_wt < (NewTargetWt-5))
	{
		less_counter++;
		records[180] = less_counter;
	}
	else if(multi_dump_total_wt==NewTargetWt)
	{
		Inbetween_more_counter++;
		records[181] = Inbetween_more_counter;
	}
	else if((multi_dump_total_wt>NewTargetWt) && (multi_dump_total_wt<=(NewTargetWt+5)))
	{
		more_counter++;
		records[182] = more_counter;
	}
	else if((multi_dump_total_wt>(NewTargetWt+5)) && (multi_dump_total_wt<=(NewTargetWt+10)) )
	{
		more_counter_1++;
		records[183] = more_counter_1;
	}
	else if((multi_dump_total_wt>(NewTargetWt+10)) && (multi_dump_total_wt<=(NewTargetWt+15)))
	{
		more_counter_2++;
		records[184] = more_counter_2;
	}
	else if(multi_dump_total_wt>(NewTargetWt+15))
	{
		more_counter_3++;
		records[185] = more_counter_3;
	}
}

void Show_Head_Status()
{
	Clear_Watchdog();

	for(jidx=0;jidx<15;jidx++)
	{
		for(iidx=15; iidx>0; iidx--)
		{
			if(Head[jidx]==iidx)
			{
				if(iidx == 1)
				{
					records[25] =  4;
				}
				else if(iidx == 2)
				{
					records[26] =  4;
				}
				else if(iidx == 3)
				{
					records[27] =  4;
				}
				else if(iidx == 4)
				{
					records[28] =  4;
				}
				else if(iidx == 5)
				{
					records[29] =  4;
				}
				else if(iidx == 6)
				{
					records[30] =  4;
				}
				else if(iidx == 7)
				{
					records[31] =  4;
				}
				else if(iidx == 8)
				{
					records[32] =  4;
				}
				else if(iidx == 9)
				{
					records[33] =  4;
				}
				else if(iidx == 10)
				{
					records[34] =  4;
				}
				else if(iidx == 11)
				{
					records[35] =  4;
				}
				else if(iidx == 12)
				{
					records[36] =  4;
				}
				else if(iidx == 13)
				{
					records[37] =  4;
				}
				else if(iidx == 14)
				{
					records[38] =  4;
				}
			}
		}
	}
}


void Recharge_Head_In_MultiDump()
{
	if(BucketWt[iidx] < (Multi_Dump_Wt/4) || (BucketWt[iidx] > 65000) || (BucketWt[iidx] == 0) )
	 {
		if(iidx == 0 && head1_wt_received_flag == 1 && dump1_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0001;
			records[25] =  17;
		}
		else if(iidx == 1 && head2_wt_received_flag == 1 && dump2_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0002;
			records[26] =  17;
		}
		else if(iidx == 2 && head3_wt_received_flag == 1 && dump3_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0004;
			records[27] =  17;
		}
		else if(iidx == 3 && head4_wt_received_flag == 1 && dump4_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0008;
			records[28] =  17;
		}
		else if(iidx == 4 && head5_wt_received_flag == 1 && dump5_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0010;
			records[29] =  17;
		}
		else if(iidx == 5 && head6_wt_received_flag == 1 && dump6_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0020;
			records[30] =  17;
		}
		else if(iidx == 6 && head7_wt_received_flag == 1 && dump7_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0040;
			records[31] =  17;
		}
		else if(iidx == 7 && head8_wt_received_flag == 1 && dump8_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0080;
			records[32] =  17;
		}
		else if(iidx == 8 && head9_wt_received_flag == 1 && dump9_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0100;
			records[33] =  17;
		}
		else if(iidx == 9 && head10_wt_received_flag == 1 && dump10_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0200;
			records[34] =  17;
		}
		else if(iidx == 10 && head11_wt_received_flag == 1 && dump11_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0400;
			records[35] =  17;
		}
		else if(iidx == 11 && head12_wt_received_flag == 1 && dump12_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0800;
			records[36] =  17;
		}
		else if(iidx == 12 && head13_wt_received_flag == 1 && dump13_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x1000;
			records[37] =  17;
		}
		else if(iidx == 13 && head14_wt_received_flag == 1 && dump14_slected_flag == 0)
		{
			Recharge_Selected = Recharge_Selected|0x2000;
			records[38] =  17;
		}

	}

}

void Recharge_Head()
{
	 if(BucketWt[iidx] < (NewTargetWt/Recharge_Divisor))
	 {
		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50)) && bucket_no[0] == 1 && iidx == 0 && head1_wt_received_flag == 1 && dump1_slected_flag == 0 && skip_for_combination_1 == 0 && Head1_Stable == 1 && skip_for_recharge_1 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0001;
			records[25] =  17;

			if(Skip_Head)
				skip_for_recharge_1 = 0;
			else
				skip_for_recharge_1 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50)) && bucket_no[1] == 1 && iidx == 1 && head2_wt_received_flag == 1 && dump2_slected_flag == 0 && skip_for_combination_2 == 0 && Head2_Stable == 1 && skip_for_recharge_2 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0002;
			records[26] =  17;
			if(Skip_Head)
				skip_for_recharge_2 = 0;
			else
				skip_for_recharge_2 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[2] == 1 && iidx == 2 && head3_wt_received_flag == 1 && dump3_slected_flag == 0 && skip_for_combination_3 == 0 && Head3_Stable == 1 && skip_for_recharge_3 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0004;
			records[27] =  17;

			if(Skip_Head)
				skip_for_recharge_3 = 0;
			else
				skip_for_recharge_3 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[3] == 1 && iidx == 3 && head4_wt_received_flag == 1 && dump4_slected_flag == 0 && skip_for_combination_4 == 0 && Head4_Stable == 1 && skip_for_recharge_4 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0008;
			records[28] =  17;

			if(Skip_Head)
				skip_for_recharge_4 = 0;
			else
				skip_for_recharge_4 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[4] == 1 && iidx == 4 && head5_wt_received_flag == 1 && dump5_slected_flag == 0 && skip_for_combination_5 == 0 && Head5_Stable == 1 && skip_for_recharge_5 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0010;
			records[29] =  17;

			if(Skip_Head)
				skip_for_recharge_5 = 0;
			else
				skip_for_recharge_5 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[5] == 1 && iidx == 5 && head6_wt_received_flag == 1 && dump6_slected_flag == 0 && skip_for_combination_6 == 0 && Head6_Stable == 1 && skip_for_recharge_6 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0020;
			records[30] =  17;
			if(Skip_Head)
				skip_for_recharge_6 = 0;
			else
				skip_for_recharge_6 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[6] == 1 && iidx == 6 && head7_wt_received_flag == 1 && dump7_slected_flag == 0 && skip_for_combination_7 == 0 && Head7_Stable == 1 && skip_for_recharge_7 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0040;
			records[31] =  17;
			if(Skip_Head)
				skip_for_recharge_7 = 0;
			else
				skip_for_recharge_7 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[7] == 1 && iidx == 7 && head8_wt_received_flag == 1 && dump8_slected_flag == 0 && skip_for_combination_8 == 0 && Head8_Stable == 1 && skip_for_recharge_8 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0080;
			records[32] =  17;
			if(Skip_Head)
				skip_for_recharge_8 = 0;
			else
				skip_for_recharge_8 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[8] == 1 && iidx == 8 && head9_wt_received_flag == 1 && dump9_slected_flag == 0 && skip_for_combination_9 == 0 && Head9_Stable == 1 && skip_for_recharge_9 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0100;
			records[33] =  17;
			if(Skip_Head)
				skip_for_recharge_9 = 0;
			else
				skip_for_recharge_9 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[9] == 1 && iidx == 9 && head10_wt_received_flag == 1 && dump10_slected_flag == 0 && skip_for_combination_10 == 0 && Head10_Stable == 1 && skip_for_recharge_10 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0200;
			records[34] =  17;
			if(Skip_Head)
				skip_for_recharge_10 = 0;
			else
				skip_for_recharge_10 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[10] == 1 && iidx == 10 && head11_wt_received_flag == 1 && dump11_slected_flag == 0 && skip_for_combination_11 == 0 && Head11_Stable == 1 && skip_for_recharge_11 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0400;
			records[35] =  17;
			if(Skip_Head)
				skip_for_recharge_11 = 0;
			else
				skip_for_recharge_11 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[11] == 1 && iidx == 11 && head12_wt_received_flag == 1 && dump12_slected_flag == 0 && skip_for_combination_12 == 0 && Head12_Stable == 1 && skip_for_recharge_12 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x0800;
			records[36] =  17;
			if(Skip_Head)
				skip_for_recharge_12 = 0;
			else
				skip_for_recharge_12 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[12] == 1 && iidx == 12 && head13_wt_received_flag == 1 && dump13_slected_flag == 0 && skip_for_combination_13 == 0 && Head13_Stable == 1 && skip_for_recharge_13 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x1000;
			records[37] =  17;
			if(Skip_Head)
				skip_for_recharge_13 = 0;
			else
				skip_for_recharge_13 = 1;
		}

		if((BucketWt[iidx] < ((NewTargetWt/Recharge_Divisor)-50) ) && bucket_no[13] == 1 && iidx == 13 && head14_wt_received_flag == 1 && dump14_slected_flag == 0 && skip_for_combination_14 == 0 && Head14_Stable == 1 && skip_for_recharge_14 == 0)
		{
			Recharge_Selected = Recharge_Selected|0x2000;
			records[38] =  17;

			if(Skip_Head)
				skip_for_recharge_14 = 0;
			else
				skip_for_recharge_14 = 1;
		}
	}
}

void Send_Recharge_Frame()
{
	Heads_To_recharge=Recharge_Selected;
	Heads_To_recharge=Heads_To_recharge|0x0;
	Heads_To_recharge=Heads_To_recharge&0X3FFF;

	Actual_Recharge = Selected_Heads;
	Heads_To_recharge = Heads_To_recharge & (Actual_Recharge>>1);

	if(Heads_To_recharge != 0)
		Recharge_Can_Frame();
	else
		Reset_Run_Parameter();


	//Heads_To_recharge     = 0;
}


void Create_Dump_Recharge_Frame()
{
	//if(Flag_Check_Combi==1)
	{
		Calculate_Speed();

		for(jidx=0;jidx<15;jidx++)
		{
			//BucWeight  = BucketWt[jidx];

			for(iidx=15; iidx>0; iidx--)
			{
				//BucWeight  = BucketWt[iidx-1];

				if(Head[jidx]==iidx)
				{
					Heads_In_Combination();
				}
				/*else
				{
					if(Coil_No_Select_Mode == 1)
					{
						Dump_Cycle_To_Reject();
					}
				}*/
			}
		}



		for(iidx=0;iidx<15;iidx++)
		{
			Recharge_Head();
		}

	//Clear_Watchdog();
	/*if(reject_flag == 1)
		OVER_UNDER_WT = 1;
	else
		OVER_UNDER_WT = 0;*/


	Send_Dump_Recharge_Frame();


	DUMP_CONFIRMATION = 1;

//	Clear_Watchdog();

	//for(iidx=0;iidx<10;iidx++)
	{
		Head[0]    = 0;
		Head[1]    = 0;
		Head[2]    = 0;
		Head[3]    = 0;
		Head[4]    = 0;
		Head[5]    = 0;
		Head[6]    = 0;
		Head[7]    = 0;
		Head[8]    = 0;
		Head[9]    = 0;
		Head[10]   = 0;
		Head[11]   = 0;
		Head[12]   = 0;
		Head[13]   = 0;
	}

		Head_counter=1;

	}

	Clear_Watchdog();

	//Reset_Run_Parameter();   // bypassed 5/12/22
	Run_Loop_Time_flag = 0;
	Clear_Watchdog();

}

void Create_Multidump_Recharge_Frame()
{

	if(Flag_Check_Combi==1)
	{
		Clear_Watchdog(); Clear_Watchdog();	Clear_Watchdog();	Clear_Watchdog();

		for(jidx=0;jidx<15;jidx++)
		{
			//BucWeight  = BucketWt[jidx];

			for(iidx=15; iidx>0; iidx--)
			{
				//BucWeight  = BucketWt[iidx-1];

				if(Head[jidx]==iidx)
				{
					Heads_In_Combination();

				}
				/*else
				{
					if(Coil_No_Select_Mode == 1)
					{
						MultiDump_Cycle_To_Reject();
					}
				}*/
			}
		}

		for(iidx=0;iidx<15;iidx++)
		{
			 BucWeight=BucketWt[iidx];
			 Recharge_Head_In_MultiDump();
		}

	Clear_Watchdog();Clear_Watchdog();	Clear_Watchdog();

	if(reject_flag == 1)
		OVER_UNDER_WT = 1;
	else
		OVER_UNDER_WT = 0;

	Send_Dump_Recharge_Frame();

	DUMP_CONFIRMATION = 1;

	Multi_Dump_Count++;

	if(Multi_Dump_Count == (MULTI_DUMP_DIV_COUNT-1))
		Multi_Dump_Wt = Multi_Dump_Target_Wt - Multi_dump_display_wt;

	if(Multi_Dump_Count == MULTI_DUMP_DIV_COUNT)
	{
		Multi_Dump_Count = 0;
	//	multi_dump_flag = 0;
		Multi_dump_display_wt = 0;
		multi_dump_total_wt = 0;

		Calculate_Speed();

	}

	Clear_Watchdog();

	for(iidx=0;iidx<14;iidx++)
	{
		Head[iidx] = 0;
	}

	Head_counter=1;
	}
	Clear_Watchdog();

	Reset_Run_Parameter();

	Clear_Watchdog();
}

void Create_Auxdump_Recharge_Frame()
{
		Clear_Watchdog();

		if(reject_flag == 1)
			OVER_UNDER_WT = 1;
		else
			OVER_UNDER_WT = 0;


		Send_Dump_Recharge_Frame();

		DUMP_CONFIRMATION = 1;

		Clear_Watchdog();

		for(iidx=0;iidx<14;iidx++)
		{
			Head[iidx]    = 0;
		}

	//Reset_Run_Parameter();

}

void Staggerdump_Recharge_Frame()
{
	if(Flag_Check_Combi==1)
	{
		Calculate_Speed();

		for(jidx=0;jidx<8;jidx++)
		{
			//BucWeight  = BucketWt[jidx];

			for(iidx=15; iidx>0; iidx--)
			{
				//BucWeight  = BucketWt[iidx-1];

				if(Head[jidx]==iidx)
				{
					if(iidx == HEAD_1)
					{
						Dump_Selected = Dump_Selected|0x0001;
						dump1_slected_flag = 1;
						records[25] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*{
							Clear_Watchdog();
						}*/
					}
					else if(iidx == HEAD_2)
					{
						Dump_Selected = Dump_Selected|0x0002;
						dump2_slected_flag = 1;
						records[26] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;

						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}9*/
					}
					else if(iidx == HEAD_3 )
					{
						Dump_Selected = Dump_Selected|0x0004;
						dump3_slected_flag = 1;
						records[27] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_4)
					{
						Dump_Selected = Dump_Selected|0x0008;
						dump4_slected_flag = 1;
						records[28] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_5)
					{
						Dump_Selected = Dump_Selected|0x0010;
						dump5_slected_flag = 1;
						records[29] =  4;
						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
 					}
					else if(iidx == HEAD_6)
					{
						Dump_Selected = Dump_Selected|0x0020;
						dump6_slected_flag = 1;
						records[30] =  4;
						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_7)
					{
						Dump_Selected = Dump_Selected|0x0040;
						dump7_slected_flag = 1;
						records[31] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_8)
					{
						Dump_Selected = Dump_Selected|0x0080;
						dump8_slected_flag = 1;
						records[32] =  4;
						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_9)
					{
						Dump_Selected = Dump_Selected|0x0100;
						dump9_slected_flag = 1;
						records[33] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_10)
					{
						Dump_Selected = Dump_Selected|0x0200;
						dump10_slected_flag = 1;
						records[34] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_11)
					{
						Dump_Selected = Dump_Selected|0x0400;
						dump11_slected_flag = 1;
						records[34] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_12)
					{
						Dump_Selected = Dump_Selected|0x0800;
						dump12_slected_flag = 1;
						records[34] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_13)
					{
						Dump_Selected = Dump_Selected|0x1000;
						dump13_slected_flag = 1;
						records[34] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}
					else if(iidx == HEAD_14)
					{
						Dump_Selected = Dump_Selected|0x2000;
						dump14_slected_flag = 1;
						records[34] =  4;

						Heads_To_Dump=Dump_Selected;
						Heads_To_Dump=Heads_To_Dump|0x0;
						Heads_To_Dump=Heads_To_Dump&0X3FFF;
						Actual_Dump = Selected_Heads;
						Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

						Dump_Can_Frame();
						Dump_Selected  = 0;
						stagger_wait_time = stagger_time;

						mS_Delay(stagger_wait_time);
						stagger_wait_time = 0;
						/*while(stagger_wait_time)
						{Clear_Watchdog();}*/
					}

				}
				/*else
				{
					if(Coil_No_Select_Mode == 1)
					{
						Dump_Cycle_To_Reject();
					}
				}*/
			}
		}

		for(iidx=0;iidx<15;iidx++)
		{
			Recharge_Head();
		}

		//	}
//	}

    Clear_Watchdog();
    if(reject_flag == 1)
    	OVER_UNDER_WT = 1;
    else
    	OVER_UNDER_WT = 0;

	/*Heads_To_Dump=Dump_Selected;
	Heads_To_Dump=Heads_To_Dump|0x0;
	Heads_To_Dump=Heads_To_Dump&0X1FFF;

	Actual_Dump = Selected_Heads;

	Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);*/

	Heads_To_recharge=Recharge_Selected;
	Heads_To_recharge=Heads_To_recharge|0x0;
	Heads_To_recharge=Heads_To_recharge&0X1FFF;

	Actual_Recharge = Selected_Heads;
	Heads_To_recharge = Heads_To_recharge & (Actual_Recharge>>1);

	Recharge_Can_Frame();

	DUMP_CONFIRMATION = 1;

	Clear_Watchdog();


	for(iidx=0;iidx<14;iidx++)
	{
		Head[iidx]    = 0;
	}

	Head_counter=1;

}

	Clear_Watchdog();

	Reset_Run_Parameter();

}

void Calculate_Speed()
{
	if(Run_Dump_Counter<65000)
		Run_Dump_Counter++;

	if(total_dump_in_day<65000)
		total_dump_in_day++;

	records[178] = total_dump_in_day;

	if(Sec_Counter < 60)
	{
		Actual_Run_Speed = Run_Dump_Counter;
	}
	else
	{
		Actual_Run_Speed=(Run_Dump_Counter * MAX_SECOND)/Sec_Counter;
	}

	records[20]=Actual_Run_Speed;

	records[14]=Actual_Run_Speed;

//	Total_Combi_head = (Combi_Head_Counter+Total_Combi_head);

//	Total_Stable_head = (Stable_Head_Counter+Total_Stable_head);

	//Average_Combi_Head = (Total_Combi_head*MAX_HEADS)/Run_Dump_Counter;

	//Average_Stable_Head = (Total_Stable_head*MAX_HEADS)/Run_Dump_Counter;

	//records[22] = Average_Combi_Head;

	//records[21] = Average_Stable_Head;


	records[22] = Combi_Head_Counter;

	records[21] = Stable_Head_Counter;

	Combi_Head_Counter  = 0;

	Stable_Head_Counter = 0;
}

void Heads_In_Combination()
{
	if(iidx == HEAD_1 && bucket_no[0] == 1)
	{
		Dump_Selected = Dump_Selected|0x0001;
		dump1_slected_flag = 1;
		records[25] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_1 = 0;
		else
			skip_for_combination_1 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[0] = bucket_no[0];
		bucket_no[0] = 0;
	}

	if(iidx == HEAD_2 && bucket_no[1] == 1)
	{
		Dump_Selected = Dump_Selected|0x0002;
		dump2_slected_flag = 1;
		records[26] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_2 = 0;
		else
			skip_for_combination_2 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[1] = bucket_no[1];
		bucket_no[1] = 0;
	}

	if(iidx == HEAD_3 && bucket_no[2] == 1)
	{
		Dump_Selected = Dump_Selected|0x0004;
		dump3_slected_flag = 1;
		records[27] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_3 = 0;
		else
			skip_for_combination_3 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[2] = bucket_no[2];
		bucket_no[2] = 0;
	}

	if(iidx == HEAD_4 && bucket_no[3] == 1)
	{
		Dump_Selected = Dump_Selected|0x0008;
		dump4_slected_flag = 1;
		records[28] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_4 = 0;
		else
			skip_for_combination_4 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[3] = bucket_no[3];
		bucket_no[3] = 0;
	}

	if(iidx == HEAD_5 && bucket_no[4] == 1)
	{
		Dump_Selected = Dump_Selected|0x0010;
		dump5_slected_flag = 1;
		records[29] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)

		if(Skip_Head)
			skip_for_combination_5 = 0;
		else
			skip_for_combination_5 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[4] = bucket_no[4];
		bucket_no[4] = 0;
	}

	if(iidx == HEAD_6 && bucket_no[5] == 1)
	{
		Dump_Selected = Dump_Selected|0x0020;
		dump6_slected_flag = 1;
		records[30] =  4;
		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_6 = 0;
		else
			skip_for_combination_6 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[5] = bucket_no[5];
		bucket_no[5] = 0;
	}

	if(iidx == HEAD_7 && bucket_no[6] == 1)
	{
		Dump_Selected = Dump_Selected|0x0040;
		dump7_slected_flag = 1;
		records[31] =  4;
		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_7 = 0;
		else
			skip_for_combination_7 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[6] = bucket_no[6];
		bucket_no[6] = 0;
	}

	if(iidx == HEAD_8 && bucket_no[7] == 1)
	{
		Dump_Selected = Dump_Selected|0x0080;
		dump8_slected_flag = 1;
		records[32] =  4;
		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_8 = 0;
		else
			skip_for_combination_8 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[7] = bucket_no[7];
		bucket_no[7] = 0;
	}

	if(iidx == HEAD_9 && bucket_no[8] == 1)
	{
		Dump_Selected = Dump_Selected|0x0100;
		dump9_slected_flag = 1;
		records[33] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_9 = 0;
		else
			skip_for_combination_9 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[8] = bucket_no[8];
		bucket_no[8] = 0;
	}

	if(iidx == HEAD_10 && bucket_no[9] == 1)
	{
		Dump_Selected = Dump_Selected|0x0200;
		dump10_slected_flag = 1;
		records[34] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_10 = 0;
		else
			skip_for_combination_10 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[9] = bucket_no[9];
		bucket_no[9] = 0;
	}

	if(iidx == HEAD_11 && bucket_no[10] == 1)
	{
		Dump_Selected = Dump_Selected|0x0400;
		dump11_slected_flag = 1;
		records[35] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_11 = 0;
		else
			skip_for_combination_11 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[10] = bucket_no[10];
		bucket_no[10] = 0;
	}

	if(iidx == HEAD_12 && bucket_no[11] == 1)
	{
		Dump_Selected = Dump_Selected|0x0800;
		dump12_slected_flag = 1;
		records[36] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_12 = 0;
		else
			skip_for_combination_12 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[11] = bucket_no[11];
		bucket_no[11] = 0;
	}

	if(iidx == HEAD_13 && bucket_no[12] == 1)
	{
		Dump_Selected = Dump_Selected|0x1000;
		dump13_slected_flag = 1;
		records[37] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_13 = 0;
		else
			skip_for_combination_13 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[12] = bucket_no[12];
		bucket_no[12] = 0;
	}

	if(iidx == HEAD_14 && bucket_no[13] == 1)
	{
		Dump_Selected = Dump_Selected|0x2000;
		dump14_slected_flag = 1;
		records[38] =  4;

		//if(Coil_Head11Button == 1 || Coil_Head12Button == 1 || Coil_Head13Button == 1 || Coil_Head14Button == 1)
		if(Skip_Head)
			skip_for_combination_14 = 0;
		else
			skip_for_combination_14 = 1;

		dump_bucket_count++;
		Bkt_Wt_BFr[13] = bucket_no[13];
		bucket_no[13] = 0;
	}
}

/*void Dump_Cycle_To_Reject()
{
	if((head1_not_in_combi_flag == 1 || (BucketWt[0] >(NewTargetWt+PosErrorWt)))&&(dump1_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0001;
		dump1_slected_flag = 1;
		reject_flag = 1;

		 if(head1_not_in_combi_flag == 1)
			records[25] =  6;
	}

	if((head2_not_in_combi_flag == 1 || (BucketWt[1] >(NewTargetWt+PosErrorWt)))&&(dump2_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0002;
		dump2_slected_flag = 1;
		reject_flag = 1;

		if(head2_not_in_combi_flag == 1)
			records[26] =  6;
	}

	if((head3_not_in_combi_flag == 1 || (BucketWt[2] >(NewTargetWt+PosErrorWt)))&&(dump3_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0004;
		dump3_slected_flag = 1;
		reject_flag = 1;
		if(head3_not_in_combi_flag == 1)
			records[27] =  6;
	}

	if((head4_not_in_combi_flag == 1 || (BucketWt[3] >(NewTargetWt+PosErrorWt)))&&(dump4_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0008;
		dump4_slected_flag = 1;
		reject_flag = 1;

		if(head4_not_in_combi_flag == 1)
			records[28] =  6;
	}

	if((head5_not_in_combi_flag == 1 || (BucketWt[4] >(NewTargetWt+PosErrorWt)))&&(dump5_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0010;
		dump5_slected_flag = 1;
		reject_flag = 1;

		if(head5_not_in_combi_flag == 1)
			records[29] =  6;
	}

	if((head6_not_in_combi_flag == 1 || (BucketWt[5] >(NewTargetWt+PosErrorWt)))&&(dump6_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0020;
		dump6_slected_flag = 1;
		reject_flag = 1;

		if(head6_not_in_combi_flag == 1)
			records[30] =  6;
	}

	if((head7_not_in_combi_flag == 1 || (BucketWt[6] >(NewTargetWt+PosErrorWt)))&&(dump6_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0040;
		dump7_slected_flag = 1;
		reject_flag = 1;

		if(head7_not_in_combi_flag == 1)
			records[31] =  6;
	}

	if((head8_not_in_combi_flag == 1 || (BucketWt[7] >(NewTargetWt+PosErrorWt)))&&(dump8_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0080;
		dump8_slected_flag = 1;
		reject_flag = 1;

		if(head8_not_in_combi_flag == 1)
			records[32] =  6;
	}

	if((head9_not_in_combi_flag == 1 || (BucketWt[8] >(NewTargetWt+PosErrorWt)))&&(dump9_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0100;
		dump9_slected_flag = 1;
		reject_flag = 1;

		if(head9_not_in_combi_flag == 1)
			records[33] =  6;
	}

	if((head10_not_in_combi_flag == 1 || (BucketWt[9] >(NewTargetWt+PosErrorWt)))&&(dump10_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0200;
		dump10_slected_flag = 1;
		reject_flag = 1;

		if(head10_not_in_combi_flag == 1)
			records[34] =  6;
	}
}
*/

void Send_Dump_Recharge_Frame()
{
	Clear_Watchdog();

	Heads_To_Dump=Dump_Selected;
	Heads_To_Dump=Heads_To_Dump|0x0;
	Heads_To_Dump=Heads_To_Dump&0X3FFF;

	Actual_Dump = Selected_Heads;

	Heads_To_Dump = Heads_To_Dump & (Actual_Dump>>1);

	records[249] = Heads_To_Dump;


	Heads_To_recharge=Recharge_Selected;
	Heads_To_recharge=Heads_To_recharge|0x0;
	Heads_To_recharge=Heads_To_recharge&0X3FFF;

	Actual_Recharge = Selected_Heads;
	Heads_To_recharge = Heads_To_recharge & (Actual_Recharge>>1);

	if(Heads_To_recharge != 0 || Heads_To_Dump != 0)
		Recharge_Dump_Can_Frame();
	else
		Reset_Run_Parameter();

}

void Reset_Run_Parameter()
{
	Clear_Watchdog();
	combination_ok_flag = 0;
	under_over_combination_ok_flag = 0;
	not_in_combi_count = 0;
	send_reacharge_frame_flag = 0;
	Recharge_Frame_Time = 0;
	aux_ready_flag = 0;
	Flag_Check_Combi = 0;
	//auxbucket_empty_flag = 1;
	//next_run_flag = 0;      //8/12/22
	//dump_frame_sent_flag = 0;
	recharge_frame_sent_flag = 0;
	all_combi_wt      = 0;
	Dump_Selected     = 0;
	Recharge_Selected = 0;
	Selected_Heads    = 0;
	Heads_To_recharge = 0;
	Heads_To_Dump     = 0;

	head1_wt_received_flag = 0;head2_wt_received_flag = 0;head3_wt_received_flag = 0;
	head4_wt_received_flag = 0;head5_wt_received_flag = 0;head6_wt_received_flag = 0;
	head7_wt_received_flag = 0;head8_wt_received_flag = 0;head9_wt_received_flag = 0;
	head10_wt_received_flag= 0;head11_wt_received_flag= 0;head12_wt_received_flag= 0;
	head13_wt_received_flag= 0;head14_wt_received_flag= 0;

	head1_not_in_combi_flag = 0;head2_not_in_combi_flag = 0;head3_not_in_combi_flag = 0;head4_not_in_combi_flag = 0;
	head5_not_in_combi_flag = 0;head6_not_in_combi_flag = 0;head7_not_in_combi_flag = 0;head8_not_in_combi_flag = 0;
	head9_not_in_combi_flag = 0;head10_not_in_combi_flag = 0;head11_not_in_combi_flag = 0;head12_not_in_combi_flag = 0;
	head13_not_in_combi_flag = 0;head14_not_in_combi_flag = 0;

	dump1_slected_flag = 0;dump2_slected_flag = 0;dump3_slected_flag = 0;dump4_slected_flag = 0;
	dump5_slected_flag = 0;dump6_slected_flag = 0;dump7_slected_flag = 0;dump8_slected_flag = 0;
	dump9_slected_flag = 0;dump10_slected_flag = 0;dump11_slected_flag = 0;dump12_slected_flag = 0;
	dump13_slected_flag = 0;dump14_slected_flag = 0;

	reject_flag       = 0;
	get_weight_counter = 0;
	Head_counter      = 1;
	combi_cycle_flag  = 0;

	Actual_Recharge  = 0;
	Actual_Dump  = 0;
	dump_frame_sent_flag = 0;
	next_run_flag = 0;

	// Modification on 26/07
	cycle_equal_time_flag = 0;
	cycle_equal_time = 0;

}

/*void MultiDump_Cycle_To_Reject()
{
	if((head1_not_in_combi_flag == 1 || (BucketWt[0] >(Multi_Dump_Wt+PosErrorWt)))&&(dump1_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0001;
		dump1_slected_flag = 1;
		reject_flag = 1;

		 if(head1_not_in_combi_flag == 1)
			records[30] =  6;
	}

	if((head2_not_in_combi_flag == 1 || (BucketWt[1] >(Multi_Dump_Wt+PosErrorWt)))&&(dump2_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0002;
		dump2_slected_flag = 1;
		reject_flag = 1;

		if(head2_not_in_combi_flag == 1)
			records[31] =  6;
	}

	if((head3_not_in_combi_flag == 1 || (BucketWt[2] >(Multi_Dump_Wt+PosErrorWt)))&&(dump3_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0004;
		dump3_slected_flag = 1;
		reject_flag = 1;
		if(head3_not_in_combi_flag == 1)
			records[32] =  6;
	}

	if((head4_not_in_combi_flag == 1 || (BucketWt[3] >(Multi_Dump_Wt+PosErrorWt)))&&(dump4_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0008;
		dump4_slected_flag = 1;
		reject_flag = 1;

		if(head4_not_in_combi_flag == 1)
			records[33] =  6;
	}

	if((head5_not_in_combi_flag == 1 || (BucketWt[4] >(Multi_Dump_Wt+PosErrorWt)))&&(dump5_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0010;
		dump5_slected_flag = 1;
		reject_flag = 1;

		if(head5_not_in_combi_flag == 1)
			records[34] =  6;
	}

	if((head6_not_in_combi_flag == 1 || (BucketWt[5] >(Multi_Dump_Wt+PosErrorWt)))&&(dump6_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0020;
		dump6_slected_flag = 1;
		reject_flag = 1;

		if(head6_not_in_combi_flag == 1)
			records[35] =  6;
	}

	if((head7_not_in_combi_flag == 1 || (BucketWt[6] >(Multi_Dump_Wt+PosErrorWt)))&&(dump7_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0040;
		dump7_slected_flag = 1;
		reject_flag = 1;

		if(head7_not_in_combi_flag == 1)
			records[36] =  6;
	}

	if((head8_not_in_combi_flag == 1 || (BucketWt[7] >(Multi_Dump_Wt+PosErrorWt)))&&(dump8_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0080;
		dump8_slected_flag = 1;
		reject_flag = 1;

		if(head8_not_in_combi_flag == 1)
			records[37] =  6;
	}

	if((head9_not_in_combi_flag == 1 || (BucketWt[8] >(Multi_Dump_Wt+PosErrorWt)))&&(dump9_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0100;
		dump9_slected_flag = 1;
		reject_flag = 1;

		if(head9_not_in_combi_flag == 1)
			records[38] =  6;
	}

	if((head10_not_in_combi_flag == 1 || (BucketWt[9] >(Multi_Dump_Wt+PosErrorWt)))&&(dump10_slected_flag == 0))
	{
		Dump_Selected = Dump_Selected|0x0200;
		dump10_slected_flag = 1;
		reject_flag = 1;

		if(head10_not_in_combi_flag == 1)
			records[39] =  6;
	}
}
*/
void No_Select_Combination()
{
	WT1=WT2=WT3=WT4=WT5=WT6=WT7=WT8=0;
	p1=q1=r1=s1=t1=u1=v1=w1 = 0;
	NegErrorWt = 10;
	WT1 = BucketWt[head_number-1];
	p1  = 0;
	p   = head_number;

	for(q=1;q<=MAX_HEADS;q++)
	{
		if(bucket_no[q-1]==1 && q!=p)
		{
			WT2 = WT1+BucketWt[q-1];
			q1 = 0;
		}

		if(WT2>=(NewTargetWt-NegErrorWt))
		{
			if(NewTargetWt>WT2)
			{
				ErrorWt = NewTargetWt - WT2;
			}
			else
			{
				ErrorWt = WT2 - NewTargetWt;
			}

			if(combination_weight_flag == 0 || (combination_weight_flag == 1 && ErrorWt<BandErrorWt))
			{
				combination_weight_flag  = 1;
				BandErrorWt = ErrorWt;
				combination_ok_flag = 1;
				under_over_combination_ok_flag = 1;
				//` = 1;
				//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
				DisplayWt = WT2;


				if(p1 == 0)
					Head[0] = p;
				if(q1 == 0)
					Head[1] = q;
				if(r1 == 0)
					Head[2] = 0;
				if(s1 == 0)
					Head[3] = 0;
				if(t1 == 0)
					Head[4] = 0;
				if(u1 == 0)
					Head[5] = 0;
				if(v1 == 0)
					Head[6] = 0;
				if(w1 == 0)
					Head[7] = 0;
			}
		}
	}

	Clear_Watchdog();

	WT1 = BucketWt[head_number-1];
	p1  = 0;
	p   = head_number;


	for(q=1;q<=MAX_HEADS;q++)
	{
		if(bucket_no[q-1]==1  && q!=p)
		{
			WT2 = WT1+BucketWt[q-1];
			q1 = 0;
		//}

		for(r=q+1;r<=MAX_HEADS;r++)
		{
			if(bucket_no[r-1]==1  && r!=p)
			{
				WT3 = WT2+BucketWt[r-1];
				r1 = 0;
			}

			/*if(NewTargetWt>WT3)
			{
				ErrorWt = NewTargetWt - WT3;
			}
			else
			{
				ErrorWt = WT3 - NewTargetWt;
			}*/

			if(WT3>=(NewTargetWt-NegErrorWt))
			{
				if(NewTargetWt>WT3)
				{
					ErrorWt = NewTargetWt - WT3;
				}
				else
				{
					ErrorWt = WT3 - NewTargetWt;
				}
				if(combination_weight_flag == 0 || (combination_weight_flag == 1 && ErrorWt<BandErrorWt))
				{
					combination_weight_flag  = 1;
					BandErrorWt = ErrorWt;
					combination_ok_flag = 1;
					under_over_combination_ok_flag = 1;
					//Trigger_For_Data_History = 1;
					//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

					DisplayWt = WT3;

					if(p1 == 0)
						Head[0] = p;
					if(q1 == 0)
						Head[1] = q;
					if(r1 == 0)
						Head[2] = r;
					if(s1 == 0)
						Head[3] = 0;
					if(t1 == 0)
						Head[4] = 0;
					if(u1 == 0)
						Head[5] = 0;
					if(v1 == 0)
						Head[6] = 0;
					if(w1 == 0)
						Head[7] = 0;
				}
			}
		}
		}   // 0802
	}

	Clear_Watchdog();

	WT1 = BucketWt[head_number-1];
	p1  = 0;
	p   = head_number;

	for(q=1;q<=MAX_HEADS;q++)
	{
		if(bucket_no[q-1]==1 && q!=p)
		{
			WT2 = WT1+BucketWt[q-1];
			q1 = 0;
		//}

		for(r=q+1;r<=MAX_HEADS;r++)
		{
			if(bucket_no[r-1]==1 && r!=p)
			{
				WT3 = WT2+BucketWt[r-1];
				r1 = 0;
			//}

			for(s=r+1;s<=MAX_HEADS;s++)
			{
				if(bucket_no[s-1]==1 && s!=p)
				{
					WT4 = WT3+BucketWt[s-1];
					s1 = 0;
				}
				/*if(NewTargetWt>WT4)
				{
					ErrorWt = NewTargetWt - WT4;
				}
				else
				{
					ErrorWt = WT4 - NewTargetWt;
				}*/

				if(WT4>=(NewTargetWt-NegErrorWt))
				{
					if(NewTargetWt>WT4)
					{
						ErrorWt = NewTargetWt - WT4;
					}
					else
					{
						ErrorWt = WT4 - NewTargetWt;
					}
				if(combination_weight_flag == 0 || (combination_weight_flag == 1 && ErrorWt<BandErrorWt))
				{
					combination_weight_flag  = 1;
					BandErrorWt = ErrorWt;
					combination_ok_flag = 1;
					under_over_combination_ok_flag = 1;
					//Trigger_For_Data_History = 1;
					//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
					DisplayWt = WT4;

					if(p1 == 0)
						Head[0] = p;
					if(q1 == 0)
						Head[1] = q;
					if(r1 == 0)
						Head[2] = r;
					if(s1 == 0)
						Head[3] = s;
					if(t1 == 0)
						Head[4] = 0;
					if(u1 == 0)
						Head[5] = 0;
					if(v1 == 0)
						Head[6] = 0;
					if(w1 == 0)
						Head[7] = 0;
				}
				}
			}
			}
			}
		}
	}

	Clear_Watchdog();

	WT1 = BucketWt[head_number-1];
	p1  = 0;
	p   = head_number;

	for(q=1;q<=MAX_HEADS;q++)
	{
		if(bucket_no[q-1]==1 && q!=p)
		{
			WT2 = WT1+BucketWt[q-1];
			q1 = 0;
		//}

		for(r=q+1;r<=MAX_HEADS;r++)
		{
			if(bucket_no[r-1]==1 && r!=p)
			{
				WT3 = WT2+BucketWt[r-1];
				r1 = 0;
			//}

			for(s=r+1;s<=MAX_HEADS;s++)
			{
				if(bucket_no[s-1]==1 && s!=p)
				{
					WT4 = WT3+BucketWt[s-1];
					s1 = 0;
				//}

				for(t=s+1;t<=MAX_HEADS;t++)
				{
					if(bucket_no[t-1]==1 && t!=p)
					{
						WT5 = WT4+BucketWt[t-1];
						t1 = 0;
					//}

					/*if(NewTargetWt>WT5)
					{
						ErrorWt = NewTargetWt - WT5;
					}
					else
					{
						ErrorWt = WT5 - NewTargetWt;
					}*/

					if(WT5>=(NewTargetWt-NegErrorWt))
					{
						if(NewTargetWt>WT5)
						{
							ErrorWt = NewTargetWt - WT5;
						}
						else
						{
							ErrorWt = WT5 - NewTargetWt;
						}
					if(combination_weight_flag == 0 || (combination_weight_flag == 1 && ErrorWt<BandErrorWt))
					{
						combination_weight_flag  = 1;
						BandErrorWt = ErrorWt;
						combination_ok_flag = 1;
						under_over_combination_ok_flag = 1;
						//Trigger_For_Data_History = 1;
						//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
						DisplayWt = WT5;

						if(p1 == 0)
							Head[0] = p;
						if(q1 == 0)
							Head[1] = q;
						if(r1 == 0)
							Head[2] = r;
						if(s1 == 0)
							Head[3] = s;
						if(t1 == 0)
							Head[4] = t;
						if(u1 == 0)
							Head[5] = 0;
						if(v1 == 0)
							Head[6] = 0;
						if(w1 == 0)
							Head[7] = 0;
					}
					}
				}
				}
				}
			}
			}
			}
		}
	}

	Clear_Watchdog();

	WT1 = BucketWt[head_number-1];
	p1  = 0;
	p   = head_number;


	for(q=1;q<=MAX_HEADS;q++)
	{
		if(bucket_no[q-1]==1 && q!=p)
		{
			WT2 = WT1+BucketWt[q-1];
			q1 = 0;
		//}

		for(r=q+1;r<=MAX_HEADS;r++)
		{
			if(bucket_no[r-1]==1 && r!=p)
			{
				WT3 = WT2+BucketWt[r-1];
				r1 = 0;
			//}

			for(s=r+1;s<=MAX_HEADS;s++)
			{
				if(bucket_no[s-1]==1 && s!=p)
				{
					WT4 = WT3+BucketWt[s-1];
					s1 = 0;
				//}
				for(t=s+1;t<=MAX_HEADS;t++)
				{
					if(bucket_no[t-1]==1 && t!=p)
					{
						WT5 = WT4+BucketWt[t-1];
						t1 = 0;
					//}

					for(u=t+1;u<=MAX_HEADS;u++)
					{
						if(bucket_no[u-1]==1 && u!=p)
						{
							WT6 = WT5+BucketWt[u-1];
							u1 = 0;
						//}

						/*if(NewTargetWt>WT6)
						{
							ErrorWt = NewTargetWt - WT6;
						}
						else
						{
							ErrorWt = WT6 - NewTargetWt;
						}*/

						if(WT6>=(NewTargetWt-NegErrorWt))
						{

							if(NewTargetWt>WT6)
							{
								ErrorWt = NewTargetWt - WT6;
							}
							else
							{
								ErrorWt = WT6 - NewTargetWt;
							}
						if(combination_weight_flag == 0 || (combination_weight_flag == 1 && ErrorWt<BandErrorWt))
						{
							combination_weight_flag  = 1;
							BandErrorWt = ErrorWt;
							combination_ok_flag = 1;
							under_over_combination_ok_flag = 1;
							//Trigger_For_Data_History = 1;
							//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
							DisplayWt = WT6;

							if(p1 == 0)
								Head[0] = p;
							if(q1 == 0)
								Head[1] = q;
							if(r1 == 0)
								Head[2] = r;
							if(s1 == 0)
								Head[3] = s;
							if(t1 == 0)
								Head[4] = t;
							if(u1 == 0)
								Head[5] = u;
							if(v1 == 0)
								Head[6] = 0;
							if(w1 == 0)
								Head[7] = 0;
						}
						}
					}
					}}}}}
				}
			}
		}
	}

	Clear_Watchdog();

	WT1 = BucketWt[head_number-1];
	p1  = 0;
	p   = head_number;


	for(q=1;q<=MAX_HEADS;q++)
	{
		if(bucket_no[q-1]==1 && q!=p)
		{
			WT2 = WT1+BucketWt[q-1];
			q1 = 0;
		//}

		for(r=q+1;r<=MAX_HEADS;r++)
		{
			if(bucket_no[r-1]==1 && r!=p)
			{
				WT3 = WT2+BucketWt[r-1];
				r1 = 0;
			//}

			for(s=r+1;s<=MAX_HEADS;s++)
			{
				if(bucket_no[s-1]==1 && s!=p)
				{
					WT4 = WT3+BucketWt[s-1];
					s1 = 0;
				//}

				for(t=s+1;t<=MAX_HEADS;t++)
				{
					if(bucket_no[t-1]==1 && t!=p)
					{
						WT5 = WT4+BucketWt[t-1];
						t1 = 0;
					//}

					for(u=t+1;u<=MAX_HEADS;u++)
					{
						if(bucket_no[u-1]==1 && u!=p)
						{
							WT6 = WT5+BucketWt[u-1];
							u1 = 0;
						//}

						for(v=u+1;v<=MAX_HEADS;v++)
						{
							if(bucket_no[v-1]==1 && v!=p)
							{
								WT7 = WT6+BucketWt[v-1];
								v1 = 0;
							//}

							/*if(NewTargetWt>WT7)
							{
								ErrorWt = NewTargetWt - WT7;
							}
							else
							{
								ErrorWt = WT7 - NewTargetWt;
							}*/

							if(WT7>=(NewTargetWt-NegErrorWt))
							{
								if(NewTargetWt>WT7)
								{
									ErrorWt = NewTargetWt - WT7;
								}
								else
								{
									ErrorWt = WT7 - NewTargetWt;
								}
							if(combination_weight_flag == 0 || (combination_weight_flag == 1 && ErrorWt<BandErrorWt))
							{
								combination_weight_flag  = 1;
								BandErrorWt = ErrorWt;
								combination_ok_flag = 1;
								under_over_combination_ok_flag = 1;
								//Trigger_For_Data_History = 1;
								//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;
								DisplayWt = WT7;

								if(p1 == 0)
									Head[0] = p;
								if(q1 == 0)
									Head[1] = q;
								if(r1 == 0)
									Head[2] = r;
								if(s1 == 0)
									Head[3] = s;
								if(t1 == 0)
									Head[4] = t;
								if(u1 == 0)
									Head[5] = u;
								if(v1 == 0)
									Head[6] = v;
								if(w1 == 0)
									Head[7] = 0;
							}
							}}}}}}}
						}
					}
				}
			}
		}
	}

	Clear_Watchdog();

	WT1 = BucketWt[head_number-1];
	p1  = 0;
	p   = head_number;


	for(q=1;q<=MAX_HEADS;q++)
	{
		if(bucket_no[q-1]==1 && q!=p)
		{
			WT2 = WT1+BucketWt[q-1];
			q1 = 0;
		//}

		for(r=q+1;r<=MAX_HEADS;r++)
		{
			if(bucket_no[r-1]==1 && r!=p)
			{
				WT3 = WT2+BucketWt[r-1];
				r1 = 0;
			//}

			for(s=r+1;s<=MAX_HEADS;s++)
			{
				if(bucket_no[s-1]==1 && s!=p)
				{
					WT4 = WT3+BucketWt[s-1];
					s1 = 0;
				//}

				for(t=s+1;t<=MAX_HEADS;t++)
				{
					if(bucket_no[t-1]==1 && t!=p)
					{
						WT5 = WT4+BucketWt[t-1];
						t1 = 0;
					//}

					for(u=t+1;u<=MAX_HEADS;u++)
					{
						if(bucket_no[u-1]==1 && u!=p)
						{
							WT6 = WT5+BucketWt[u-1];
							u1 = 0;
						//}

						for(v=u+1;v<=MAX_HEADS;v++)
						{
							if(bucket_no[v-1]==1 && v!=p)
							{
								WT7 = WT6+BucketWt[v-1];
								v1 = 0;
							//}

							for(w=v+1;w<=MAX_HEADS;w++)
							{
								if(bucket_no[w-1]==1 && w!=p)
								{
									WT8 = WT7+BucketWt[w-1];
									w1 = 0;
								//}
								/*if(NewTargetWt>WT8)
								{
									ErrorWt = NewTargetWt - WT8;
								}
								else
								{
									ErrorWt = WT8 - NewTargetWt;
								}*/

								if(WT8>=(NewTargetWt-NegErrorWt))
								{
									if(NewTargetWt>WT8)
									{
										ErrorWt = NewTargetWt - WT8;
									}
									else
									{
										ErrorWt = WT8 - NewTargetWt;
									}

								if(combination_weight_flag == 0 || (combination_weight_flag == 1 && ErrorWt<BandErrorWt))
								{
									combination_weight_flag  = 1;
									BandErrorWt = ErrorWt;
									combination_ok_flag = 1;
									under_over_combination_ok_flag = 1;
									//Trigger_For_Data_History = 1;
									//Trigger_Data_History_Time = MAX_TRIGGER_DATA_HISTORY_TIME;

									DisplayWt = WT8;

									if(p1 == 0)
										Head[0] = p;
									if(q1 == 0)
										Head[1] = q;
									if(r1 == 0)
										Head[2] = r;
									if(s1 == 0)
										Head[3] = s;
									if(t1 == 0)
										Head[4] = t;
									if(u1 == 0)
										Head[5] = u;
									if(v1 == 0)
										Head[6] = v;
									if(w1 == 0)
										Head[7] = w;
								}
								}}}}}}}}
							}
						}
					}
				}
			}
		}
	}
	OVER_UNDER_WT = 1;
	over_weight_popup = 1;
}


void Reset_Skip_Combi_Para()
{

}
