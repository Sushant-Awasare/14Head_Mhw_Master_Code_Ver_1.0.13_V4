/*
 * data_flash_functions.h
 *
 *  Created on: Apr 10, 2023
 *      Author: COE04
 */

#ifndef DATA_FLASH_FUNCTIONS_H_
#define DATA_FLASH_FUNCTIONS_H_

void read_run_data(void);
void write_run_data(uint16_t gs_test_w_buff[]);
void open_dataflash();
static void testwrite(uint16_t gs_test_w_buff[]);
static void ucb_function(void* error_code);
void erase(void);
static void trap(uint32_t test_no);
void flash_read_data();
void flash_write_data();

#endif /* DATA_FLASH_FUNCTIONS_H_ */
