/*
 * pid_main_conf_func.c
 *
 *  Created on: Jul 13, 2026
 *      Author: kcigd
 */

#include "pid_main_conf_func.h"


//////// Counter Veriable ////////


uint16_t ms_counter=0;
uint8_t delay_counter=0;
uint8_t timer4_value=0;
uint8_t watchdog_key=1;

////////////////////////////////////////////////////// IRQHandler ////////////////////////////////////////////////////////////////////////


void TIM2_IRQHandler(void) {

	if (TIM2->SR & (1 << 0)) {

		ms_counter++;

		TIM2->SR &= ~(1 << 0);

	}}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TIM4_IRQHandler(void) {

	if (TIM4->SR & (1 << 0)) {

		TIM4->SR &= ~(1 << 0);

		Flow_Select=1;



	}}


////////////////////////////////////////////////////////   System_Clock_Config   //////////////////////////////////////////////////////////////////////////////


// tamamlandı

void System_Clock_Config() {

    RCC->CR |= (1 << 16);
    while(!(RCC->CR & (1 << 17)));

    FLASH->ACR = 0;
    FLASH->ACR |= (2 << 0);
    FLASH->ACR |= (1 << 4);

    while(!(FLASH->ACR & (1 << 5)));



    RCC->CFGR =0;
    RCC->CFGR |= (7 << 18);
    RCC->CFGR |= (1 << 16);
    RCC->CFGR |= (2 << 14);
    RCC->CFGR |= (4 << 8);


    RCC->CR |= (1 << 24);
    while(!(RCC->CR & (1 << 25)));


    RCC->CFGR |= (2 << 0);

    while(!(RCC->CFGR & (2 << 2)));

}


//////////////////////////////////////////////////////////   Timer_Config    /////////////////////////////////////////////////////////////////////////////////////


// tamamlandı

void Timer_Config() {

	 // 1 ms

	RCC->APB1ENR |= (1 << 0);

	TIM2->CR1 =0;
	TIM2->CR1 |= (1 << 7);

	TIM2->DIER |= (1 << 0);

	TIM2->PSC = 71;
	TIM2->ARR = 999;
	TIM2->CNT = 0;

	TIM2->EGR |= (1 << 0);

	NVIC_SetPriority(TIM2_IRQn,0);
	NVIC_EnableIRQ(TIM2_IRQn);

}


//////////////////////////////////////////////////////////   delay_ms   //////////////////////////////////////////////////////////////////////////////////////////


uint8_t delay_ms(uint16_t ms_value) {

	if (delay_counter == 0) {

		delay_counter=1;
		ms_counter=0;

     	TIM2->CNT =0;
	    TIM2->CR1 |= (1 << 0);

	}

	if (ms_counter >= ms_value) {

		TIM2->CR1 &= ~(1 << 0);
		TIM2->CNT =0;
		ms_counter=0;
		delay_counter=0;

		return 1;

	}

	return 0;
}


//////////////////////////////////////////////////////////   delay stop   //////////////////////////////////////////////////////////////////////////////////////////


void delay_stop() {


	        TIM2->CR1 &= ~(1 << 0);
			TIM2->CNT =0;
			ms_counter=0;
			delay_counter=0;
}


////////////////////////////////////////////////////////   SPI_Config   ///////////////////////////////////////////////////////////////////////////////////////////

