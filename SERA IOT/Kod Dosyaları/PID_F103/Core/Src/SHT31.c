/*
 * SHT31.c
 *
 *  Created on: Jul 13, 2026
 *      Author: kcigd
 */

#include "SHT31.h"


uint8_t SHT_Coming_Data[6]={0};
uint16_t SHT_Status_Guide=0;

float SHT_Temperature=0;
float SHT_Moisture=0;



void SHT_Send_Command(uint16_t Command) {

	uint8_t cmd_buffer[2]={0};

    cmd_buffer[0] = (uint8_t)(Command >> 8);
	cmd_buffer[1] = (uint8_t)(Command & 0xFF);

	I2C1_Single_Write(SHT_I2C_ADDR,cmd_buffer[0],cmd_buffer[1]);

}


void SHT_Take_Data(uint16_t Command,uint8_t* SHT_Coming_Data,uint8_t data_len) {

	uint8_t cmd_buffer[2]={0};

	for (uint8_t x=0; x < data_len; x++) {

		SHT_Coming_Data[x]=0;

	}

	    cmd_buffer[0] = (uint8_t)(Command >> 8);
		cmd_buffer[1] = (uint8_t)(Command & 0xFF);


	I2C1_Read(SHT_I2C_ADDR,cmd_buffer[0],cmd_buffer[1],SHT_Coming_Data,data_len);

}


void SHT_Reset(void) {
	SHT_Send_Command(SHT_CMD_SOFT_RESET);
}

void SHT_Heater_ON(void) {
	SHT_Send_Command(SHT_CMD_HEATER_ON);
}

void SHT_Heater_OFF(void) {
	SHT_Send_Command(SHT_CMD_HEATER_OFF);
}

void SHT_Clear_Status(void) {
	SHT_Send_Command(SHT_CMD_CLEAR_STATUS);
}

void SHT_Periodic_Mode_OFF(void)
{
	SHT_Send_Command(SHT_CMD_BREAK);
}

void SHT_Periodic_Read(){

	SHT_Send_Command(SHT_CMD_FETCH_DATA);

}

void SHT_CRC_Mark(SHT_Status_Bit_t error) {

	SHT_Status_Guide |= (1 << error);

}


void SHT_CRC_Clear(SHT_Status_Bit_t error) {

	SHT_Status_Guide &= ~(1 << error);

}


uint8_t SHT_CRC_Read(SHT_Status_Bit_t error) {


	if (SHT_Status_Guide & (1 << error)) {

		return 1;
	}

	else {

		return 0;

	}}





void SHT_Status(void) {

	uint8_t data[3]={0};

	SHT_Take_Data(SHT_CMD_READ_STATUS, data,3);

	SHT_Status_Guide |= (data[0] << 8);
	SHT_Status_Guide |= (data[1]);

	

}




uint8_t SHT_Status_Read(SHT_Status_Bit_t Status) {

	SHT_Status();

	if (SHT_Status_Guide & (1 << Status)) {

		return 1;
	}


	else {

		return 0;
	}

}





void SHT_Periodic_Start(void) {


    SHT_Send_Command(SHT_CMD_PERI_10HZ_HIGH);

    while(!(delay_ms(20)));
    delay_stop();

}



void SHT_Temp_Moi_Ref(void) {

    uint16_t raw_temp;
    uint16_t raw_moi;

    // Periyodik moddaki son ölçümü çek
    SHT_Take_Data(SHT_CMD_FETCH_DATA, SHT_Coming_Data, 6);

    while(!(delay_ms(250)));
    delay_stop();


    /// Ham verileri birleştir
    raw_temp = (SHT_Coming_Data[0] << 8) | SHT_Coming_Data[1];
    raw_moi  = (SHT_Coming_Data[3] << 8) | SHT_Coming_Data[4];

    // Ham veriyi fiziksel değere çevir
    node3_pid_current_temp = (175.0f * ((float)raw_temp / 65535.0f)) - 45.0f;
    node3_pid_Moisture    = (100.0f * ((float)raw_moi  / 65535.0f));
}
