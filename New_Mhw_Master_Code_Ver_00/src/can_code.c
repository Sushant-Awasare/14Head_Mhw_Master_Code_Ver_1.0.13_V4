/*
 * can_code.c
 *
 *  Created on: Feb 4, 2022
 *      Author: Dnyaneshwar
 */

/***********************************************************************************************************************
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : can_code.c
* Device(s)    : RSKRX231
* Tool-Chain   : Renesas RXC
* Description  : This is file contains RSCAN initialization routines and interrupt
*                callback functions which support the main.c application.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*           08.09.2016 1.00     Initial Release
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include <stdio.h>              // sprintf()
#include <string.h>             // strlen()
#include "platform.h"
#include "r_rscan_rx_if.h"      // The RSCAN FIT module API interface file
#include "r_cg_wdt.h"
#include "r_cg_userdefine.h"
#include "can_variables.h"
#include "can_functions.h"

#include "global_defines.h"
#include "global_variables.h"
#include "global_functions.h"

#include "manual_functions.h"
#include "manual_variables.h"

#include "calibration_variables.h"
#include "calibration_functions.h"

#include "runcycle_variables.h"
#include "runcycle_functions.h"

#include "modbus_variables.h"

#include "delay_functions.h"
/*****************************************************************************

Private global variables and functions
******************************************************************************/


/*****************************************************************************
* Function Name : void Init_CAN(void)
* Description   : This function initializes the MCU pins for RSCAN channel 0.
*                 It then opens the CAN driver and configures it for a 500kHz
*                 network, the use of a receive FIFO, a receive rule that
*                 accepts all messages, and then puts the channel into
*                 communications mode. Also, a transmit message buffer is
*                 initialized.
* Arguments     : none
* Return Value  : none
******************************************************************************/
void Initialize_CAN_Communication(void)
{
   /* Configure CAN port for channel 0 */
	can_rx231_init_ports();

    /* Open driver. Timestamp source is arbitrary. */
    config.timestamp_src = CAN_TIMESTAMP_SRC_HALF_PCLK;
    config.timestamp_div = CAN_TIMESTAMP_DIV_1024;

    err = R_CAN_Open(&config, MainCallback);

    if (err != CAN_SUCCESS)
    {
        while(1);
    }

    /* Initialize channel 0 (includes mailboxes) */
    baud.prescaler = CAN_RSK_8MHZ_XTAL_500KBPS_PRESCALER;
    baud.tseg1 = CAN_RSK_8MHZ_XTAL_500KBPS_TSEG1;
    baud.tseg2 = CAN_RSK_8MHZ_XTAL_500KBPS_TSEG2;
    baud.sjw = CAN_RSK_8MHZ_XTAL_500KBPS_SJW;

    err = R_CAN_InitChan(CAN_CH0, &baud, ChanCallback);     // CAN_CFG_CLOCK_SOURCE 1

    if (err != CAN_SUCCESS)
    {
        while(1);
    }

    /* Initialize a receive FIFO */
    err = R_CAN_ConfigFIFO(CAN_BOX_RXFIFO_0,
                           CAN_FIFO_THRESHOLD_1,    // set int flag at every message received
                           CAN_BOX_NONE);           // unused field for RXFIFO
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }

    /* Do not need transmit FIFO */
    /* Do not need history FIFO */

    /*Initialize a receive rule filter for all message IDs */
    g_filter.check_ide = TRUE;    // check the IDE field in message
    g_filter.ide = 0;             // 0 = 11-bit ID
    g_filter.check_rtr = FALSE;   // do not check the RTR field in message

    g_filter.rtr = 0;             // (value does not matter here; not checking)
    g_filter.id = 0x0E;          // (value does not matter here; not checking)
    g_filter.id_mask = 0x000;     // all messages are accepted

    g_filter.min_dlc = 0;         // message data can be 0 to 8 bytes
    g_filter.label   = 0x800;       // arbitrary label (unused)

    /* Add rule to channel 0. Route filtered messages to receive FIFO 0 */
    err = R_CAN_AddRxRule(CAN_CH0, &g_filter, CAN_BOX_RXFIFO_0);
    if (err != CAN_SUCCESS)
    {
        while(1);
    }

    /* Initialization complete. Go to communications mode */
    err = R_CAN_Control(CAN_CMD_SET_MODE_COMM, 0);

    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }

    /* Setup a message to transmit. ID, DLC, and data will be filled in by by user via UART */
    g_txmsg.ide = 0;
    g_txmsg.rtr = 0;
    g_txmsg.one_shot = FALSE;       // do normal retries when sending
    g_txmsg.log_history = FALSE;    // do not log this message in history FIFO

    return;
}

/******************************************************************************
* Function Name: can_rx231_init_ports
* Description  : Initializes RX231 port 5 for CAN channel 0
* Arguments    : None
* Return Value : None
*******************************************************************************/
void can_rx231_init_ports(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* init CAN channel 0 */
    PORT1.PODR.BIT.B5 = 1;
    PORT1.PODR.BIT.B4 = 0;
    MPC.P14PFS.BYTE = 0x10;     // Pin Function Select P14 CTXD0
    MPC.P15PFS.BYTE = 0x10;     // Pin Function Select P15 CRXD0
    PORT1.PDR.BIT.B4 = 1;       // set TX pin direction to output
    PORT1.DSCR.BIT.B4 = 1;      // High-drive output
    PORT1.PDR.BIT.B5 = 0;       // set RX pin direction to input (dflt)
    PORT1.PMR.BIT.B4 = 1;       // set TX pin mode to peripheral
    PORT1.PMR.BIT.B5 = 1;       // set RX pin mode to peripheral

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

    return;
}


/******************************************************************************
* Function Name: MainCallback
* Description  : This function is called from interrupt level whenever a message
*                is received in a receive FIFO or a global error occurs. Global
*                errors include FIFO overflows or a DLC error detection.
* Arguments    : event-
*                   CAN_EVT_RXFIFO_THRESHOLD when threshold messages in an RXFIFO
*                   CAN_EVT_GLOBAL_ERR when a global error is detected
*                p_args-
*                   unused (future)
* Return Value : None
*******************************************************************************/
void MainCallback(can_cb_evt_t event, void *p_args)
{
    uint32_t    mask;
    can_err_t   err;


    if (event == CAN_EVT_GLOBAL_ERR)    // only event interrupts are configured for
    {
        /* Get mask of which global error(s) occurred */
        mask = R_CAN_GetStatusMask(CAN_STAT_GLOBAL_ERR, 0, &err);

        if (mask & CAN_MASK_ERR_DLC)
        {
            nop();  // no processing of this error to be done
        }

        if (mask & CAN_MASK_ERR_FIFO_OVFL)
        {
            g_ovfl_cnt++;   // rx FIFO overflowed
        }
    }

    return;
}


/******************************************************************************
* Function Name: ChanCallback
* Description  : This function is called from interrupt level whenever a channel
*                transmit or channel error occurs.
* Arguments    : chan-
*                    Channel in which the interrupt occurred on
*                event-
*                    CAN_EVT_TRANSMIT for mailbox transmit complete, abort
*                           complete, or TX/GW/HIST FIFO at threshold
*                    CAN_EVT_CHANNEL_ERR for channel error detection, which
*                           includes (among others) error passive state entry,
*                           bus off entry, and bus off recovery.
*                    CAN_EVT_GWFIFO_RX_THRESHOLD for GW FIFO at receive
*                           threshhold
* Return Value : None
*******************************************************************************/
void ChanCallback(uint8_t       chan,
                  can_cb_evt_t  event,
                  void          *p_args)
{

    if (event == CAN_EVT_TRANSMIT)
    {
        /* No processing needed.
         * Only using interrupts instead of blocking so that the rx FIFO
         * can be serviced more frequently at application level.
         */
        nop();
    }
    else if (event == CAN_EVT_CHANNEL_ERR)
    {
        /* No processing needed.
         * Errors are polled for at the application level to insure error messages
         * do not interfere with possible UART communications in progress
         */
        nop();
    }

    return;
}


void Check_CAN_Status(void)
{
	uint32_t    status;
	can_err_t   can_err;

	/* Check if received CAN message */
	if (R_CAN_GetMsg(CAN_BOX_RXFIFO_0, &g_rxmsg) == CAN_SUCCESS)
	{
		Process_Can_Msg(&g_rxmsg);
	}

	/* Check if any CAN channel errors */
	status = R_CAN_GetStatusMask(CAN_STAT_CH_ERROR, CAN_CH0, &can_err);
	if (status != 0)
	{
	//	process_ch_err_msg(status);
	}
}


