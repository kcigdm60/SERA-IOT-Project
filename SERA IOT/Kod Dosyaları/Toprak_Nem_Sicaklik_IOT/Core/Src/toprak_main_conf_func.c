/*
 * main_conf_func.c
 *
 *  Created on: Jun 27, 2026
 *      Author: kcigd
 */

#include <toprak_main_conf_func.h>


//////// Counter Veriable ////////

uint16_t ms_counter=0;
uint8_t delay_counter=0;


/////// ADC Veriable ////////////

float adc_raw_volt=0;

float adc_air_volt=0;
float adc_water_volt=0;

uint8_t watchdog_key=1;


////////////////////////////////////////////////////// IRQHandler ////////////////////////////////////////////////////////////////////////

void TIM2_IRQHandler(void) {

	if (TIM2->SR & (1 << 0)) {

		ms_counter++;

		TIM2->SR &= ~(1 << 0);

	}}

////////////////////////////////////////////////////////   System_Clock_Config   //////////////////////////////////////////////////////////////////////////////

void System_Clock_Config() {

    RCC->CR |= (1 << 16);
    while(!(RCC->CR & (1 << 17)));

    FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10) | (2 << 0);

    RCC->PLLCFGR = 0;
    RCC->PLLCFGR |= (25 << 0);
    RCC->PLLCFGR |= (336 << 6);
    RCC->PLLCFGR |= (1 << 16);
    RCC->PLLCFGR |= (1 << 22);
    RCC->PLLCFGR |= (7 << 24);


    RCC->CR |= (1 << 24);
    while(!(RCC->CR & (1 << 25)));


    RCC->CFGR = 0;
    RCC->CFGR |= (3 << 30);
    RCC->CFGR |= (1 << 23);
    RCC->CFGR |= (3 << 21);
    RCC->CFGR |= (25 << 16);
    RCC->CFGR |= (4 << 10);


    RCC->CFGR |= (2 << 0);
    while(((RCC->CFGR >> 2) & 3) != 2);

}

//////////////////////////////////////////////////////////   Timer_Config    /////////////////////////////////////////////////////////////////////////////////////


void Timer_Config() {

	 // 1 ms

	RCC->APB1ENR |= (1 << 0);

	TIM2->CR1 =0;
	TIM2->CR1 |= (1 << 7);

	TIM2->DIER |= (1 << 0);

	TIM2->PSC = 83;
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


	RCC->AHB1ENR |= (1 << 0);
	RCC->APB2ENR |= (1 << 12);


	GPIOA->MODER &= ~(3 << (2*4));
	GPIOA->MODER |= (1 << (2*4));
	GPIOA->OTYPER &= ~(1 << 4);
	GPIOA->OSPEEDR |= (3 << (2*4));
	GPIOA->PUPDR &= ~(3 << (2*4));
	GPIOA->PUPDR |= (2 << (2*4));
	GPIOA->ODR |= (1 << 4);


	GPIOA->MODER &= ~(3 << (2*5) | (3 << (2*6)) | (3 << (2*7)));
	GPIOA->MODER |= (2 << (2*5)) | (2 << (2*6)) | (2 << (2*7));
	GPIOA->OTYPER &= ~((1 << 5) | (1 << 6) | (1 << 7));
	GPIOA->OSPEEDR |= (3 << (2*5)) | (3 << (2*6)) | (3 << (2*7));
	GPIOA->PUPDR &= ~(3 << (2*5) | (3 << (2*6)) | (3 << (2*7)));
	GPIOA->AFR[0] |= (5 << (4*5)) | (5 << (4*6)) | (5 << (4*7));

	SPI1->CR1 =0;
	SPI1->CR1 |= (1 << 9);
	SPI1->CR1 |= (1 << 8);
	SPI1->CR1 |= (3 << 3);
	SPI1->CR1 |= (1 << 2);

	SPI1->CR2 |= (1 << 1);
	SPI1->CR2 |= (1 << 0);

	SPI1->CR1 |= (1 << 6);

}

//////////////////////////////////////////////////////////  DMA_Config  //////////////////////////////////////////////////////////////////////////////////////////

