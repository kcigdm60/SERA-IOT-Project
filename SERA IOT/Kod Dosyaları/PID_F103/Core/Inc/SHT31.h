/*
 * SHT31.h
 *
 *  Created on: Jul 13, 2026
 *      Author: kcigd
 */

#ifndef INC_SHT31_H_
#define INC_SHT31_H_


#include "main.h"


extern void I2C1_Multi_Write(uint8_t slave_addr,uint8_t control_byte,uint8_t* data_array,uint8_t data_size);
extern void I2C1_Single_Write(uint8_t slave_addr, uint8_t control_byte, uint8_t data);
void I2C1_Read(uint8_t slave_addr, uint8_t register_addr,uint8_t data, uint8_t* data_array, uint8_t data_size);
extern void delay_stop(void);
extern uint8_t delay_ms(uint16_t ms_value);

extern uint8_t SHT_Coming_Data[6];


extern float node3_pid_current_temp;
extern float node3_pid_Moisture;



#define SHT_I2C_ADDR        0x44


/* ========================================================================== */
/*                   2. TEK SEFERLIK (SINGLE-SHOT) KOMUTLAR                   */
/* ========================================================================== */

// A. Clock Stretching Etkin (Saat Kilitlemeli)
#define SHT_CMD_CS_EN_HIGH_REP    0x2C06
#define SHT_CMD_CS_EN_MED_REP     0x2C0D
#define SHT_CMD_CS_EN_LOW_REP     0x2C10

// B. Clock Stretching Kapalı (Saat Serbest)
#define SHT_CMD_CS_DIS_HIGH_REP   0x2400
#define SHT_CMD_CS_DIS_MED_REP    0x240B
#define SHT_CMD_CS_DIS_LOW_REP    0x2416

/* ========================================================================== */
/*                     3. PERIYODIK OLCUM MODLARI & VERI                      */
/* ========================================================================== */

// A. Saniyede 0.5 Ölçüm (0.5 Hz)
#define SHT_CMD_PERI_0_5HZ_HIGH   0x2032
#define SHT_CMD_PERI_0_5HZ_MED    0x2024
#define SHT_CMD_PERI_0_5HZ_LOW    0x202F

// B. Saniyede 1 Ölçüm (1 Hz)
#define SHT_CMD_PERI_1HZ_HIGH     0x2130
#define SHT_CMD_PERI_1HZ_MED      0x2126
#define SHT_CMD_PERI_1HZ_LOW      0x212D

// C. Saniyede 2 Ölçüm (2 Hz)
#define SHT_CMD_PERI_2HZ_HIGH     0x2236
#define SHT_CMD_PERI_2HZ_MED      0x2220
#define SHT_CMD_PERI_2HZ_LOW      0x222B

// D. Saniyede 4 Ölçüm (4 Hz)
#define SHT_CMD_PERI_4HZ_HIGH     0x2334
#define SHT_CMD_PERI_4HZ_MED      0x2322
#define SHT_CMD_PERI_4HZ_LOW      0x2329

// E. Saniyede 10 Ölçüm (10 Hz)
#define SHT_CMD_PERI_10HZ_HIGH    0x2737
#define SHT_CMD_PERI_10HZ_MED     0x2721
#define SHT_CMD_PERI_10HZ_LOW     0x272A

// Periyodik Veriyi Çekme Komutu
#define SHT_CMD_FETCH_DATA        0xE000

/* ========================================================================== */
/*                4. OZEL FONKSIYON VE DONANIM KOMUTLARI                      */
/* ========================================================================== */
#define SHT_CMD_BREAK             0x3093
#define SHT_CMD_SOFT_RESET        0x3066
#define SHT_CMD_HEATER_ON         0x306D
#define SHT_CMD_HEATER_OFF        0x305E

#define SHT_CMD_ART               0x2B32
#define SHT_CMD_CLEAR_STATUS      0x3041

/* ========================================================================== */
/*                     5. DURUM KAYIT DEFTERI (STATUS)                       */
/* ========================================================================== */
#define SHT_CMD_READ_STATUS       0xF32D



typedef enum {

	SHT31_STATUS_WRITE_CRC_FAIL   = 0,   /* Bit 0: Son yazma işleminde CRC hatası */
    SHT31_STATUS_COMMAND_FAILED   = 1,   /* Bit 1: Son komut başarısız/işlenemedi */
    SHT31_STATUS_RESET_DETECTED   = 4,   /* Bit 4: Cihaz resetlendi (Güç gidip gelmesi vb.) */
    SHT31_STATUS_T_ALERT          = 10,  /* Bit 10: Sıcaklık alarm sınırları aşıldı */
    SHT31_STATUS_RH_ALERT         = 11,  /* Bit 11: Nem alarm sınırları aşıldı */
    SHT31_STATUS_HEATER_ON        = 13,  /* Bit 13: Dahili ısıtıcı aktif durumda */
    SHT31_STATUS_ALERT_PENDING    = 15,   /* Bit 15: Bekleyen aktif bir alarm var */

	SHT31_STATUS_TEMP_CRC_ERROR   = 2, // TEMP CRC ERROR
	SHT31_STATUS_MOI_CRC_ERROR   = 3   // MOI CRC ERROR


} SHT_Status_Bit_t;



void SHT_Send_Command(uint16_t Command);

void SHT_Take_Data(uint16_t Command, uint8_t* SHT_Coming_Data, uint8_t data_len);

void SHT_Reset(void);

void SHT_Heater_ON(void);

void SHT_Heater_OFF(void);

void SHT_Clear_Status(void);

void SHT_Periodic_Mode_OFF(void);

void SHT_Periodic_Read(void);

void SHT_Status(void);

void SHT_CRC_Mark(SHT_Status_Bit_t error);
void SHT_CRC_Clear(SHT_Status_Bit_t error);
uint8_t SHT_CRC_Read(SHT_Status_Bit_t error);


uint8_t SHT_Status_Read(SHT_Status_Bit_t Status);

void SHT_Temp_Moi_Ref(void);

uint8_t SHT31_CRC_16Bit(uint16_t value);


void SHT_Periodic_Start(void);



#endif /* INC_SHT31_H_ */
