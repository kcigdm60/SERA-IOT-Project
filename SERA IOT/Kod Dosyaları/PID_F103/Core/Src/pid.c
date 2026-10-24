/*
 * pid.c
 *
 *  Created on: Jul 15, 2026
 *      Author: kcigd
 */

#include "pid.h"

float PID_error=0; // hata
float PID_total_error=0; // hata toplamı
float PID_previous_error=0; // önceki hata

float PID_total_power=0; // toplam güç


 float PID_P;
 float PID_I;
 float PID_D;

uint16_t PWM_VALUE;


uint8_t PID_status=0;


#define TS 3.0f



// status 0 ısıtma , status 1 doğutma

void PID_System()
{

	// veriyi çek
	  SHT_Temp_Moi_Ref();

	  // hata hesap
	  PID_error = node3_pid_desired_temp - node3_pid_current_temp;

	  // P
	  PID_P=node3_pid_Kp * PID_error;


      PID_total_error += (PID_error * TS);
	  PID_I = node3_pid_Ki * PID_total_error;


	  if (PID_I > 50.0f) {
	         PID_I = 50.0f;
	         PID_total_error = PID_I / node3_pid_Ki;

	     } else if (PID_I < -50.0f) {
	         PID_I = -50.0f;
	         PID_total_error = PID_I / node3_pid_Ki;
	     }


	    // D
	    PID_D = (node3_pid_Kd * (PID_error - PID_previous_error)) / TS;


	    // Toplam Çıkış Gücü
	      PID_total_power = PID_P + PID_I + PID_D;


	      // Çıkış Sınırla (-100 ile +100 arası)
	      if (PID_total_power > 100.0f) {
	          PID_total_power = 100.0f;
	      }
	      else if (PID_total_power < -100.0f) {
	          PID_total_power = -100.0f;
	      }

	      //Isıtma-Soğutma Durum
	         if (PID_total_power >= 0.0f) {
	             PID_status = 0;
	         }
	         else {
	             PID_total_power = -PID_total_power;
	             PID_status = 1;
	         }

	         PID_previous_error = PID_error;

	         PWM_VALUE=(PID_total_power*10);


	     	TIM1_PWM_Update(PWM_VALUE);


	     	PWM_Options(PID_status);

}