void DMA_Config() {

	RCC->AHB1ENR |= (1 << 22);

 // RX

	DMA2_Stream0->CR &= ~(1 << 0);
	while (DMA2_Stream0->CR & (1 << 0));

	DMA2_Stream0->CR =0;
	DMA2_Stream0->CR |= (3 << 25);
	DMA2_Stream0->CR |= (2 << 16);
	DMA2_Stream0->CR |= (1 << 10);
	DMA2_Stream0->CR |= (1 << 4);

	DMA2_Stream0->FCR =0;

  // TX

	DMA2_Stream5->CR &=~(1 << 0);
	while(DMA2_Stream5->CR & (1 << 0));

	DMA2_Stream5->CR =0;
	DMA2_Stream5->CR |= (3 << 25);
	DMA2_Stream5->CR |= (3 << 16);
	DMA2_Stream5->CR |= (1 << 10);
	DMA2_Stream5->CR |= (1 << 6);
	DMA2_Stream5->CR |=(1 << 4);

	DMA2_Stream5->FCR =0;

}


/////////////////////////////////////////////////////////  SPI_Write   //////////////////////////////////////////////////////////////////////////////////////////

void SPI_Write(uint8_t reg_adress, uint8_t veri) {

	static uint8_t spi_tx_buf[2];
	static uint8_t spi_rx_buf[2];

	GPIOA->ODR &= ~(1 << 4);

	spi_tx_buf[0] = reg_adress | 0x80;
	spi_tx_buf[1] = veri;

	DMA2->HIFCR = (61 << 6);
	DMA2->LIFCR = (61 << 0);

	DMA2_Stream5->CR &= ~(1 << 0);
	while (DMA2_Stream5->CR & (1 << 0));

	DMA2_Stream0->CR &= ~(1 << 0);
	while (DMA2_Stream0->CR & (1 << 0));

	DMA2_Stream5->NDTR = 2;
	DMA2_Stream5->PAR  = (uint32_t)&SPI1->DR;
	DMA2_Stream5->M0AR = (uint32_t)spi_tx_buf;

	DMA2_Stream0->NDTR = 2;
	DMA2_Stream0->PAR  = (uint32_t)&SPI1->DR;
	DMA2_Stream0->M0AR = (uint32_t)spi_rx_buf;

	DMA2_Stream0->CR |= (1 << 0);
	DMA2_Stream5->CR |= (1 << 0);

	while (!(DMA2->LISR & (1 << 5)));
	while (!(DMA2->HISR & (1 << 11)));
	while (SPI1->SR & (1 << 7));

	GPIOA->ODR |= (1 << 4);

}

//////////////////////////////////////////////////////////   SPI_Read   /////////////////////////////////////////////////////////////////////////////////////

uint8_t SPI_Read(uint8_t reg_adress) {

	static uint8_t spi_tx_buf[2];
	static uint8_t spi_rx_buf[2];

	GPIOA->ODR &= ~(1 << 4);

	spi_tx_buf[0] = reg_adress | 0x00;
	spi_tx_buf[1] = 0xFF;

	DMA2->HIFCR = (61 << 6);
	DMA2->LIFCR = (61 << 0);

	DMA2_Stream5->CR &= ~(1 << 0);
	while (DMA2_Stream5->CR & (1 << 0));

	DMA2_Stream0->CR &= ~(1 << 0);
	while (DMA2_Stream0->CR & (1 << 0));

	DMA2_Stream5->NDTR = 2;
	DMA2_Stream5->PAR  = (uint32_t)&SPI1->DR;
	DMA2_Stream5->M0AR = (uint32_t)spi_tx_buf;

	DMA2_Stream0->NDTR = 2;
	DMA2_Stream0->PAR  = (uint32_t)&SPI1->DR;
	DMA2_Stream0->M0AR = (uint32_t)spi_rx_buf;

	DMA2_Stream0->CR |= (1 << 0);
	DMA2_Stream5->CR |= (1 << 0);

	while (!(DMA2->LISR & (1 << 5)));
	while (!(DMA2->HISR & (1 << 11)));
	while (SPI1->SR & (1 << 7));

	GPIOA->ODR |= (1 << 4);

	return spi_rx_buf[1];
}

//////////////////////////////////////////////////////////  ADC_Config  /////////////////////////////////////////////////////////////////////////////////////



