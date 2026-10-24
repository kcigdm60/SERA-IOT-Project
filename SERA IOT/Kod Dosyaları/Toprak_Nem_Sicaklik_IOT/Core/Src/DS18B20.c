/*
 * DS18B20.c
 *
 *  Created on: Jul 2, 2026
 *      Author: kcigd
 */


#include "DS18B20.h"


uint8_t DS_Read_Bit(void) {
    uint8_t bit_value;

    Wire_Low();
    delay_us(2);        
    Wire_High();
    delay_us(10);       

    bit_value = DS_Line_Read();

    delay_us(50);        

    return bit_value;
}





void DS_Logic_Low() {

	Wire_Low();
	delay_us(60);
	Wire_High();
	delay_us(2);

}


void DS_Logic_High() {

	Wire_Low();
	delay_us(10);

	Wire_High();
	delay_us(60);

}




uint8_t DS_Line_Read() {

	return Line_Read();

}



uint8_t DS_Reset_Presence() {

	Wire_Low();
	delay_us(500);
	Wire_High();
	delay_us(60);


	if (!(Line_Read())) {

		delay_us(280);



		return 1;

	}

	else {

		delay_us(5);

		return 0;


	}


}



void DS_Data_Send(uint8_t data) {


for (uint8_t x=0; x < 8; x++) {


	if (data & (1 << x)) {

		DS_Logic_High();

	}

	else {

		DS_Logic_Low();

	}

}}



uint8_t DS_Data_Read_8bit() {

	uint8_t value = 0;

	for (uint8_t x = 0; x < 8; x++) {

		if (DS_Read_Bit()) {
			value |= (1 << x);
		}

	}

	return value;
}



uint16_t DS_Data_Read_16bit() {

	uint16_t value = 0;

	for (uint8_t x = 0; x < 16; x++) {

		if (DS_Read_Bit()) {
			value |= (1 << x);
		}

	}

	return value;
}



float DS_Temperature_Read() {

	uint8_t temp_lsb=0;
	uint8_t temp_msb=0;

	uint16_t full_temp=0;


	// Sıcaklık dönüşümünü başlat
	if (DS_Reset_Presence()) {

	DS_Data_Send(DS18B20_CMD_SKIP_ROM);
	DS_Data_Send(DS18B20_CMD_CONVERT_T);

	// 750 ms lik bekleme

	while(!(delay_ms(750)));
	delay_stop();


	//  Scratchpad belleği oku
	if (DS_Reset_Presence()) {
	DS_Data_Send(DS18B20_CMD_SKIP_ROM);
	DS_Data_Send(DS18B20_CMD_READ_SCRATCHPAD);


	 temp_lsb = DS_Data_Read_8bit();
	 temp_msb = DS_Data_Read_8bit();


	 full_temp = (int16_t)((temp_msb << 8) | temp_lsb);


	 GPIOA->ODR &= ~(1 << 11);


	 return (float)full_temp*0.0625f;


	}




	else {


GPIOA->ODR |= (1 << 11);

		return 0;


	}




	}


	else {

		GPIOA->ODR |= (1 << 11);


		return 0;
	}

}





