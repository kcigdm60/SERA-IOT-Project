#include "main.h"
#include "lora.h"

uint8_t Lora_Error_Reg=0;

uint8_t RX_BUFFER[128];
uint8_t TX_BUFFER[128];

uint8_t Lora_LENG;



void Lora_Reset_Pin_Config() {


	RCC->AHB1ENR |= (1 << 1);


	    GPIOB->MODER &= ~(3 << 0);
		GPIOB->MODER |= (1 << 0);
		GPIOB->OTYPER &= ~(1 << 0);
		GPIOB->OSPEEDR |=(3 << 0);
		GPIOB->PUPDR &= ~(3 << 0);
		GPIOB->PUPDR |= (2 << 0);

}



void Lora_Reset() {

	GPIOB->ODR &= ~(1 << 0);

	while(!(delay_ms(100)));
	delay_stop();

	GPIOB->ODR |= (1 << 0);
	while(!(delay_ms(20)));
	delay_stop();


}




void Lora_Init(const Lora_Config_t *config) {



	uint8_t reg=0;
	uint16_t reg1=0;

//////////////////////////////////////////////////// SLEEP MODE ///////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////RegOpMode (0x01) CONFIG /////////////////////////////////////////////////////////////////////////////

	  reg |= (config->LongRangeMode << 7);
	     reg |= (config->AccessSharedReg << 6);
	     reg |= (config->LowFrequencyModeOn << 3);
	     reg |= (config->Mode << 0);

	     SPI_Write(RegOpMode,reg);



////////////////////////////////////// RegFr (0x06-0x07-0x08) CONFIG /////////////////////////////////////////////////////////////////////////////


 switch(config->Frf) {


 // if 434Mhz
 case 0:

	 SPI_Write(RegFrMsb,0x6c);
	 SPI_Write(RegFrMid,0x80);
	 SPI_Write(RegFrLsb,0x00);

	 break;

// if 868 Mhz

 case 1:

	     SPI_Write(RegFrMsb,0xD9);
	 	 SPI_Write(RegFrMid,0x06);
	 	 SPI_Write(RegFrLsb,0x66);


	 break;

// if 9151 Mhz

 case 2:

	     SPI_Write(RegFrMsb,0xE4);
	 	 SPI_Write(RegFrMid,0xC0);
	 	 SPI_Write(RegFrLsb,0x00);

	 break;


 }


 //////////////////////////////////////////// RegPaConfig (0x09) CONFIG /////////////////////////////////////////////////////////////////////////////


 	 	   reg=0;

           reg |= ((config->PaSelect)  << 7);
           reg |= ((config->MaxPower)  << 4);
           reg |= (config->OutputPower << 0);

           SPI_Write(RegPaConfig,reg);



//////////////////////////////////////////// RegPaRamp (0x0A) CONFIG /////////////////////////////////////////////////////////////////////////////


   reg=0;

   reg |= (config->PaRamp << 0);

   SPI_Write(RegPaRamp,reg);




//////////////////////////////////////////// RegOcp (0x0B) CONFIG /////////////////////////////////////////////////////////////////////////////

   reg=0;

   reg |= (config->OcpOn << 5);
   reg |= (config->OcpTrim << 0);

   SPI_Write(RegOcp,reg);



//////////////////////////////////////////// RegLna (0x0C) CONFIG /////////////////////////////////////////////////////////////////////////////


   reg=0;

   reg |= ((config->LnaGain) << 5);
   reg |= ((config->LnaBoostHf) << 0);

   SPI_Write(RegLna,reg);



//////////////////////////////////////////// RegFifoTxBaseAddr (0x0E) CONFIG /////////////////////////////////////////////////////////////////////////////

   reg=0;

   reg = config->FifoTxBaseAddr;

   SPI_Write(RegFifoTxBaseAddr,reg);


//////////////////////////////////////////// RegFifoRxBaseAddr (0x0F) CONFIG /////////////////////////////////////////////////////////////////////////////


   reg=0;

   reg = config->FifoRxBaseAddr;

   SPI_Write(RegFifoRxBaseAddr,reg);


//////////////////////////////////////////// RegIrqFlagsMask(0x11) CONFIG /////////////////////////////////////////////////////////////////////////////

   reg=0;

   reg |= ((config->RxTimeoutMask) << 7);
   reg |= ((config->RxDoneMask) << 6);
   reg |= ((config->PayloadCrcErrorMask) << 5);
   reg |= ((config->ValidHeaderMask) << 4);
   reg |= ((config->TxDoneMask) << 3);
   reg |= ((config->CadDoneMask) << 2);
   reg |= ((config->FhssChangeChannelMask) << 1);
   reg |= ((config->CadDetectedMask) << 0);

   SPI_Write(RegIrqFlagsMask,reg);


//////////////////////////////////////////// RegModemConfig1 (0x1D) CONFIG /////////////////////////////////////////////////////////////////////////////

  reg=0;

 reg |= (config->Bw << 4);
 reg |= (config->CodingRate << 1);
 reg |= (config->ImplicitHeaderModeOn << 0);

 SPI_Write(RegModemConfig1,reg);


 //////////////////////////////////////////// RegSymbTimeoutLsb (0x1F) CONFIG /////////////////////////////////////////////////////////////////////////////

 reg = 0;
 reg1 = config->SymbTimeout;

 SPI_Write(RegSymbTimeoutLsb,(reg1 & 0xFF));

 reg = ((reg1 >> 8) & 0x03);


///////////////////////////////////////////// RegModemConfig2 (0x1E) CONFIG ////////////////////////////////////////////////////////////////////////////////



 reg |= ((config->SpreadingFactor & 0x0F) << 4); // Bit 7-4 (Maske koyduk ki taşmasın)
 reg |= ((config->TxContinuousMode & 0x01) << 3); // Bit 3
 reg |= ((config->RxPayloadCrcOn & 0x01) << 2);   // Bit 2

 SPI_Write(RegModemConfig2, reg);


 //////////////////////////////////////////// RegPreamble (0x20)-(0x21) CONFIG /////////////////////////////////////////////////////////////////////////////



reg=0;
reg1=0;

reg1 = config->PreambleLength;
reg |= (reg1 >> 8) & 0xFF;

SPI_Write(RegPreambleMsb,reg);

reg=0;
reg = reg1 & 0xFF;

SPI_Write(RegPreambleLsb,reg);



//////////////////////////////////////////// RegPayloadLength(0x22) CONFIG /////////////////////////////////////////////////////////////////////////////


reg=0;
reg1=0;

 reg = (config->PayloadLength);

SPI_Write(RegPayloadLength,reg);



//////////////////////////////////////////// RegMaxPayloadLength(0x23) CONFIG /////////////////////////////////////////////////////////////////////////////


 reg =0;

 reg = (config->PayloadMaxLength);

 SPI_Write(RegMaxPayloadLength,reg);



 //////////////////////////////////////////// RegHopPeriod(0x24) CONFIG /////////////////////////////////////////////////////////////////////////////



 reg =0;

 reg = (config->FreqHoppingPeriod);

 SPI_Write(RegHopPeriod,reg);



 //////////////////////////////////////////// RegModemConfig3 (0x26) CONFIG /////////////////////////////////////////////////////////////////////////////



 reg =0;

reg |= (config->LowDataRateOptimize << 3);
reg |= (config->AgcAutoOn << 2);
SPI_Write(RegModemConfig3,reg);



//////////////////////////////////////////// empty (0x27) CONFIG /////////////////////////////////////////////////////////////////////////////



reg =0;

reg = (config->PpmCorrection);

SPI_Write(empty,reg);



//////////////////////////////////////////// RegDetectOptimize(0x31) CONFIG /////////////////////////////////////////////////////////////////////////////

reg=0;

reg = 0xC0 | (config->DetectionOptimize & 0x07);
SPI_Write(RegDetectOptimize,reg);



//////////////////////////////////////////// RegInvertIQ (0x33) CONFIG /////////////////////////////////////////////////////////////////////////////



reg=0;

reg |= (config->InvertIQ << 6);
reg |= (0x27 << 0);

SPI_Write(RegInvertIQ,reg);



//////////////////////////////////////////// RegDetectionThreshold (0x37) CONFIG ////////////////////////////////////////////////////////////////////////////



reg =0;

reg  |= (config->DetectionThreshold);

SPI_Write(RegDetectionThreshold,reg);




//////////////////////////////////////////// RegSyncWord (0x39) ////////////////////////////////////////////////////////////////////////////

reg =0;

reg |= (config->SyncWord);

SPI_Write(RegSyncWord,reg);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_Mode(Mode_t mode) {

	uint8_t mode_reg=0;

	mode_reg=SPI_Read(RegOpMode);
	mode_reg &= ~(7 << 0);
	mode_reg |=(mode << 0);

	SPI_Write(RegOpMode,mode_reg);}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_FifoAddrPtr_Write(uint8_t AddrPtr) {

	SPI_Write(RegFifoAddrPtr,AddrPtr);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_FifoRxCurrentAddr_Read() {

	return SPI_Read(RegFifoRxCurrentAddr);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_FifoTxBaseAddr(uint8_t FifoTxBaseAddr) {

	SPI_Write(RegFifoTxBaseAddr, FifoTxBaseAddr);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_FifoRxBaseAddr(uint8_t FifoRxBaseAddr) {

	SPI_Write(RegFifoRxBaseAddr, FifoRxBaseAddr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_IrqFlagsMask_Config(FlagsMask_t flags,Disabel_Enable_t disable_enable) {

	uint8_t flags_reg=0;

	flags_reg= SPI_Read(RegIrqFlagsMask);
	flags_reg &=  ~(1 << flags);
	flags_reg |= (!(disable_enable) << flags);
	SPI_Write(RegIrqFlagsMask,flags_reg);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_RegIrqFlags_Clear(Lora_IrqBit_t flags) {

	uint8_t flag_reg=0;


	if (flags == IRQ_FLAGS_ALL) {

		SPI_Write(RegIrqFlags, 255);

	}

	else {
		flag_reg |= (1 << flags);

		SPI_Write(RegIrqFlags,flag_reg);
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_IrqFlagsMask_Read(Lora_IrqBit_t flags) {

	uint8_t Irq_reg=0;

	Irq_reg=SPI_Read(RegIrqFlags);

	if (Irq_reg & (1 << flags)) {

		return 1;
	}

	else {
		return 0;
	}}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_FifoRxBytes_Read() {

	return SPI_Read(RegRxNbBytes);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_PacketSnr_Read() {

	return SPI_Read(RegPktSnrValue);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_FifoRxByteAddrPtr() {

	return SPI_Read(RegFifoRxByteAddr);}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_Payload_Length(uint8_t length) {

SPI_Write(RegPayloadLength,length);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_FIFO_Read() {

return SPI_Read(FIFO); }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_FifoRxBytesNb() {

	return SPI_Read(RegRxNbBytes);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_Err_Mark(Error_reg_t err) {


	Lora_Error_Reg |= (1 << err);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_Err_Read(Error_reg_t err) {

	if (Lora_Error_Reg & (1 << err)) {

		return 1;

	}

	return 0;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_Err_Clean(Error_reg_t err) {

	Lora_Error_Reg &= ~(1 << err);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// veriyi al işle ve gönder


uint8_t Lora_SendData(uint8_t *buffer,
                   uint8_t *data,
                   uint8_t leng,
                   uint8_t sender_adress,
                   uint8_t receive_adress,
                   uint8_t process)
{

        buffer[0] = sender_adress;
        buffer[1] = receive_adress;
        buffer[2] = process;
        buffer[3] = leng;

        for (uint8_t x = 0; x < leng - 4; x++) {
            buffer[x + 4] = data[x];
        }


        Lora_Mode(MODE_STDBY);

        Lora_FifoAddrPtr_Write(SPI_Read(RegFifoTxBaseAddr));

        for (uint8_t i = 0; i < leng; i++) {
            SPI_Write(FIFO, buffer[i]);
        }

        Lora_Payload_Length(leng);


        Lora_Mode(MODE_TX);

        while (!(Lora_IrqFlagsMask_Read(IRQ_TX_DONE))) {

        	if (delay_ms(Lora_Tx_Wait_Time)) {
        		delay_stop();

                Lora_RegIrqFlags_Clear(IRQ_TX_DONE);
        		Lora_Err_Mark(TX_ERROR);

        		return 0;
        	}

        }

        Lora_RegIrqFlags_Clear(IRQ_TX_DONE);

          delay_stop();


          return 1;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_ReceiveData(uint8_t *buffer,
                      uint8_t *data,
                      uint8_t *leng,
                      uint8_t *sender_adress,
                      uint8_t *receive_adress,
                      uint8_t *process)
{

// kullanıldığı yerde ilk başta RX_DONE kontrol edilir ve bu fonksiyona girilir,


	Lora_RegIrqFlags_Clear(IRQ_RX_DONE);


	// burada normalde başlığa da bakıyorduk ama gereksiz olduğu için kaldırdım


            if ((Lora_IrqFlagsMask_Read(IRQ_PAYLOAD_CRC_ERROR))) {

                           Lora_RegIrqFlags_Clear(IRQ_PAYLOAD_CRC_ERROR);
                           Lora_Err_Mark(CRC_HARDWARE_ERROR);
                           return 0;
                       }

                       Lora_RegIrqFlags_Clear(IRQ_PAYLOAD_CRC_ERROR);



                // pointerı rx başlangıcına çeker
                Lora_FifoAddrPtr_Write(Lora_FifoRxCurrentAddr_Read());

                // gelenin kaç paket olduğunu LENG e atar
                Lora_LENG = Lora_FifoRxBytesNb();


                if (Lora_LENG > 128) {              // <-- EKLENECEK KONTROL
                    Lora_Err_Mark(LENG_ERROR);
                    return 0;
                }



                // gelen paketleri rx_buffer a atar

                for (uint8_t receive_count = 0; receive_count < Lora_LENG; receive_count++) {
                    buffer[receive_count] = Lora_FIFO_Read();
                }

                *sender_adress  = buffer[0];
                *receive_adress = buffer[1];
                *process        = buffer[2];
                *leng           = buffer[3];


                if (Lora_LENG != *leng) {
                    Lora_Err_Mark(LENG_ERROR);
                    return 0;
                }


                    for (uint8_t i = 0; i < Lora_LENG - 4; i++) {
                        data[i] = buffer[i + 4];
                    }



                    return 1;

                }




void Lora_Buffer_Clean(uint8_t *buffer_type) {


	for (uint8_t x=0; x < 128; x++) {

	buffer_type[x]=0;

	}


}



