/*
 * delay.h
 *
 *  Created on: Feb 11, 2022
 *      Author: Dnyaneshwar
 */

#ifndef DELAY_VARIABLES_H_
#define DELAY_VARIABLES_H_


#ifndef DELAY_VAR
# define _DELAY extern
# define _DELAY_INIT(x)
#else
# define _DELAY
# define _DELAY_INIT(x)  = x
#endif

_DELAY _Bool msflag _DELAY_INIT(0);

_DELAY unsigned int mS_Cnt _DELAY_INIT(0);

#endif /* DELAY_VARIABLES_H_ */
