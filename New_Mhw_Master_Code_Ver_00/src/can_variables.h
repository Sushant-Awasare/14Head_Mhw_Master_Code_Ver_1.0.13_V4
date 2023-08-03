/*
 * Can.h
 *
 *  Created on: Feb 4, 2022
 *      Author: Dnyaneshwar
 */

#ifndef CAN_VARIABLES_H_
#define CAN_VARIABLES_H_

//#include "platform.h"
//#include "r_rscan_rx_if.h"


/*#ifndef CAN_VAR
# define _CAN extern
# define _CAN_INIT(x)
#else
# define _CAN
# define _CAN_INIT(x)  = x
#endif*/

typedef enum e_state
{
    STATE_CONNECTING_UART,
    STATE_CONNECTING_CAN,
    STATE_CAN_TO_UART,
    STATE_UART_TO_CAN
} state_t;
//state_t       g_state;




 struct CFrame
 {
 	 unsigned long int 	id;

 	 uint8_t rtr;
 	 uint8_t dlc;

 	 unsigned char 	data[8];
 };
 struct CFrame CANFrame;



 uint32_t      g_ovfl_cnt = 0;
 can_filter_t  g_filter;
 can_err_t     err;
 can_cfg_t     config;
 can_bitrate_t baud;

 can_txmsg_t         g_txmsg;
 can_rxmsg_t         g_rxmsg;

 union bytetoword{
 	struct{
 		int8_t byte0;
 		int8_t byte1;
 	}byte;
 	int16_t word;
 };

 union bytetoword byte_to_word;


#endif /* CAN_VARIABLES_H_ */


