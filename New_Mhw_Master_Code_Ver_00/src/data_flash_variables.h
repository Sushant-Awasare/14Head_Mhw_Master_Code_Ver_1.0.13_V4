/*
 * data_flash_variables.h
 *
 *  Created on: Apr 10, 2023
 *      Author: COE04
 */

#ifndef DATA_FLASH_VARIABLES_H_
#define DATA_FLASH_VARIABLES_H_

#ifndef DATA_FLASH_VAR
# define _DATA_FLASH extern
# define _DATA_FLASH_INIT(x)
#else
# define _DATA_FLASH
# define _DATA_FLASH_INIT(x)  = x
#endif

#define TEST_BUFF_NUM (120)

_DATA_FLASH uint16_t gs_test_w_buff[TEST_BUFF_NUM];
_DATA_FLASH uint16_t gs_test_r_buff[TEST_BUFF_NUM];


#endif /* DATA_FLASH_VARIABLES_H_ */
