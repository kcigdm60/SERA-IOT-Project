/*
 * lora.h
 *
 *  Created on: May 8, 2026
 *      Author: kcigd
 */

#ifndef INC_LORA_H_
#define INC_LORA_H_


#include "main.h"


extern void SPI_Write(uint8_t reg_adress,uint8_t veri);
extern uint8_t SPI_Read(uint8_t reg_adress);
extern uint8_t delay_ms(uint16_t);
extern void delay_stop();


extern uint8_t Lora_Error_Reg;

extern uint8_t RX_BUFFER[128];
extern uint8_t TX_BUFFER[128];
extern uint8_t Lora_LENG;

extern uint16_t Lora_Tx_Wait_Time;
extern uint16_t Lora_Rx_Wait_Time;



/////////////////////////////////////////////////// REGISTER ADRESS //////////////////////////////////////////////////////////////

#define FIFO 0x00
#define RegOpMode 0x01
#define RegFrMsb 0x06
#define RegFrMid 0x07
#define RegFrLsb 0x08
#define RegPaConfig 0x09
#define RegPaRamp 0x0A
#define RegOcp 0x0B
#define RegLna 0x0C
#define RegFifoAddrPtr 0x0D
#define RegFifoTxBaseAddr 0x0E
#define RegFifoRxBaseAddr 0x0F
#define RegFifoRxCurrentAddr 0x10
#define RegIrqFlagsMask 0x11
#define RegIrqFlags 0x12
#define RegRxNbBytes 0x13
#define RegRxHeaderCntValueMsb 0x14
#define RegRxHeaderCntValueLsb 0x15
#define RegRxPacketCntValueMsb 0x16
#define RegRxPacketCntValueLsb 0x17
#define RegModemStat 0x18
#define RegPktSnrValue 0x19
#define RegPktRssiValue 0x1A
#define RegRssiValue 0x1B
#define RegHopChannel 0x1C
#define RegModemConfig1 0x1D
#define RegModemConfig2 0x1E
#define RegSymbTimeoutLsb 0x1F
#define RegPreambleMsb 0x20
#define RegPreambleLsb 0x21
#define RegPayloadLength 0x22
#define RegMaxPayloadLength 0x23
#define RegHopPeriod 0x24
#define RegFifoRxByteAddr 0x25
#define RegModemConfig3 0x26
#define empty 0x27
#define RegFeiMsb 0x28
#define RegFeiMid 0x29
#define RegFeiLsb 0x2A
#define RegRssiWideband 0x2C
#define RegDetectOptimize 0x31
#define RegInvertIQ 0x33
#define RegDetectionThreshold 0x37
#define RegSyncWord 0x39

/////////////////////////////////////////////// ENUM LongRangeMode_t ////////////////////////////////////////////////////////////////////////



typedef enum {

	FSK_OOK_Mode,
	LoRa_Mode

} LongRangeMode_t;


/////////////////////////////////////////////// ENUM AccessSharedReg_t ////////////////////////////////////////////////////////////////////////


typedef enum {


	Access_LoRa_registers_page,
	Access_FSK_registers_page


} AccessSharedReg_t;


/////////////////////////////////////////////// ENUM LowFrequencyModeOn_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	High_Frequency_Mode,
	Low_Frequency_Mode
} LowFrequencyModeOn_t;


/////////////////////////////////////////////// ENUM Mode_t (LoRa Operating Modes) ////////////////////////////////////////////////////////////////////////


typedef enum {
	MODE_SLEEP,
	MODE_STDBY,
	MODE_FSTX,
	MODE_TX,
	MODE_FSRX,
	MODE_RXCONTINUOUS,
	MODE_RXSINGLE,
	MODE_CAD
} Mode_t;


/////////////////////////////////////////////// ENUM RF_Channel_t (Operating Frequency Bands) ////////////////////////////////////////////////////////////////////////


typedef enum {
	CH_434_MHZ,
	CH_868_MHZ,
	CH_915_MHZ
} RF_Channel_t;


/////////////////////////////////////////////// ENUM PaSelect_t (Power Amplifier Selection) ////////////////////////////////////////////////////////////////////////


typedef enum {
	RFO_pin,
	PA_BOOST_pin
} PaSelect_t;


/////////////////////////////////////////////// ENUM MaxPower_t (Maximum Output Power Settings) ////////////////////////////////////////////////////////////////////////


typedef enum {
	dBm_10_8,
	dBm_11_4,
	dBm_12,
	dBm_12_6,
	dBm_13_2,
	dBm_13_8,
	dBm_14_4,
	dBm_15
} MaxPower_t;


/////////////////////////////////////////////// ENUM OutputPower_t (TX Output Power Level) ////////////////////////////////////////////////////////////////////////