void Process_Can_Msg(can_rxmsg_t *rxmsg)
{
	//Clear_Watchdog();Clear_Watchdog();Clear_Watchdog();

	if((rxmsg->id) == 0xD2)
	{
		auxbucket_empty_flag = 0;
		previous_dump_flag = 0;
	}


	if((rxmsg->id) == 0xD3)
	{
		 if((rxmsg->data[0]&0x01)==1)
		 {
			 //aux_at_homing = 1;
			// Coil_AuxHoming_Error = 0;
			 aux_ready_flag = 1;
		 }
		 else
		 {
			 //aux_at_homing = 0;
			 //Coil_AuxHoming_Error = 1;
			 aux_ready_flag = 0;
		 }
	}

	if((rxmsg->id) == 0x61 )
	{
		 if((rxmsg->data[2]&0x01)==1)
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[0] = byte_to_word.word;
			 records[0] = BucketWt[0];

			 Head1_Stable = 1;
			 head1_wt_received_flag = 1;
		//	 records[25] =  7;
		 }
		 else
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[0] = byte_to_word.word;
			 records[0] = BucketWt[0];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head1_Stable = 1;
			 else if(RunCycle_Stable)
				 Head1_Stable = 1;
			 else
				 Head1_Stable = 0;

			 head1_wt_received_flag = 1;
		//	 records[25] =  18;
		 }

		 if((rxmsg->data[2]&0x02)==1 && wt_feedback_flag == 1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[0] + Fb_Dump_Wt;
		 }

		 Wb_feedback_1 = (_Bool)(rxmsg->data[2]&0x02);
		 //Wb_feedback_1 = Wb_feedback_1 << 8;


		 Fb_feedback_1 = (_Bool)(rxmsg->data[2]&0x04);
		 //Fb_feedback_1 = Fb_feedback_1 << 8;


		 if(Wb_feedback_1 == 1 && Fb_feedback_1 == 1)
			 records[235] = 3;
		 else
		 {
			 if(Wb_feedback_1)
				 records[235] = 1;

			 if(Fb_feedback_1)
				records[235] = 2;

			 if(Wb_feedback_1 == 0 && Fb_feedback_1 == 0)
				 records[235] = 0;
		 }

		 if((rxmsg->data[3]&0x01)==1)
		 {
			 wb1_nothoming_flag = 0;
		 }
		 else
		 {
			 wb1_nothoming_flag = 1;
		//	 records[25] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb1_nothoming_flag = 0;
		 }
		 else
		 {
			 fb1_nothoming_flag = 1;
		//	 records[25] = 13;
		 }

		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[25] = 6;
			 head1_not_in_combi_flag = 1;
		 }

	}

	if((rxmsg->id) == 0x62)
	{
		if((rxmsg->data[2]&0x01)==1)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[1] = byte_to_word.word;
			 records[1] = BucketWt[1];

			head2_wt_received_flag = 1;
			Head2_Stable = 1;
		//	records[26] =  7;
		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[1] = byte_to_word.word;
			 records[1] = BucketWt[1];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head2_Stable = 1;
			 else if(RunCycle_Stable)
				 Head2_Stable = 1;
			 else
				 Head2_Stable = 0;

			head2_wt_received_flag = 1;
		//	records[26] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1 && wt_feedback_flag == 1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[1] + Fb_Dump_Wt;
		 }

		 Wb_feedback_2 = (_Bool)(rxmsg->data[2]&0x02);
		// Wb_feedback_2 = Wb_feedback_2 << 8;


		 Fb_feedback_2 = (_Bool)(rxmsg->data[2]&0x04);
		 //Fb_feedback_2 = Fb_feedback_2 << 8;

		 if(Wb_feedback_2 == 1 && Fb_feedback_2 == 1)
			 records[236] = 3;
		 else
		 {
			 if(Wb_feedback_2)
				 records[236] = 1;

			 if(Fb_feedback_2)
				records[236] = 2;

			 if(Wb_feedback_2 == 0 && Fb_feedback_2 == 0)
				 records[236] = 0;
		 }


		if((rxmsg->data[3]&0x01)==1)
		 {
			 wb2_nothoming_flag = 0;
		 }
		 else
		 {
			 wb2_nothoming_flag = 1;
		//	 records[26] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb2_nothoming_flag = 0;
		 }
		 else
		 {
			 fb2_nothoming_flag = 1;
		//	 records[26] = 13;
		 }

		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[26] = 6;
			 head2_not_in_combi_flag = 1;
		 }

	}

	if((rxmsg->id) == 0x10)
	{
		 if((rxmsg->data[2]&0x01)==1)
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[2] = byte_to_word.word;
			 records[2] = BucketWt[2];

			 head3_wt_received_flag = 1;
			 Head3_Stable = 1;
		//	 records[27] =  7;
		 }
		 else
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[2] = byte_to_word.word;
			 records[2] = BucketWt[2];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head3_Stable = 1;
			 else if(RunCycle_Stable)
				 Head3_Stable = 1;
			 else
				 Head3_Stable = 0;

			 head3_wt_received_flag = 1;
		//	 records[27] =  18;
		 }

		 if((rxmsg->data[2]&0x02)==1 && wt_feedback_flag == 1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[2] + Fb_Dump_Wt;
		 }

		 Wb_feedback_3 = (_Bool)(rxmsg->data[2]&0x02);
		// Wb_feedback_3 = Wb_feedback_3 << 8;


		 Fb_feedback_3 = (_Bool)(rxmsg->data[2]&0x04);
		// Fb_feedback_3 = Fb_feedback_3 << 8;

		 if(Wb_feedback_3 == 1 && Fb_feedback_3 == 1)
			 records[237] = 3;
		 else
		 {
			 if(Wb_feedback_3)
				 records[237] = 1;

			 if(Fb_feedback_3)
				records[237] = 2;

			 if(Wb_feedback_3 == 0 && Fb_feedback_3 == 0)
				 records[237] = 0;
		 }

		 if((rxmsg->data[3]&0x01)==1)
		 {
			 wb3_nothoming_flag = 0;
		 }
		 else
		 {
			 wb3_nothoming_flag = 1;
		//	 records[27] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb3_nothoming_flag = 0;
		 }
		 else
		 {
			 fb3_nothoming_flag = 1;
	//		 records[27] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
	//		 records[27] = 6;
			 head3_not_in_combi_flag = 1;
		 }

	}

	if((rxmsg->id) == 0x11)
	{
		 if((rxmsg->data[2]&0x01)==1)
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[3] = byte_to_word.word;
			 records[3] = BucketWt[3];

			 head4_wt_received_flag = 1;

			 Head4_Stable = 1;
	//		 records[28] =  7;
		 }
		 else
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[3] = byte_to_word.word;
			 records[3] = BucketWt[3];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head4_Stable = 1;
			 else if(RunCycle_Stable)
				 Head4_Stable = 1;
			 else
				 Head4_Stable = 0;
			 head4_wt_received_flag = 1;
		//	 records[28] =  18;
		 }

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[3] + Fb_Dump_Wt;
		 }

		 Wb_feedback_4 = (_Bool)(rxmsg->data[2]&0x02);
		// Wb_feedback_4 = Wb_feedback_4 << 8;


		 Fb_feedback_4 = (_Bool)(rxmsg->data[2]&0x04);
		 //Fb_feedback_4 = Fb_feedback_4 << 8;

		 if(Wb_feedback_4 == 1 && Fb_feedback_4 == 1)
			 records[238] = 3;
		 else
		 {
			 if(Wb_feedback_4)
				 records[238] = 1;

			 if(Fb_feedback_4)
				records[238] = 2;


			 if(Wb_feedback_4 == 0 && Fb_feedback_4 == 0)
				 records[238] = 0;
		 }


		 if((rxmsg->data[3]&0x01)==1)
		 {
			 wb4_nothoming_flag = 0;
		 }
		 else
		 {
			 wb4_nothoming_flag = 1;
		//	 records[28] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb4_nothoming_flag = 0;
		 }
		 else
		 {
			 fb4_nothoming_flag = 1;
		//	 records[28] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[28] = 6;
			 head4_not_in_combi_flag = 1;
		 }

	}

	if((rxmsg->id) == 0x12)
	{
		 if((rxmsg->data[2]&0x01)==1)
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[4] = byte_to_word.word;
			 records[4] = BucketWt[4];

			 head5_wt_received_flag = 1;
			 Head5_Stable = 1;
		//	 records[29] =  7;
		 }
		 else
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[4] = byte_to_word.word;
			 records[4] = BucketWt[4];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head5_Stable = 1;
			 else if(RunCycle_Stable)
				 Head5_Stable = 1;
			 else
				 Head5_Stable = 0;

			 head5_wt_received_flag = 1;
		//	 records[29] =  18;
		 }

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[4] + Fb_Dump_Wt;
		 }

		 Wb_feedback_5 = (_Bool)(rxmsg->data[2]&0x02);
		// Wb_feedback_5 = Wb_feedback_5 << 8;


		 Fb_feedback_5 = (_Bool)(rxmsg->data[2]&0x04);
		// Fb_feedback_5 = Fb_feedback_5 << 8;

		 if(Wb_feedback_5 == 1 && Fb_feedback_5 == 1)
			 records[239] = 3;
		 else
		 {
			 if(Wb_feedback_5)
				 records[239] = 1;

			 if(Fb_feedback_5)
				records[239] = 2;

			 if(Wb_feedback_5 == 0 && Fb_feedback_5 == 0)
				records[239] = 0;
		 }

		 if((rxmsg->data[3]&0x01)==1)
		 {
			 wb5_nothoming_flag = 0;   // at homing
		 }
		 else
		 {
			 wb5_nothoming_flag = 1;   // not at homing
		//	 records[29] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb5_nothoming_flag = 0;
		 }
		 else
		 {
			 fb5_nothoming_flag = 1;
		//	 records[29] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[29] = 6;
			 head5_not_in_combi_flag = 1;
		 }
	}

	if((rxmsg->id) == 0x13)
	{
		 if((rxmsg->data[2]&0x01)==1)
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[5] = byte_to_word.word;
			 records[5] = BucketWt[5];

			 head6_wt_received_flag = 1;
			 Head6_Stable = 1;
		//	 records[30] =  7;
		 }
		 else
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[5] = byte_to_word.word;
			 records[5] = BucketWt[5];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head6_Stable = 1;
			 else if(RunCycle_Stable)
				 Head6_Stable = 1;
			 else
				 Head6_Stable = 0;

			 head6_wt_received_flag = 1;
		//	 records[30] =  18;
		 }

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[5] + Fb_Dump_Wt;
		 }

		 Wb_feedback_6 = (_Bool)(rxmsg->data[2]&0x02);
		// Wb_feedback_6 = Wb_feedback_6 << 8;


		 Fb_feedback_6 = (_Bool)(rxmsg->data[2]&0x04);
		// Fb_feedback_6 = Fb_feedback_6 << 8;

		 if(Wb_feedback_6 == 1 && Fb_feedback_6 == 1)
			 records[240] = 3;
		 else
		 {
			 if(Wb_feedback_6)
				 records[240] = 1;

			 if(Fb_feedback_6)
				records[240] = 2;

			 if(Wb_feedback_6 == 0 && Fb_feedback_6 == 0)
				 records[240] = 0;
		 }

		 if((rxmsg->data[3]&0x01)==1)
		 {
			 wb6_nothoming_flag = 0;
		 }
		 else
		 {
			 wb6_nothoming_flag = 1;
	//		 records[30] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb6_nothoming_flag = 0;
		 }
		 else
		 {
			 fb6_nothoming_flag = 1;
		//	 records[30] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[30] = 6;
			 head6_not_in_combi_flag = 1;
		 }

	}
	if((rxmsg->id) == 0x14)
	{
		 if((rxmsg->data[2]&0x01)==1)
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[6] = byte_to_word.word;
			 records[6] = BucketWt[6];

			 head7_wt_received_flag = 1;
			 Head7_Stable = 1;
		//	 records[31] =  7;

		 }
		 else
		 {
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[6] = byte_to_word.word;
			 records[6] = BucketWt[6];

			 head7_wt_received_flag = 1;

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head7_Stable = 1;
			 else if(RunCycle_Stable)
				 Head7_Stable = 1;
			 else
				 Head7_Stable = 0;

		//	 records[31] =  18;
		 }


		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[6] + Fb_Dump_Wt;
		 }

		 Wb_feedback_7 = (_Bool)(rxmsg->data[2]&0x02);
		// Wb_feedback_7 = Wb_feedback_7 << 8;

		 Fb_feedback_7 = (_Bool)(rxmsg->data[2]&0x04);
		// Fb_feedback_7 = Fb_feedback_7 << 8;

		 if(Wb_feedback_7 == 1 && Fb_feedback_7 == 1)
			 records[241] = 3;
		 else
		 {
			 if(Wb_feedback_7)
				 records[241] = 1;

			 if(Fb_feedback_7)
				records[241] = 2;

			 if(Wb_feedback_7 == 0 && Fb_feedback_7 == 0)
				 records[241] = 0;
		 }




		 if((rxmsg->data[3]&0x01)==1)
		 {
			 wb7_nothoming_flag = 0;
		 }
		 else
		 {
			 wb7_nothoming_flag = 1;
		//	 records[31] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb7_nothoming_flag = 0;
		 }
		 else
		 {
			 fb7_nothoming_flag = 1;
		//	 records[31] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[31] = 6;
			 head7_not_in_combi_flag = 1;
		 }

	}
	if((rxmsg->id) == 0x15)
	{
		if((rxmsg->data[2]&0x01)==1)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[7] = byte_to_word.word;
			 records[7] = BucketWt[7];
			head8_wt_received_flag = 1;
			Head8_Stable = 1;
		//	records[32] =  7;
		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[7] = byte_to_word.word;
			 records[7] = BucketWt[7];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head8_Stable = 1;
			 else if(RunCycle_Stable)
				 Head8_Stable = 1;
			 else
				 Head8_Stable = 0;

			head8_wt_received_flag = 1;
		//	records[32] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[7] + Fb_Dump_Wt;
		 }

		 Wb_feedback_8 = (_Bool)(rxmsg->data[2]&0x02);
		// Wb_feedback_8 = Wb_feedback_8 << 8;


		 Fb_feedback_8 = (_Bool)(rxmsg->data[2]&0x04);
		// Fb_feedback_8 = Fb_feedback_8 << 8;


		 if(Wb_feedback_8 == 1 && Fb_feedback_8 == 1)
			 records[242] = 3;
		 else
		 {
			 if(Wb_feedback_8)
				 records[242] = 1;

			 if(Fb_feedback_8)
				records[242] = 2;

			 if(Wb_feedback_8 == 0 && Fb_feedback_8 == 0)
				 records[242] = 0;
		 }


		if((rxmsg->data[3]&0x01)==1)
		 {
			 wb8_nothoming_flag = 0;
		 }
		 else
		 {
			 wb8_nothoming_flag = 1;
	//		 records[32] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb8_nothoming_flag = 0;
		 }
		 else
		 {
			 fb8_nothoming_flag = 1;
	//		 records[32] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[32] = 6;
			 head8_not_in_combi_flag = 1;
		 }

	}
	if((rxmsg->id) == 0x16)
	{
		if(rxmsg->data[2]&0x01)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[8] = byte_to_word.word;
			 records[8] = BucketWt[8];

			head9_wt_received_flag = 1;
			Head9_Stable = 1;
		//	records[33] =  7;

		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[8] = byte_to_word.word;
			 records[8] = BucketWt[8];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head9_Stable = 1;
			 else if(RunCycle_Stable)
				 Head9_Stable = 1;
			 else
				 Head9_Stable = 0;

			head9_wt_received_flag = 1;
		//	records[33] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[8] + Fb_Dump_Wt;
		 }

		Wb_feedback_9 = (_Bool)(rxmsg->data[2]&0x02);
		//Wb_feedback_9 = Wb_feedback_9 << 8;


		Fb_feedback_9 = (_Bool)(rxmsg->data[2]&0x02);
		//Fb_feedback_9 = Fb_feedback_9 << 8;


		if(Wb_feedback_9 == 1 && Fb_feedback_9 == 1)
			records[243] = 3;
		else
		{
			 if(Wb_feedback_9)
				 records[243] = 1;

			 if(Fb_feedback_9)
				records[243] = 2;

			 if(Wb_feedback_9 == 0 && Fb_feedback_9 == 0)
				 records[243] = 0;

		}

		if((rxmsg->data[3]&0x01)==1)
		 {
			 wb9_nothoming_flag = 0;

		 }
		 else
		 {
			 wb9_nothoming_flag = 1;
		//	 records[33] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb9_nothoming_flag = 0;
		 }
		 else
		 {
			 fb9_nothoming_flag = 1;
		//	 records[33] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[33] = 6;
			 head9_not_in_combi_flag = 1;
		 }

	}

	if((rxmsg->id) == 0x17)
	{
		if((rxmsg->data[2]&0x01)==1)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[9] = byte_to_word.word;
			 records[9] = BucketWt[9];

			head10_wt_received_flag = 1;

			Head10_Stable = 1;
		//	records[34] =  7;
		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[9] = byte_to_word.word;
			 records[9] = BucketWt[9];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head10_Stable = 1;
			 else if(RunCycle_Stable)
				 Head10_Stable = 1;
			 else
				 Head10_Stable = 0;

			head10_wt_received_flag = 1;
		//	records[34] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[9] + Fb_Dump_Wt;
		 }

		Wb_feedback_10 = (_Bool)(rxmsg->data[2]&0x02);
		//Wb_feedback_10 = Wb_feedback_10 << 8;


		Fb_feedback_10 = (_Bool)(rxmsg->data[2]&0x04);
		//Fb_feedback_10 = Fb_feedback_10 << 8;


		 if(Wb_feedback_10 == 1 && Fb_feedback_10 == 1)
			 records[244] = 3;
		 else
		 {
			 if(Wb_feedback_10)
				 records[244] = 1;

			 if(Fb_feedback_10)
				records[244] = 2;


			 if(Wb_feedback_10 == 0 && Fb_feedback_10 == 0)
				 records[244] = 0;
		 }


		if((rxmsg->data[3]&0x01)==1)
		 {
			 wb10_nothoming_flag = 0;
		 }
		 else
		 {
			 wb10_nothoming_flag = 1;
		//	 records[34] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb10_nothoming_flag = 0;
		 }
		 else
		 {
			 fb10_nothoming_flag = 1;
		//	 records[34] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[34] = 6;
			 head10_not_in_combi_flag = 1;
		 }
	}

	if((rxmsg->id) == 0x18)
	{
		if((rxmsg->data[2]&0x01)==1)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[10] = byte_to_word.word;
			 records[10] = BucketWt[10];

			head11_wt_received_flag = 1;
			Head11_Stable = 1;
		//	records[35] =  7;
		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[10] = byte_to_word.word;
			 records[10] = BucketWt[10];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head11_Stable = 1;
			 else if(RunCycle_Stable)
				 Head11_Stable = 1;
			 else
				 Head11_Stable = 0;
			head11_wt_received_flag = 1;
		//	records[35] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[10] + Fb_Dump_Wt;
		 }
		Wb_feedback_11 = (_Bool)(rxmsg->data[2]&0x02);
		//Wb_feedback_11 = Wb_feedback_11 << 8;


		Fb_feedback_11 = (_Bool)(rxmsg->data[2]&0x04);
		//Fb_feedback_11 = Fb_feedback_11 << 8;


		 if(Wb_feedback_11 == 1 && Fb_feedback_11 == 1)
			 records[245] = 3;
		 else
		 {
			 if(Wb_feedback_11)
				 records[245] = 1;

			 if(Fb_feedback_11)
				records[245] = 2;

			 if(Wb_feedback_11 == 0 && Fb_feedback_11 == 0)
				 records[245] = 0;
		 }


		if((rxmsg->data[3]&0x01)==1)
		 {
			 wb11_nothoming_flag = 0;
		 }
		 else
		 {
			 wb11_nothoming_flag = 1;
		//	 records[35] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb11_nothoming_flag = 0;
		 }
		 else
		 {
			 fb11_nothoming_flag = 1;
		//	 records[35] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[35] = 6;
			 head11_not_in_combi_flag = 1;
		 }
	}


	if((rxmsg->id) == 0x19)
	{
		if((rxmsg->data[2]&0x01)==1)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[11] = byte_to_word.word;
			 records[11] = BucketWt[11];

			head12_wt_received_flag = 1;

			Head12_Stable = 1;
		//	records[36] =  7;
		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[11] = byte_to_word.word;
			 records[11] = BucketWt[11];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head12_Stable = 1;
			 else if(RunCycle_Stable)
				 Head12_Stable = 1;
			 else
				 Head12_Stable = 0;
			head12_wt_received_flag = 1;
		//	records[36] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[11] + Fb_Dump_Wt;
		 }

		Wb_feedback_12 = (_Bool)(rxmsg->data[2]&0x02);
		//Wb_feedback_12 = Wb_feedback_12 << 8;


		Fb_feedback_12 = (_Bool)(rxmsg->data[2]&0x04);
		//Fb_feedback_12= Fb_feedback_12 << 8;

		 if(Wb_feedback_12 == 1 && Fb_feedback_12 == 1)
							 records[246] = 3;
		 else
		 {
			 if(Wb_feedback_12)
				 records[246] = 1;

			 if(Fb_feedback_12)
				records[246] = 2;

			 if(Wb_feedback_12 == 0 && Fb_feedback_12 == 0)
				 records[246] = 0;
		 }



		if((rxmsg->data[3]&0x01)==1)
		 {
			 wb12_nothoming_flag = 0;
		 }
		 else
		 {
			 wb12_nothoming_flag = 1;
		//	 records[36] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb12_nothoming_flag = 0;
		 }
		 else
		 {
			 fb12_nothoming_flag = 1;
		//	 records[36] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[36] = 6;
			 head12_not_in_combi_flag = 1;
		 }
	}



	if((rxmsg->id) == 0x1A)
	{
		if((rxmsg->data[2]&0x01)==1)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[12] = byte_to_word.word;
			 records[12] = BucketWt[12];

			head13_wt_received_flag = 1;
			Head13_Stable = 1;
		//	records[37] =  7;
		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[12] = byte_to_word.word;
			 records[12] = BucketWt[12];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head13_Stable = 1;
			 else if(RunCycle_Stable)
				 Head13_Stable = 1;
			 else
				 Head13_Stable = 0;

			head13_wt_received_flag = 1;
		//	records[37] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[12] + Fb_Dump_Wt;
		 }

		Wb_feedback_13 = (_Bool)(rxmsg->data[2]&0x02);
		//Wb_feedback_13 = Wb_feedback_13 << 8;


		Fb_feedback_13 = (_Bool)(rxmsg->data[2]&0x04);
		//Fb_feedback_13= Fb_feedback_13 << 8;

		if(Wb_feedback_13 == 1 && Fb_feedback_13 == 1)
			 records[247] = 3;
		else
		{
			 if(Wb_feedback_13)
				 records[247] = 1;

			 if(Fb_feedback_13)
				records[247] = 2;

			 if(Wb_feedback_13 == 0 && Fb_feedback_13 == 0)
				 records[247] = 0;
		}


		if((rxmsg->data[3]&0x01)==1)
		 {
			 wb13_nothoming_flag = 0;
		 }
		 else
		 {
			 wb13_nothoming_flag = 1;
		//	 records[37] = 14;
		 }

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb13_nothoming_flag = 0;
		 }
		 else
		 {
			 fb13_nothoming_flag = 1;
		//	 records[37] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[37] = 6;
			 head13_not_in_combi_flag = 1;
		 }
	}


	if((rxmsg->id) == 0x1B)
	{
		if((rxmsg->data[2]&0x01)==1)
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[13] = byte_to_word.word;
			 records[13] = BucketWt[13];

			head14_wt_received_flag = 1;
			Head14_Stable = 1;
		//	records[38] =  7;
		}
		else
		{
			 byte_to_word.byte.byte0 = rxmsg->data[0];
			 byte_to_word.byte.byte1 = rxmsg->data[1];
			 BucketWt[13] = byte_to_word.word;
			 records[13] = BucketWt[13];

			 if(dry_runcycle_flag == 1 && Dry_RunStable == 0)
				 Head14_Stable = 1;
			 else if(RunCycle_Stable)
				 Head14_Stable = 1;
			 else
				 Head14_Stable = 0;

			head14_wt_received_flag = 1;
		//	records[38] =  18;
		}

		 if((rxmsg->data[2]&0x02)==1)
		 {
			 Fb_Dump_Wt = Bkt_Wt_BFr[13] + Fb_Dump_Wt;
		 }
		Wb_feedback_14 = (_Bool)(rxmsg->data[2]&0x02);
		//Wb_feedback_14 = Wb_feedback_14 << 8;


		Fb_feedback_14 = (_Bool)(rxmsg->data[2]&0x04);
		//Fb_feedback_14= Fb_feedback_14 << 8;


		 if(Wb_feedback_14 == 1 && Fb_feedback_14 == 1)
			 records[248] = 3;
		 else
		 {
			 if(Wb_feedback_14)
				 records[248] = 1;

			 if(Fb_feedback_14)
				records[248] = 2;


			 if(Wb_feedback_14 == 0 && Fb_feedback_14 == 0)
				 records[248] = 0;
		 }


		if((rxmsg->data[3]&0x01)==1)
		{
			wb14_nothoming_flag = 0;
		}
		else
		{
			wb14_nothoming_flag = 1;
		//	records[38] = 14;
		}

		 if((rxmsg->data[4]&0x01)==1)
		 {
			 fb14_nothoming_flag = 0;
		 }
		 else
		 {
			 fb14_nothoming_flag = 1;
		//	 records[38] = 13;
		 }
		 if((rxmsg->data[5]&0x01)==1)
		 {
		//	 records[38] = 6;
			 head14_not_in_combi_flag = 1;
		 }
	}


		if((rxmsg->id) == 0x131)
		{
			Tare_Count[1] = (signed int)(rxmsg->data[0]);
			Tare_Count[1] = (signed int)(Tare_Count[1] << 8);
			Tare_Count[1] |= (signed int)(rxmsg->data[1]);
		//	records[25] = 2;
			records[130] = Tare_Count[1];
		}
		else if((rxmsg->id) == 0x132)
		{
			Tare_Count[2] = (signed int)(rxmsg->data[0]);
			Tare_Count[2] = (signed int)(Tare_Count[2] << 8);
			Tare_Count[2] |= (signed int)(rxmsg->data[1]);
		//	records[26] = 2;
			records[132] = Tare_Count[2];
		}
		else if((rxmsg->id) == 0x134)
		{
			Tare_Count[3] = (signed int)(rxmsg->data[0]);
			Tare_Count[3] = (signed int)(Tare_Count[3] << 8);
			Tare_Count[3] |= (signed int)(rxmsg->data[1]);
		//	records[27] = 2;
			records[134] = Tare_Count[3];
		}
		else if((rxmsg->id) == 0x136)
		{
			Tare_Count[4] = (signed int)(rxmsg->data[0]);
			Tare_Count[4] = (signed int)(Tare_Count[4] << 8);
			Tare_Count[4] |= (signed int)(rxmsg->data[1]);
		//	records[28] = 2;
			records[136] = Tare_Count[4];
		}
		else if((rxmsg->id) == 0x138)
		{
			Tare_Count[5] = (signed int)(rxmsg->data[0]);
			Tare_Count[5] = (signed int)(Tare_Count[5] << 8);
			Tare_Count[5] |= (signed int)(rxmsg->data[1]);
		//	records[29] = 2;
			records[138] = Tare_Count[5];
		}
		else if((rxmsg->id) == 0x140)
		{
			Tare_Count[6] = (signed int)(rxmsg->data[0]);
			Tare_Count[6] = (signed int)(Tare_Count[6] << 8);
			Tare_Count[6] |= (signed int)(rxmsg->data[1]);
		//	records[30] = 2;
			records[140] = Tare_Count[6];
		}
		else if((rxmsg->id) == 0x142)
		{
			Tare_Count[7] = (signed int)(rxmsg->data[0]);
			Tare_Count[7] = (signed int)(Tare_Count[7] << 8);
			Tare_Count[7] |= (signed int)(rxmsg->data[1]);
		//	records[31] = 2;
			records[142] = Tare_Count[7];
		}
		else if((rxmsg->id) == 0x144)
		{
			Tare_Count[8] = (signed int)(rxmsg->data[0]);
			Tare_Count[8] = (signed int)(Tare_Count[8] << 8);
			Tare_Count[8] |= (signed int)(rxmsg->data[1]);
		//	records[32] = 2;
			records[144] = Tare_Count[8];
		}
		else if((rxmsg->id) == 0x146)
		{
			Tare_Count[9] = (signed int)(rxmsg->data[0]);
			Tare_Count[9] = (signed int)(Tare_Count[9] << 8);
			Tare_Count[9] |= (signed int)(rxmsg->data[1]);
		//	records[33] = 2;
			records[146] = Tare_Count[9];
		}
		else if((rxmsg->id) == 0x148)
		{
			Tare_Count[10] = (signed int)(rxmsg->data[0]);
			Tare_Count[10] = (signed int)(Tare_Count[10] << 8);
			Tare_Count[10] |= (signed int)(rxmsg->data[1]);
		//	records[34] = 2;
			records[148] = Tare_Count[10];
		}
		else if((rxmsg->id) == 0x170)
		{
			Tare_Count[11] = (signed int)(rxmsg->data[0]);
			Tare_Count[11] = (signed int)(Tare_Count[11] << 8);
			Tare_Count[11] |= (signed int)(rxmsg->data[1]);
		//	records[35] = 2;
			records[230] = Tare_Count[11];
		}
		else if((rxmsg->id) == 0x171)
		{
			Tare_Count[12] = (signed int)(rxmsg->data[0]);
			Tare_Count[12] = (signed int)(Tare_Count[12] << 8);
			Tare_Count[12] |= (signed int)(rxmsg->data[1]);
			records[36] = 2;
			records[231] = Tare_Count[12];
		}
		else if((rxmsg->id) == 0x172)
		{
			Tare_Count[13] = (signed int)(rxmsg->data[0]);
			Tare_Count[13] = (signed int)(Tare_Count[13] << 8);
			Tare_Count[13] |= (signed int)(rxmsg->data[1]);
		//	records[37] = 2;
			records[232] = Tare_Count[13];
		}
		else if((rxmsg->id) == 0x173)
		{
			Tare_Count[14] = (signed int)(rxmsg->data[0]);
			Tare_Count[14] = (signed int)(Tare_Count[14] << 8);
			Tare_Count[14] |= (signed int)(rxmsg->data[1]);
		//	records[38] = 2;
			records[233] = Tare_Count[14];
		}

		if((rxmsg->id) == 0x130)
		{
			Calib_Count[1] = (signed int)(rxmsg->data[0]);
			Calib_Count[1] = (signed int)(Calib_Count[1] << 8);
			Calib_Count[1] |= (signed int)(rxmsg->data[1]);

			Calib_Slope[1] = (signed int)(rxmsg->data[2]);
			Calib_Slope[1] = (signed int)(Calib_Slope[1] << 8);
			Calib_Slope[1] |= (signed int)(rxmsg->data[3]);

			records[150] = Calib_Slope[1];
			records[131] = Calib_Count[1];
		}
		else if((rxmsg->id) == 0x133)
		{
			Calib_Count[2] = (signed int)(rxmsg->data[0]);
			Calib_Count[2] = (signed int)(Calib_Count[2] << 8);
			Calib_Count[2] |= (signed int)(rxmsg->data[1]);

			Calib_Slope[2] = (signed int)(rxmsg->data[2]);
			Calib_Slope[2] = (signed int)(Calib_Slope[2] << 8);
			Calib_Slope[2] |= (signed int)(rxmsg->data[3]);

			records[151] = Calib_Slope[2];
			records[133] = Calib_Count[2];
		}
		else if((rxmsg->id) == 0x135)
		{
			Calib_Count[3] = (signed int)(rxmsg->data[0]);
			Calib_Count[3] = (signed int)(Calib_Count[3] << 8);
			Calib_Count[3] |= (signed int)(rxmsg->data[1]);

			Calib_Slope[3] = (signed int)(rxmsg->data[2]);
			Calib_Slope[3] = (signed int)(Calib_Slope[3] << 8);
			Calib_Slope[3] |= (signed int)(rxmsg->data[3]);

			records[152] = Calib_Slope[3];
			records[135] = Calib_Count[3];
		}
		else if((rxmsg->id) == 0x137)
		{
			Calib_Count[4] = (signed int)(rxmsg->data[0]);
			Calib_Count[4] = (signed int)(Calib_Count[4] << 8);
			Calib_Count[4] |= (signed int)(rxmsg->data[1]);


			Calib_Slope[4] = (signed int)(rxmsg->data[2]);
			Calib_Slope[4] = (signed int)(Calib_Slope[4] << 8);
			Calib_Slope[4] |= (signed int)(rxmsg->data[3]);

			records[153] = Calib_Slope[4];
			records[137] = Calib_Count[4];
		}
		else if((rxmsg->id) == 0x139)
		{
			Calib_Count[5] = (signed int)(rxmsg->data[0]);
			Calib_Count[5] = (signed int)(Calib_Count[5] << 8);
			Calib_Count[5] |= (signed int)(rxmsg->data[1]);

			Calib_Slope[5] = (signed int)(rxmsg->data[2]);
			Calib_Slope[5] = (signed int)(Calib_Slope[5] << 8);
			Calib_Slope[5] |= (signed int)(rxmsg->data[3]);

			records[154] = Calib_Slope[5];
			records[139] = Calib_Count[5];
		}
		else if((rxmsg->id) == 0x141)
		{
			Calib_Count[6] = (signed int)(rxmsg->data[0]);
			Calib_Count[6] = (signed int)(Calib_Count[6] << 8);
			Calib_Count[6] |= (signed int)(rxmsg->data[1]);

			Calib_Slope[6] = (signed int)(rxmsg->data[2]);
			Calib_Slope[6] = (signed int)(Calib_Slope[6] << 8);
			Calib_Slope[6] |= (signed int)(rxmsg->data[3]);

			records[155] = Calib_Slope[6];
			records[141] = Calib_Count[6];
		}
		else if((rxmsg->id) == 0x143)
		{
			Calib_Count[7] = (signed int)(rxmsg->data[0]);
			Calib_Count[7] = (signed int)(Calib_Count[7] << 8);
			Calib_Count[7] |= (signed int)(rxmsg->data[1]);

			Calib_Slope[7] = (signed int)(rxmsg->data[2]);
			Calib_Slope[7] = (signed int)(Calib_Slope[7] << 8);
			Calib_Slope[7] |= (signed int)(rxmsg->data[3]);

			records[156] = Calib_Slope[7];
			records[143] = Calib_Count[7];
		}
		else if((rxmsg->id) == 0x145)
		{
			Calib_Count[8] = (signed int)rxmsg->data[0];
			Calib_Count[8] = (signed int)Calib_Count[8] << 8;
			Calib_Count[8] |= (signed int)rxmsg->data[1];

			Calib_Slope[8] = (signed int)rxmsg->data[2];
			Calib_Slope[8] = (signed int)Calib_Slope[8] << 8;
			Calib_Slope[8] |= (signed int)rxmsg->data[3];

			records[157] = Calib_Slope[8];
			records[145] = Calib_Count[8];
		}
		else if((rxmsg->id) == 0x147)
		{
			Calib_Count[9] = (signed int)rxmsg->data[0];
			Calib_Count[9] = (signed int)Calib_Count[9] << 8;
			Calib_Count[9] |= (signed int)rxmsg->data[1];

			Calib_Slope[9] = (signed int)rxmsg->data[2];
			Calib_Slope[9] = (signed int)Calib_Slope[9] << 8;
			Calib_Slope[9] |= (signed int)rxmsg->data[3];

			records[158] = Calib_Slope[9];
			records[147] = Calib_Count[9];
		}
		else if((rxmsg->id) == 0x149)
		{
			Calib_Count[10] = (signed int)rxmsg->data[0];
			Calib_Count[10] = (signed int)Calib_Count[10] << 8;
			Calib_Count[10] |= (signed int)rxmsg->data[1];

			Calib_Slope[10] = (signed int)rxmsg->data[2];
			Calib_Slope[10] = (signed int)Calib_Slope[10] << 8;
			Calib_Slope[10] |= (signed int)rxmsg->data[3];

			records[159] = Calib_Slope[10];
			records[149] = Calib_Count[10];
		}
		else if((rxmsg->id) == 0x186)
		{
			Calib_Count[11] = (signed int)rxmsg->data[0];
			Calib_Count[11] = (signed int)Calib_Count[11] << 8;
			Calib_Count[11] |= (signed int)rxmsg->data[1];

			Calib_Slope[11] = (signed int)rxmsg->data[2];
			Calib_Slope[11] = (signed int)Calib_Slope[11] << 8;
			Calib_Slope[11] |= (signed int)rxmsg->data[3];

			records[220] = Calib_Slope[11];
			records[210] = Calib_Count[11];
		}

		else if((rxmsg->id) == 0x187)
		{
			Calib_Count[12] = (signed int)rxmsg->data[0];
			Calib_Count[12] = (signed int)Calib_Count[12] << 8;
			Calib_Count[12] |= (signed int)rxmsg->data[1];

			Calib_Slope[12] = (signed int)rxmsg->data[2];
			Calib_Slope[12] = (signed int)Calib_Slope[12] << 8;
			Calib_Slope[12] |= (signed int)rxmsg->data[3];

			records[221] = Calib_Slope[12];
			records[211] = Calib_Count[12];
		}

		else if((rxmsg->id) == 0x188)
		{
			Calib_Count[13] = (signed int)rxmsg->data[0];
			Calib_Count[13] = (signed int)Calib_Count[13] << 8;
			Calib_Count[13] |= (signed int)rxmsg->data[1];

			Calib_Slope[13] = (signed int)rxmsg->data[2];
			Calib_Slope[13] = (signed int)Calib_Slope[13] << 8;
			Calib_Slope[13] |= (signed int)rxmsg->data[3];

			records[222] = Calib_Slope[13];
			records[212] = Calib_Count[13];
		}
		else if((rxmsg->id) == 0x189)
		{
			Calib_Count[14] = (signed int)rxmsg->data[0];
			Calib_Count[14] = (signed int)Calib_Count[14] << 8;
			Calib_Count[14] |= (signed int)rxmsg->data[1];

			Calib_Slope[14] = (signed int)rxmsg->data[2];
			Calib_Slope[14] = (signed int)Calib_Slope[14] << 8;
			Calib_Slope[14] |= (signed int)rxmsg->data[3];

			records[223] = Calib_Slope[14];
			records[213] = Calib_Count[14];
		}


		if((rxmsg->id) == 0x201 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x202 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x203 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x204 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x205 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x206 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x207 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x208 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x209 && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x20A && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x20B && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x20C && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x20D && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;
		else if((rxmsg->id) == 0x20E && (dump_feedback_count < dump_bucket_count))
			dump_feedback_count++;


			rxmsg->data[0]=0;
			rxmsg->data[1]=0;
			rxmsg->data[2]=0;
			rxmsg->data[3]=0;
			rxmsg->data[4]=0;
			rxmsg->data[5]=0;
			rxmsg->data[6]=0;
			rxmsg->data[7]=0;
}


