/*
 * pid.h
 *
 *  Created on: Jul 15, 2026
 *      Author: kcigd
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"


extern float SHT_Temperature;
extern void SHT_Temp_Moi_Ref();
extern void PWM_Options(uint8_t pwm_options);
extern void TIM1_PWM_Update(uint16_t pwm_value);


extern float node3_pid_desired_temp;
extern float node3_pid_current_temp;
extern float node3_pid_Kp;
extern float node3_pid_Ki;
extern float node3_pid_Kd;


typedef enum {

	HEAT_DOWN=0,
	HEAT_UP=1

}Options_t;



extern float PID_error;
extern float PID_total_error;
extern float PID_previous_error;

extern float PID_total_power;

extern float PID_current_temperature;
extern float PID_target_temperature;

extern float PID_Kp;
extern float PID_Ki;
extern float PID_Kd;

extern float PID_P;
extern float PID_I;
extern float PID_D;

extern uint8_t PID_status;

// H. A. PID Fonksiyon Prototipleri
void PID_System(void);
void PID_Options(Options_t options);




#endif