typedef enum {
	OutputPower_0,
	OutputPower_1,
	OutputPower_2,
	OutputPower_3,
	OutputPower_4,
	OutputPower_5,
	OutputPower_6,
	OutputPower_7,
	OutputPower_8,
	OutputPower_9,
	OutputPower_10,
	OutputPower_11,
	OutputPower_12,
	OutputPower_13,
	OutputPower_14,
	OutputPower_15
} OutputPower_t;


/////////////////////////////////////////////// ENUM PaRamp_t (Power Amplifier Ramp Time) ////////////////////////////////////////////////////////////////////////


typedef enum {
	PA_RAMP_3_4_MS,
	PA_RAMP_2_MS,
	PA_RAMP_1_MS,
	PA_RAMP_500_US,
	PA_RAMP_250_US,
	PA_RAMP_125_US,
	PA_RAMP_100_US,
	PA_RAMP_62_US,
	PA_RAMP_50_US,
	PA_RAMP_40_US,
	PA_RAMP_31_US,
	PA_RAMP_25_US,
	PA_RAMP_20_US,
	PA_RAMP_15_US,
	PA_RAMP_12_US,
	PA_RAMP_10_US
} PaRamp_t;


/////////////////////////////////////////////// ENUM OcpOn_t (Over Current Protection Enable/Disable) ////////////////////////////////////////////////////////////////////////


typedef enum {
	OCP_disabled,
	OCP_enabled
} OcpOn_t;


/////////////////////////////////////////////// ENUM OCP_Trim_t (Over Current Protection Threshold) ////////////////////////////////////////////////////////////////////////


typedef enum {
	OCP_45_mA,
	OCP_50_mA,
	OCP_55_mA,
	OCP_60_mA,
	OCP_65_mA,
	OCP_70_mA,
	OCP_75_mA,
	OCP_80_mA,
	OCP_85_mA,
	OCP_90_mA,
	OCP_95_mA,
	OCP_100_mA,
	OCP_105_mA,
	OCP_110_mA,
	OCP_115_mA,
	OCP_120_mA,

	OCP_130_mA,
	OCP_140_mA,
	OCP_150_mA,
	OCP_160_mA,
	OCP_170_mA,
	OCP_180_mA,
	OCP_190_mA,
	OCP_200_mA,
	OCP_210_mA,
	OCP_220_mA,
	OCP_230_mA,
	OCP_240_mA
} OCP_Trim_t;


/////////////////////////////////////////////// ENUM LnaGain_t (LNA Gain Settings) ////////////////////////////////////////////////////////////////////////


typedef enum {
	GAIN_G1_MAXIMUM = 1,
	GAIN_G2 = 2,
	GAIN_G3 = 3,
	GAIN_G4 = 4,
	GAIN_G5 = 5,
	GAIN_G6_MINIMUM = 6
} LnaGain_t;


/////////////////////////////////////////////// ENUM LnaBoostHf_t (LNA High Frequency Boost) ////////////////////////////////////////////////////////////////////////


typedef enum {
	Default_LNA_current,
	Boost_on
} LnaBoostHf_t;


/////////////////////////////////////////////// ENUM FifoTxBaseAddr_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	FIFO_TX_BASE_ADDR_0x00 = 0x00,
	FIFO_TX_BASE_ADDR_0x80 = 0x80
} FifoTxBaseAddr_t;


/////////////////////////////////////////////// ENUM FifoRxBaseAddr_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	FIFO_RX_BASE_ADDR_0x00 = 0x00,
	FIFO_RX_BASE_ADDR_0x80 = 0x80
} FifoRxBaseAddr_t;


/////////////////////////////////////////////// ENUM RxTimeoutMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////


typedef enum {
	RX_TIMEOUT_INTERRUPT_ENABLE = 0,
	RX_TIMEOUT_INTERRUPT_DISABLE = 1
} RxTimeoutMask_t;


/////////////////////////////////////////////// ENUM RxDoneMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////

typedef enum {
	RX_DONE_INTERRUPT_ENABLE = 0,
	RX_DONE_INTERRUPT_DISABLE = 1
} RxDoneMask_t;


/////////////////////////////////////////////// ENUM PayloadCrcErrorMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////

typedef enum {
	PAYLOAD_CRC_ERROR_INTERRUPT_ENABLE = 0,
	PAYLOAD_CRC_ERROR_INTERRUPT_DISABLE = 1
} PayloadCrcErrorMask_t;


/////////////////////////////////////////////// ENUM ValidHeaderMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////

typedef enum {
	VALID_HEADER_INTERRUPT_ENABLE = 0,
	VALID_HEADER_INTERRUPT_DISABLE = 1
} ValidHeaderMask_t;