void Manual_Operation_Can_Frame()
{
	CANFrame.id=0x30;
	CANFrame.rtr=0;
	CANFrame.dlc=4;

	//CANFrame.data[0]=(m_command>>8)&0xFF;
	//CANFrame.data[1]=m_command&0xFF;

	CANFrame.data[0]=(m_command>>16)&0xFF;
	CANFrame.data[1]=(m_command>>8)&0xFF;
	CANFrame.data[2]=m_command&0xFF;
	CANFrame.data[3]=Manual_Operation_Number;

	CAN_SEND_MEG(&CANFrame);
}

void Calibration_Operation_Can_Frame()
{
	CANFrame.id = 0x31;
	CANFrame.rtr = 0;
	CANFrame.dlc = 3;

	//CANFrame.data[0]=(c_command>>8)&0xFF;
	//CANFrame.data[1]=c_command&0xFF;

	CANFrame.data[0]=(c_command>>16)&0xFF;
	CANFrame.data[1]=(c_command>>8)&0xFF;
	CANFrame.data[2]=c_command&0xFF;
	CAN_SEND_MEG(&CANFrame);

}

void Get_Weight()
{
	CANFrame.id      = Head_counter;
	CANFrame.dlc     = 1;
	CANFrame.rtr     = 0;
	CANFrame.data[0] = 1;
	CAN_SEND_MEG(&CANFrame);
}

