/*
 * manual.h
 *
 *  Created on: Feb 15, 2022
 *      Author: Dnyaneshwar
 */

#ifndef MANUAL_VARIABLES_H_
#define MANUAL_VARIABLES_H_

#ifndef MANUAL_VAR
# define _MANUAL extern
# define _MANUAL_INIT(x)
#else
# define _MANUAL
# define _MANUAL_INIT(x)  = x
#endif

_MANUAL _Bool	 all_drain_flag _MANUAL_INIT(0);

_MANUAL unsigned int m_command _MANUAL_INIT(0);
_MANUAL unsigned int send_time _MANUAL_INIT(0);
_MANUAL unsigned int send_time_flag _MANUAL_INIT(0);
_MANUAL unsigned int Manual_Operation_Number _MANUAL_INIT(0);
_MANUAL unsigned int Manual_Cycle_Time _MANUAL_INIT(0);

#endif /* MANUAL_VARIABLES_H_ */
