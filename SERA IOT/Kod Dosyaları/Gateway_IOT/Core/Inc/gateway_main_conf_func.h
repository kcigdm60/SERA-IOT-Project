/*
 * main_conf_func.h
 *
 *  Created on: Jun 26, 2026
 *      Author: kcigd
 */

#ifndef INC_GATEWAY_MAIN_CONF_FUNC_H_
#define INC_GATEWAY_MAIN_CONF_FUNC_H_

#include "main.h"

extern uint16_t str_len(const char *string);
extern uint16_t usart_rx_mark;

#define OLED_SLAVE_ADDR 0x3C

typedef enum {


	TRANMISSION_COMPLETE,
	TRANMISSION_COMPLETE_ERROR,
	IDLE


}Usart_Status_t;



////////// Counter Veriable /////////

extern volatile uint16_t ms_counter;
extern volatile uint8_t delay_counter;

////////// USART Veriable //////////

extern uint16_t coming_data_len;
extern uint16_t usart_index;


///// USART Status Veriable ////////

extern uint8_t usart_status;

// 0. bit Transmission complete flag
// 1. bit Transmission complete error flag
// 2. IDLE flag

extern char Usart_Rx[1024];

extern uint8_t Lora_Value_Show_Counter;
extern uint8_t Lora_Flow_Count;
extern uint8_t Lora_Flow_Inside_Count;

extern uint8_t tim3_counter;
extern uint8_t watchdog_key;


/////////////// IRQHandler  /////////////////////

void TIM2_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void USART2_IRQHandler(void);
void TIM3_IRQHandler(void);

////////////////////////////////////////////////

void System_Clock_Config();
void Timer_Config();
void TIM3_Config();

////////////////////////////////////////////////

uint8_t delay_ms(volatile uint16_t ms_value);
void delay_stop();

////////////////////////////////////////////////

void SPI_Config();
void DMA_Config();

////////////////////////////////////////////////

void SPI_Write(uint8_t reg_adress,uint8_t veri);
uint8_t SPI_Read(uint8_t reg_adress);

///////////////////////////////////////////////

void USART_2_Config();
void DMA1_USART2_Config();

///////////////////////////////////////////////

uint16_t str_len(const char *string);

/////////////////////////////////////////////////////////////////////

uint8_t USART_Send(char *at_commond);
void Usart_Message_Len_Index(uint16_t *index,uint16_t *coming_data);


uint8_t Usart_Reader(char *desired_value);
/////////////////////////////////////////////////////////////////////

void Usart_Flag_Marking(Usart_Status_t Flag);
uint8_t Usart_Flag_Read(Usart_Status_t flag);
void Usart_Flag_Clean(Usart_Status_t flag);

////////////////////////////////////////////////////////////////////

void I2C1_Config();
void I2C1_Write(uint8_t control_byte,uint8_t* data_array,uint8_t data_size);
void I2C_Reset();

////////////////////////////////////////////////////////////////////



void WatchDog_Config(void);
void WatchDog_Feed(void);
void TIM3_Reset();




#endif /* INC_GATEWAY_MAIN_CONF_FUNC_H_ */
