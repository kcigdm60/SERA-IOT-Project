/*
 * gateway_main_conf_func.c
 *
 *  Created on: Jun 26, 2026
 *      Author: kcigd
 */

#include <gateway_main_conf_func.h>

//////// Counter Variable ////////

volatile uint16_t ms_counter = 0;
volatile uint16_t start_time = 0;
volatile uint8_t delay_counter = 0;

////////// USART Variable ////////
uint16_t coming_data_len=0;
uint16_t usart_index=0;

///// USART Status Variable //////

uint8_t usart_status = 0;


char Usart_Rx[1024];

// Ekran değişkenleri
uint8_t Lora_Value_Show_Counter=0;
uint8_t Lora_Main_Count=0;


uint8_t watchdog_key=1;

uint8_t tim3_counter=0;


////////////////////////////////////////////////////// IRQHandler ////////////////////////////////////////////////////////////////////////

void TIM2_IRQHandler(void) {
    if (TIM2->SR & (1 << 0)) {
        ms_counter++;
        TIM2->SR &= ~(1 << 0);
    }
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & (1 << 0)) {

    	tim3_counter++;


    	if (tim3_counter >= 255) {

    		tim3_counter=0;
    	}

        TIM3->SR &= ~(1 << 0);}}



void DMA1_Stream6_IRQHandler(void) {
    if (DMA1->HISR & (1 << 21)) {
        Usart_Flag_Marking(TRANMISSION_COMPLETE);
        DMA1->HIFCR |= (1 << 21);
    }
}


void USART2_IRQHandler(void) {
    if (USART2->SR & (1 << 4)) {
        volatile uint32_t dummy = USART2->SR;
        dummy = USART2->DR;
        (void)dummy;


        Usart_Message_Len_Index(&usart_index, &coming_data_len);
        Usart_Flag_Marking(IDLE);
    }
}


//////////////////////////////////////////////////////// System_Clock_Config //////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////// Timer_Config /////////////////////////////////////////////////////////////////////////////////////

void Timer_Config(void) {

    RCC->APB1ENR |= (1 << 0);

    TIM2->CR1 = 0;
    TIM2->CR1 |= (1 << 7);
    TIM2->DIER |= (1 << 0);

    TIM2->PSC = 83;
    TIM2->ARR = 999;
    TIM2->CNT = 0;

    TIM2->EGR |= (1 << 0);

    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);

}
////////////////////////////////////////////////////////// delay_ms //////////////////////////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////// SPI_Config ///////////////////////////////////////////////////////////////////////////


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

    GPIOA->MODER &= ~((3 << (2*5)) | (3 << (2*6)) | (3 << (2*7)));
    GPIOA->MODER |= (2 << (2*5)) | (2 << (2*6)) | (2 << (2*7));
    GPIOA->OTYPER &= ~((1 << 5) | (1 << 6) | (1 << 7));
    GPIOA->OSPEEDR |= (3 << (2*5)) | (3 << (2*6)) | (3 << (2*7));
    GPIOA->PUPDR &= ~((3 << (2*5)) | (3 << (2*6)) | (3 << (2*7)));
    GPIOA->AFR[0] |= (5 << (4*5)) | (5 << (4*6)) | (5 << (4*7));

    SPI1->CR1 = 0;
    SPI1->CR1 |= (1 << 9);
    SPI1->CR1 |= (1 << 8);
    SPI1->CR1 |= (3 << 3);
    SPI1->CR1 |= (1 << 2);

    SPI1->CR2 |= (1 << 1);
    SPI1->CR2 |= (1 << 0);

    SPI1->CR1 |= (1 << 6);
}

////////////////////////////////////////////////////////// DMA_Config /////////////////////////////////////////////////////////////////////////////////

void DMA_Config() {
    RCC->AHB1ENR |= (1 << 22);

    // RX
    DMA2_Stream0->CR &= ~(1 << 0);
    while (DMA2_Stream0->CR & (1 << 0));

    DMA2_Stream0->CR = 0;
    DMA2_Stream0->CR |= (3 << 25);
    DMA2_Stream0->CR |= (2 << 16);
    DMA2_Stream0->CR |= (1 << 10);
    DMA2_Stream0->CR |= (1 << 4);
    DMA2_Stream0->FCR = 0;

    // TX
    DMA2_Stream5->CR &= ~(1 << 0);
    while(DMA2_Stream5->CR & (1 << 0));

    DMA2_Stream5->CR = 0;
    DMA2_Stream5->CR |= (3 << 25);
    DMA2_Stream5->CR |= (3 << 16);
    DMA2_Stream5->CR |= (1 << 10);
    DMA2_Stream5->CR |= (1 << 6);
    DMA2_Stream5->CR |= (1 << 4);
    DMA2_Stream5->FCR = 0;
}


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

/////////////////////////////////////////////////////////// USART_2_Config ///////////////////////////////////////////////////////////////////

