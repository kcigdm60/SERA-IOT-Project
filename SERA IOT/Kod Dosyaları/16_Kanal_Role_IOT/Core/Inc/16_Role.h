/*
 * 16_Role.h
 *
 *  Created on: Jul 10, 2026
 *      Author: kcigd
 */
#ifndef INC_16_ROLE_H_
#define INC_16_ROLE_H_

#include "main.h"

extern uint8_t delay_ms(uint16_t ms_value);
extern void delay_stop();

typedef struct {
    GPIO_TypeDef *Port; 
    uint8_t Number;
} PIN_t;

extern PIN_t PIN_1;
extern PIN_t PIN_2;
extern PIN_t PIN_3;
extern PIN_t PIN_4;
extern PIN_t PIN_5;
extern PIN_t PIN_6;
extern PIN_t PIN_7;
extern PIN_t PIN_8;
extern PIN_t PIN_9;
extern PIN_t PIN_10;
extern PIN_t PIN_11;
extern PIN_t PIN_12;
extern PIN_t PIN_13;
extern PIN_t PIN_14;
extern PIN_t PIN_15;
extern PIN_t PIN_16;

void GPIO_Config(void);
void PIN_Set(PIN_t pin);
void PIN_Reset(PIN_t pin);

#endif /* INC_16_ROLE_H_ */
