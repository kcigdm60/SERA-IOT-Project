
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"



typedef enum {


	Node1_Repeat_Send=0,
	Node2_Repeat_Send=1,
	Node3_Repeat_Send=2,

	Node1_Message_Not_Send=3,
	Node2_Message_Not_Send=4,
	Node3_Message_Not_Send=5,


}Lora_Err_Main_t;



typedef enum {

	 Lora_Node1_Addr = 0x00,
	 Lora_Node2_Addr = 0x01,
	 Lora_Node3_Addr = 0x02,

	 Lora_Gateway_Addr=0x0A


}Lora_Node_Addr_t;




void Lora_Config(void);
void Lora_Value_Reset(void);
uint8_t Lora_Node1_Gateway_Send();
void NODE1_Flow(void);

void Lora_Start_Conf();
void Lora_Node1_Gateway_Send_Flow();
void Soil_Moisture_Calculation();

void Lora_Main_Reset();

void Lora_Query();



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