void USART_2_Config() {
    RCC->APB1ENR |= (1 << 17);
    RCC->AHB1ENR |= (1 << 0);

    GPIOA->MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2)));
    GPIOA->MODER |=  ((2 << (2 * 2)) | (2 << (3 * 2)));

    GPIOA->OSPEEDR &= ~((3 << (2 * 2)) | (3 << (3 * 2)));
    GPIOA->OSPEEDR |=  ((2 << (2 * 2)) | (2 << (3 * 2)));

    GPIOA->PUPDR &= ~((3 << (2 * 2)) | (3 << (3 * 2)));
    GPIOA->PUPDR |=  ((1 << (2 * 2)) | (1 << (3 * 2)));

    GPIOA->AFR[0] &= ~((15 << (2 * 4)) | (15 << (3 * 4)));
    GPIOA->AFR[0] |=  ((7  << (2 * 4)) | (7  << (3 * 4)));

    USART2->CR1 = 0;
    USART2->CR1 |= (1 << 15);
    USART2->CR1 |= (1 << 4); // IDLE Interrupt

    USART2->CR2 = 0;
    USART2->CR3 = 0;
    USART2->CR3 |= (1 << 7); // DMAT
    USART2->CR3 |= (1 << 6); // DMAR
    USART2->GTPR = 0;

    USART2->BRR = (546 << 4) | 14;

    USART2->CR1 |= (1 << 2);  // RE
    USART2->CR1 |= (1 << 3);  // TE
    USART2->CR1 |= (1 << 13); // UE

    USART2->SR |= (1 << 4);

    NVIC_SetPriority(USART2_IRQn, 1);
    NVIC_EnableIRQ(USART2_IRQn);

    Usart_Flag_Marking(TRANMISSION_COMPLETE);
}

////////////////////////////////////////////// DMA1_USART2_Config ////////////////////////////////////////////////////////////////////////////

void DMA1_USART2_Config() {
    RCC->AHB1ENR |= (1 << 21);

    // STREAM 5 RX
    DMA1_Stream5->CR = 0;
    DMA1_Stream5->CR |= (4 << 25);
    DMA1_Stream5->CR |= (3 << 16);
    DMA1_Stream5->CR |= (1 << 10);
    DMA1_Stream5->CR |= (1 << 8);
    DMA1_Stream5->FCR = 0;

    DMA1_Stream5->NDTR = 1024;
    DMA1_Stream5->M0AR = (uint32_t)Usart_Rx;
    DMA1_Stream5->PAR  = (uint32_t)&USART2->DR;

    DMA1_Stream5->CR |= (1 << 0);

    // STREAM 6 TX
    DMA1_Stream6->CR = 0;
    DMA1_Stream6->CR |= (4 << 25);
    DMA1_Stream6->CR |= (2 << 16);
    DMA1_Stream6->CR |= (1 << 10);
    DMA1_Stream6->CR |= (1 << 6);
    DMA1_Stream6->CR |= (1 << 4);

    DMA1_Stream6->FCR = 0;

    NVIC_SetPriority(DMA1_Stream6_IRQn, 2);
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

//////////////////////////////////////// str_len /////////////////////////////////////////////////////////////////////////////////////////////

uint16_t str_len(const char *string) {
    uint16_t x = 0;
    while (string[x] != '\0')
        x++;
    return x;
}

///////////////////////////////////// USART_Send /////////////////////////////////////////////////////////////////////////////////////////////

uint8_t USART_Send(char *at_commond) {

    while(!Usart_Flag_Read(TRANMISSION_COMPLETE));

    Usart_Flag_Clean(TRANMISSION_COMPLETE);
    Usart_Flag_Clean(IDLE);

    DMA1_Stream6->CR &= ~(1 << 0);
    while (DMA1_Stream6->CR & (1 << 0));

    DMA1_Stream6->NDTR = str_len(at_commond);
    DMA1_Stream6->M0AR = (uint32_t)at_commond;
    DMA1_Stream6->PAR  = (uint32_t)&USART2->DR;
    DMA1_Stream6->CR |= (1 << 0);

    return 1;
}

/////////////////////////////////// Usart_Message_Len_Index //////////////////////////////////////////////////////////////////////////////////

void Usart_Message_Len_Index(uint16_t *index, uint16_t *coming_data) {

    static uint16_t old_NDTR_value = 1024;
    uint16_t current_NDTR = DMA1_Stream5->NDTR;

    // Yeni verinin başladığı ilk indeksi hesaplar
    *index = (1024 - old_NDTR_value) % 1024;

    // Gelen veri miktarını hesaplar
    if (old_NDTR_value >= current_NDTR) {
        *coming_data = old_NDTR_value - current_NDTR;
    } else {
        *coming_data = old_NDTR_value + (1024 - current_NDTR);
    }

    old_NDTR_value = current_NDTR;
}

/////////////////////////////////// Usart_Reader //////////////////////////////////////////////////////////////////////////////////

// delay fonkla sıkıntı çıakrdığı için değiştirdik

uint8_t Usart_Reader(char *desired_value) {
    uint16_t value_len = 0;
    uint16_t i = 0, j = 0;
    uint8_t match = 0;

    volatile uint32_t wait_counter = 0;
    const uint32_t wait_limit = 200000UL;

    while (!Usart_Flag_Read(IDLE)) {
        wait_counter++;
        if (wait_counter >= wait_limit) {
            return 0;
        }
    }

    value_len = str_len(desired_value);

    if (value_len == 0 || value_len > 1024) {
        return 0;
    }

    for (i = 0; i < 1024; i++) {
        match = 1;

        for (j = 0; j < value_len; j++) {
            uint16_t current_pos = (usart_index + i + j) % 1024;

            if (Usart_Rx[current_pos] != desired_value[j]) {
                match = 0;
                break;
            }
        }

        if (match == 1) {
            return 1;
        }
    }

    return 0;
}

///////////////////////////////////////////// Usart_Flag Operations ////////////////////////////////////////////////////////////////////////

void Usart_Flag_Marking(Usart_Status_t Flag) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    usart_status |= (1 << Flag);
    __set_PRIMASK(primask);
}