/////////////////////////////////////////////// ENUM TxDoneMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////

typedef enum {
	TX_DONE_INTERRUPT_ENABLE = 0,
	TX_DONE_INTERRUPT_DISABLE = 1
} TxDoneMask_t;


/////////////////////////////////////////////// ENUM CadDoneMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////

typedef enum {
	CAD_DONE_INTERRUPT_ENABLE = 0,
	CAD_DONE_INTERRUPT_DISABLE = 1
} CadDoneMask_t;


/////////////////////////////////////////////// ENUM FhssChangeChannelMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////

typedef enum {
	FHSS_CHANGE_CHANNEL_INTERRUPT_ENABLE = 0,
	FHSS_CHANGE_CHANNEL_INTERRUPT_DISABLE = 1
} FhssChangeChannelMask_t;


/////////////////////////////////////////////// ENUM CadDetectedMask_t (IRQ Mask) ////////////////////////////////////////////////////////////////////////

typedef enum {
	CAD_DETECTED_INTERRUPT_ENABLE = 0,
	CAD_DETECTED_INTERRUPT_DISABLE = 1
} CadDetectedMask_t;


/////////////////////////////////////////////// ENUM Bw_t (LoRa Bandwidth) ////////////////////////////////////////////////////////////////////////

typedef enum {
	BW_7_8_KHZ,
	BW_10_4_KHZ,
	BW_15_6_KHZ,
	BW_20_8_KHZ,
	BW_31_25_KHZ,
	BW_41_7_KHZ,
	BW_62_5_KHZ,
	BW_125_KHZ,
	BW_250_KHZ,
	BW_500_KHZ
} Bw_t;


/////////////////////////////////////////////// ENUM CodingRate_t ////////////////////////////////////////////////////////////////////////

typedef enum {
	CR_4_5 = 1,
	CR_4_6 = 2,
	CR_4_7 = 3,
	CR_4_8 = 4
} CodingRate_t;


/////////////////////////////////////////////// ENUM ImplicitHeaderModeOn_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	Explicit_Header_mode,
	Implicit_Header_mode
} ImplicitHeaderModeOn_t;


/////////////////////////////////////////////// ENUM SpreadingFactor_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	SF_6 = 6,
	SF_7 = 7,
	SF_8 = 8,
	SF_9 = 9,
	SF_10 = 10,
	SF_11 = 11,
	SF_12 = 12
} SpreadingFactor_t;


/////////////////////////////////////////////// ENUM TxContinuousMode_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	NORMAL_MODE,
	CONTINUOUS_MODE

} TxContinuousMode_t;


/////////////////////////////////////////////// ENUM RxPayloadCrcOn_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	CRC_disable,
	CRC_enable
} RxPayloadCrcOn_t;


/////////////////////////////////////////////// ENUM LowDataRateOptimize_t ////////////////////////////////////////////////////////////////////////


typedef enum {
	disable,
	enable
} LowDataRateOptimize_t;


/////////////////////////////////////////////// ENUM AgcAutoOn_t ////////////////////////////////////////////////////////////////////////


typedef enum {

	LNA_LnaGain,
	LNA_AGC_loop

} AgcAutoOn_t;


/////////////////////////////////////////////// ENUM DetectionOptimize_t ////////////////////////////////////////////////////////////////////////

typedef enum {
	SF7_to_SF12 = 3,
	SF6 = 5
} DetectionOptimize_t;


/////////////////////////////////////////////// ENUM InvertIQ_t ////////////////////////////////////////////////////////////////////////

typedef enum {
	IQ_NORMAL,
	IQ_INVERTED
} InvertIQ_t;


/////////////////////////////////////////////// ENUM DetectionThreshold_t ////////////////////////////////////////////////////////////////////////

typedef enum {
	SF7_to_SF12_ = 10,
	SF6_ = 12
} DetectionThreshold_t;


/////////////////////////////////////////////// ENUM Disabel_Enable_t ////////////////////////////////////////////////////////////////////////

typedef enum {
	lORA_DISABLE,
	LORA_ENABLE
} Disabel_Enable_t;


/////////////////////////////////////////////// ENUM FlagsMask_t ////////////////////////////////////////////////////////////////////////

typedef enum {

	CadDetectedMask,
	FhssChangeChannelMask,
	CadDoneMask,
	TxDoneMask,
	ValidHeaderMask,
	PayloadCrcErrorMask,
	RxDoneMask,
	RxTimeoutMask

} FlagsMask_t;


/////////////////////////////////////////////// ENUM Lora_IrqBit_t ////////////////////////////////////////////////////////////////////////