void SPI_Config() {

		// GPIOA AKTİF
		RCC->APB2ENR |= (1 << 0) | (1 << 2);

		// SPI1 AKTİF
		RCC->APB2ENR |= (1 << 12);


		// PA4 NSS
		GPIOA->CRL |= (3 << 16);
		GPIOA->CRL &= ~(3 << 18);

		// PA5 SCK
		GPIOA->CRL |=(3 << 20);
		GPIOA->CRL &= ~(3 << 22);
		GPIOA->CRL |= (2 << 22);

		// PA6 MISO
		GPIOA->CRL &= ~(3 << 24);
		GPIOA->CRL &= ~(3 << 26);
		GPIOA->CRL |= (1 << 26);

		// PA7 MOSI
		GPIOA->CRL |= (3 << 28);
		GPIOA->CRL &= ~(3 << 30);
		GPIOA->CRL |= (2 << 30);


	SPI1->CR1 =0;

	SPI1->CR1 |= (1 << 9);
	SPI1->CR1 |= (1 << 8);
	SPI1->CR1 |= (3 << 3);
	SPI1->CR1 |= (1 << 2);

	SPI1->CR2 |= (1 << 1);
	SPI1->CR2 |= (1 << 0);
	SPI1->CR1 |= (1 << 6);

}


//////////////////////////////////////////////////////////  DMA_Config  /////////////////////////////////////////////////////////////////////////////////////////////////


void DMA_Config() {

    RCC->AHBENR |= (1 << 0);


    DMA1_Channel2->CCR = 0;

    DMA1_Channel2->CCR |= (1 << 7);

    DMA1_Channel3->CCR = 0;

    DMA1_Channel3->CCR |= (1 << 7) | (1 << 4);
}


/////////////////////////////////////////////////////////  SPI_Write   //////////////////////////////////////////////////////////////////////////////////////////////////////


void SPI_Write(uint8_t reg_adress, uint8_t veri) {
    static uint8_t spi_tx_buf[2];
    static uint8_t spi_rx_buf[2];

    GPIOA->ODR &= ~(1 << 4);

    spi_tx_buf[0] = reg_adress | 0x80;
    spi_tx_buf[1] = veri;

    DMA1_Channel2->CCR &= ~(1 << 0);
    DMA1_Channel3->CCR &= ~(1 << 0);



    DMA1->IFCR |= (0xF << 4) | (0xF << 8);

    DMA1_Channel2->CNDTR = 2;
    DMA1_Channel2->CPAR  = (uint32_t)&SPI1->DR;
    DMA1_Channel2->CMAR  = (uint32_t)spi_rx_buf;

    DMA1_Channel3->CNDTR = 2;
    DMA1_Channel3->CPAR  = (uint32_t)&SPI1->DR;
    DMA1_Channel3->CMAR  = (uint32_t)spi_tx_buf;

    SPI1->CR2 |= (1 << 0) | (1 << 1);

    DMA1_Channel2->CCR |= (1 << 0);
    DMA1_Channel3->CCR |= (1 << 0);


    while (!(DMA1->ISR & (1 << 5)));
    while (!(DMA1->ISR & (1 << 9)));
    while (SPI1->SR & (1 << 7));

    DMA1_Channel2->CCR &= ~(1 << 0);
    DMA1_Channel3->CCR &= ~(1 << 0);
    SPI1->CR2 &= ~((1 << 0) | (1 << 1));

    GPIOA->ODR |= (1 << 4);
}


//////////////////////////////////////////////////////////   SPI_Read   /////////////////////////////////////////////////////////////////////////////////////


uint8_t SPI_Read(uint8_t reg_adress) {
    static uint8_t spi_tx_buf[2];
    static uint8_t spi_rx_buf[2];

    GPIOA->ODR &= ~(1 << 4);

    spi_tx_buf[0] = reg_adress | 0x00;
    spi_tx_buf[1] = 0xFF;          // dumy bye (saat sinyali için)


    DMA1_Channel2->CCR &= ~(1 << 0);
    DMA1_Channel3->CCR &= ~(1 << 0);


    DMA1->IFCR |= (0xF << 4) | (0xF << 8);

    DMA1_Channel2->CNDTR = 2;
    DMA1_Channel2->CPAR  = (uint32_t)&SPI1->DR;
    DMA1_Channel2->CMAR  = (uint32_t)spi_rx_buf;

    DMA1_Channel3->CNDTR = 2;
    DMA1_Channel3->CPAR  = (uint32_t)&SPI1->DR;
    DMA1_Channel3->CMAR  = (uint32_t)spi_tx_buf;

    SPI1->CR2 |= (1 << 0) | (1 << 1);


    DMA1_Channel2->CCR |= (1 << 0);
    DMA1_Channel3->CCR |= (1 << 0);

    while (!(DMA1->ISR & (1 << 5)));
    while (!(DMA1->ISR & (1 << 9)));
    while (SPI1->SR & (1 << 7));

    DMA1_Channel2->CCR &= ~(1 << 0);
    DMA1_Channel3->CCR &= ~(1 << 0);
    SPI1->CR2 &= ~((1 << 0) | (1 << 1));

    GPIOA->ODR |= (1 << 4);

    return spi_rx_buf[1];
}


