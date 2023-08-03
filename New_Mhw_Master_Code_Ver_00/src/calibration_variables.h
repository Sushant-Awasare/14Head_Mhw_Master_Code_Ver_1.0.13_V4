/*
 * calibration.h
 *
 *  Created on: Feb 15, 2022
 *      Author: Dnyaneshwar
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#ifndef CALIBRATION_VAR
# define _CALIBRATION extern
# define _CALIBRATION_INIT(x)
#else
# define _CALIBRATION
# define _CALIBRATION_INIT(x)  = x
#endif

_CALIBRATION _Bool calib_button_flag _CALIBRATION_INIT(0);
_CALIBRATION _Bool tare_button_flag _CALIBRATION_INIT(0);

_CALIBRATION unsigned int calib_weight_time _CALIBRATION_INIT(0);
_CALIBRATION unsigned int c_command _CALIBRATION_INIT(0);

#endif /* CALIBRATION_H_ */