void ADC_Config() {

	RCC->AHB1ENR |= (1 << 0);
	RCC->APB2ENR |= (1 << 8);

	GPIOA->MODER |= (3 << (0*2));
	GPIOA->OTYPER &= ~(1 << 0);
	GPIOA->OSPEEDR |= (3 << (0*2));
	GPIOA->PUPDR &= ~(3 << (0*2));

	ADC1->CR1 =0;
	ADC1->CR1 |= (1 << 11);

	ADC1->CR2 =0;
	ADC1->CR2 |= (1 << 10);

	ADC1->SMPR2 =0;
	ADC1->SMPR2 |= (7 << 0);

	ADC1->SQR1 &= ~(15 << 20);
	ADC1->SQR3 &= ~(31 << 0);

	ADC->CCR = 0;

	ADC1->CR2 |= (1 << 0);



}


//////////////////////////////////////////////////////////  ADC_Read_Value  /////////////////////////////////////////////////////////////////////////////////////


void ADC_Read_Value(float *percent_value) {

	uint8_t x=0;
	float value=0;

	while(x < 10) {

while(!delay_ms(10));
delay_stop();

	 ADC1->CR2 |= (1 << 30);

	 while(!(ADC1->SR & (1 << 1))) {

		 if (delay_ms(1000) == 1) {

				ADC1->SR &= ~(1 << 1);

				return;
		 // ADC HATA

		}}

	 delay_stop();
		ADC1->SR &= ~(1 << 1);


	 if (adc_air_volt-adc_water_volt != 0) {

	 adc_raw_volt=((ADC1->DR)*3.3f) / 4095;
	 value+=(((adc_air_volt-adc_raw_volt)/(adc_air_volt-adc_water_volt))*100)/10;

	 x++;

	 }

	 else {

		 // tanımsızlık hatası
		 return;

	 }

}

*percent_value=value;

}

//////////////////////////////////////////////////////////  ADC_Pin_Calibration_Conf  /////////////////////////////////////////////////////////////////////////////////////

void ADC_Pin_Calibration_Conf() {

	RCC->AHB1ENR |= (1 << 0);


	 // PA1-2 INPUT

	GPIOA->MODER &= ~(3 << (1*2));
	GPIOA->MODER &= ~(3 << (2*2));

	GPIOA->OTYPER &= ~(1 << 1);
	GPIOA->OTYPER &= ~(1 << 2);

	GPIOA->OSPEEDR |= (3 << (1*2));
	GPIOA->OSPEEDR |= (3 << (2*2));

	GPIOA->PUPDR &= ~(3 << (1*2));
	GPIOA->PUPDR &= ~(3 << (2*2));

	GPIOA->PUPDR |= (1 << (1*2));
	GPIOA->PUPDR |= (1 << (2*2));


	// PA 10-11 LED OUTPUT

	GPIOA->MODER &= ~(3 << (10*2));
	GPIOA->MODER &= ~(3 << (11*2));

	GPIOA->MODER |= (1 << (10*2));
	GPIOA->MODER |= (1 << (11*2));

	GPIOA->OTYPER &= ~(1 << 10);
	GPIOA->OTYPER &= ~(1 << 11);

	GPIOA->OSPEEDR |= (3 << (10*2));
	GPIOA->OSPEEDR |= (3 << (11*2));

	GPIOA->PUPDR &= ~(3 << (10*2));
	GPIOA->PUPDR &= ~(3 << (11*2));

	GPIOA->PUPDR |= (2 << (10*2));
	GPIOA->PUPDR |= (2 << (11*2));

	GPIOA->ODR &= ~(1 << 10);
	GPIOA->ODR &= ~(1 << 11);




}
//////////////////////////////////////////////////////////  ADC_Air_Calibration  /////////////////////////////////////////////////////////////////////////////////////


