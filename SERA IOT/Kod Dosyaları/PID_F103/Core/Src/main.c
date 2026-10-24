#include "main.h"
#include "lora.h"
#include "pid.h"
#include "pid_main_conf_func.h"
#include "SHT31.h"


///////////////////////////////////////////////////////

uint8_t Lora_Main_Err=0;

uint8_t Lora_Sender_ADDR=0;
uint8_t Lora_Receive_ADDR=0;
uint8_t Lora_Process=0;
uint8_t Lora_Main_Leng=0;
uint8_t Lora_Data_Buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

////////////////////////////////////////////////////

float node3_pid_desired_temp;
float node3_pid_current_temp;
float node3_pid_Kp;
float node3_pid_Ki;
float node3_pid_Kd;
float node3_pid_Moisture;



uint8_t pid_desired_temperature_integer;
uint8_t pid_desired_temperature_decimal;


uint8_t pid_current_temperature_integer;
uint8_t pid_current_temperature_decimal;


uint8_t pid_Kp_integer;
uint8_t pid_Kp_decimal;


uint8_t pid_Ki_integer;
uint8_t pid_Ki_decimal;


uint8_t pid_Kd_integer;
uint8_t pid_Kd_decimal;

uint8_t Flow_Select=0;
uint8_t Lora_Main_Count=0;


uint16_t Lora_Tx_Wait_Time=3000;
uint16_t Lora_Rx_Wait_Time=5000;

void ornek();


/////////////////////////////////////////////////////

int main(void)
{

	System_Clock_Config();
	Timer_Config();

	SPI_Config();
	DMA_Config();
	I2C1_Config();
    Lora_Reset_Pin_Config();

    Lora_Start_Conf();
    Lora_Config();

	PID_Options_Conf();

    SHT_Periodic_Start();

    signal_led_conf();

	TIM1_PWM_Config();

    TIM4_Config();

    TIM1_PWM_Update(999);


   Lora_Mode(MODE_RXCONTINUOUS);

   WatchDog_Config();


  while (1)

  {

	  WatchDog_Feed();

	  switch(Flow_Select) {

	  case 0 : {

		  Node3_Send_Flow();


break;

	  }

	  case 1: {


		  PID_Flow();
		  Flow_Select=0;




		  break;
	  }



	  }




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



void Node3_Send_Flow() {



	while (!(delay_ms(100)));
	delay_stop();


	 if (!(Lora_Node3_Gateway_Send())) {

		    	Lora_Mode(MODE_RXCONTINUOUS);


		    }


		    Lora_Query();

		    }








uint8_t Lora_Node3_Gateway_Send() {


	if (Lora_IrqFlagsMask_Read(IRQ_RX_DONE)) {


		// bufferları temizle
		Lora_Buffer_Clean(TX_BUFFER);
		Lora_Buffer_Clean(RX_BUFFER);


	   	 Lora_Mode(MODE_STDBY);


    	if(!(Lora_ReceiveData(RX_BUFFER,Lora_Data_Buf,&Lora_Main_Leng,&Lora_Sender_ADDR,&Lora_Receive_ADDR,&Lora_Process))) {


    		return 0;

    	}


		if(Lora_Sender_ADDR != Lora_Gateway_Addr) {

			return 0;	}

			// gelen mesaj gatewaye değilse hata

				if (Lora_Receive_ADDR != Lora_Node3_Addr) {

					return 0;
				}

		        GPIOC->ODR |= (1 << 13);



				// okuma isteği gönderdiyse
				if (Lora_Process == 0) {


					// Desired Temperature
					pid_desired_temperature_integer = (uint8_t)node3_pid_desired_temp;
					pid_desired_temperature_decimal = (uint8_t)(((float)node3_pid_desired_temp - pid_desired_temperature_integer) * 100 + 0.5f);

					// Current Temperature
					pid_current_temperature_integer = (uint8_t)node3_pid_current_temp;
					pid_current_temperature_decimal = (uint8_t)((node3_pid_current_temp - pid_current_temperature_integer) * 100 + 0.5f);

					// Kp Parameter
					pid_Kp_integer = (uint8_t)node3_pid_Kp;
					pid_Kp_decimal = (uint8_t)((node3_pid_Kp - pid_Kp_integer) * 100 + 0.5f);

					// Ki Parameter
					pid_Ki_integer = (uint8_t)node3_pid_Ki;
					pid_Ki_decimal = (uint8_t)((node3_pid_Ki - pid_Ki_integer) * 100 + 0.5f);

					// Kd Parameter
					pid_Kd_integer = (uint8_t)node3_pid_Kd;
					pid_Kd_decimal = (uint8_t)((node3_pid_Kd - pid_Kd_integer) * 100 + 0.5f);



				Lora_Data_Buf[0]=pid_desired_temperature_integer;
				Lora_Data_Buf[1]=pid_desired_temperature_decimal;

				Lora_Data_Buf[2]=pid_current_temperature_integer;
				Lora_Data_Buf[3]=pid_current_temperature_decimal;

				Lora_Data_Buf[4]=pid_Kp_integer;
				Lora_Data_Buf[5]=pid_Kp_decimal;

				Lora_Data_Buf[6]=pid_Ki_integer;
				Lora_Data_Buf[7]=pid_Ki_decimal;

				Lora_Data_Buf[8]=pid_Kd_integer;
				Lora_Data_Buf[9]=pid_Kd_decimal;




				if(!(Lora_SendData(TX_BUFFER,Lora_Data_Buf,14, Lora_Node3_Addr, Lora_Gateway_Addr,1))) {


					return 0;

				}

				}


				// yazma isteği gönderdiyse

				else if (Lora_Process == 1) {

					pid_desired_temperature_integer = Lora_Data_Buf[0];
					pid_desired_temperature_decimal = Lora_Data_Buf[1];

					pid_Kp_integer = Lora_Data_Buf[2];
					pid_Kp_decimal = Lora_Data_Buf[3];

					pid_Ki_integer = Lora_Data_Buf[4];
					pid_Ki_decimal = Lora_Data_Buf[5];

					pid_Kd_integer = Lora_Data_Buf[6];
					pid_Kd_decimal = Lora_Data_Buf[7];



					node3_pid_desired_temp=(float)(pid_desired_temperature_integer+(0.01f*pid_desired_temperature_decimal));

					node3_pid_Kp=(float)(pid_Kp_integer+(0.01f*pid_Kp_decimal));
					node3_pid_Ki=(float)(pid_Ki_integer+(0.01f*pid_Ki_decimal));
					node3_pid_Kd=(float)(pid_Kd_integer+(0.01f*pid_Kd_decimal));



					if (!(Lora_SendData(TX_BUFFER,Lora_Data_Buf,4, Lora_Node3_Addr, Lora_Gateway_Addr,2))) {



						return 0;
					}




				}


			   	 Lora_Mode(MODE_RXCONTINUOUS);

			   	timer4_value=0;

			   	GPIOC->ODR &= ~(1 << 13);



	}

	return 1;

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





void signal_led_conf() {
    RCC->APB2ENR |= (1 << 4);

    GPIOC->CRH &= ~(15 << 20);
    GPIOC->CRH |=  (3 << 20);

    GPIOC->ODR &= ~(1 << 13);
}



void PID_Flow() {


		  PID_System();



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