void Run_Get_weight()
{
	CANFrame.id  = Head_counter;
	CANFrame.dlc = 1;
	CANFrame.rtr = 0;
	CANFrame.data[0] = 0;
	CAN_SEND_MEG(&CANFrame);
}

void Recharge_Can_Frame()
{
	CANFrame.id=0xD8;
	CANFrame.dlc=5;
	CANFrame.rtr=0;
	CANFrame.data[0]=0;
	CANFrame.data[1]=0;

	CANFrame.data[2]= (Heads_To_recharge>>8)&0xFF;
	CANFrame.data[3]=  Heads_To_recharge&0xFF;

	CANFrame.data[4] = run_with_aux_bucket_flag;

	Clear_Watchdog();

	CAN_SEND_MEG(&CANFrame);

	recharge_frame_sent_flag = 1;

	return;

}

void Dump_Can_Frame()
{
	CANFrame.id=0x00;
	CANFrame.dlc=4;
	CANFrame.rtr=0;
	CANFrame.data[0]=(Heads_To_Dump>>8)&0xFFF;
	CANFrame.data[1]=Heads_To_Dump&0xFFF;

	CANFrame.data[2]= 0;//(Heads_To_recharge>>8)&0xFF;
	CANFrame.data[3]= 0;// Heads_To_recharge&0xFF;
//	Clear_Watchdog();

	CAN_SEND_MEG(&CANFrame);
}

