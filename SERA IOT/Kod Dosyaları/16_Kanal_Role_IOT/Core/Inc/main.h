
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"



extern uint8_t Lora_Main_Err;


typedef enum {

	        Node2_Read_SendData_ERR,
		    Node2_Read_RXDone_ERR,
			Node2_Read_Receive_ERR,
			Node2_Read_Sender_Addr_ERR,
			Node2_Read_Receive_Addr_ERR,
			Node2_Flow_ERR,


			Node2_Write_SendData_ERR,
			Node2_Write_RXDone_ERR,
			Node2_Write_Receive_ERR,
			Node2_Write_Sender_Addr_ERR,
			Node2_Write_Receive_Addr_ERR,
			Node2_Write_Process_ERR,


}Lora_Err_Main_t;


typedef enum {

	 Lora_Node1_Addr = 0x00,
	 Lora_Node2_Addr = 0x01,
	 Lora_Node3_Addr = 0x02,

	 Lora_Gateway_Addr=0x0A


}Lora_Node_Addr_t;


void Lora_Main_Err_Marking(Lora_Err_Main_t err);
uint8_t Lora_Main_Err_Read(Lora_Err_Main_t err);
void Lora_Main_Err_Clean(Lora_Err_Main_t err);
void Lora_Config(void);
void Lora_Value_Reset(void);
uint8_t Lora_Node2_Gateway_Send(void);
void Main_16_Role_Driver(void);
void Lora_Start_Conf();
void Lora_Node2_Gateway_Send_Flow();
void Lora_Main_Reset();
void Lora_Query_Gateway();




#ifdef __cplusplus
}
#endif

#endif
