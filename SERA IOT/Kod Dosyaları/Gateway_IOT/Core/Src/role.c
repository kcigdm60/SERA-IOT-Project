/*
 * role.c
 *
 *  Created on: May 22, 2026
 *      Author: kcigd
 */

#include "role.h"


void Role_Config(void) {
    RCC->AHB1ENR |= (1 << 0) | (1 << 1);

    GPIOA->MODER &= ~((3 << (8*2)) | (3 << (9*2)) | (3 << (10*2)) | (3 << (11*2)));
    GPIOA->MODER |=  ((1 << (8*2)) | (1 << (9*2)) | (1 << (10*2)) | (1 << (11*2)));

    GPIOB->MODER &= ~((3 << (12*2)) | (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2)));
    GPIOB->MODER |=  ((1 << (12*2)) | (1 << (13*2)) | (1 << (14*2)) | (1 << (15*2)));

    GPIOA->OTYPER &= ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
    GPIOB->OTYPER &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));

    GPIOA->OSPEEDR &= ~((3 << (8*2)) | (3 << (9*2)) | (3 << (10*2)) | (3 << (11*2)));
    GPIOB->OSPEEDR &= ~((3 << (12*2)) | (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2)));

    GPIOA->PUPDR &= ~((3 << (8*2)) | (3 << (9*2)) | (3 << (10*2)) | (3 << (11*2)));
    GPIOA->PUPDR |=  ((2 << (8*2)) | (2 << (9*2)) | (2 << (10*2)) | (2 << (11*2)));

    GPIOB->PUPDR &= ~((3 << (12*2)) | (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2)));
    GPIOB->PUPDR |=  ((2 << (12*2)) | (2 << (13*2)) | (2 << (14*2)) | (2 << (15*2)));

    GPIOA->ODR &= ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
    GPIOB->ODR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
}


void Role_Drive(ROLE_NAME_t role_name,ROLE_STATUS_t role_status) {

switch (role_status) {

case ROLE_RESET: {

	if (role_name <= 11 ) {

		GPIOA->ODR &= ~(1 << role_name);

		while(!(delay_ms(100)));
		delay_stop();

	}

	else {

		GPIOB->ODR &= ~(1 << role_name);
		while(!(delay_ms(100)));
				delay_stop();

	}

	break;
}


case ROLE_SET: {
	if (role_name <= 11 ) {

			GPIOA->ODR |= (1 << role_name);
			while(!(delay_ms(100)));
					delay_stop();

		}

		else {

			GPIOB->ODR |= (1 << role_name);
			while(!(delay_ms(100)));
					delay_stop();

		}

		break;
	}
}
}

