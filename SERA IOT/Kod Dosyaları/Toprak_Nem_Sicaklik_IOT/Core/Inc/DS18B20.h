/*
 * DS18B20.h
 *
 *  Created on: Jul 2, 2026
 *      Author: kcigd
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "main.h"



// --- ROM Komutları ---

#define DS18B20_CMD_SKIP_ROM          0xCC  // Benzersiz ID aramayı atla (Tek sensör için)
#define DS18B20_CMD_MATCH_ROM         0x55  // Belirli bir benzersiz ID'ye sahip sensörü seç
#define DS18B20_CMD_SEARCH_ROM        0xF0  // Hatta bağlı tüm cihazların ID'lerini ara
#define DS18B20_CMD_READ_ROM          0x33  // Cihazın 64-bitlik benzersiz ID'sini oku

// --- Fonksiyon (Scratchpad) Komutları ---

#define DS18B20_CMD_CONVERT_T         0x44  // Sıcaklık dönüşümünü/ölçümünü başlat
#define DS18B20_CMD_READ_SCRATCHPAD   0xBE  // Sensör hafızasını (Sıcaklık dahil) okuma modu
#define DS18B20_CMD_WRITE_SCRATCHPAD  0x4E  // Alarm ve Çözünürlük sınırlarını yaz
#define DS18B20_CMD_COPY_SCRATCHPAD   0x48  // Scratchpad verilerini EEPROM'a (kalıcı hafıza) kopyala

// --- Sıcaklık Çözünürlük Maskeleri (İsteğe Bağlı) ---

#define DS18B20_RES_9BIT              0x1F  // 9-bit çözünürlük (Dönüşüm süresi: 93.75 ms)
#define DS18B20_RES_10BIT             0x3F  // 10-bit çözünürlük (Dönüşüm süresi: 187.5 ms)
#define DS18B20_RES_11BIT             0x5F  // 11-bit çözünürlük (Dönüşüm süresi: 375 ms)
#define DS18B20_RES_12BIT             0x7F  // 12-bit çözünürlük (Varsayılan - Dönüşüm süresi: 750 ms)







extern uint8_t DS_ERROR;


extern uint8_t delay_ms(uint16_t ms_value);
extern void delay_stop();

void delay_us(uint16_t us);
void us_delay_stop();
void block_us_delay(uint16_t us_valuee);



extern void Wire_High();
extern void Wire_Low();

extern uint8_t Line_Read();



void DS_Delay_Us(uint16_t micro_s);

void DS_Logic_Low(void);
void DS_Logic_High(void);

uint8_t DS_Line_Read(void);

uint8_t DS_Reset_Presence();

void DS_Data_Send(uint8_t data);

uint8_t DS_Data_Read_8bit(void);
uint16_t DS_Data_Read_16bit(void);

float DS_Temperature_Read(void);

uint8_t DS_Read_Bit(void);














#endif /* INC_DS18B20_H_ */
