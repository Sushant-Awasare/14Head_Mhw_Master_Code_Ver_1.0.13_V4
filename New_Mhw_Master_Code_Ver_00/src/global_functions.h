/*
 * global_functions.h
 *
 *  Created on: Feb 9, 2022
 *      Author: Dnyaneshwar
 */

#ifndef GLOBAL_FUNCTIONS_H_
#define GLOBAL_FUNCTIONS_H_

void Interval_Timer_Isr(void);
void Initialize_Port_Pins();
void Initialize_Peripherals();
void Initial_Values();

void Aux_Bucket_Dump();
void Reset_Statistics();
void Calculate_Standard_Deviation();

void check_new_firing(void);
void check_new_time(void);
void Mhs_Start_Stop();
void Check_AllHeads_Ontime();
void Check_AllHeads_Firing();
void Check_Bucket_Status();
void Check_Bucket_Selected();
//void Set_Stepper_Pattern();
void Set_Para();
void Clear_Watchdog();
void Zeroing();
void check_aux_home();
void Aux_Close();
void Aux_Open();
void AutoCorrection();
void aux_can_frame();
void read_bit_as_word();
void write_bit_as_word();
#endif /* GLOBAL_FUNCTIONS_H_ */