void Recharge_Dump_Can_Frame()
{
	CANFrame.id=0x00;
	CANFrame.dlc=5;
	CANFrame.rtr=0;
	CANFrame.data[0]=(Heads_To_Dump>>8)&0xFF;
	CANFrame.data[1]=Heads_To_Dump&0xFF;

	CANFrame.data[2]= (Heads_To_recharge>>8)&0xFF;
	CANFrame.data[3]= Heads_To_recharge&0xFF;

	CANFrame.data[4] = run_with_aux_bucket_flag;

	Clear_Watchdog();

	CAN_SEND_MEG(&CANFrame);

	dump_frame_sent_flag = 1;
	return;
}

void Aux_Bucket_Dump()
{
	CANFrame.id  = 0xD1;
	CANFrame.dlc = 1;
	CANFrame.rtr = 0;
	CANFrame.data[0] = 0;
	CAN_SEND_MEG(&CANFrame);
	aux_frame_sent_flag = 1;
	return;

}

void CAN_SEND_MEG(struct CFrame *CANFrame1)
{
//	Clear_Watchdog();
	/*Do not make any changes in the following sequence*/
	g_txmsg.ide 	= 0;
	g_txmsg.rtr 	= CANFrame1->rtr;					// Remote Transmission request byte
	g_txmsg.id		= CANFrame1->id;		// Slave or Frame ID
	g_txmsg.dlc 	= CANFrame1->dlc;					// Frame Size / Must not be more than 8 in Byte
	g_txmsg.data[0] = CANFrame1->data[0];	// Frame Structure in 8 Bytes
	g_txmsg.data[1] = CANFrame1->data[1];
	g_txmsg.data[2] = CANFrame1->data[2];
	g_txmsg.data[3] = CANFrame1->data[3];
	g_txmsg.data[4] = CANFrame1->data[4];
	g_txmsg.data[5] = CANFrame1->data[5];
	g_txmsg.data[6] = CANFrame1->data[6];
	g_txmsg.data[7] = CANFrame1->data[7];
	g_txmsg.one_shot = FALSE;       		// Do normal retries when sending
	g_txmsg.log_history = FALSE;    		// Do not log this message in history FIFO


	R_CAN_SendMsg(CAN_BOX_TXMBX_2, &g_txmsg);		// Send the CAN frame over the Network
}

