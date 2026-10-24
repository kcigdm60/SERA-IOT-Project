/*
 * pid_main_conf_func.h
 *
 *  Created on: Jul 13, 2026
 *      Author: kcigd
 */

#ifndef INC_PID_MAIN_CONF_FUNC_H_
#define INC_PID_MAIN_CONF_FUNC_H_


#include "main.h"


extern uint16_t ms_counter;
extern uint8_t delay_counter;

extern  void SHT_Temp_Moi_Ref();
extern  float node3_pid_current_temp;
extern float SHT_Temperature;
extern void PID_System();
extern uint8_t Flow_Select;

extern void NODE3_Flow();

extern uint8_t Lora_Main_Count;
extern uint8_t I2C_Wait_Time;

extern uint8_t watchdog_key;


void TIM2_IRQHandler(void);


void System_Clock_Config(void);
void Timer_Config(void);

uint8_t delay_ms(uint16_t ms_value);
void delay_stop(void);

void SPI_Config(void);
void DMA_Config(void);
void SPI_Write(uint8_t reg_adress, uint8_t veri);
uint8_t SPI_Read(uint8_t reg_adress);

void I2C1_Config(void);
void I2C1_Multi_Write(uint8_t slave_addr,uint8_t control_byte,uint8_t* data_array,uint8_t data_size);
void I2C1_Single_Write(uint8_t slave_addr, uint8_t control_byte, uint8_t data);
void I2C1_Read(uint8_t slave_addr, uint8_t register_addr,uint8_t data, uint8_t* data_array, uint8_t data_size);

void TIM4_Config();
void TIM4_IRQHandler(void);

void TIM1_PWM_Update(uint16_t pwm_value);
void TIM1_PWM_Config();

void PID_Options_Conf();

void WatchDog_Config(void);
void WatchDog_Feed(void);




#endif /* INC_PID_MAIN_CONF_FUNC_H_ */
