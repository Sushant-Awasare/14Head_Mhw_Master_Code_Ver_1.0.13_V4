/*
 * includes.h
 *
 *  Created on: Feb 22, 2022
 *      Author: Dnyaneshwar
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include "r_smc_entry.h"
#include <stdio.h>              // sprintf()
#include <string.h>             // strlen()
#include <stdint.h>
#include "platform.h"
#include "r_rscan_rx_if.h"
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_SCI5.h"
#include "r_cg_wdt.h"
#include "r_sci_rx_if.h"        // The SCI FIT module API interface file.
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "math.h"


//#include "can_variables.h"
#include "can_functions.h"

#include "delay_variables.h"
#include "delay_functions.h"

#define GLOBAL_VAR
#include "global_variables.h"
#include "global_functions.h"
#include "global_defines.h"

#define MODBUS_VAR
#include "modbus_variables.h"
#include "modbus_functions.h"

#define MANUAL_VAR
#include "manual_variables.h"
#include "manual_functions.h"


#define CALIBRATION_VAR
#include "calibration_variables.h"
#include "calibration_functions.h"

#define RUN_VAR
#include "runcycle_variables.h"
#include "runcycle_functions.h"

#define DATA_FLASH_VAR
#include "data_flash_variables.h"
#include "data_flash_functions.h"

#endif /* INCLUDES_H_ */