void check_new_firing(void)
{
	if(Mainhead_Firing_Flag)
	{
		if(Coil_MainIncrement_Firing)
		{
			Mainhead_Firing_angle++;
			records[84]=Mainhead_Firing_angle;

			CANFrame.id=0x84;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Mainhead_Firing_angle>>8)&0xFF;
			CANFrame.data[1]=Mainhead_Firing_angle&0xFF;
			CAN_SEND_MEG(&CANFrame);
			records[104] = Mainhead_Firing_angle; //changes done on 31/07/2023
		}
		else if(Coil_MainDecrement_Firing)
		{
			Mainhead_Firing_angle--;
			records[84]=Mainhead_Firing_angle;

			CANFrame.id=0x84;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Mainhead_Firing_angle>>8)&0xFF;
			CANFrame.data[1]=Mainhead_Firing_angle&0xFF;
			CAN_SEND_MEG(&CANFrame);
			records[104] = Mainhead_Firing_angle; //changes done on 31/07/2023
		}

	}
	else if(Mainhead_Ontime_Flag)
	{
		if(Coil_MainIncrement_Firing)
		{
			Mainhead_Ontime++;
			records[85]=Mainhead_Ontime;

			CANFrame.id=0x85;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Mainhead_Ontime>>8)&0xFF;
			CANFrame.data[1]=Mainhead_Ontime&0xFF;
			CAN_SEND_MEG(&CANFrame);

			records[107] = Mainhead_Ontime; //changes done on 31/07/2023


		}
		else if(Coil_MainDecrement_Firing)
		{
			Mainhead_Ontime--;
			records[85]=Mainhead_Ontime;

			CANFrame.id=0x85;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Mainhead_Ontime>>8)&0xFF;
			CANFrame.data[1]=Mainhead_Ontime&0xFF;
			CAN_SEND_MEG(&CANFrame);

			records[107] = Mainhead_Ontime; //changes done on 31/07/2023
		}


	}

	if(Flag_AllHead_firing==0)
	{

		if(Coil_Firing_Angle_1)
		{
			if(Coil_Increment_Firing)
			{
				Firing_angle_1++;
				//Coil_Increment_Firing=0;
				records[40]=Firing_angle_1;

				CANFrame.id=0x40;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_1>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_1&0xFF;
				CAN_SEND_MEG(&CANFrame);

			}
			else if(Coil_Decrement_Firing)
			{
				Firing_angle_1--;
				//Coil_Decrement_Firing=0;

				records[40]=Firing_angle_1;
				CANFrame.id=0x40;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_1>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_1&0xFF;
				CAN_SEND_MEG(&CANFrame);
			}
	   }

	if(Coil_Firing_Angle_2)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_2++;
			//Coil_Increment_Firing=0;

			records[41]=Firing_angle_2;
			CANFrame.id=0x41;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_2>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_2&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_2--;
			//Coil_Decrement_Firing=0;

			records[41]=Firing_angle_2;
			CANFrame.id=0x41;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_2>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_2&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}

	}

	if(Coil_Firing_Angle_3)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_3++;
			//Coil_Increment_Firing=0;

			records[42]=Firing_angle_3;
			CANFrame.id=0x42;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_3>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_3&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_3--;
			//Coil_Decrement_Firing=0;
			records[42]=Firing_angle_3;
			CANFrame.id=0x42;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_3>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_3&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_Firing_Angle_4)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_4++;
			//Coil_Increment_Firing=0;

			records[43]=Firing_angle_4;
			CANFrame.id=0x43;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_4>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_4&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_4--;
			//Coil_Decrement_Firing=0;
			records[43]=Firing_angle_4;
			CANFrame.id=0x43;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_4>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_4&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_Firing_Angle_5)
	{
		if(Coil_Increment_Firing)
		{
			Firing_angle_5++;
			//Coil_Increment_Firing=0;

			records[44]=Firing_angle_5;
			CANFrame.id=0x44;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_5>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_5&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_5--;
			//Coil_Decrement_Firing=0;
			records[44]=Firing_angle_5;
			CANFrame.id=0x44;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_5>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_5&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}

	}

	if(Coil_Firing_Angle_6)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_6++;
			//Coil_Increment_Firing=0;

			records[45]=Firing_angle_6;
			CANFrame.id=0x45;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_6>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_6&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_6--;
			//Coil_Decrement_Firing=0;
			records[45]=Firing_angle_6;
			CANFrame.id=0x45;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_6>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_6&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}

	}

	if(Coil_Firing_Angle_7)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_7++;
			//Coil_Increment_Firing=0;

			records[46]=Firing_angle_7;
			CANFrame.id=0x46;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_7>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_7&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_7--;
			//Coil_Decrement_Firing=0;
			records[46]=Firing_angle_7;
			CANFrame.id=0x46;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_7>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_7&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}

	}

	if(Coil_Firing_Angle_8)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_8++;
			//Coil_Increment_Firing=0;

			records[47]=Firing_angle_8;
			CANFrame.id=0x47;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_8>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_8&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_8--;
			//Coil_Decrement_Firing=0;
			records[47]=Firing_angle_8;
			CANFrame.id=0x47;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_8>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_8&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}

	}

	if(Coil_Firing_Angle_9)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_9++;
			//Coil_Increment_Firing=0;

			records[48]=Firing_angle_9;
			CANFrame.id=0x48;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_9>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_9&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_9--;
			//Coil_Decrement_Firing=0;
			records[48]=Firing_angle_9;
			CANFrame.id=0x48;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_9>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_9&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}

	}


	if(Coil_Firing_Angle_10)
	{

		if(Coil_Increment_Firing)
		{
			Firing_angle_10++;
			//Coil_Increment_Firing=0;

			records[49]=Firing_angle_10;
			CANFrame.id=0x49;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_10>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_10&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_10--;
			//Coil_Decrement_Firing=0;
			records[49]=Firing_angle_10;
			CANFrame.id=0x49;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_10>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_10&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}

	}

	if(Coil_Firing_Angle_11)
	{
		if(Coil_Increment_Firing)
		{
			Firing_angle_11++;
			//Coil_Increment_Firing=0;

			records[50]=Firing_angle_11;
			CANFrame.id=0x50;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_11>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_11&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_11--;
			//Coil_Decrement_Firing=0;
			records[50]=Firing_angle_11;
			CANFrame.id=0x50;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_11>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_11&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_Firing_Angle_12)
	{
		if(Coil_Increment_Firing)
		{
			Firing_angle_12++;
			//Coil_Increment_Firing=0;

			records[51]=Firing_angle_12;
			CANFrame.id=0x51;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_12>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_12&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_12--;
			//Coil_Decrement_Firing=0;
			records[51]=Firing_angle_12;
			CANFrame.id=0x51;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_12>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_12&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_Firing_Angle_13)
	{
		if(Coil_Increment_Firing)
		{
			Firing_angle_13++;
			//Coil_Increment_Firing=0;

			records[52]=Firing_angle_13;
			CANFrame.id=0x52;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_13>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_13&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_13--;
			//Coil_Decrement_Firing=0;
			records[52]=Firing_angle_13;
			CANFrame.id=0x52;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_13>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_13&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_Firing_Angle_14)
	{
		if(Coil_Increment_Firing)
		{
			Firing_angle_14++;
			//Coil_Increment_Firing=0;

			records[53]=Firing_angle_14;
			CANFrame.id=0x53;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_14>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_14&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			Firing_angle_14--;
			//Coil_Decrement_Firing=0;
			records[53]=Firing_angle_14;
			CANFrame.id=0x53;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(Firing_angle_14>>8)&0xFF;
			CANFrame.data[1]=Firing_angle_14&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}
}

else if(Flag_AllHead_firing)
{
	if(Coil_Increment_Firing)
	{
		fir_flag=0;
//		fir=0;
//		fir_time=0;
		//Coil_Increment_Firing=0;
		records[40]=++Firing_angle_1;
		records[41]=++Firing_angle_2;
		records[42]=++Firing_angle_3;
		records[43]=++Firing_angle_4;
		records[44]=++Firing_angle_5;
		records[45]=++Firing_angle_6;
		records[46]=++Firing_angle_7;
		records[47]=++Firing_angle_8;
		records[48]=++Firing_angle_9;
		records[49]=++Firing_angle_10;
		records[50]=++Firing_angle_11;
		records[51]=++Firing_angle_12;
		records[52]=++Firing_angle_13;
		records[53]=++Firing_angle_14;

		do
		{
			if(fir==0)
			{
				records[40]=Firing_angle_1;
				CANFrame.id=0x40;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_1>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_1&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=1;
				mS_Delay(5);
			}
			else if(fir==1)
			{
				records[41]=Firing_angle_2;
				CANFrame.id=0x41;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_2>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_2&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=2;
				mS_Delay(5);
			}
			else if(fir==2)
			{
				records[42]=Firing_angle_3;
				CANFrame.id=0x42;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_3>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_3&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=3;
				mS_Delay(5);

			}
			else if(fir==3)
			{
				records[43]=Firing_angle_4;
				CANFrame.id=0x43;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_4>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_4&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=4;
				mS_Delay(5);

			}
			else if(fir==4)
			{
				records[44]=Firing_angle_5;
				CANFrame.id=0x44;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_5>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_5&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=5;
				mS_Delay(5);
			}
			else if(fir==5)
			{
				records[45]=Firing_angle_6;
				CANFrame.id=0x45;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_6>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_6&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=6;
				mS_Delay(5);
			}
			else if(fir==6)
			{
				records[46]=Firing_angle_7;
				CANFrame.id=0x46;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_7>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_7&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=7;
				mS_Delay(5);

			}
			else if(fir==7)
			{
				records[47]=Firing_angle_8;
				CANFrame.id=0x47;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_8>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_8&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=8;
				mS_Delay(5);
			}
			else if(fir==8)
			{
				records[48]=Firing_angle_9;
				CANFrame.id=0x48;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_9>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_9&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=9;
				mS_Delay(5);
			}
			else if(fir==9)
			{
				records[49]=Firing_angle_10;
				CANFrame.id=0x49;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_10>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_10&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=10;
				mS_Delay(5);
			}
			else if(fir==10)
			{
				records[50]=Firing_angle_11;
				CANFrame.id=0x50;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_11>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_11&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=11;
				mS_Delay(5);
			}
			else if(fir==11)
			{
				records[51]=Firing_angle_12;
				CANFrame.id=0x51;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_12>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_12&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=12;
				mS_Delay(5);
			}
			else if(fir==12)
			{
				records[52]=Firing_angle_13;
				CANFrame.id=0x52;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_13>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_13&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=13;
				mS_Delay(5);
			}
			else if(fir==13)
			{
				records[53]=Firing_angle_14;
				CANFrame.id=0x53;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_14>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_14&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=0;
				mS_Delay(5);
				fir_flag = 1;
			}
		}while(fir_flag!=1);

	}

	if(Coil_Decrement_Firing)
	{
		fir_flag=0;
//		fir=0;
	//	fir_time=0;
		//Coil_Decrement_Firing=0;
		records[40]=--Firing_angle_1;
		records[41]=--Firing_angle_2;
		records[42]=--Firing_angle_3;
		records[43]=--Firing_angle_4;
		records[44]=--Firing_angle_5;
		records[45]=--Firing_angle_6;
		records[46]=--Firing_angle_7;
		records[47]=--Firing_angle_8;
		records[48]=--Firing_angle_9;
		records[49]=--Firing_angle_10;
		records[50]=--Firing_angle_11;
		records[51]=--Firing_angle_12;
		records[52]=--Firing_angle_13;
		records[53]=--Firing_angle_14;

	//	for(fir=0;fir<10;fir++)
	do
	{
			if(fir==0)
			{
				records[40]=Firing_angle_1;
				CANFrame.id=0x40;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_1>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_1&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=1;
				mS_Delay(5);
			}
			else if(fir==1)
			{
				records[41]=Firing_angle_2;
				CANFrame.id=0x41;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_2>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_2&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=2;
				mS_Delay(5);
			}
			else if(fir==2)
			{
				records[42]=Firing_angle_3;
				CANFrame.id=0x42;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_3>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_3&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=3;
				mS_Delay(5);

			}
			else if(fir==3)
			{
				records[43]=Firing_angle_4;
				CANFrame.id=0x43;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_4>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_4&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=4;
				mS_Delay(5);

			}
			else if(fir==4)
			{
				records[44]=Firing_angle_5;
				CANFrame.id=0x44;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_5>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_5&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=5;
				mS_Delay(5);
			}
			else if(fir==5)
			{
				records[45]=Firing_angle_6;
				CANFrame.id=0x45;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_6>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_6&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=6;
				mS_Delay(5);
			}
			else if(fir==6)
			{
				records[46]=Firing_angle_7;
				CANFrame.id=0x46;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_7>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_7&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=7;
				mS_Delay(5);

			}
			else if(fir==7)
			{
				records[47]=Firing_angle_8;
				CANFrame.id=0x47;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_8>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_8&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=8;
				mS_Delay(5);
			}
			else if(fir==8)
			{
				records[48]=Firing_angle_9;
				CANFrame.id=0x48;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_9>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_9&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=9;
				mS_Delay(5);
			}
			else if(fir==9)
			{
				records[49]=Firing_angle_10;
				CANFrame.id=0x49;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_10>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_10&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=10;
				mS_Delay(5);
				//fir_flag=1;
			}
			else if(fir==10)
			{
				records[50]=Firing_angle_11;
				CANFrame.id=0x50;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_11>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_11&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=11;
				mS_Delay(5);
			}
			else if(fir==11)
			{
				records[51]=Firing_angle_12;
				CANFrame.id=0x51;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_12>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_12&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=12;
				mS_Delay(5);
			}
			else if(fir==12)
			{
				records[52]=Firing_angle_13;
				CANFrame.id=0x52;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_13>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_13&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=13;
				mS_Delay(5);
			}
			else if(fir==13)
			{
				records[53]=Firing_angle_14;
				CANFrame.id=0x53;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(Firing_angle_14>>8)&0xFF;
				CANFrame.data[1]=Firing_angle_14&0xFF;
				CAN_SEND_MEG(&CANFrame);

				fir=0;
				mS_Delay(5);
				fir_flag = 1;
			}
		}while(fir_flag!=1);

		}
	}


}

void check_new_time(void)
{

	if(Flag_AllHead_ontime==0)
	{

		if(Coil_On_Time_1)
		{

			if(Coil_Increment_Firing)
			{
				On_Time_1++;

				records[70]=On_Time_1;
				CANFrame.id=0x70;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_1>>8)&0xFF;
				CANFrame.data[1]=On_Time_1&0xFF;
				CAN_SEND_MEG(&CANFrame);
			}
			else if(Coil_Decrement_Firing)
			{
				On_Time_1--;

				records[70]=On_Time_1;
				CANFrame.id=0x70;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_1>>8)&0xFF;
				CANFrame.data[1]=On_Time_1&0xFF;
				CAN_SEND_MEG(&CANFrame);
			}
		}

		if(Coil_On_Time_2)
		{

			if(Coil_Increment_Firing)
			{
				On_Time_2++;

				records[71]=On_Time_2;
				CANFrame.id=0x71;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_2>>8)&0xFF;
				CANFrame.data[1]=On_Time_2&0xFF;
				CAN_SEND_MEG(&CANFrame);

			}
			else if(Coil_Decrement_Firing)
			{
				On_Time_2--;

				records[71]=On_Time_2;
				CANFrame.id=0x71;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_2>>8)&0xFF;
				CANFrame.data[1]=On_Time_2&0xFF;
				CAN_SEND_MEG(&CANFrame);
			}
		}

		if(Coil_On_Time_3)
		{

			if(Coil_Increment_Firing)
			{
				On_Time_3++;

				records[72]=On_Time_3;
				CANFrame.id=0x72;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_3>>8)&0xFF;
				CANFrame.data[1]=On_Time_3&0xFF;
				CAN_SEND_MEG(&CANFrame);

			}
			else if(Coil_Decrement_Firing)
			{
				On_Time_3--;

				records[72]=On_Time_3;
				CANFrame.id=0x72;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_3>>8)&0xFF;
				CANFrame.data[1]=On_Time_3&0xFF;
				CAN_SEND_MEG(&CANFrame);
			}
		}

		if(Coil_On_Time_4)
		{

		if(Coil_Increment_Firing)
		{
			On_Time_4++;

			records[73]=On_Time_4;
			CANFrame.id=0x73;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_4>>8)&0xFF;
			CANFrame.data[1]=On_Time_4&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_4--;

			records[73]=On_Time_4;
			CANFrame.id=0x73;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_4>>8)&0xFF;
			CANFrame.data[1]=On_Time_4&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_5)
	{
		if(Coil_Increment_Firing)
		{
			On_Time_5++;

			records[74]=On_Time_5;
			CANFrame.id=0x74;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_5>>8)&0xFF;
			CANFrame.data[1]=On_Time_5&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_5--;

			records[74]=On_Time_5;
			CANFrame.id=0x74;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_5>>8)&0xFF;
			CANFrame.data[1]=On_Time_5&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_6)
	{
		if(Coil_Increment_Firing)
		{
			On_Time_6++;

			records[75]=On_Time_6;
			CANFrame.id=0x75;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_6>>8)&0xFF;
			CANFrame.data[1]=On_Time_6&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_6--;

			records[75]=On_Time_6;
			CANFrame.id=0x75;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_6>>8)&0xFF;
			CANFrame.data[1]=On_Time_6&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_7)
	{
		if(Coil_Increment_Firing)
		{
			On_Time_7++;

			records[76]=On_Time_7;
			CANFrame.id=0x76;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_7>>8)&0xFF;
			CANFrame.data[1]=On_Time_7&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_7--;

			records[76]=On_Time_7;
			CANFrame.id=0x76;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_7>>8)&0xFF;
			CANFrame.data[1]=On_Time_7&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_8)
	{

		if(Coil_Increment_Firing)
		{
			On_Time_8++;

			records[77]=On_Time_8;
			CANFrame.id=0x77;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_8>>8)&0xFF;
			CANFrame.data[1]=On_Time_8&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_8--;

			records[77]=On_Time_8;
			CANFrame.id=0x77;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_8>>8)&0xFF;
			CANFrame.data[1]=On_Time_8&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_9)
	{

		if(Coil_Increment_Firing)
		{
			On_Time_9++;

			records[78]=On_Time_9;
			CANFrame.id=0x78;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_9>>8)&0xFF;
			CANFrame.data[1]=On_Time_9&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_9--;

			records[78]=On_Time_9;
			CANFrame.id=0x78;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_9>>8)&0xFF;
			CANFrame.data[1]=On_Time_9&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_10)
	{

		if(Coil_Increment_Firing)
		{
			On_Time_10++;

			records[79]=On_Time_10;
			CANFrame.id=0x79;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_10>>8)&0xFF;
			CANFrame.data[1]=On_Time_10&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_10--;

			records[79]=On_Time_10;
			CANFrame.id=0x79;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_10>>8)&0xFF;
			CANFrame.data[1]=On_Time_10&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_11)
	{
		if(Coil_Increment_Firing)
		{
			On_Time_11++;

			records[80]=On_Time_11;
			CANFrame.id=0x80;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_11>>8)&0xFF;
			CANFrame.data[1]=On_Time_11&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_11--;

			records[80]=On_Time_11;
			CANFrame.id=0x80;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_11>>8)&0xFF;
			CANFrame.data[1]=On_Time_11&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_12)
	{
		if(Coil_Increment_Firing)
		{
			On_Time_12++;

			records[81]=On_Time_12;
			CANFrame.id=0x81;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_12>>8)&0xFF;
			CANFrame.data[1]=On_Time_12&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_12--;

			records[81]=On_Time_12;
			CANFrame.id=0x81;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_12>>8)&0xFF;
			CANFrame.data[1]=On_Time_12&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_13)
	{
		if(Coil_Increment_Firing)
		{
			On_Time_13++;

			records[82]=On_Time_13;
			CANFrame.id=0x82;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_13>>8)&0xFF;
			CANFrame.data[1]=On_Time_13&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_13--;

			records[82]=On_Time_13;
			CANFrame.id=0x82;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_13>>8)&0xFF;
			CANFrame.data[1]=On_Time_13&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
	}

	if(Coil_On_Time_14)
	{
		if(Coil_Increment_Firing)
		{
			On_Time_14++;

			records[83]=On_Time_14;
			CANFrame.id=0x83;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_14>>8)&0xFF;
			CANFrame.data[1]=On_Time_14&0xFF;
			CAN_SEND_MEG(&CANFrame);

		}
		else if(Coil_Decrement_Firing)
		{
			On_Time_14--;

			records[83]=On_Time_14;
			CANFrame.id=0x83;
			CANFrame.rtr=0;
			CANFrame.dlc=2;
			CANFrame.data[0]=(On_Time_14>>8)&0xFF;
			CANFrame.data[1]=On_Time_14&0xFF;
			CAN_SEND_MEG(&CANFrame);
		}
		}

	}

	else if(Flag_AllHead_ontime)
	{
		if(Coil_Increment_Firing)
		{
			on_flag=0;

			records[70]=++On_Time_1;
			records[71]=++On_Time_2;
			records[72]=++On_Time_3;
			records[73]=++On_Time_4;
			records[74]=++On_Time_5;
			records[75]=++On_Time_6;
			records[76]=++On_Time_7;
			records[77]=++On_Time_8;
			records[78]=++On_Time_9;
			records[79]=++On_Time_10;
			records[80]=++On_Time_11;
			records[81]=++On_Time_12;
			records[82]=++On_Time_13;
			records[83]=++On_Time_14;

			do
			{
			if(ontime==0)
			{
				records[70]=On_Time_1;
				CANFrame.id=0x70;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_1>>8)&0xFF;
				CANFrame.data[1]=On_Time_1&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=1;
				mS_Delay(5);
			}
			else if(ontime==1)
			{
				records[71]=On_Time_2;
				CANFrame.id=0x71;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_2>>8)&0xFF;
				CANFrame.data[1]=On_Time_2&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=2;
				mS_Delay(5);
			}
			else if(ontime==2)
			{
				records[72]=On_Time_3;
				CANFrame.id=0x72;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_3>>8)&0xFF;
				CANFrame.data[1]=On_Time_3&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=3;
				mS_Delay(5);
			}
			else if(ontime==3)
			{
				records[73]=On_Time_4;
				CANFrame.id=0x73;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_4>>8)&0xFF;
				CANFrame.data[1]=On_Time_4&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=4;
				mS_Delay(5);
			}
			else if(ontime==4)
			{
				records[74]=On_Time_5;
				CANFrame.id=0x74;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_5>>8)&0xFF;
				CANFrame.data[1]=On_Time_5&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=5;
				mS_Delay(5);
			}
			else if(ontime==5)
			{
				records[75]=On_Time_6;
				CANFrame.id=0x75;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_6>>8)&0xFF;
				CANFrame.data[1]=On_Time_6&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=6;
				mS_Delay(5);
			}
			else if(ontime==6)
			{
				records[76]=On_Time_7;
				CANFrame.id=0x76;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_7>>8)&0xFF;
				CANFrame.data[1]=On_Time_7&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=7;
				mS_Delay(5);
			}
			else if(ontime==7)
			{
				records[77]=On_Time_8;
				CANFrame.id=0x77;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_8>>8)&0xFF;
				CANFrame.data[1]=On_Time_8&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=8;
				mS_Delay(5);
			}
			else if(ontime==8)
			{
				records[78]=On_Time_9;
				CANFrame.id=0x78;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_9>>8)&0xFF;
				CANFrame.data[1]=On_Time_9&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=9;
				mS_Delay(5);
			}
			else if(ontime==9)
			{
				records[79]=On_Time_10;
				CANFrame.id=0x79;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_10>>8)&0xFF;
				CANFrame.data[1]=On_Time_10&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=10;
				mS_Delay(5);
				//on_flag=1;
			}
			else if(ontime==10)
			{
				records[80]=On_Time_11;
				CANFrame.id=0x80;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_11>>8)&0xFF;
				CANFrame.data[1]=On_Time_11&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=11;
				mS_Delay(5);
			}
			else if(ontime==11)
			{
				records[81]=On_Time_12;
				CANFrame.id=0x81;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_12>>8)&0xFF;
				CANFrame.data[1]=On_Time_12&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=12;
				mS_Delay(5);
			}
			else if(ontime==12)
			{
				records[82]=On_Time_13;
				CANFrame.id=0x82;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_13>>8)&0xFF;
				CANFrame.data[1]=On_Time_13&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=13;
				mS_Delay(5);
			}
			else if(ontime==13)
			{
				records[83]=On_Time_13;
				CANFrame.id=0x83;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_13>>8)&0xFF;
				CANFrame.data[1]=On_Time_13&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=0;
				mS_Delay(5);
				on_flag=1;
			}

			}while(on_flag!=1);

		}

		if(Coil_Decrement_Firing)
		{
			on_flag=0;

			records[70]=--On_Time_1;
			records[71]=--On_Time_2;
			records[72]=--On_Time_3;
			records[73]=--On_Time_4;
			records[74]=--On_Time_5;
			records[75]=--On_Time_6;
			records[76]=--On_Time_7;
			records[77]=--On_Time_8;
			records[78]=--On_Time_9;
			records[79]=--On_Time_10;
			records[80]=--On_Time_11;
			records[81]=--On_Time_12;
			records[82]=--On_Time_13;
			records[83]=--On_Time_14;

			do
			{

			if(ontime==0)
			{
				records[70]=On_Time_1;
				CANFrame.id=0x70;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_1>>8)&0xFF;
				CANFrame.data[1]=On_Time_1&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=1;
				mS_Delay(5);
			}
			else if(ontime==1)
			{
				records[71]=On_Time_2;
				CANFrame.id=0x71;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_2>>8)&0xFF;
				CANFrame.data[1]=On_Time_2&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=2;
				mS_Delay(5);
			}
			else if(ontime==2)
			{
				records[72]=On_Time_3;
				CANFrame.id=0x72;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_3>>8)&0xFF;
				CANFrame.data[1]=On_Time_3&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=3;
				mS_Delay(5);
			}
			else if(ontime==3)
			{
				records[73]=On_Time_4;
				CANFrame.id=0x73;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_4>>8)&0xFF;
				CANFrame.data[1]=On_Time_4&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=4;
				mS_Delay(5);
			}
			else if(ontime==4)
			{
				records[74]=On_Time_5;
				CANFrame.id=0x74;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_5>>8)&0xFF;
				CANFrame.data[1]=On_Time_5&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=5;
				mS_Delay(5);
			}
			else if(ontime==5)
			{
				records[75]=On_Time_6;
				CANFrame.id=0x75;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_6>>8)&0xFF;
				CANFrame.data[1]=On_Time_6&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=6;
				mS_Delay(5);
			}
			else if(ontime==6)
			{
				records[76]=On_Time_7;
				CANFrame.id=0x76;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_7>>8)&0xFF;
				CANFrame.data[1]=On_Time_7&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=7;
				mS_Delay(5);
			}
			else if(ontime==7)
			{
				records[77]=On_Time_8;
				CANFrame.id=0x77;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_8>>8)&0xFF;
				CANFrame.data[1]=On_Time_8&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=8;
				mS_Delay(5);
			}
			else if(ontime==8)
			{
				records[78]=On_Time_9;
				CANFrame.id=0x78;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_9>>8)&0xFF;
				CANFrame.data[1]=On_Time_9&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=9;
				mS_Delay(5);
			}
			else if(ontime==9)
			{
				records[79]=On_Time_10;
				CANFrame.id=0x79;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_10>>8)&0xFF;
				CANFrame.data[1]=On_Time_10&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=10;
				mS_Delay(5);
			}

			else if(ontime==10)
			{
				records[80]=On_Time_11;
				CANFrame.id=0x80;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_11>>8)&0xFF;
				CANFrame.data[1]=On_Time_11&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=11;
				mS_Delay(5);
			}
			else if(ontime==11)
			{
				records[81]=On_Time_12;
				CANFrame.id=0x81;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_12>>8)&0xFF;
				CANFrame.data[1]=On_Time_12&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=12;
				mS_Delay(5);
			}
			else if(ontime==12)
			{
				records[82]=On_Time_13;
				CANFrame.id=0x82;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_13>>8)&0xFF;
				CANFrame.data[1]=On_Time_13&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=13;
				mS_Delay(5);
			}
			else if(ontime==13)
			{
				records[83]=On_Time_13;
				CANFrame.id=0x83;
				CANFrame.rtr=0;
				CANFrame.dlc=2;
				CANFrame.data[0]=(On_Time_13>>8)&0xFF;
				CANFrame.data[1]=On_Time_13&0xFF;
				CAN_SEND_MEG(&CANFrame);

				ontime=0;
				mS_Delay(5);
				on_flag=1;
			}


			}while(on_flag!=1);
		}
	}

}