/////////////////////////////////////////////  I2C1_Config  ////////////////////////////////////////////////////////////////////////////////////////
void I2C1_Config() {
    RCC->APB1ENR |= (1 << 21);
    RCC->APB2ENR |= (1 << 3);

    GPIOB->CRL |= (0xF << 24) | (0xF << 28);

    I2C1->CR1 = 0;
    I2C1->CR2 = 0;
    I2C1->CCR = 0;
    I2C1->TRISE = 0;

    I2C1->CR2 |= (36 << 0);

    I2C1->CCR = 180;

    I2C1->TRISE = 37;

    I2C1->CR1 |= (1 << 0);
}


/////////////////////////////////////////////  I2C1_Multi_Write  ////////////////////////////////////////////////////////////////////////////////////////

void I2C1_Multi_Write(uint8_t slave_addr, uint8_t control_byte, uint8_t *data_array, uint8_t data_size) {
    while (I2C1->SR2 & (1 << 1));

    I2C1->CR1 |= (1 << 8);
    while(!(I2C1->SR1 & (1 << 0)));

    I2C1->DR = (slave_addr << 1) | 0;
    while(!(I2C1->SR1 & (1 << 1)));

    (void)I2C1->SR1;
    (void)I2C1->SR2;

    I2C1->DR = control_byte;
    while(!(I2C1->SR1 & (1 << 7)));

    for (uint8_t x = 0; x < data_size; x++) {
        I2C1->DR = data_array[x];
        while(!(I2C1->SR1 & (1 << 7)));
    }

    while(!(I2C1->SR1 & (1 << 2)));
    I2C1->CR1 |= (1 << 9);
}



void I2C1_Single_Write(uint8_t slave_addr, uint8_t control_byte, uint8_t data) {
    while (I2C1->SR2 & (1 << 1));

    I2C1->CR1 |= (1 << 8);
    while(!(I2C1->SR1 & (1 << 0)));

    I2C1->DR = (slave_addr << 1) | 0;
    while(!(I2C1->SR1 & (1 << 1)));

    (void)I2C1->SR1;
    (void)I2C1->SR2;

    I2C1->DR = control_byte;
    while(!(I2C1->SR1 & (1 << 7)));

    I2C1->DR = data;
    while(!(I2C1->SR1 & (1 << 7)));

    while(!(I2C1->SR1 & (1 << 2)));
    I2C1->CR1 |= (1 << 9);
}


