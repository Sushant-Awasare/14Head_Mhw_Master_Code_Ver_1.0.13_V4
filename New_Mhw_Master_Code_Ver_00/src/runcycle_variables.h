/*
 * runcycle_variables.h
 *
 *  Created on: Feb 17, 2022
 *      Author: Dnyaneshwar
 */

#ifndef RUNCYCLE_VARIABLES_H_
#define RUNCYCLE_VARIABLES_H_

#ifndef RUN_VAR
# define _RUN extern
# define _RUN_INIT(x)
#else
# define _RUN
# define _RUN_INIT(x)  = x
#endif


_RUN _Bool combination_ok_flag _RUN_INIT(0);
_RUN _Bool under_over_combination_ok_flag _RUN_INIT(0);
_RUN _Bool previous_dump_flag _RUN_INIT(0);
_RUN _Bool dry_runcycle_flag _RUN_INIT(0);
_RUN _Bool Flag_Check_Combi _RUN_INIT(0);
_RUN _Bool skip_for_combination _RUN_INIT(0);

_RUN _Bool aux_frame_once _RUN_INIT(0);
_RUN _Bool run_with_aux_bucket_flag _RUN_INIT(0);
_RUN _Bool send_reacharge_frame_flag _RUN_INIT(0);
_RUN _Bool abort_recharge_frame_flag _RUN_INIT(0);

_RUN _Bool next_combination_ready_flag _RUN_INIT(0);


_RUN _Bool ready_on_flag _RUN_INIT(0);
_RUN unsigned int ready_time _RUN_INIT(0);

//--------------variables for actual speed--------------------------------//

//_RUN unsigned int Actual_Dump_Speed _RUN_INIT(0);
//_RUN unsigned int actual_speed_flag _RUN_INIT(0),speed_flag _RUN_INIT(0);

//_RUN unsigned int Actual_Dump_Counter _RUN_INIT(0);

//_RUN long unsigned int total_dump_in_day;


//------------------------variables for speed in run cycle------------//
_RUN unsigned int Sec_Counter _RUN_INIT(0),Ms_Counter _RUN_INIT(0);
_RUN unsigned int Run_Dump_Counter _RUN_INIT(0);
_RUN signed int Actual_Run_Speed _RUN_INIT(0);

//----------------------variables for average heads in combination-----//
_RUN unsigned int avg _RUN_INIT(0),Combi_Head_Counter _RUN_INIT(0);
_RUN unsigned int Total_Combi_head _RUN_INIT(0),Average_Combi_Head _RUN_INIT(0);

//----------------------variables for average stable heads -----------//
_RUN unsigned int Total_Stable_head _RUN_INIT(0),Stable_Head_Counter _RUN_INIT(0),Average_Stable_Head _RUN_INIT(0);

//----------------------only selected heads recharge and dump------///
_RUN unsigned int Actual_Dump _RUN_INIT(0),Actual_Recharge _RUN_INIT(0);


//---------------------combination time check-------------------///
_RUN unsigned int check_combi_time _RUN_INIT(0);



//------------------ for run cycle check--------------------//

_RUN unsigned int get_weight_counter _RUN_INIT(0),cycle_equal_time_flag _RUN_INIT(0),weight_flag _RUN_INIT(0),error_wt_flag _RUN_INIT(0);

_RUN unsigned int Trigger_Data_History_Flag _RUN_INIT(0),Trigger_Data_History_Time _RUN_INIT(0);

_RUN unsigned int Selected_For_Run _RUN_INIT(0),Actual_Check _RUN_INIT(0);

_RUN unsigned int mhs_on_flag _RUN_INIT(0),mhs_off_flag _RUN_INIT(0),mhs_start_counter _RUN_INIT(0),mhs_stop_counter _RUN_INIT(0),pin_status _RUN_INIT(0);
_RUN _Bool hold_run_cycle_tmr _RUN_INIT(0);