void Set_Para(void)
{

	if(Firing_angle_1 == 0 && Firing_angle_2 == 0 && Firing_angle_3 == 0  && Firing_angle_4 == 0 &&
	   Firing_angle_5 == 0 && Firing_angle_6 == 0 && Firing_angle_7 == 0 && Firing_angle_8 == 0 &&
	   Firing_angle_9 == 0 && Firing_angle_10 == 0 && Firing_angle_11 == 0 && Firing_angle_12 == 0 &&
	   Firing_angle_13 == 0 && Firing_angle_14 == 0)
	{
		//Firing_angle_1=Firing_angle_2=Firing_angle_3=Firing_angle_4=Firing_angle_5=Firing_angle_6=Firing_angle_7=Firing_angle_8=Firing_angle_9=Firing_angle_10=Firing_angle_11=Firing_angle_12=Firing_angle_13=Firing_angle_14=records[105];
		//On_Time_1=On_Time_2=On_Time_3=On_Time_4=On_Time_5=On_Time_6=On_Time_7=On_Time_8=On_Time_9=On_Time_10=On_Time_11=On_Time_12=On_Time_13=On_Time_14=records[107];
		Firing_angle_1 = records[40]; //Changes done on 31/07/2023
		Firing_angle_2 = records[41];
		Firing_angle_3 = records[42];
		Firing_angle_4 = records[43];
		Firing_angle_5 = records[44];
		Firing_angle_6 = records[45];
		Firing_angle_7 = records[46];
		Firing_angle_8 = records[47];
		Firing_angle_9 = records[48];
		Firing_angle_10 = records[49];
		Firing_angle_11 = records[50];
		Firing_angle_12 = records[51];
		Firing_angle_13 = records[52];
		Firing_angle_14 = records[53];

		On_Time_1 = records[70];
		On_Time_2 = records[71];
		On_Time_3 = records[72];
		On_Time_4 = records[73];
		On_Time_5 = records[74];
		On_Time_6 = records[75];
		On_Time_7 = records[76];
		On_Time_8 = records[77];
		On_Time_9 = records[78];
		On_Time_10 = records[79];
		On_Time_11 = records[80];
		On_Time_12 = records[81];
		On_Time_13 = records[82];
		On_Time_14 = records[83];
	}

	Mainhead_Firing=records[104];
	Mainhead_Firing_angle = Mainhead_Firing;
	Mainhead_Ontime = records[107];		//changes done on 31/07/2023

	records[70]=On_Time_1;
	records[71]=On_Time_2;
	records[72]=On_Time_3;
	records[73]=On_Time_4;
	records[74]=On_Time_5;
	records[75]=On_Time_6;
	records[76]=On_Time_7;
	records[77]=On_Time_8;
	records[78]=On_Time_9;
	records[79]=On_Time_10;
	records[80]=On_Time_11;
	records[81]=On_Time_12;
	records[82]=On_Time_13;
	records[83]=On_Time_14;

	records[40]=Firing_angle_1;
	records[41]=Firing_angle_2;
	records[42]=Firing_angle_3;
	records[43]=Firing_angle_4;
	records[44]=Firing_angle_5;
	records[45]=Firing_angle_6;
	records[46]=Firing_angle_7;
	records[47]=Firing_angle_8;
	records[48]=Firing_angle_9;
	records[49]=Firing_angle_10;
	records[50]=Firing_angle_11;
	records[51]=Firing_angle_12;
	records[52]=Firing_angle_13;
	records[53]=Firing_angle_14;

	records[84] = Mainhead_Firing;
	records[85] = All_head_lf_ontime;


	if((Adc_para_flag==0)&&(Parameter_flag==0))
	{
		CANFrame.id=0xAD;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(ADC_GAIN>>8)&0xFF;
		CANFrame.data[1]=ADC_GAIN&0xFF;
		CANFrame.data[2]=(ADC_SPEED>>8)&0xFF;
		CANFrame.data[3]=ADC_SPEED&0xFF;
		CANFrame.data[4]=(ADC_SAMPLE>>8)&0xFF;
		CANFrame.data[5]=ADC_SAMPLE&0xFF;
		CANFrame.data[6]=(ADC_DIFFERENCE_COUNT>>8)&0xFF;
		CANFrame.data[7]=ADC_DIFFERENCE_COUNT&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Adc_para_flag=1;
		mS_Delay(5);

	}
	else if((Adc_para_flag==1)&&(Parameter_flag==0))
	{
		CANFrame.id=0xAE;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(ADC_STABLE_COUNT>>8)&0xFF;
		CANFrame.data[1]=ADC_STABLE_COUNT&0xFF;
		CANFrame.data[2]=(AUTO_ZERO_INTERVAL>>8)&0xFF;
		CANFrame.data[3]=AUTO_ZERO_INTERVAL&0xFF;
		CANFrame.data[4]=(WEIGHING_RANGE>>8)&0xFF;
		CANFrame.data[5]=WEIGHING_RANGE&0xFF;
		CANFrame.data[6]=(STABLE_TIME>>8)&0xFF;
		CANFrame.data[7]=STABLE_TIME&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=1;
		mS_Delay(5);

	}
	else if(Parameter_flag==1)
	{
		CANFrame.id=0xAf;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Mainhead_Firing>>8)&0xFF;
		CANFrame.data[1]=Mainhead_Firing&0xFF;
		CANFrame.data[2]=(All_head_firing>>8)&0xFF;
		CANFrame.data[3]=All_head_firing&0xFF;
		CANFrame.data[4]=(All_head_lf_ondelay>>8)&0xFF;
		CANFrame.data[5]=All_head_lf_ondelay&0xFF;
		CANFrame.data[6]=(All_head_lf_ontime>>8)&0xFF;
		CANFrame.data[7]=All_head_lf_ontime&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=2;
		mS_Delay(5);
	}
	else if(Parameter_flag==2)
	{
		CANFrame.id=0xB1;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(All_head_fb_ondelay>>8)&0xFF;
		CANFrame.data[1]=All_head_fb_ondelay&0xFF;
		CANFrame.data[2]=(All_head_fb_ontime>>8)&0xFF;
		CANFrame.data[3]=All_head_fb_ontime&0xFF;
		CANFrame.data[4]=(All_head_wb_ondelay>>8)&0xFF;
		CANFrame.data[5]=All_head_wb_ondelay&0xFF;
		CANFrame.data[6]=(All_head_wb_ontime>>8)&0xFF;
		CANFrame.data[7]=All_head_wb_ontime&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=3;
		mS_Delay(5);
	}
	else if(Parameter_flag==3)
	{
		CANFrame.id=0xB2;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(All_head_cb_ondelay>>8)&0xFF;
		CANFrame.data[1]=All_head_cb_ondelay&0xFF;
		CANFrame.data[2]=(All_head_cb_ontime>>8)&0xFF;
		CANFrame.data[3]=All_head_cb_ontime&0xFF;
		CANFrame.data[4]=(All_head_stepper_pattern>>8)&0xFF;
		CANFrame.data[5]=All_head_stepper_pattern&0xFF;
		CANFrame.data[6]=(Discharge_ontime>>8)&0xFF;
		CANFrame.data[7]=Discharge_ontime&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=4;
		mS_Delay(5);
	}
	else if(Parameter_flag==4)
	{
		CANFrame.id=0xB3;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(TargetWt>>8)&0xFF;
		CANFrame.data[1]=TargetWt&0xFF;

		/*CANFrame.data[2]=(All_head_cb_ontime>>8)&0xFF;
		CANFrame.data[3]=All_head_cb_ontime&0xFF;
		CANFrame.data[4]=(All_head_stepper_pattern>>8)&0xFF;
		CANFrame.data[5]=All_head_stepper_pattern&0xFF;
		CANFrame.data[6]=(Discharge_ontime>>8)&0xFF;
		CANFrame.data[7]=Discharge_ontime&0xFF;*/

		CANFrame.data[2]=(Calib_Slope[9]>>8)&0xFF;
		CANFrame.data[3]=Calib_Slope[9]&0xFF;
		CANFrame.data[4]=(Calib_Slope[10]>>8)&0xFF;
		CANFrame.data[5]=Calib_Slope[10]&0xFF;
		CANFrame.data[6]=(Recharge_Divisor>>8)&0xFF;
		CANFrame.data[7]=Recharge_Divisor&0xFF;



		CAN_SEND_MEG(&CANFrame);


	/*	Parameter_flag=0;
		Adc_para_flag=0;
		Coil_SetParaADC=0;*/

		Parameter_flag = 5;
		mS_Delay(5);

	}

	else if(Parameter_flag==5)
	{
		CANFrame.id=0xB4;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Tare_Count[1]>>8)&0xFF;
		CANFrame.data[1]=Tare_Count[1]&0xFF;
		CANFrame.data[2]=(Tare_Count[2]>>8)&0xFF;
		CANFrame.data[3]=Tare_Count[2]&0xFF;
		CANFrame.data[4]=(Tare_Count[3]>>8)&0xFF;
		CANFrame.data[5]=Tare_Count[3]&0xFF;
		CANFrame.data[6]=(Tare_Count[4]>>8)&0xFF;
		CANFrame.data[7]=Tare_Count[4]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=6;
		mS_Delay(5);

	}

	else if(Parameter_flag==6)
	{
		CANFrame.id=0xB5;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Tare_Count[5]>>8)&0xFF;
		CANFrame.data[1]=Tare_Count[5]&0xFF;
		CANFrame.data[2]=(Tare_Count[6]>>8)&0xFF;
		CANFrame.data[3]=Tare_Count[6]&0xFF;
		CANFrame.data[4]=(Tare_Count[7]>>8)&0xFF;
		CANFrame.data[5]=Tare_Count[7]&0xFF;
		CANFrame.data[6]=(Tare_Count[8]>>8)&0xFF;
		CANFrame.data[7]=Tare_Count[8]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=7;
		mS_Delay(5);

	}

	else if(Parameter_flag==7)
	{
		CANFrame.id=0xB6;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Tare_Count[9]>>8)&0xFF;
		CANFrame.data[1]=Tare_Count[9]&0xFF;
		CANFrame.data[2]=(Tare_Count[10]>>8)&0xFF;
		CANFrame.data[3]=Tare_Count[10]&0xFF;
		CANFrame.data[4]=(Calib_Count[1]>>8)&0xFF;
		CANFrame.data[5]=Calib_Count[1]&0xFF;
		CANFrame.data[6]=(Calib_Count[2]>>8)&0xFF;
		CANFrame.data[7]=Calib_Count[2]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=8;
		mS_Delay(5);

	}

	else if(Parameter_flag==8)
	{
		CANFrame.id=0xB7;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Calib_Count[3]>>8)&0xFF;
		CANFrame.data[1]=Calib_Count[3]&0xFF;
		CANFrame.data[2]=(Calib_Count[4]>>8)&0xFF;
		CANFrame.data[3]=Calib_Count[4]&0xFF;
		CANFrame.data[4]=(Calib_Count[5]>>8)&0xFF;
		CANFrame.data[5]=Calib_Count[5]&0xFF;
		CANFrame.data[6]=(Calib_Count[6]>>8)&0xFF;
		CANFrame.data[7]=Calib_Count[6]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=9;
		mS_Delay(5);

	}

	else if(Parameter_flag==9)
	{
		CANFrame.id=0xB8;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Calib_Count[7]>>8)&0xFF;
		CANFrame.data[1]=Calib_Count[7]&0xFF;
		CANFrame.data[2]=(Calib_Count[8]>>8)&0xFF;
		CANFrame.data[3]=Calib_Count[8]&0xFF;
		CANFrame.data[4]=(Calib_Count[9]>>8)&0xFF;
		CANFrame.data[5]=Calib_Count[9]&0xFF;
		CANFrame.data[6]=(Calib_Count[10]>>8)&0xFF;
		CANFrame.data[7]=Calib_Count[10]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=10;
		mS_Delay(5);

	}

	else if(Parameter_flag==10)
	{
		CANFrame.id=0xB9;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Calib_Slope[1]>>8)&0xFF;
		CANFrame.data[1]=Calib_Slope[1]&0xFF;
		CANFrame.data[2]=(Calib_Slope[2]>>8)&0xFF;
		CANFrame.data[3]=Calib_Slope[2]&0xFF;
		CANFrame.data[4]=(Calib_Slope[3]>>8)&0xFF;
		CANFrame.data[5]=Calib_Slope[3]&0xFF;
		CANFrame.data[6]=(Calib_Slope[4]>>8)&0xFF;
		CANFrame.data[7]=Calib_Slope[4]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=11;
		mS_Delay(5);

	}

	else if(Parameter_flag==11)
	{
		CANFrame.id=0xC1;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Calib_Slope[5]>>8)&0xFF;
		CANFrame.data[1]=Calib_Slope[5]&0xFF;
		CANFrame.data[2]=(Calib_Slope[6]>>8)&0xFF;
		CANFrame.data[3]=Calib_Slope[6]&0xFF;
		CANFrame.data[4]=(Calib_Slope[7]>>8)&0xFF;
		CANFrame.data[5]=Calib_Slope[7]&0xFF;
		CANFrame.data[6]=(Calib_Slope[8]>>8)&0xFF;
		CANFrame.data[7]=Calib_Slope[8]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=12;
		mS_Delay(5);

	/*	Parameter_flag=0;
		Adc_para_flag=0;
		Coil_SetParaADC=0;*/

	}

	else if(Parameter_flag==12)
	{
		CANFrame.id=0xC2;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Calib_Slope[11]>>8)&0xFF;
		CANFrame.data[1]=Calib_Slope[11]&0xFF;
		CANFrame.data[2]=(Calib_Slope[12]>>8)&0xFF;
		CANFrame.data[3]=Calib_Slope[12]&0xFF;
		CANFrame.data[4]=(Calib_Slope[13]>>8)&0xFF;
		CANFrame.data[5]=Calib_Slope[13]&0xFF;
		CANFrame.data[6]=(Calib_Slope[14]>>8)&0xFF;
		CANFrame.data[7]=Calib_Slope[14]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=13;
		mS_Delay(5);
	}
	else if(Parameter_flag==13)
	{
		CANFrame.id=0xC3;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Calib_Count[11]>>8)&0xFF;
		CANFrame.data[1]=Calib_Count[11]&0xFF;
		CANFrame.data[2]=(Calib_Count[12]>>8)&0xFF;
		CANFrame.data[3]=Calib_Count[12]&0xFF;
		CANFrame.data[4]=(Calib_Count[13]>>8)&0xFF;
		CANFrame.data[5]=Calib_Count[13]&0xFF;
		CANFrame.data[6]=(Calib_Count[14]>>8)&0xFF;
		CANFrame.data[7]=Calib_Count[14]&0xFF;
		CAN_SEND_MEG(&CANFrame);

		Parameter_flag=14;
		mS_Delay(5);
	}
	else if(Parameter_flag==14)
	{
		CANFrame.id=0xC4;
		CANFrame.rtr=0;
		CANFrame.dlc=8;
		CANFrame.data[0]=(Tare_Count[11]>>8)&0xFF;
		CANFrame.data[1]=Tare_Count[11]&0xFF;
		CANFrame.data[2]=(Tare_Count[12]>>8)&0xFF;
		CANFrame.data[3]=Tare_Count[12]&0xFF;
		CANFrame.data[4]=(Tare_Count[13]>>8)&0xFF;
		CANFrame.data[5]=Tare_Count[13]&0xFF;
		CANFrame.data[6]=(Tare_Count[14]>>8)&0xFF;
		CANFrame.data[7]=Tare_Count[14]&0xFF;
		CAN_SEND_MEG(&CANFrame);


		Parameter_flag=0;
		Adc_para_flag=0;
		Coil_SetParaADC=0;
		Power_On_Para_Write = 0;  //changes done on 31/07/2023
	}


}

