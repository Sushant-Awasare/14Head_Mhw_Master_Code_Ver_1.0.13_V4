/*
 * xyx.h
 *
 *  Created on: Feb 9, 2022
 *      Author: Dnyaneshwar
 */

#ifndef CAN_FUNCTIONS_H_
#define CAN_FUNCTIONS_H_


 void Initialize_CAN_Communication();
 void can_rx231_init_ports(void);
 void MainCallback(can_cb_evt_t event, void *p_args);
 void ChanCallback(uint8_t chan, can_cb_evt_t event, void *p_args);
 void Process_Can_Msg(can_rxmsg_t *rxmsg);
 void Check_CAN_Status(void);
 void CAN_SEND_MEG(struct CFrame *CANFrame1);
 void Slave_Flash_Write();


#endif /* CAN_FUNCTIONS_H_ */