//------------------ for Multi Dump operation--------------------//

/*_RUN unsigned int Multi_Dump_Wt,Multi_Dump_Target_Wt,Multi_Dump_Count,Multi_dump_display_wt,New_multidump_targetwt,Multi_Dump_Total;
_RUN unsigned int multi_dump_flag;
_RUN unsigned int MULTI_DUMP_DIV_COUNT;*/

_RUN _Bool skip_for_combination_1 _RUN_INIT(0);
_RUN _Bool skip_for_combination_2 _RUN_INIT(0);
_RUN _Bool skip_for_combination_3 _RUN_INIT(0);
_RUN _Bool skip_for_combination_4 _RUN_INIT(0);
_RUN _Bool skip_for_combination_5 _RUN_INIT(0);
_RUN _Bool skip_for_combination_6 _RUN_INIT(0);
_RUN _Bool skip_for_combination_7 _RUN_INIT(0);
_RUN _Bool skip_for_combination_8 _RUN_INIT(0);
_RUN _Bool skip_for_combination_9 _RUN_INIT(0);
_RUN _Bool skip_for_combination_10 _RUN_INIT(0);
_RUN _Bool skip_for_combination_11 _RUN_INIT(0);
_RUN _Bool skip_for_combination_12 _RUN_INIT(0);
_RUN _Bool skip_for_combination_13 _RUN_INIT(0);
_RUN _Bool skip_for_combination_14 _RUN_INIT(0);

_RUN unsigned int skip_for_combination_count _RUN_INIT(0);

/*_RUN unsigned int skip_for_combination_1 _RUN_INIT(0),skip_for_combination_2 _RUN_INIT(0),skip_for_combination_3 _RUN_INIT(0),skip_for_combination_4 _RUN_INIT(0),skip_for_combination_5 _RUN_INIT(0);
_RUN unsigned int skip_for_combination_6 _RUN_INIT(0),skip_for_combination_7 _RUN_INIT(0),skip_for_combination_8 _RUN_INIT(0),skip_for_combination_9 _RUN_INIT(0),skip_for_combination_10 _RUN_INIT(0);
_RUN unsigned int skip_for_combination_11 _RUN_INIT(0),skip_for_combination_12 _RUN_INIT(0),skip_for_combination_13 _RUN_INIT(0),skip_for_combination_14 _RUN_INIT(0);*/

//_RUN unsigned int skip_for_combination _RUN_INIT(0);
_RUN _Bool skip_flag _RUN_INIT(0);

//_RUN unsigned int Power_On_Run _RUN_INIT(0);


_RUN _Bool fb1_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb2_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb3_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb4_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb5_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb6_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb7_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb8_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb9_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb10_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb11_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb12_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb13_nothoming_flag _RUN_INIT(0);
_RUN _Bool fb14_nothoming_flag _RUN_INIT(0);

_RUN _Bool wb1_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb2_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb3_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb4_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb5_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb6_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb7_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb8_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb9_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb10_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb11_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb12_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb13_nothoming_flag _RUN_INIT(0);
_RUN _Bool wb14_nothoming_flag _RUN_INIT(0);