void Usart_Flag_Clean(Usart_Status_t flag) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    usart_status &= ~(1 << flag);
    __set_PRIMASK(primask);
}


uint8_t Usart_Flag_Read(Usart_Status_t flag) {
    if (usart_status & (1 << flag)) {
        return 1;
    }
    return 0;
}

///////////////////////////////////////////// I2C1_Config ////////////////////////////////////////////////////////////////////////

void I2C1_Config() {
    RCC->APB1ENR |= (1 << 21);
    RCC->AHB1ENR |= (1 << 1);

    GPIOB->MODER &= ~((3 << (6*2)) | (3 << 7*2));
    GPIOB->MODER |= (2 << (6*2)) | (2 << (7*2));

    GPIOB->OSPEEDR |= (3 << (6*2)) | (3 << (7*2));

    GPIOB->OTYPER |= (1 << 6) | (1 << 7);
    GPIOB->PUPDR &= ~((3 << (6*2)) | (3 << (7*2)));
    GPIOB->PUPDR |=  ((1 << (6*2)) | (1 << (7*2)));

    GPIOB->AFR[0] &= ~((15 << (6*4)) | (15 << (7*4)));
    GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4));

    I2C1->CR1 = 0;
    I2C1->CR2 = 0;
    I2C1->TRISE = 0;
    I2C1->CCR = 0;
    I2C1->FLTR = 0;

    I2C1->CR2 |= (42 << 0);
    I2C1->CCR = 210;
    I2C1->TRISE = 43;

    I2C1->FLTR |= (2 << 0);
    I2C1->CR1 |= (1 << 0);
}




void I2C1_Write(uint8_t control_byte, uint8_t *data_array, uint8_t data_size) {
    uint32_t timeout_val = 500000;
    uint32_t timeout;

    I2C1->CR1 |= (1 << 8);
    timeout = timeout_val;
    while (!(I2C1->SR1 & (1 << 0))) {
        if (--timeout == 0) {
            I2C_Reset();
            return;
        }
    }


    I2C1->DR = (OLED_SLAVE_ADDR << 1);
    timeout = timeout_val;
    while (!(I2C1->SR1 & (1 << 1))) {
        if (--timeout == 0) {
            I2C_Reset();
            return;
        }
    }

    (void)I2C1->SR1;
    (void)I2C1->SR2;

    timeout = timeout_val;
    while (!(I2C1->SR1 & (1 << 7))) {
        if (--timeout == 0) {
            I2C_Reset();
            return;
        }
    }
    I2C1->DR = control_byte;

    for (uint8_t x = 0; x < data_size; x++) {
        timeout = timeout_val;
        while (!(I2C1->SR1 & (1 << 7))) {
            if (--timeout == 0) {
                I2C_Reset();
                return;
            }
        }
        I2C1->DR = data_array[x];
    }

    timeout = timeout_val;
    while (!(I2C1->SR1 & (1 << 2))) {
        if (--timeout == 0) {
            I2C_Reset();
            return;
        }
    }

    I2C1->CR1 |= (1 << 9);
}



void I2C_Reset(void) {
    I2C1->CR1 |= (1 << 9);

    I2C1->CR1 |= (1 << 15);
    for (volatile int i = 0; i < 1000; i++);
    I2C1->CR1 &= ~(1 << 15);
    for (volatile int i = 0; i < 1000; i++);


    I2C1->CR2 = 42;
    I2C1->CCR = 210;
    I2C1->TRISE = 43;


    I2C1->CR1 |= (1 << 0);
}

void TIM3_Config() {
    RCC->APB1ENR |= (1 << 1);

    TIM3->CNT = 0;
    TIM3->PSC = 8399;
    TIM3->ARR = 29999;

    TIM3->DIER |= (1 << 0);

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 3);

    TIM3->CR1 |= (1 << 0);
}



void TIM3_Reset() {
    TIM3->CNT = 0;
    tim3_counter=0;
}




void WatchDog_Config() {
    if (watchdog_key == 1) {

        IWDG->KR = 0xCCCC;

        IWDG->KR = 0x5555;

        IWDG->PR = 0x06;
        IWDG->RLR = 3750;


        while (IWDG->SR != 0);


        IWDG->KR = 0xAAAA;
    }
}


void WatchDog_Feed() {
	if (watchdog_key == 1) {
	    IWDG->KR = 0xAAAA;
	}
}


