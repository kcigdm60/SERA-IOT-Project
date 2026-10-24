#include "main.h"
#include "16_Role.h"
#include "16_Role_Main_Conf_Func.h"
#include "lora.h"


uint8_t Lora_Main_Err=0;

uint8_t Lora_Sender_ADDR=0;
uint8_t Lora_Receive_ADDR=0;
uint8_t Lora_Process=0;
uint8_t Lora_Main_Leng=0;
uint8_t Lora_Data_Buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


uint8_t read_relay_status_high;
uint8_t read_relay_status_low;


uint8_t write_relay_status_high=0;
uint8_t write_relay_status_low=0;


uint16_t Lora_Tx_Wait_Time=3000;
uint16_t Lora_Rx_Wait_Time=5000;



int main(void)
{
	System_Clock_Config();
	Timer_Config();
	GPIO_Config();

	Lora_Reset_Pin_Config();

	SPI_Config();
	DMA_Config();

	Lora_Start_Conf();
	Lora_Config();

  	Lora_Mode(MODE_RXCONTINUOUS);

  	WatchDog_Config();


  while (1)
  {

	  Lora_Node2_Gateway_Send_Flow();
	  Main_16_Role_Driver();

	  WatchDog_Feed();


  }}


void Lora_Main_Err_Marking(Lora_Err_Main_t err) {


	Lora_Main_Err |= (1 << err);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t Lora_Main_Err_Read(Lora_Err_Main_t err) {

	if (Lora_Main_Err & (1 << err)) {

		return 1;

	}

	return 0;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Lora_Main_Err_Clean(Lora_Err_Main_t err) {

	Lora_Main_Err &= ~(1 << err);

}


void Lora_Config() {

	const Lora_Config_t Lora1 = {

		    .LongRangeMode        = LoRa_Mode,
		    .AccessSharedReg      = Access_LoRa_registers_page,
		    .LowFrequencyModeOn   = Low_Frequency_Mode,
		    .Mode                 = MODE_STDBY,
		    .Frf                  = CH_434_MHZ,

		    .PaSelect             = PA_BOOST_pin,
		    .MaxPower             = dBm_15,
		    .OutputPower          = OutputPower_15,
		    .PaRamp               = PA_RAMP_50_US,
		    .OcpOn                = OCP_enabled,
		    .OcpTrim              = OCP_240_mA,

		    .LnaGain              = GAIN_G1_MAXIMUM,
		    .LnaBoostHf           = Boost_on,

		    .FifoTxBaseAddr       = FIFO_TX_BASE_ADDR_0x80,
		    .FifoRxBaseAddr       = FIFO_RX_BASE_ADDR_0x00,

		    .RxTimeoutMask        = RX_TIMEOUT_INTERRUPT_DISABLE,
		    .RxDoneMask           = RX_DONE_INTERRUPT_ENABLE,
		    .PayloadCrcErrorMask  = PAYLOAD_CRC_ERROR_INTERRUPT_ENABLE,
		    .ValidHeaderMask      = VALID_HEADER_INTERRUPT_DISABLE,
		    .TxDoneMask           = TX_DONE_INTERRUPT_ENABLE,
		    .CadDoneMask          = CAD_DONE_INTERRUPT_DISABLE,
		    .FhssChangeChannelMask= FHSS_CHANGE_CHANNEL_INTERRUPT_DISABLE,
		    .CadDetectedMask      = CAD_DETECTED_INTERRUPT_DISABLE,

		    .Bw                   = BW_125_KHZ,
		    .CodingRate           = CR_4_8,
		    .ImplicitHeaderModeOn = Explicit_Header_mode,
		    .SpreadingFactor      = SF_12,
		    .TxContinuousMode     = NORMAL_MODE,
		    .RxPayloadCrcOn       = CRC_enable,

		    .SymbTimeout          = 0x03FF,
		    .PreambleLength       = 32,
		    .PayloadLength        = 1,
		    .PayloadMaxLength     = 255,
		    .FreqHoppingPeriod    = 0,

		    .LowDataRateOptimize  = enable,
			.AgcAutoOn            = LNA_AGC_loop,
		    .PpmCorrection        = 0,
		    .DetectionOptimize    = SF7_to_SF12,
		    .InvertIQ             = IQ_NORMAL,
		    .DetectionThreshold   = SF7_to_SF12_,
		    .SyncWord             = 0x19
	};


	Lora_Init(&Lora1);
}


void Lora_Value_Reset() {

	Lora_Buffer_Clean(RX_BUFFER);
	Lora_Buffer_Clean(TX_BUFFER);

	 Lora_Sender_ADDR=0;
	 Lora_Receive_ADDR=0;
	 Lora_Process=0;
	 Lora_Main_Leng=0;

	 for (uint8_t x=0; x < 16; x++) {

		 Lora_Data_Buf[x]=0;

	 }

}



void Lora_Node2_Gateway_Send_Flow() {

// delayı sakın kaldırma (SPI'ı stabil yapıyor)

while (!(delay_ms(100)));
delay_stop();

	    if (!(Lora_Node2_Gateway_Send())) {

	    	Lora_Mode(MODE_RXCONTINUOUS);


	    }

	    GPIOC->ODR &= ~(1 << 13);


	    Lora_Query();
	    }



uint8_t Lora_Node2_Gateway_Send() {


	if (Lora_IrqFlagsMask_Read(IRQ_RX_DONE)) {


		GPIOC->ODR |= (1 << 13);

		// bufferları temizle
		Lora_Buffer_Clean(TX_BUFFER);
		Lora_Buffer_Clean(RX_BUFFER);


	   	 Lora_Mode(MODE_STDBY);

    	if (!(Lora_ReceiveData(RX_BUFFER,Lora_Data_Buf,&Lora_Main_Leng,&Lora_Sender_ADDR,&Lora_Receive_ADDR,&Lora_Process))) {


    		return 0;

    	}



		// gelen mesaj gatewaye değilse hata
		if(Lora_Sender_ADDR != Lora_Gateway_Addr) {


			// sender addr hata

			return 0;	}

// 			// gelen mesaj node 2 ye  değilse hata

				if (Lora_Receive_ADDR != Lora_Node2_Addr) {


					// receive hata


					return 0;
				}


				// okuma isteği gönderdiyse
				if (Lora_Process == 0) {

				Lora_Data_Buf[0]=read_relay_status_high;
				Lora_Data_Buf[1]=read_relay_status_low;

				if (!(Lora_SendData(TX_BUFFER,Lora_Data_Buf,6, Lora_Node2_Addr, Lora_Gateway_Addr,1))) {


					// send data hata

					return 0;

				}


				}


				// yazma isteği gönderdiyse
				else if (Lora_Process == 1) {

					write_relay_status_high=Lora_Data_Buf[0];
					write_relay_status_low=Lora_Data_Buf[1];


					if(!(Lora_SendData(TX_BUFFER,Lora_Data_Buf,4, Lora_Node2_Addr, Lora_Gateway_Addr,2))) {


						return 0;


					}


				}


				else {

				// okuma veya yazma değil büyük hata

					return 0;

				}


				  Lora_Mode(MODE_RXCONTINUOUS);


	}

	return 1;

}


void Main_16_Role_Driver() {


	if ((read_relay_status_high != write_relay_status_high) || (read_relay_status_low != write_relay_status_low)) {


		static const PIN_t Pins[16] = {
		    {GPIOA, 1},  {GPIOA, 2},  {GPIOA, 3},  {GPIOA, 8},
		    {GPIOA, 9},  {GPIOA, 10}, {GPIOA, 11}, {GPIOA, 12},
		    {GPIOB, 3},  {GPIOB, 2},  {GPIOB, 4},  {GPIOB, 5},
		    {GPIOB, 6},  {GPIOB, 13}, {GPIOB, 14}, {GPIOB, 15}
		};


		for(uint8_t x=0; x < 8; x++) {

			if (write_relay_status_low & (1 << x)) {

				PIN_Set(Pins[x]);

			}

			else {
				PIN_Reset(Pins[x]);
			}

			while(!delay_ms(10));
			delay_stop();



		}


		for(uint8_t x=8; x < 16; x++) {

					if (write_relay_status_high & (1 << (x-8))) {

						PIN_Set(Pins[x]);

					}

					else {
						PIN_Reset(Pins[x]);
					}

					while(!delay_ms(10));
					delay_stop();

				}

		read_relay_status_low = write_relay_status_low;
		read_relay_status_high = write_relay_status_high;

	}

}



void Lora_Start_Conf(){

	Lora_Reset();
	// FSK sleep
	SPI_Write(RegOpMode, 0x00);
	while(!(delay_ms(10)));
	delay_stop();
	// LoRa sleep
	SPI_Write(RegOpMode, 0x80);
	while(!(delay_ms(10)));
	delay_stop();
	 // LoRa standby
	SPI_Write(RegOpMode, 0x81);
}




void Lora_Main_Reset() {

	Lora_Start_Conf();
    Lora_Config();
    Lora_Mode(MODE_RXCONTINUOUS);

}



void Lora_Query() {

	 if (SPI_Read(RegOpMode) != 0x8D) {

	       	    		Lora_Main_Reset();
	       	    	}
	       	    }




