/*
 * runcycle_functions.h
 *
 *  Created on: Feb 17, 2022
 *      Author: Dnyaneshwar
 */

#ifndef RUNCYCLE_FUNCTIONS_H_
#define RUNCYCLE_FUNCTIONS_H_

void Run_Get_weight();
void Run_Operation();
void Check_Combination(void);

void Recharge_Can_Frame();
void Dump_Can_Frame();
void Recharge_Dump_Can_Frame();

void Aux_Bucket_Dump();
void Reset_Statistics();
void Calculate_Standard_Deviation();
void Check_Bucket_Selected();
void Check_Bucket_Status();
void Check_AllHeads_Firing();
void Check_AllHeads_Ontime();
void Check_mainhead_Firing_Ontime();
void Mhs_Start_Stop();
void Calculate_Weights_Counter();
void Calculate_Multidump_Weights_Counter();
void Show_Head_Status();
void Recharge_Head_In_MultiDump();
void Recharge_Head();
void Send_Recharge_Frame();
void Create_Dump_Recharge_Frame();
void Create_Multidump_Recharge_Frame();
void Create_Auxdump_Recharge_Frame();
void Staggerdump_Recharge_Frame();
void Calculate_Speed();
void Send_Dump_Recharge_Frame();
void MultiDump_Cycle_To_Reject();
void Reset_Run_Parameter();
void Dump_Cycle_To_Reject();
void Heads_In_Combination();
void Reset_Skip_Combi_Para();
void DryRunCycle();
void No_Select_Combination();
void Over_Under_Weight();

#endif /* RUNCYCLE_FUNCTIONS_H_ */