void I2C1_Read(uint8_t slave_addr, uint8_t register_addr, uint8_t data, uint8_t *data_array, uint8_t data_size) {
    while (I2C1->SR2 & (1 << 1));

    I2C1->CR1 |= (1 << 8);
    while(!(I2C1->SR1 & (1 << 0)));

    I2C1->DR = (slave_addr << 1) | 0;
    while(!(I2C1->SR1 & (1 << 1)));

    (void)I2C1->SR1;
    (void)I2C1->SR2;

    I2C1->DR = register_addr;
    while(!(I2C1->SR1 & (1 << 7)));

    I2C1->DR = data;
    while(!(I2C1->SR1 & (1 << 7)));

    I2C1->CR1 |= (1 << 8);
    while(!(I2C1->SR1 & (1 << 0)));

    I2C1->DR = (slave_addr << 1) | 1;
    while(!(I2C1->SR1 & (1 << 1)));

    I2C1->CR1 |= (1 << 10);

    (void)I2C1->SR1;
    (void)I2C1->SR2;

    for (uint8_t x = 0; x < data_size; x++) {
        if (data_size == 1) {
            I2C1->CR1 &= ~(1 << 10);
            (void)I2C1->SR1;
            (void)I2C1->SR2;
            I2C1->CR1 |= (1 << 9);
            while(!(I2C1->SR1 & (1 << 6)));
            data_array[x] = I2C1->DR;
            return;
        }

        if (x == 0) {
            (void)I2C1->SR1;
            (void)I2C1->SR2;
        }

        if (x == (data_size - 1)) {
            I2C1->CR1 &= ~(1 << 10);
            I2C1->CR1 |= (1 << 9);
        }

        while(!(I2C1->SR1 & (1 << 6)));
        data_array[x] = I2C1->DR;
    }
}


/////////////////////////////////////////////  TIMER1_PWM_CONFIG  ////////////////////////////////////////////////////////////////////////////////////////

void TIM1_PWM_Config(void) {
    RCC->APB2ENR |= (1 << 11);
    RCC->APB2ENR |= (1 << 2);
    RCC->APB2ENR |= (1 << 0);

    GPIOA->CRH &= ~(0xF << (0 * 4));
    GPIOA->CRH |=  (0xB << (0 * 4));

    TIM1->CR1 = 0;
    TIM1->CR1 |= (1 << 7);

    TIM1->CCMR1 = 0;
    TIM1->CCMR1 |= (6 << 4);
    TIM1->CCMR1 |= (1 << 3);

    TIM1->CCER = 0;
    TIM1->CCER |= (1 << 0);

    TIM1->PSC  = 71;
    TIM1->ARR  = 999;
    TIM1->CCR1 = 500;

    TIM1->BDTR |= (1 << 15);

    TIM1->EGR |= (1 << 0);
    TIM1->CR1 |= (1 << 0);
}


void TIM1_PWM_Update(uint16_t pwm_value) {
    if (pwm_value > 999) pwm_value = 999;
    TIM1->CCR1 = pwm_value;
}


void PID_Options_Conf() {
    RCC->APB2ENR |= (1 << 3);

    GPIOB->CRH &= ~(0xF << (4 * (12 - 8)));
    GPIOB->CRH |=  (0x3 << (4 * (12 - 8)));

    GPIOB->ODR &= ~(1 << 12);
}


void PWM_Options(uint8_t pwm_options) {
    if (pwm_options) {
        GPIOB->ODR |= (1 << 12);
    } else {
        GPIOB->ODR &= ~(1 << 12);
    }
}


void TIM4_Config() {
    RCC->APB1ENR |= (1 << 2);

    TIM4->CR1 = 0;
    TIM4->CR1 |= (1 << 7);

    TIM4->CR2 = 0;
    TIM4->SMCR = 0;

    TIM4->DIER |= (1 << 0);

    TIM4->PSC = 7199;
    TIM4->ARR = 29999;
    TIM4->CNT = 0;

    NVIC_SetPriority(TIM4_IRQn, 1);
    NVIC_EnableIRQ(TIM4_IRQn);

    TIM4->EGR |= (1 << 0);
    TIM4->CR1 |= (1 << 0);
}


void WatchDog_Config() {
    if (watchdog_key == 1) {
        IWDG->KR = 0xCCCC;
        IWDG->KR = 0x5555;
        IWDG->PR = 0x05;
        IWDG->RLR = 2500;

        while (IWDG->SR != 0);

        IWDG->KR = 0xAAAA;
    }
}


void WatchDog_Feed() {
    if (watchdog_key == 1) {
        IWDG->KR = 0xAAAA;
    }
}
