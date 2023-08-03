/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : 1.1.104
* Device(s)    : R5F52315AxFL
* Description  : User header file for code generation.
* Creation Date: 2022-02-11
***********************************************************************************************************************/

#ifndef CG_USER_DEF_H
#define CG_USER_DEF_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/* Start user code for register. Do not edit comment generated here */
#define DIRN	PORTA.PODR.BIT.B6		// MODBUS Serial Communication Direction Pin
#define MHW_READY	PORT1.PODR.BIT.B6		// Relay 1 Output Pin
#define MHS_ON_OFF	PORTC.PODR.BIT.B4		// Relay 2 Output Pin
#define DUMP_CONFIRMATION	PORTC.PODR.BIT.B5		// Relay 3 Output Pin
#define OVER_UNDER_WT	PORTC.PODR.BIT.B6		// Relay 4 Output Pin
#define RLY5	PORTE.PODR.BIT.B1		// Relay 5 Output Pin
#define RLY6	PORTE.PODR.BIT.B2		// Relay 6 Output Pin

#define Loop_time_Check	 PORTE.PODR.BIT.B3		//to measure time for run operation  loop  by dny 26/2/21

#define IP1		PORT3.PIDR.BIT.B5		// Isolated Digital Input 1 Pin
#define DUMP_TRIGGER		PORT3.PIDR.BIT.B1		// Isolated Digital Input 2 Pin
#define LEVEL_SENSOR		PORT3.PIDR.BIT.B0		// Isolated Digital Input 3 Pin
#define IP4		PORT2.PIDR.BIT.B7		// Isolated Digital Input 4 Pin
#define IP5		PORT2.PIDR.BIT.B6		// Isolated Digital Input 5 Pin
#define IP6		PORT1.PIDR.BIT.B7		// Isolated Digital Input 6 Pin


extern volatile _Bool Rx_Flag;
extern volatile _Bool Tx_Flag;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Start user code for macro define. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Start user code for type define. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

