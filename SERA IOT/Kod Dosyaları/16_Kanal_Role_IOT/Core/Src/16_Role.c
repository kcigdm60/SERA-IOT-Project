/*
 * 16_Role.c
 *
 *  Created on: Jul 10, 2026
 *      Author: kcigd
 */



#include "16_Role.h"

PIN_t PIN_1  = {GPIOA, 1};
PIN_t PIN_2  = {GPIOA, 2};
PIN_t PIN_3  = {GPIOA, 3};
PIN_t PIN_4  = {GPIOA, 8};
PIN_t PIN_5  = {GPIOA, 9};
PIN_t PIN_6  = {GPIOA, 10};
PIN_t PIN_7  = {GPIOA, 11};
PIN_t PIN_8  = {GPIOA, 12};
PIN_t PIN_9  = {GPIOB, 3};
PIN_t PIN_10 = {GPIOB, 2};
PIN_t PIN_11 = {GPIOB, 4};
PIN_t PIN_12 = {GPIOB, 5};
PIN_t PIN_13 = {GPIOB, 6};
PIN_t PIN_14 = {GPIOB, 13};
PIN_t PIN_15 = {GPIOB, 14};
PIN_t PIN_16 = {GPIOB, 15};


void GPIO_Config(void) {

    RCC->AHB1ENR |= (1 << 0) | (1 << 1);

    GPIOA->MODER &= ~((3 << 2)  | (3 << 4)  | (3 << 6)  |
                      (3 << 16) | (3 << 18) | (3 << 20) | (3 << 22) | (3 << 24));
    GPIOA->MODER |=  ((1 << 2)  | (1 << 4)  | (1 << 6)  |
                      (1 << 16) | (1 << 18) | (1 << 20) | (1 << 22) | (1 << 24));

    GPIOA->OTYPER &= ~((1 << 1) | (1 << 2)  | (1 << 3)  |
                       (1 << 8) | (1 << 9)  | (1 << 10) | (1 << 11) | (1 << 12));

    GPIOA->OSPEEDR |= ((3 << 2)  | (3 << 4)  | (3 << 6)  |
                       (3 << 16) | (3 << 18) | (3 << 20) | (3 << 22) | (3 << 24));

    GPIOA->PUPDR &= ~((3 << 2)  | (3 << 4)  | (3 << 6)  |
                      (3 << 16) | (3 << 18) | (3 << 20) | (3 << 22) | (3 << 24));
    GPIOA->PUPDR |=  ((2 << 2)  | (2 << 4)  | (2 << 6)  |
                      (2 << 16) | (2 << 18) | (2 << 20) | (2 << 22) | (2 << 24));

    GPIOA->ODR &= ~((1 << 1) | (1 << 2)  | (1 << 3)  |
                    (1 << 8) | (1 << 9)  | (1 << 10) | (1 << 11) | (1 << 12));

    GPIOB->MODER &= ~((3 << 4)  | (3 << 6)  | (3 << 8)  | (3 << 10) | (3 << 12) |
                      (3 << 26) | (3 << 28) | (3 << 30));
    GPIOB->MODER |=  ((1 << 4)  | (1 << 6)  | (1 << 8)  | (1 << 10) | (1 << 12) |
                      (1 << 26) | (1 << 28) | (1 << 30));

    GPIOB->OTYPER &= ~((1 << 2)  | (1 << 3)  | (1 << 4)  | (1 << 5)  | (1 << 6)  |
                       (1 << 13) | (1 << 14) | (1 << 15));

    GPIOB->OSPEEDR |= ((3 << 4)  | (3 << 6)  | (3 << 8)  | (3 << 10) | (3 << 12) |
                       (3 << 26) | (3 << 28) | (3 << 30));

    GPIOB->PUPDR &= ~((3 << 4)  | (3 << 6)  | (3 << 8)  | (3 << 10) | (3 << 12) |
                      (3 << 26) | (3 << 28) | (3 << 30));
    GPIOB->PUPDR |=  ((2 << 4)  | (2 << 6)  | (2 << 8)  | (2 << 10) | (2 << 12) |
                      (2 << 26) | (2 << 28) | (2 << 30));

    GPIOB->ODR &= ~((1 << 2)  | (1 << 3)  | (1 << 4)  | (1 << 5)  | (1 << 6)  |
                    (1 << 13) | (1 << 14) | (1 << 15));

    // sinyal led

    RCC->AHB1ENR |= (1 << 2);

       GPIOC->MODER &= ~(3 << 26);
       GPIOC->MODER |=  (1 << 26);

       GPIOC->OTYPER &= ~(1 << 13);

       GPIOC->OSPEEDR |= (3 << 26);

       GPIOC->PUPDR &= ~(3 << 26);
       GPIOC->PUPDR |=  (2 << 26);

       GPIOC->ODR &= ~(1 << 13);
}


void PIN_Set(PIN_t pin) {
    while(!(delay_ms(5)));
    delay_stop();
    pin.Port->ODR |= (1 << pin.Number);
}



void PIN_Reset(PIN_t pin) {
    while(!(delay_ms(5)));
    delay_stop();
    pin.Port->ODR &= ~(1 << pin.Number);
}