void Zeroing()
{

	if(state == 100)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 101;
		}
	}
	if(state == 101)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 102;
		}
	}
	if(state == 102)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 103;
		}
	}
	if(state == 103)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 104;
		}
	}
	if(state == 104)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 105;
		}
	}
	if(state == 105)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 106;
		}
	}
	if(state == 106)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 107;
		}
	}
	if(state == 107)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 108;
		}
	}
	if(state == 108)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 109;
		}
	}
	if(state == 109)
	{
		CANFrame.id = (0x8F + zeroing_head_count);
		CANFrame.rtr=0;
		CANFrame.dlc=0;
		CAN_SEND_MEG(&CANFrame);
		mS_Delay(1000);

		if(zeroing_head_count == MAX_HEADS)
		{
			zeroing_head_count = 1;
		}
		else
		{
			zeroing_head_count++;
			state = 100;
		}
	}
}


void aux_can_frame()
{
	m_command = 0;
	Manual_Operation_Number = 5;
	Manual_Operation_Can_Frame();
}

void check_aux_home()
{
	CANFrame.id = 0xD9;
	CANFrame.rtr=0;
	CANFrame.dlc=0;
	CAN_SEND_MEG(&CANFrame);

}

void Aux_Open()
{
	auxbucket_empty_flag = 1;
	CANFrame.id = 0xD7;
	CANFrame.rtr=0;
	CANFrame.dlc=1;
	CANFrame.data[0]=1;
	CAN_SEND_MEG(&CANFrame);
	aux_open_flag = 1;
}

void Aux_Close()
{
	CANFrame.id = 0xD7;
	CANFrame.rtr=0;
	CANFrame.dlc=1;
	CANFrame.data[0]=0;
	CAN_SEND_MEG(&CANFrame);
	aux_open_flag = 0;
}

void DryRunCycle()
{
	CANFrame.id = 0xDA;
	CANFrame.rtr=0;
	CANFrame.dlc=5;
	CANFrame.data[0]=dry_runcycle_flag;
	CANFrame.data[1]=(Dryrun_weight_Division>>8)&0xFF;
	CANFrame.data[2]=Dryrun_weight_Division&0xFF;
	CANFrame.data[3]=(Dryrun_weight_addition>>8)&0xFF;
	CANFrame.data[4]=Dryrun_weight_addition&0xFF;
	CAN_SEND_MEG(&CANFrame);
}

void AutoCorrection()
{
	CANFrame.id = 0xDB;
	CANFrame.rtr=0;
	CANFrame.dlc=1;
	CANFrame.data[0]=auto_correction_flag;
	CAN_SEND_MEG(&CANFrame);
}


void Over_Under_Weight()
{
	CANFrame.id = 0xDC;
	CANFrame.rtr=0;
	CANFrame.dlc=2;
	CANFrame.data[0]=(NO_SELECT_COUNT>>8)&0xFF;
	CANFrame.data[1]=NO_SELECT_COUNT&0xFF;
	CAN_SEND_MEG(&CANFrame);
}


void Slave_Flash_Write()
{
	CANFrame.id = 0xD5;
	CANFrame.rtr=0;
	CANFrame.dlc=1;
	CANFrame.data[0]= slave_flash_write_flash;
	CAN_SEND_MEG(&CANFrame);
}
