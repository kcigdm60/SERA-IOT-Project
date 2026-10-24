/*
 * main_conf_func.h
 *
 *  Created on: Jun 27, 2026
 *      Author: kcigd
 *
 */
#ifndef INC_TOPRAK_MAIN_CONF_FUNC_H_
#define INC_TOPRAK_MAIN_CONF_FUNC_H_

#include "main.h"


////////// Counter Veriable ////////

extern uint16_t ms_counter;
extern uint8_t delay_counter;
extern uint8_t Lora_Main_Count;
extern uint8_t watchdog_key;

////////////////////////////////////////////////

void TIM2_IRQHandler(void);
void TIM4_Config(void);


////////////////////////////////////////////////

void System_Clock_Config();
void Timer_Config();

////////////////////////////////////////////////

uint8_t delay_ms(uint16_t ms_value);
void delay_stop();

////////////////////////////////////////////////

void SPI_Config();
void DMA_Config();

////////////////////////////////////////////////

void SPI_Write(uint8_t reg_adress,uint8_t veri);
uint8_t SPI_Read(uint8_t reg_adress);

////////////////////////////////////////////////


void ADC_Config();
void ADC_Read_Value(float *percent_value);

void ADC_Pin_Calibration_Conf();

void ADC_Air_Calibration();
void ADC_Water_Calibration();


///////////////////////////////////////////////

void TIM3_Config();

void Wire_1_Pin_Conf();

void Wire_High();
void Wire_Low();

uint8_t Line_Read();
void delay_us(uint16_t us);



void WatchDog_Config(void);
void WatchDog_Feed(void);



//////////////////////////////////////////////




#endif /* INC_TOPRAK_MAIN_CONF_FUNC_H_ */
