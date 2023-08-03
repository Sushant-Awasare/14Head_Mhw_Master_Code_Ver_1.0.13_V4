/*
 * modbus_functions.h
 *
 *  Created on: Feb 9, 2022
 *      Author: Dnyaneshwar
 */

#ifndef MODBUS_FUNCTIONS_H_
#define MODBUS_FUNCTIONS_H_


void send(void);
void calCRC(unsigned char *bfptr, unsigned char nbytes);
void getvals(void);
void xtrbuff(void);
void decode(void);
void respns3(void);
void respns6(void);
void respns10(void);
void respns1(void);
void respns5(void);

void Delete_ALL_Records(void);
void rxisr(uint8_t);
void Copy_Records(void);
void Init_Sci_Peripheral(void);
void Init_Variables();

#endif /* MODBUS_FUNCTIONS_H_ */