/*_RUN unsigned int fb1_nothoming_flag _RUN_INIT(0),wb1_nothoming_flag _RUN_INIT(0),fb2_nothoming_flag _RUN_INIT(0),wb2_nothoming_flag _RUN_INIT(0),fb3_nothoming_flag _RUN_INIT(0),wb3_nothoming_flag _RUN_INIT(0);
_RUN unsigned int fb4_nothoming_flag _RUN_INIT(0),wb4_nothoming_flag _RUN_INIT(0),fb5_nothoming_flag _RUN_INIT(0),wb5_nothoming_flag _RUN_INIT(0),fb6_nothoming_flag _RUN_INIT(0),wb6_nothoming_flag _RUN_INIT(0);
_RUN unsigned int fb7_nothoming_flag _RUN_INIT(0),wb7_nothoming_flag _RUN_INIT(0),fb8_nothoming_flag _RUN_INIT(0),wb8_nothoming_flag _RUN_INIT(0),fb9_nothoming_flag _RUN_INIT(0),wb9_nothoming_flag _RUN_INIT(0);
_RUN unsigned int fb10_nothoming_flag _RUN_INIT(0),wb10_nothoming_flag _RUN_INIT(0),fb11_nothoming_flag _RUN_INIT(0),wb11_nothoming_flag _RUN_INIT(0),fb12_nothoming_flag _RUN_INIT(0),wb12_nothoming_flag _RUN_INIT(0);
_RUN unsigned int fb13_nothoming_flag _RUN_INIT(0),wb13_nothoming_flag _RUN_INIT(0),fb14_nothoming_flag _RUN_INIT(0),wb14_nothoming_flag _RUN_INIT(0);*/

/*_RUN unsigned int head1_wt_received_flag _RUN_INIT(0),head2_wt_received_flag _RUN_INIT(0),head3_wt_received_flag _RUN_INIT(0),head4_wt_received_flag _RUN_INIT(0),head5_wt_received_flag _RUN_INIT(0);
_RUN unsigned int head6_wt_received_flag _RUN_INIT(0),head7_wt_received_flag _RUN_INIT(0),head8_wt_received_flag _RUN_INIT(0),head9_wt_received_flag _RUN_INIT(0),head10_wt_received_flag _RUN_INIT(0);
_RUN unsigned int head11_wt_received_flag _RUN_INIT(0),head12_wt_received_flag _RUN_INIT(0),head13_wt_received_flag _RUN_INIT(0),head14_wt_received_flag _RUN_INIT(0);*/

_RUN _Bool head1_wt_received_flag _RUN_INIT(0);
_RUN _Bool head2_wt_received_flag _RUN_INIT(0);
_RUN _Bool head3_wt_received_flag _RUN_INIT(0);
_RUN _Bool head4_wt_received_flag _RUN_INIT(0);
_RUN _Bool head5_wt_received_flag _RUN_INIT(0);
_RUN _Bool head6_wt_received_flag _RUN_INIT(0);
_RUN _Bool head7_wt_received_flag _RUN_INIT(0);
_RUN _Bool head8_wt_received_flag _RUN_INIT(0);
_RUN _Bool head9_wt_received_flag _RUN_INIT(0);
_RUN _Bool head10_wt_received_flag _RUN_INIT(0);
_RUN _Bool head11_wt_received_flag _RUN_INIT(0);
_RUN _Bool head12_wt_received_flag _RUN_INIT(0);
_RUN _Bool head13_wt_received_flag _RUN_INIT(0);
_RUN _Bool head14_wt_received_flag _RUN_INIT(0);

_RUN _Bool dump1_slected_flag _RUN_INIT(0);
_RUN _Bool dump2_slected_flag _RUN_INIT(0);
_RUN _Bool dump3_slected_flag _RUN_INIT(0);
_RUN _Bool dump4_slected_flag _RUN_INIT(0);
_RUN _Bool dump5_slected_flag _RUN_INIT(0);
_RUN _Bool dump6_slected_flag _RUN_INIT(0);
_RUN _Bool dump7_slected_flag _RUN_INIT(0);
_RUN _Bool dump8_slected_flag _RUN_INIT(0);
_RUN _Bool dump9_slected_flag _RUN_INIT(0);
_RUN _Bool dump10_slected_flag _RUN_INIT(0);
_RUN _Bool dump11_slected_flag _RUN_INIT(0);
_RUN _Bool dump12_slected_flag _RUN_INIT(0);
_RUN _Bool dump13_slected_flag _RUN_INIT(0);
_RUN _Bool dump14_slected_flag _RUN_INIT(0);

