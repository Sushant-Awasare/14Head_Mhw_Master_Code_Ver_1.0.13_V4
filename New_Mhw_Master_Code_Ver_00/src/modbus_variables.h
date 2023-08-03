/*
 * modbus_variables.h
 *
 *  Created on: Feb 9, 2022
 *      Author: Dnyaneshwar
 */

#ifndef MODBUS_VARIABLES_H_
#define MODBUS_VARIABLES_H_

#ifndef MODBUS_VAR
# define _MODBUS extern
# define _MODBUS_INIT(x)
#else
# define _MODBUS
# define _MODBUS_INIT(x)  = x
#endif

#define Coil_Head1Button  			Mod_Coils[0]
#define Coil_Head2Button  			Mod_Coils[1]
#define Coil_Head3Button  			Mod_Coils[2]
#define Coil_Head4Button  			Mod_Coils[3]
#define Coil_Head5Button  			Mod_Coils[4]
#define Coil_Head6Button  			Mod_Coils[5]
#define Coil_Head7Button  			Mod_Coils[6]
#define Coil_Head8Button  			Mod_Coils[7]
#define Coil_Head9Button  			Mod_Coils[8]
#define Coil_Head10Button 			Mod_Coils[9]
//for 14 head
#define Coil_Head11Button			Mod_Coils[10]
#define Coil_Head12Button  			Mod_Coils[11]
#define Coil_Head13Button 			Mod_Coils[12]
#define Coil_Head14Button			Mod_Coils[13]

#define Coil_Head15Button			Mod_Coils[14]  //?

#define Coil_AllHeadsButton			Mod_Coils[15]  //?

#define Coil_MainFeederButton		Mod_Coils[21]
#define Coil_LinearFeederButton		Mod_Coils[22]
#define Coil_FeedBucketButton		Mod_Coils[23]
#define Coil_WeighBucketButton		Mod_Coils[24]
#define Coil_AuxBucketButton		Mod_Coils[25]
#define Coil_SingleCycleButton		Mod_Coils[26]
#define Coil_ContCycleButton		Mod_Coils[27]
#define Coil_DrainCycleButton		Mod_Coils[28]

#define Coil_ManualOprButton		Mod_Coils[30]
#define Coil_CalibOprButton			Mod_Coils[31]
#define Coil_RunStartButton			Mod_Coils[32]     // start

#define Trigger_For_Data_History	Mod_Coils[33]

#define Coil_TeraLoadCellButton		Mod_Coils[40]
#define Coil_CalibLoadCellButton	Mod_Coils[41]
#define Coil_DumpBucketButton		Mod_Coils[42]

#define Coil_RunButton				Mod_Coils[50]   // run
#define Coil_DumpTrigger			Mod_Coils[60]
#define Coil_CombiReady				Mod_Coils[61]
#define Coil_SetParaADC				Mod_Coils[62]
#define Coil_AuxHoming_Error	    Mod_Coils[63]

#define Coil_Firing_Angle_1        	Mod_Coils[70]
#define Coil_Firing_Angle_2        	Mod_Coils[71]
#define Coil_Firing_Angle_3        	Mod_Coils[72]
#define Coil_Firing_Angle_4        	Mod_Coils[73]
#define Coil_Firing_Angle_5        	Mod_Coils[74]
#define Coil_Firing_Angle_6        	Mod_Coils[75]
#define Coil_Firing_Angle_7        	Mod_Coils[76]
#define Coil_Firing_Angle_8        	Mod_Coils[77]
#define Coil_Firing_Angle_9        	Mod_Coils[78]
#define Coil_Firing_Angle_10       	Mod_Coils[79]

//for 14 head
#define Coil_Firing_Angle_11        Mod_Coils[80]
#define Coil_Firing_Angle_12        Mod_Coils[81]
#define Coil_Firing_Angle_13        Mod_Coils[82]
#define Coil_Firing_Angle_14       	Mod_Coils[83]

#define Coil_AllHead_Firing         Mod_Coils[84]
#define Coil_AllHead_OnTime         Mod_Coils[85]

#define Coil_Increment_Firing	   	Mod_Coils[86]
#define Coil_Decrement_Firing	   	Mod_Coils[87]