void ADC_Air_Calibration() {


	if (!(GPIOA->IDR & (1 << 1))) {
		while(!(delay_ms(300)));
		delay_stop();
		GPIOA->ODR |= (1 << 10);
		while(!(delay_ms(500)));
				delay_stop();



		uint8_t x=0;
		float value=0;



		while(x < 10) {

			while(!delay_ms(10));
			delay_stop();

		ADC1->CR2 |= (1 << 30);


			 while(!(ADC1->SR & (1 << 1))) {

				 if (delay_ms(1000) == 1) {
						delay_stop();

						GPIOA->ODR &= ~(1 << 10);
						while(!(delay_ms(500)));
						delay_stop();
						GPIOA->ODR |= (1 << 10);
						while(!(delay_ms(500)));
						delay_stop();
						GPIOA->ODR &= ~(1 << 10);
						while(!(delay_ms(500)));
						delay_stop();
						GPIOA->ODR |= (1 << 10);
						while(!(delay_ms(500)));
						delay_stop();
						GPIOA->ODR &= ~(1 << 10);
						while(!(delay_ms(500)));
						delay_stop();



				 // ADC HATA
				 return;

				}}

				ADC1->SR &= ~(1 << 1);

			 value+=(((ADC1->DR)*3.3f) / 4095)/10;
			 x++;


		}

		adc_air_volt=value;

			GPIOA->ODR &= ~(1 << 10);


	}}




//////////////////////////////////////////////////////////  ADC_Water_Calibration  /////////////////////////////////////////////////////////////////////////////////////


void ADC_Water_Calibration() {

	if (!(GPIOA->IDR & (1 << 2)))
	{
		while(!(delay_ms(300)));
			delay_stop();

			GPIOA->ODR |= (1 << 11);
			while(!(delay_ms(500)));
					delay_stop();

			uint8_t x=0;
			float value=0;


while(x < 10) {

				ADC1->CR2 |= (1 << 30);

					 while(!(ADC1->SR & (1 << 1))) {

						 if (delay_ms(3000) == 1) {
								delay_stop();

								GPIOA->ODR &= ~(1 << 10);
														while(!(delay_ms(300)));
														delay_stop();
														GPIOA->ODR |= (1 << 11);
														while(!(delay_ms(300)));
														delay_stop();
														GPIOA->ODR &= ~(1 << 11);
														while(!(delay_ms(300)));
														delay_stop();
														GPIOA->ODR |= (1 << 11);
														while(!(delay_ms(300)));
														delay_stop();
														GPIOA->ODR &= ~(1 << 11);
														while(!(delay_ms(300)));
														delay_stop();


						 // ADC HATA
						 return;

						}}
					 ADC1->SR &= ~(1 << 1);



			value+=(((ADC1->DR)*3.3f) / 4095)/10;

			x++;
}

           adc_water_volt=value;


				GPIOA->ODR &= ~(1 << 11);



	}


}


//////////////////////////////////////////////////////////  TIM3_Config  /////////////////////////////////////////////////////////////////////////////////////


void TIM3_Config(void) {

	RCC->APB1ENR |= (1 << 1);

	TIM3->CR1 =0;

    TIM3->CR1 = (1 << 7);

    TIM3->SMCR=0;

    TIM3->CNT=0;

    TIM3->PSC = 83;

    TIM3->ARR = 65535;

    TIM3->EGR |= (1 << 0);

    TIM3->CR1 |= (1 << 0);

}


void delay_us(uint16_t us) {

    TIM3->CNT = 0;
    while (TIM3->CNT < us);
}


//////////////////////////////////////////////////////////  1-Wire Pin  /////////////////////////////////////////////////////////////////////////////////////

void Wire_1_Pin_Conf() {

	RCC->AHB1ENR |= (1 << 0);


	GPIOA->MODER &= ~(3 << (8*2));
	GPIOA->MODER |= (1 << (8*2));

	GPIOA->OTYPER |= (1 << 8);

	GPIOA->OSPEEDR |= (3 << (8*2));

	GPIOA->PUPDR &=  ~(3 << (8*2));

	GPIOA->ODR |= (1 << 8);

}


////////////////////////////////////////////////////////// Wire_High  /////////////////////////////////////////////////////////////////////////////////////


void Wire_High() {

	GPIOA->ODR |= (1 << 8);

}

////////////////////////////////////////////////////////// Wire_Low  /////////////////////////////////////////////////////////////////////////////////////


void Wire_Low() {

	GPIOA->ODR &= ~(1 << 8);

}

////////////////////////////////////////////////////////// Line_Read  /////////////////////////////////////////////////////////////////////////////////////

uint8_t Line_Read() {

	if (GPIOA->IDR & (1 << 8)) {

		return 1;

	}

	else {

		return 0;

	}}



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






