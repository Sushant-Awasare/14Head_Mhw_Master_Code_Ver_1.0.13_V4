/*
 * global_variables.h
 *
 *  Created on: Feb 9, 2022
 *      Author: Dnyaneshwar
 */

#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

#include <stdint.h>

#ifndef GLOBAL_VAR
# define _GLOBAL extern
# define _GLOBAL_INIT(x)
#else
# define _GLOBAL
# define _GLOBAL_INIT(x)  = x
#endif

_GLOBAL _Bool  	dump_frame_sent_flag _GLOBAL_INIT(0);
_GLOBAL _Bool  	recharge_frame_sent_flag _GLOBAL_INIT(0);
_GLOBAL _Bool  	aux_frame_sent_flag _GLOBAL_INIT(0);

_GLOBAL _Bool  	Power_On_Run _GLOBAL_INIT(0);
_GLOBAL _Bool 	wt_change_flag _GLOBAL_INIT(0);
_GLOBAL _Bool 	wt_change _GLOBAL_INIT(0);
_GLOBAL _Bool 	Adc_para_flag _GLOBAL_INIT(0);

_GLOBAL _Bool 	fir_flag _GLOBAL_INIT(0);
_GLOBAL _Bool	on_flag _GLOBAL_INIT(0);

_GLOBAL _Bool	multi_dump_flag _GLOBAL_INIT(0);


_GLOBAL _Bool Fb_feedback_1 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_2 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_3 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_4 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_5 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_6 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_7 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_8 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_9 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_10 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_11 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_12 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_13 _GLOBAL_INIT(0);
_GLOBAL _Bool Fb_feedback_14 _GLOBAL_INIT(0);


_GLOBAL _Bool Wb_feedback_1 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_2 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_3 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_4 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_5 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_6 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_7 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_8 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_9 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_10 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_11 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_12 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_13 _GLOBAL_INIT(0);
_GLOBAL _Bool Wb_feedback_14 _GLOBAL_INIT(0);

_GLOBAL _Bool single_cycle_flag _GLOBAL_INIT(0);

_GLOBAL _Bool auto_correction_flag _GLOBAL_INIT(0);

_GLOBAL _Bool no_select_coil_flag _GLOBAL_INIT(0);

_GLOBAL unsigned int Recharge_Divisor _GLOBAL_INIT(3);
_GLOBAL unsigned int Dryrun_weight_addition _GLOBAL_INIT(1);
_GLOBAL unsigned int Dryrun_weight_Division _GLOBAL_INIT(3);


_GLOBAL unsigned int dump_bucket_count _GLOBAL_INIT(0), dump_feedback_count _GLOBAL_INIT(0);

_GLOBAL unsigned int MHW_READY_TIME _GLOBAL_INIT(0);
_GLOBAL unsigned int MAX_STAGGER_TIME _GLOBAL_INIT(0);
_GLOBAL unsigned int state _GLOBAL_INIT(0);
_GLOBAL unsigned int Selected_Heads _GLOBAL_INIT(0),Head_counter _GLOBAL_INIT(0);
_GLOBAL unsigned int RunCycleTime _GLOBAL_INIT(0);
_GLOBAL signed int NewTargetWt _GLOBAL_INIT(0);
_GLOBAL unsigned int previous_weight _GLOBAL_INIT(0);


_GLOBAL unsigned int Firing_angle_1 _GLOBAL_INIT(0),Firing_angle_2 _GLOBAL_INIT(0),Firing_angle_3 _GLOBAL_INIT(0),Firing_angle_4 _GLOBAL_INIT(0),Firing_angle_5 _GLOBAL_INIT(0);
_GLOBAL unsigned int Firing_angle_6 _GLOBAL_INIT(0),Firing_angle_7 _GLOBAL_INIT(0),Firing_angle_8 _GLOBAL_INIT(0),Firing_angle_9 _GLOBAL_INIT(0),Firing_angle_10 _GLOBAL_INIT(0);
_GLOBAL unsigned int Firing_angle_11 _GLOBAL_INIT(0),Firing_angle_12 _GLOBAL_INIT(0),Firing_angle_13 _GLOBAL_INIT(0),Firing_angle_14 _GLOBAL_INIT(0);
_GLOBAL unsigned int All_head_firing _GLOBAL_INIT(0);
_GLOBAL unsigned int All_head_lf_delay _GLOBAL_INIT(0);
_GLOBAL unsigned int All_head_lf_ondelay _GLOBAL_INIT(0),All_head_lf_ontime _GLOBAL_INIT(0);
_GLOBAL unsigned int All_head_fb_ondelay _GLOBAL_INIT(0),All_head_fb_ontime _GLOBAL_INIT(0);
_GLOBAL unsigned int All_head_wb_ondelay _GLOBAL_INIT(0),All_head_wb_ontime _GLOBAL_INIT(0);
_GLOBAL unsigned int All_head_cb_ondelay _GLOBAL_INIT(0),All_head_cb_ontime _GLOBAL_INIT(0);

