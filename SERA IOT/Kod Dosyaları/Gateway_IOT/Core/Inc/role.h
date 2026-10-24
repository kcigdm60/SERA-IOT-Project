/*
 * role.h
 *
 *  Created on: May 22, 2026
 *      Author: kcigd
 */

#ifndef INC_ROLE_H_
#define INC_ROLE_H_

#include "main.h"


extern uint8_t delay_ms(uint16_t ms_value);
extern void delay_stop();


typedef enum {

	ROLE_1=8,
	ROLE_2=9,
	ROLE_3=10,
	ROLE_4=11,

	ROLE_5=12,
	ROLE_6=13,
	ROLE_7=14,
	ROLE_8=15


}ROLE_NAME_t;



typedef enum {

	ROLE_RESET,
	ROLE_SET

}ROLE_STATUS_t;



void Role_Config();
void Role_Drive(ROLE_NAME_t role_name,ROLE_STATUS_t role_status);




#endif /* INC_ROLE_H_ */
