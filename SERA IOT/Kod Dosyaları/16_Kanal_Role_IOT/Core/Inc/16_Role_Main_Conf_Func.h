/*
 * 16_Role_Main_Conf_Func.h
 *
 *  Created on: Jul 10, 2026
 *      Author: kcigd
 */

#ifndef INC_16_ROLE_MAIN_CONF_FUNC_H_
#define INC_16_ROLE_MAIN_CONF_FUNC_H_


#include "main.h"



////////// Counter Veriable /////////

extern  uint16_t ms_counter;
extern   uint8_t delay_counter;
extern uint8_t Lora_Main_Count;
extern uint8_t watchdog_key;


/////////////// IRQHandler  /////////////////////

void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);

////////////////////////////////////////////////

void System_Clock_Config();
void Timer_Config();
void TIM4_Config(void);

////////////////////////////////////////////////

void SPI_Config();
void DMA_Config();

////////////////////////////////////////////////

void SPI_Write(uint8_t reg_adress,uint8_t veri);
uint8_t SPI_Read(uint8_t reg_adress);

////////////////////////////////////////////////

uint8_t delay_ms(uint16_t ms_value);
void delay_stop();


////////////////////////////////////////////////////////////////////


void WatchDog_Config(void);
void WatchDog_Feed(void);






#endif /* INC_GATEWAY_MAIN_CONF_FUNC_H_ */


#endif /* INC_16_ROLE_MAIN_CONF_FUNC_H_ */