_GLOBAL unsigned int All_head_stepper_pattern _GLOBAL_INIT(0);
_GLOBAL unsigned int Discharge_ontime _GLOBAL_INIT(0);

_GLOBAL unsigned int NO_SELECT_COUNT _GLOBAL_INIT(0);
_GLOBAL unsigned int Previous_no_select_count _GLOBAL_INIT(0);

_GLOBAL unsigned int Mainhead_Firing_angle _GLOBAL_INIT(0),Mainhead_Firing _GLOBAL_INIT(0);
_GLOBAL unsigned int Mainhead_Ontime _GLOBAL_INIT(0);

_GLOBAL unsigned int ADC_GAIN _GLOBAL_INIT(0),ADC_SPEED _GLOBAL_INIT(0),ADC_SAMPLE _GLOBAL_INIT(0),ADC_DIFFERENCE_COUNT _GLOBAL_INIT(0),ADC_STABLE_COUNT _GLOBAL_INIT(0),
	                 AUTO_ZERO_INTERVAL _GLOBAL_INIT(0),WEIGHING_RANGE _GLOBAL_INIT(0),COMPENSATION _GLOBAL_INIT(0),STABLE_TIME _GLOBAL_INIT(0);

_GLOBAL unsigned int Adc_para_time _GLOBAL_INIT(0);



_GLOBAL unsigned int Parameter_flag _GLOBAL_INIT(0);
_GLOBAL unsigned int On_Time_1 _GLOBAL_INIT(0),On_Time_2 _GLOBAL_INIT(0),On_Time_3 _GLOBAL_INIT(0),On_Time_4 _GLOBAL_INIT(0),On_Time_5 _GLOBAL_INIT(0);
_GLOBAL unsigned int On_Time_6 _GLOBAL_INIT(0),On_Time_7 _GLOBAL_INIT(0),On_Time_8 _GLOBAL_INIT(0),On_Time_9 _GLOBAL_INIT(0),On_Time_10 _GLOBAL_INIT(0);
_GLOBAL unsigned int On_Time_11 _GLOBAL_INIT(0),On_Time_12 _GLOBAL_INIT(0),On_Time_13 _GLOBAL_INIT(0),On_Time_14 _GLOBAL_INIT(0);

_GLOBAL unsigned int fir _GLOBAL_INIT(0);
_GLOBAL unsigned int ontime _GLOBAL_INIT(0),on_time_flag _GLOBAL_INIT(0);

_GLOBAL signed int 	Tare_Count[15];
_GLOBAL signed int 	Calib_Count[15];
_GLOBAL signed int 	Calib_Slope[15];

_GLOBAL signed int Multi_Dump_Wt _GLOBAL_INIT(0),Multi_Dump_Target_Wt _GLOBAL_INIT(0),Multi_Dump_Count _GLOBAL_INIT(0),Multi_dump_display_wt _GLOBAL_INIT(0),New_multidump_targetwt _GLOBAL_INIT(0),Multi_Dump_Total _GLOBAL_INIT(0);
_GLOBAL signed int MULTI_DUMP_DIV_COUNT _GLOBAL_INIT(0);
_GLOBAL signed int NegErrorWt _GLOBAL_INIT(0), PosErrorWt _GLOBAL_INIT(0), ErrorBandWt _GLOBAL_INIT(0);
_GLOBAL signed int under_over_NegErrorWt _GLOBAL_INIT(0), under_over_PosErrorWt _GLOBAL_INIT(0);
//--------------variables for actual speed--------------------------------//

_GLOBAL long unsigned int total_dump_in_day _GLOBAL_INIT(0);

//--------------variables for Calibration--------------------------------//

_GLOBAL unsigned char index _GLOBAL_INIT(0);
_GLOBAL unsigned char idy _GLOBAL_INIT(0);

_GLOBAL _Bool Flag_AllHead _GLOBAL_INIT(0);
_GLOBAL _Bool Flag_AllHead_firing _GLOBAL_INIT(0);
_GLOBAL _Bool Flag_AllHead_ontime _GLOBAL_INIT(0);

_GLOBAL _Bool Firing_Increment_Flag _GLOBAL_INIT(0);
_GLOBAL _Bool Firing_Decrement_Flag _GLOBAL_INIT(0);

_GLOBAL _Bool Mainhead_Firing_Flag _GLOBAL_INIT(0);
_GLOBAL _Bool Mainhead_Ontime_Flag _GLOBAL_INIT(0);

_GLOBAL _Bool Mainhead_Firing_Increment_Flag _GLOBAL_INIT(0);
_GLOBAL _Bool Mainhead_Firing_Decrement_Flag _GLOBAL_INIT(0);

_GLOBAL _Bool check_in1 _GLOBAL_INIT(0);

_GLOBAL _Bool aux_at_homing _GLOBAL_INIT(0);
_GLOBAL _Bool aux_open_flag _GLOBAL_INIT(0);
_GLOBAL _Bool aux_command_flag _GLOBAL_INIT(0);
_GLOBAL _Bool aux_frame_flag _GLOBAL_INIT(0);
_GLOBAL _Bool mhw_pin _GLOBAL_INIT(0);
_GLOBAL _Bool next_run_flag _GLOBAL_INIT(0);