/*_RUN unsigned int dump1_slected_flag _RUN_INIT(0),dump2_slected_flag _RUN_INIT(0),dump3_slected_flag _RUN_INIT(0),dump4_slected_flag _RUN_INIT(0),dump5_slected_flag _RUN_INIT(0);
_RUN unsigned int dump6_slected_flag _RUN_INIT(0),dump7_slected_flag _RUN_INIT(0),dump8_slected_flag _RUN_INIT(0),dump9_slected_flag _RUN_INIT(0),dump10_slected_flag _RUN_INIT(0);
_RUN unsigned int dump11_slected_flag _RUN_INIT(0),dump12_slected_flag _RUN_INIT(0),dump13_slected_flag _RUN_INIT(0),dump14_slected_flag _RUN_INIT(0);*/

//----------------------------------for aux bucket---------------------------//
_RUN unsigned int aux_ready_time _RUN_INIT(0);

_RUN _Bool auxbucket_empty_flag _RUN_INIT(0);
_RUN _Bool aux_ready_flag _RUN_INIT(0);

//---------------------for avr,min,max-------------------------------------//
_RUN unsigned int Avg_weight _RUN_INIT(0), Min_Wt _RUN_INIT(0),Max_Wt _RUN_INIT(0),Disp_Avg_weight _RUN_INIT(0),multi_dump_total_wt _RUN_INIT(0);


//---------------------for standard deviation----------------------------//
_RUN unsigned int Dev_weights[100],Avg_Dev_weights[100],standard_deviation _RUN_INIT(0),dev_wt _RUN_INIT(0),dev_k _RUN_INIT(0),dev_j _RUN_INIT(0),dev_i _RUN_INIT(0),dev_counter _RUN_INIT(0),calculate_devi_flag _RUN_INIT(0);
_RUN unsigned int dev_l _RUN_INIT(0),total_dev_wt _RUN_INIT(0),sum_devi_wt _RUN_INIT(0);
_RUN unsigned int avg_devi_wt _RUN_INIT(0),mean_dev_wt _RUN_INIT(0);

//---------------------for weight graph----------------------------------//
_RUN unsigned int Inbetween_less_counter _RUN_INIT(0),less_counter _RUN_INIT(0),Inbetween_more_counter _RUN_INIT(0),more_counter _RUN_INIT(0),more_counter_1 _RUN_INIT(0),more_counter_2 _RUN_INIT(0),more_counter_3 _RUN_INIT(0);
_RUN unsigned int Inbetween_less_counter_1 _RUN_INIT(0),more_counter_4 _RUN_INIT(0),more_counter_5 _RUN_INIT(0),more_counter_6 _RUN_INIT(0);
/*_RUN unsigned char head1_not_in_combi_flag _RUN_INIT(0),head2_not_in_combi_flag _RUN_INIT(0),head3_not_in_combi_flag _RUN_INIT(0),head4_not_in_combi_flag _RUN_INIT(0),head5_not_in_combi_flag _RUN_INIT(0);
_RUN unsigned char head6_not_in_combi_flag _RUN_INIT(0),head7_not_in_combi_flag _RUN_INIT(0),head8_not_in_combi_flag _RUN_INIT(0),head9_not_in_combi_flag _RUN_INIT(0),head10_not_in_combi_flag _RUN_INIT(0);
_RUN unsigned char head11_not_in_combi_flag _RUN_INIT(0),head12_not_in_combi_flag _RUN_INIT(0),head13_not_in_combi_flag _RUN_INIT(0),head14_not_in_combi_flag _RUN_INIT(0);
_RUN unsigned char reject_flag _RUN_INIT(0);*/

_RUN _Bool head1_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head2_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head3_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head4_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head5_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head6_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head7_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head8_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head9_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head10_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head11_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head12_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head13_not_in_combi_flag _RUN_INIT(0);
_RUN _Bool head14_not_in_combi_flag _RUN_INIT(0);