#define Coil_On_Time_1             	Mod_Coils[90]
#define Coil_On_Time_2             	Mod_Coils[91]
#define Coil_On_Time_3             	Mod_Coils[92]
#define Coil_On_Time_4             	Mod_Coils[93]
#define Coil_On_Time_5             	Mod_Coils[94]
#define Coil_On_Time_6             	Mod_Coils[95]
#define Coil_On_Time_7             	Mod_Coils[96]
#define Coil_On_Time_8             	Mod_Coils[97]
#define Coil_On_Time_9             	Mod_Coils[98]
#define Coil_On_Time_10            	Mod_Coils[99]

//for 14 head
#define Coil_On_Time_11             Mod_Coils[100]
#define Coil_On_Time_12             Mod_Coils[101]
#define Coil_On_Time_13             Mod_Coils[102]
#define Coil_On_Time_14            	Mod_Coils[103]



#define Coil_MainHead_Firing         Mod_Coils[104]
#define Coil_MainHead_OnTime         Mod_Coils[105]

#define Coil_MainIncrement_Firing  	Mod_Coils[106]
#define Coil_MainDecrement_Firing  	Mod_Coils[107]

#define Coil_WtReady1Indc			Mod_Coils[110]
#define Coil_WtTare1Indc			Mod_Coils[111]
#define Coil_WtCalib1Indc			Mod_Coils[112]

#define Coil_WtReady2Indc			Mod_Coils[113]
#define Coil_WtTare2Indc			Mod_Coils[114]
#define Coil_WtCalib2Indc			Mod_Coils[115]

#define Coil_WtReady3Indc			Mod_Coils[120]
#define Coil_WtTare3Indc			Mod_Coils[121]
#define Coil_WtCalib3Indc			Mod_Coils[122]

#define Coil_WtReady4Indc			Mod_Coils[130]
#define Coil_WtTare4Indc			Mod_Coils[131]
#define Coil_WtCalib4Indc			Mod_Coils[132]

#define Coil_WtReady5Indc			Mod_Coils[140]
#define Coil_WtTare5Indc			Mod_Coils[141]
#define Coil_WtCalib5Indc			Mod_Coils[142]

#define Coil_WtReady6Indc			Mod_Coils[150]
#define Coil_WtTare6Indc			Mod_Coils[151]
#define Coil_WtCalib6Indc			Mod_Coils[152]

#define Coil_WtReady7Indc			Mod_Coils[160]
#define Coil_WtTare7Indc			Mod_Coils[161]
#define Coil_WtCalib7Indc			Mod_Coils[162]

#define Coil_WtReady8Indc			Mod_Coils[170]
#define Coil_WtTare8Indc			Mod_Coils[171]
#define Coil_WtCalib8Indc			Mod_Coils[172]

#define Coil_WtReady9Indc			Mod_Coils[180]
#define Coil_WtTare9Indc			Mod_Coils[181]
#define Coil_WtCalib9Indc			Mod_Coils[182]

#define Coil_WtReady10Indc			Mod_Coils[190]
#define Coil_WtTare10Indc			Mod_Coils[191]
#define Coil_WtCalib10Indc			Mod_Coils[192]

#define Coil_Multi_Dump            	Mod_Coils[193]
#define Coil_Standard_Devi         	Mod_Coils[194]
#define Coil_Reset                 	Mod_Coils[195]

#define Coil_Stagger_Dump           Mod_Coils[196]
#define Coil_Set_Stepper_Pattern    Mod_Coils[197]
#define Coil_No_Select_Mode         Mod_Coils[198]

#define COIL_ZEROING                Mod_Coils[199]

#define Coil_WtReady11Indc			Mod_Coils[200]
#define Coil_WtTare11Indc			Mod_Coils[201]
#define Coil_WtCalib11Indc			Mod_Coils[202]

#define Coil_WtReady12Indc			Mod_Coils[203]
#define Coil_WtTare12Indc			Mod_Coils[204]
#define Coil_WtCalib12Indc			Mod_Coils[205]

#define Coil_WtReady13Indc			Mod_Coils[206]
#define Coil_WtTare13Indc			Mod_Coils[207]
#define Coil_WtCalib13Indc			Mod_Coils[208]

#define Coil_WtReady14Indc			Mod_Coils[209]
#define Coil_WtTare14Indc			Mod_Coils[210]
#define Coil_WtCalib14Indc			Mod_Coils[211]