typedef enum {

	IRQ_CAD_DETECTED,
	IRQ_FHSS_CHANGE_CHANNEL,
	IRQ_CAD_DONE,
	IRQ_TX_DONE,
	IRQ_VALID_HEADER,
	IRQ_PAYLOAD_CRC_ERROR,
	IRQ_RX_DONE,
	IRQ_RX_TIMEOUT,
	IRQ_FLAGS_ALL

} Lora_IrqBit_t;

/////////////////////////////////////////////////////////Eror_Reg//////////////////////////////////////////////////////////////////////////////////

typedef enum {

	TX_ERROR,
	RX_ERROR,
	HEADER_ERROR,
	LENG_ERROR,
	CRC_HARDWARE_ERROR,
	CRC_SOFTWARE_ERROR

}Error_reg_t;



typedef struct {

	LongRangeMode_t LongRangeMode;
	AccessSharedReg_t AccessSharedReg;
	LowFrequencyModeOn_t LowFrequencyModeOn;
	Mode_t Mode;
	RF_Channel_t Frf;
	PaSelect_t PaSelect;
	MaxPower_t MaxPower;
	OutputPower_t OutputPower;
	PaRamp_t PaRamp;
	OcpOn_t OcpOn;
	OCP_Trim_t OcpTrim;
	LnaGain_t LnaGain;
	LnaBoostHf_t LnaBoostHf;
	FifoTxBaseAddr_t FifoTxBaseAddr;
	FifoRxBaseAddr_t FifoRxBaseAddr;
	RxTimeoutMask_t RxTimeoutMask;
	RxDoneMask_t RxDoneMask;
	PayloadCrcErrorMask_t PayloadCrcErrorMask;
	ValidHeaderMask_t ValidHeaderMask;
	TxDoneMask_t TxDoneMask;
	CadDoneMask_t CadDoneMask;
	FhssChangeChannelMask_t FhssChangeChannelMask;
	CadDetectedMask_t CadDetectedMask;
	Bw_t Bw;
	CodingRate_t CodingRate;
	ImplicitHeaderModeOn_t ImplicitHeaderModeOn;
	SpreadingFactor_t SpreadingFactor;
	TxContinuousMode_t TxContinuousMode;
	RxPayloadCrcOn_t RxPayloadCrcOn;

    uint16_t SymbTimeout;
    uint16_t PreambleLength;

    uint8_t PayloadLength;
    uint8_t PayloadMaxLength;
    uint8_t FreqHoppingPeriod;

    LowDataRateOptimize_t LowDataRateOptimize;

    AgcAutoOn_t AgcAutoOn;
    uint8_t PpmCorrection;

    DetectionOptimize_t DetectionOptimize;
    InvertIQ_t InvertIQ;

    DetectionThreshold_t DetectionThreshold;
    uint8_t SyncWord;


}Lora_Config_t;

void Lora_Reset_Pin_Config();

void Lora_Reset();



void Lora_Init(const Lora_Config_t *config);

void Lora_Mode(Mode_t mode);



void Lora_Payload_Length(uint8_t length);


void Lora_FifoAddrPtr_Write(uint8_t AddrPtr);

uint8_t Lora_FifoRxCurrentAddr_Read();

void Lora_FifoTxBaseAddr(uint8_t FifoTxBaseAddr);

void Lora_FifoRxBaseAddr(uint8_t FifoRxBaseAddr);

uint8_t Lora_FifoRxBytes_Read();

uint8_t Lora_FifoRxByteAddrPtr();

uint8_t Lora_FifoRxBytesNb();

uint8_t Lora_FIFO_Read();




void Lora_IrqFlagsMask_Config(FlagsMask_t flags,
                              Disabel_Enable_t disable_enable);

uint8_t Lora_IrqFlagsMask_Read(Lora_IrqBit_t flags);

void Lora_RegIrqFlags_Clear(Lora_IrqBit_t flags);




uint8_t Lora_PacketSnr_Read();



void Lora_Err_Mark(Error_reg_t err);

uint8_t Lora_Err_Read(Error_reg_t err);

void Lora_Err_Clean(Error_reg_t err);



uint8_t Lora_SendData(uint8_t *buffer,
                   uint8_t *data,
                   uint8_t leng,
                   uint8_t sender_adress,
                   uint8_t receive_adress,
                   uint8_t process);

uint8_t Lora_ReceiveData(uint8_t *buffer,
                      uint8_t *data,
                      uint8_t *leng,
                      uint8_t *sender_adress,
                      uint8_t *receive_adress,
                      uint8_t *process);


void Lora_Buffer_Clean(uint8_t *buffer_type);



#endif /* INC_LORA_H_ */