_RUN _Bool reject_flag _RUN_INIT(0);

//------------------for stagger operation--------------------------//
_RUN unsigned int 	stagger_time _RUN_INIT(0),stagger_count _RUN_INIT(0),stagger_wait_time _RUN_INIT(0);
_RUN unsigned int 	cycle_equal_time _RUN_INIT(0);
_RUN unsigned int 	WT1 _RUN_INIT(0),WT2 _RUN_INIT(0),WT3 _RUN_INIT(0),WT4 _RUN_INIT(0),WT5 _RUN_INIT(0),WT6 _RUN_INIT(0),WT7 _RUN_INIT(0),WT8 _RUN_INIT(0);
_RUN unsigned int 	WT9 _RUN_INIT(0),WT10 _RUN_INIT(0),WT11 _RUN_INIT(0),WT12 _RUN_INIT(0);
_RUN unsigned int	Dump_Selected _RUN_INIT(0),Recharge_Selected _RUN_INIT(0);
_RUN unsigned int 	ErrorWt _RUN_INIT(0);
_RUN unsigned int	Check_Selected _RUN_INIT(0),Weight_Selected _RUN_INIT(0);
_RUN unsigned int 	weight_cycle_flag _RUN_INIT(1),dump_cycle_flag _RUN_INIT(1),idle_flag _RUN_INIT(0);
_RUN unsigned int 	DisplayWt _RUN_INIT(0);
_RUN signed int     BucWeight _RUN_INIT(0);
_RUN signed int 	Bucket[10];
_RUN unsigned int   iidx _RUN_INIT(0),jidx _RUN_INIT(0);
_RUN volatile signed int 	BucketWt[14] ;
_RUN unsigned int 	Head[15];
_RUN signed int 	bucket_no[15];
_RUN unsigned char 	p _RUN_INIT(0),q _RUN_INIT(0),r _RUN_INIT(0),s _RUN_INIT(0),t _RUN_INIT(0),u _RUN_INIT(0),v _RUN_INIT(0),w _RUN_INIT(0);
_RUN unsigned int 	BandErrorWt _RUN_INIT(0);
_RUN unsigned int   Heads_To_Dump _RUN_INIT(0),Heads_To_recharge _RUN_INIT(0);



_RUN _Bool p1 _RUN_INIT(0);
_RUN _Bool q1 _RUN_INIT(0);
_RUN _Bool r1 _RUN_INIT(0);
_RUN _Bool s1 _RUN_INIT(0);
_RUN _Bool t1 _RUN_INIT(0);
_RUN _Bool u1 _RUN_INIT(0);
_RUN _Bool v1 _RUN_INIT(0);
_RUN _Bool w1 _RUN_INIT(0);

_RUN _Bool skip_recharge_flag _RUN_INIT(0);
_RUN _Bool skip_for_recharge _RUN_INIT(0);
_RUN _Bool combi_cycle_flag _RUN_INIT(0);
_RUN _Bool stagger_flag _RUN_INIT(0);

_RUN _Bool skip_for_recharge_1 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_2 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_3 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_4 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_5 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_6 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_7 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_8 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_9 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_10 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_11 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_12 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_13 _RUN_INIT(0);
_RUN _Bool skip_for_recharge_14 _RUN_INIT(0);

_RUN unsigned int Recharge_Frame_Time _RUN_INIT(0);
_RUN unsigned int not_in_combi_count _RUN_INIT(0);
_RUN unsigned int head_number _RUN_INIT(0);

_RUN unsigned int combi_i _RUN_INIT(0),all_combi_wt _RUN_INIT(0);

_RUN _Bool no_combi_popup_flag _RUN_INIT(0);
_RUN unsigned int no_combi_popup_time _RUN_INIT(0);


//Added on 28/07
_RUN signed int Bkt_Wt_BFr[15];

#endif /* RUNCYCLE_VARIABLES_H_ */