#define Coil_DryRunCycle			Mod_Coils[215]

#define Dry_RunStable				Mod_Coils[217]
#define RunCycle_Stable				Mod_Coils[218]

#define Skip_Head					Mod_Coils[219]
#define Coil_AutoCorrection			Mod_Coils[220]
#define Coil_Under_Over			    Mod_Coils[221]

#define HMI_Flash_Write			    Mod_Coils[35]
#define flash_popup		 			Mod_Coils[223]
#define flash_write_done		 	Mod_Coils[224]

#define no_combi_popup		 		Mod_Coils[239]
#define over_weight_popup		 	Mod_Coils[240]
#define popup_reset				 	Mod_Coils[241]






#define MAXREGS 	255
#define NPNTS 		250
#define MAXBYTES 	MAXREGS*2+12

#define MAXCOILS	320
#define FIXSLAVEID	1


_MODBUS   int records[MAXREGS];			// Total number of records
_MODBUS  _Bool Mod_Coils[MAXCOILS];		// Total number of coils

struct Rbit
{
    char  f0	:1;
    char  f1	:1;
    char  f2	:1;
    char  f3	:1;
    char  f4	:1;
    char  f5	:1;
    char  f6	:1;
    char  f7	:1;
    char  f8	:1;
    char  f9	:1;
    char  f10	:1;
    char  f11	:1;
    char  f12	:1;
    char  f13	:1;
    char  f14	:1;
    char  f15	:1;
};
 union RWord
{
	struct Rbit f_bit;
	int f_word;
};
 _MODBUS union RWord records1[MAXREGS];

struct bit_define
{
    char  f0:1;
    char  f1:1;
    char  f2:1;
    char  f3:1;
    char  f4:1;
    char  f5:1;
    char  f6:1;
    char  f7:1;
};

union  byte_define
{
  	struct bit_define f_bit;
  	unsigned char   f_byte;
};

_MODBUS unsigned char rbuff[MAXREGS*2+12];
_MODBUS unsigned char xbuff[MAXREGS*2+12];
_MODBUS unsigned char xbfptr _MODBUS_INIT(0);
_MODBUS unsigned char function _MODBUS_INIT(0);
_MODBUS unsigned char xbytes _MODBUS_INIT(0);
_MODBUS unsigned char rbytes _MODBUS_INIT(0);
_MODBUS unsigned char bytecnt _MODBUS_INIT(0);
_MODBUS unsigned char slaveid _MODBUS_INIT(0);
_MODBUS unsigned char rxbyte _MODBUS_INIT(0);
_MODBUS unsigned char rxtimer _MODBUS_INIT(0);
_MODBUS unsigned char rbyte _MODBUS_INIT(0);

_MODBUS unsigned int crcval _MODBUS_INIT(0);

_MODBUS unsigned int regptr _MODBUS_INIT(0);
_MODBUS unsigned int npoints _MODBUS_INIT(0);

_MODBUS union byte_define	modflag ;
_MODBUS union byte_define	SLAVE ;
_MODBUS union byte_define	COILS[15];
//union CData			CANSlaveID[SlaveNo],Broadcast;

_MODBUS _Bool etxtflg _MODBUS_INIT(0);
_MODBUS _Bool xmitflg _MODBUS_INIT(0);
_MODBUS _Bool xtrflg _MODBUS_INIT(0);
_MODBUS _Bool stxflg _MODBUS_INIT(0);
_MODBUS _Bool rxflag _MODBUS_INIT(0);
_MODBUS _Bool chsumflg _MODBUS_INIT(0);
_MODBUS _Bool set6ok _MODBUS_INIT(0);
_MODBUS _Bool set10ok _MODBUS_INIT(0);
_MODBUS _Bool speed_change_flag _MODBUS_INIT(0);
_MODBUS _Bool speed_change _MODBUS_INIT(0);

//_DECL const unsigned char CRLF[2]={0x0a,0x0d};
_MODBUS unsigned char asc2hx(signed char ch);
_MODBUS unsigned char hx2asc(signed char ch);

_MODBUS signed int TargetWt _MODBUS_INIT(0);
_MODBUS unsigned int Target_Speed _MODBUS_INIT(0),New_Target_Speed _MODBUS_INIT(0),Previous_Speed _MODBUS_INIT(0);


#endif /* MODBUS_VARIABLES_H_ */
