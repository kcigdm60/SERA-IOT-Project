#include "main.h"
#include "DS18B20.h"
#include "lora.h"
#include "toprak_main_conf_func.h"

////////////////////////////////////////////////////
uint8_t Lora_Main_Err=0;

uint8_t Lora_Sender_ADDR=0;
uint8_t Lora_Receive_ADDR=0;
uint8_t Lora_Process=0;
uint8_t Lora_Main_Leng=0;
uint8_t Lora_Data_Buf[16]={0,0,0,0,0,0,0,0,0,0};

//////////////////////////////////////////////////////

// değerler okunup işlendikten sonra float'lar içine atılacak daha sonra ise ayrıştırılarak uint8_t lere atılır


float node1_moisture_value=0;
float node1_temperature_value=0;


uint8_t moisture_value_integer=0;
uint8_t moisture_value_decimal=0;


uint8_t temperature_sign=0;


uint8_t temperature_value_integer=0;
uint8_t temperature_value_decimal=0;

uint8_t Lora_Main_Count=0;


uint16_t Lora_Tx_Wait_Time=3000;
uint16_t Lora_Rx_Wait_Time=5000;

/////////////////////////////////////////////////


int main(void)
{
	System_Clock_Config();
	Timer_Config();
	SPI_Config();
	DMA_Config();
	ADC_Config();
	ADC_Pin_Calibration_Conf();

	TIM3_Config();
	Wire_1_Pin_Conf();
	Lora_Reset_Pin_Config();
	Lora_Start_Conf();
	Lora_Config();

  	Lora_Mode(MODE_RXCONTINUOUS);

  	WatchDog_Config();



  while (1)
  {
	  WatchDog_Feed();

	  Lora_Node1_Gateway_Send_Flow();
	  NODE1_Flow();



  }
}




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


void Lora_Node1_Gateway_Send_Flow() {


	while (!(delay_ms(100)));
	delay_stop();

		    if (!(Lora_Node1_Gateway_Send())) {

		    	Lora_Mode(MODE_RXCONTINUOUS);
		     	 GPIOA->ODR |= (1 << 11);

		    }


		    else {

		    	GPIOA->ODR &= ~(1 << 11);
		    }

		    Lora_Query();



}







uint8_t Lora_Node1_Gateway_Send() {

	if (Lora_IrqFlagsMask_Read(IRQ_RX_DONE)) {

		// bufferları temizle
		Lora_Buffer_Clean(TX_BUFFER);
		Lora_Buffer_Clean(RX_BUFFER);

	   	 Lora_Mode(MODE_STDBY);

	if (!Lora_ReceiveData(RX_BUFFER,Lora_Data_Buf,&Lora_Main_Leng,&Lora_Sender_ADDR,&Lora_Receive_ADDR,&Lora_Process)) {

		return 0;

	}


			// gelen mesaj node 1 den değilse hata
			if(Lora_Sender_ADDR != Lora_Gateway_Addr) {


						return 0;
			}




			// gelen mesaj gatewaye değilse hata

				if (Lora_Receive_ADDR != Lora_Node1_Addr) {


							return 0;
				}



				Lora_Data_Buf[0]=moisture_value_integer;
				Lora_Data_Buf[1]=moisture_value_decimal;
				Lora_Data_Buf[2]=temperature_sign;
				Lora_Data_Buf[3]=temperature_value_integer;
				Lora_Data_Buf[4]=temperature_value_decimal;


			if(!(Lora_SendData(TX_BUFFER,Lora_Data_Buf,9, Lora_Node1_Addr, Lora_Gateway_Addr,1))) {

				return 0;
			}



		   	Lora_Mode(MODE_RXCONTINUOUS);

		   	GPIOA->ODR |= (1 << 10);
			while(!(delay_ms(50)));
		   	delay_stop();
			GPIOA->ODR &= ~(1 << 10);


			Lora_Main_Count=0;

			return 1;

	}

	return 1;

}


void NODE1_Flow() {

	  WatchDog_Feed();
	ADC_Air_Calibration();
	  WatchDog_Feed();
	ADC_Water_Calibration();
	  WatchDog_Feed();
	ADC_Read_Value(&node1_moisture_value);
	  WatchDog_Feed();
	node1_temperature_value = DS_Temperature_Read();
	  WatchDog_Feed();
	Soil_Moisture_Calculation();
	  WatchDog_Feed();


	}







void Lora_Start_Conf(){

	Lora_Reset();

	SPI_Write(RegOpMode, 0x00);      // FSK sleep
	while(!(delay_ms(10)));
	delay_stop();

	SPI_Write(RegOpMode, 0x80);      // LoRa sleep
	while(!(delay_ms(10)));
	delay_stop();

	SPI_Write(RegOpMode, 0x81);      // LoRa standby
}





void Soil_Moisture_Calculation() {


				moisture_value_integer=(uint8_t)node1_moisture_value;
				moisture_value_decimal=(uint8_t)((node1_moisture_value-moisture_value_integer)*100+0.5);




				if (node1_temperature_value >= 0) {

					temperature_sign=0;

				}

				else {
					temperature_sign=1;
					node1_temperature_value=(-1)*node1_temperature_value;

				}

				temperature_value_integer=(uint8_t)node1_temperature_value;
				temperature_value_decimal=(uint8_t)((node1_temperature_value-temperature_value_integer)*100+0.05f);




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