_GLOBAL unsigned int zeroing_head_count _GLOBAL_INIT(1);
_GLOBAL unsigned int combi_time_flag _GLOBAL_INIT(0),comi _GLOBAL_INIT(0);
_GLOBAL unsigned int manual_once_flag _GLOBAL_INIT(0);
_GLOBAL unsigned int Head_Selected _GLOBAL_INIT(0);


// for cycle time calculations//

_GLOBAL unsigned int While_Loop_Time _GLOBAL_INIT(0),While_Loop_Time_flag _GLOBAL_INIT(0),While_Loop_Time_Count _GLOBAL_INIT(0) ;
_GLOBAL unsigned char  While_Loop_Time_buff[100];

_GLOBAL unsigned int Run_Loop_Time _GLOBAL_INIT(0),Run_Loop_Time_flag _GLOBAL_INIT(0),Run_Loop_Time_Count _GLOBAL_INIT(0) ;
_GLOBAL unsigned char  Run_Loop_Time_buff[150];



//for stable head
/*_GLOBAL unsigned int Head1_Stable _GLOBAL_INIT(0),Head2_Stable _GLOBAL_INIT(0),Head3_Stable _GLOBAL_INIT(0),Head4_Stable _GLOBAL_INIT(0),Head5_Stable _GLOBAL_INIT(0);
_GLOBAL unsigned int Head6_Stable _GLOBAL_INIT(0),Head7_Stable _GLOBAL_INIT(0),Head8_Stable _GLOBAL_INIT(0),Head9_Stable _GLOBAL_INIT(0),Head10_Stable _GLOBAL_INIT(0);
_GLOBAL unsigned int Head11_Stable _GLOBAL_INIT(0),Head12_Stable _GLOBAL_INIT(0),Head13_Stable _GLOBAL_INIT(0),Head14_Stable _GLOBAL_INIT(0);*/

_GLOBAL _Bool Head1_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head2_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head3_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head4_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head5_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head6_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head7_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head8_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head9_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head10_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head11_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head12_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head13_Stable _GLOBAL_INIT(0);
_GLOBAL _Bool Head14_Stable _GLOBAL_INIT(0);


//for checking combination as per J.L.Patil sir logic

_GLOBAL long int Target_Wt _GLOBAL_INIT(0),tmp_wt _GLOBAL_INIT(0),Last_found_wt _GLOBAL_INIT(0),Target_Wt_ULimit _GLOBAL_INIT(0),Target_Wt_LLimit _GLOBAL_INIT(0),tmp_wt_diff _GLOBAL_INIT(0);
_GLOBAL unsigned int Wt_Band _GLOBAL_INIT(0);

_GLOBAL unsigned int TMP_COUNT _GLOBAL_INIT(0);
_GLOBAL unsigned int lp_ctr0 _GLOBAL_INIT(0),lp_ctr1 _GLOBAL_INIT(0),tmp_ptr0 _GLOBAL_INIT(0),tmp_ptr1 _GLOBAL_INIT(0),aa _GLOBAL_INIT(0);

_GLOBAL unsigned int combination_weight _GLOBAL_INIT(0);
_GLOBAL _Bool combination_weight_flag _GLOBAL_INIT(0);

_GLOBAL signed int test1 _GLOBAL_INIT(61665);

// Added for Flash Code
_GLOBAL _Bool HMI_Flash_Write_PE _GLOBAL_INIT(0), HMI_Flash_Write_LCS _GLOBAL_INIT(0),flash_write_done_flag _GLOBAL_INIT(0);
_GLOBAL unsigned int flash_popup_off_time _GLOBAL_INIT(0);
_GLOBAL unsigned int flash_write_time _GLOBAL_INIT(0);
_GLOBAL _Bool slave_flash_write_flash _GLOBAL_INIT(0);

//Added On 28/07/2023
_GLOBAL signed int Fb_Dump_Wt _GLOBAL_INIT(0);

_GLOBAL _Bool wt_feedback_flag _GLOBAL_INIT(0);

//Added on 31/07/2023
union Bit_As_Word{
	struct {
		_Bool Bit0 : 1;
		_Bool Bit1 : 1;
		_Bool Bit2 : 1;
		_Bool Bit3 : 1;
		_Bool Bit4 : 1;
		_Bool Bit5 : 1;
		_Bool Bit6 : 1;
		_Bool Bit7 : 1;
		_Bool Bit8 : 1;
		_Bool Bit9 : 1;
		_Bool Bit10 : 1;
		_Bool Bit11 : 1;
		_Bool Bit12 : 1;
		_Bool Bit13 : 1;
		_Bool Bit14 : 1;
		_Bool Bit15 : 1;
	}Bit_Val;
	int16_t Word_Val;
};
_GLOBAL union Bit_As_Word BAW_1;
_GLOBAL _Bool Power_On_Para_Write _GLOBAL_INIT(0);

#endif /* GLOBAL_VARIABLES_H_ */
