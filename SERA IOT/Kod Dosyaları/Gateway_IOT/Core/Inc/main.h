/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"

extern uint16_t Lora_Tx_Wait_Time;
extern uint16_t Lora_Rx_Wait_Time;

#define Lora_Read    0x00
#define Lora_Write   0x01


extern uint8_t gateway_write_relay_status;
extern float node1_moisture_value;
extern float node1_temperature_value;
extern uint8_t write_relay_status_high;
extern uint8_t write_relay_status_low;

extern float node3_read_pid_desired_temperature;
extern float node3_read_pid_current_temperature;
extern float node3_read_pid_Kp;
extern float node3_read_pid_Ki;
extern float node3_read_pid_Kd;

extern uint8_t Switch_Count;
extern uint8_t SIM800L_Key;


typedef enum {

	SEND_DATA_ERR,
	RX_DONE_ERR,
	RECEIVE_ERR,
    SENDER_ADDR_ERR,
    RECEIVE_ADDR_ERR,
    WRITE_PROCESS_ERR,
    FLOW_ERR

}Err_t;



typedef enum {

	 Lora_Node1_Addr = 0x00,
	 Lora_Node2_Addr = 0x01,
	 Lora_Node3_Addr = 0x02,

	 Lora_Gateway_Addr=0x0A


}Lora_Node_Addr_t;





void Lora_Config();
void Lora_Error_Show();
void Node1_Flow();
void Node2_Flow();
void Node3_Flow();
uint8_t Lora_Node3_Write();
uint8_t Lora_Node3_Read();
uint8_t Lora_Node2_Write_Read();
uint8_t Lora_Node1_Read();
void Lora_Value_Reset();

void Lora_Main_Err_Marking (uint8_t *ERR_Reg ,Err_t err);
uint8_t Lora_Main_Err_Read(uint8_t *ERR_reg ,Err_t err);
void Lora_Main_Err_Clean(uint8_t *ERR_reg ,Err_t err);




void Lora_Value_Show();
void Lora_Float_Show_Reg_Reset();
void Lora_Relay_Show_Reg_Reset();
void Main_8_Role_Driver();
void Lora_Start_Conf();
void Lora_Uint_Show_Reg_Reset();

void Lora_Main_Reset();
void deneme();
void Lora_Query_Gateway();

//void Error_Handler(void);



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
