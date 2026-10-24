#include "main.h"
#include "lora.h"
#include "ssd1306_oled.h"
#include "role.h"
#include "gateway_main_conf_func.h"
#include "SIM800L.h"


uint8_t Lora_Node1_Read_ERR=0;
uint8_t Lora_Node2_Read_ERR=0;
uint8_t Lora_Node2_Write_ERR=0;
uint8_t Lora_Node3_Read_ERR=0;
uint8_t Lora_Node3_Write_ERR=0;


uint8_t Lora_Sender_ADDR=0;
uint8_t Lora_Receive_ADDR=0;
uint8_t Lora_Process=0;
uint8_t Lora_Main_Leng=0;


uint8_t Lora_Data_Buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


/////////////////////////////////////////////////////////////////////


// GATEWAY RELAY İÇİN DEĞİŞKENLER

uint8_t gateway_read_relay_status=0;
uint8_t gateway_write_relay_status=0;


// NODE1  İÇİN DEĞİŞKENLER

uint8_t read_moisture_value_integer;
uint8_t read_moisture_value_decimal;

uint8_t read_temperature_sign;

uint8_t read_temperature_value_integer;
uint8_t read_temperature_value_decimal;

float node1_moisture_value=0;
float node1_temperature_value=0;

/////////////////////////////////////////////////////////////////////

// NODE 2 İÇİN DEĞİŞKENLER

uint8_t read_relay_status_high;
uint8_t read_relay_status_low;

uint8_t write_relay_status_high=0;
uint8_t write_relay_status_low=0;


/////////////////////////////////////////////////////////////////////

// NODE 3 İÇİN DEĞİŞKENLER


uint8_t read_pid_desired_temperature_integer;
uint8_t read_pid_desired_temperature_decimal;

uint8_t read_pid_current_temperature_integer;
uint8_t read_pid_current_temperature_decimal;

uint8_t read_pid_Kp_integer;
uint8_t read_pid_Kp_decimal;

uint8_t read_pid_Ki_integer;
uint8_t read_pid_Ki_decimal;

uint8_t read_pid_Kd_integer;
uint8_t read_pid_Kd_decimal;


float node3_read_pid_desired_temperature=0;
float node3_read_pid_current_temperature=0;
float node3_read_pid_Kp=0;
float node3_read_pid_Ki=0;
float node3_read_pid_Kd=0;


/////////////////////////////////////////////////////////////////////

// NODE3 DEĞİŞKENLER

float node3_write_pid_desired_temperature=23;
float node3_write_pid_Kp=10;
float node3_write_pid_Ki=0.1;
float node3_write_pid_Kd=3;

uint8_t write_pid_desired_temperature_integer;
uint8_t write_pid_desired_temperature_decimal;


uint8_t write_pid_Kp_integer;
uint8_t write_pid_Kp_decimal;


uint8_t write_pid_Ki_integer;
uint8_t write_pid_Ki_decimal;


uint8_t write_pid_Kd_integer;
uint8_t write_pid_Kd_decimal;

/////////////////////////////////////////////////////////////////////

char Lora_Float_Show_Reg[6]={'\0','\0','\0','\0','\0','\0'};
char Lora_Relay_Show_Reg[9]={'\0','\0','\0','\0','\0','\0','\0','\0','\0'};
char Lora_Uint_Show_Reg[9]={'\0','\0','\0','\0','\0','\0','\0','\0','\0'};

/////////////////////////////////////////////////////////////////////

uint8_t Lora_Value_Show_Count=0;

uint16_t Lora_Tx_Wait_Time=3000;
uint16_t Lora_Rx_Wait_Time=5000;


/////////////////////////////////////////////////////////////////////

uint8_t Switch_Count=0;
uint8_t SIM800L_Key=0;


int main(void)
{

    // Konfigürasyonlar

    System_Clock_Config();
    Timer_Config();
    I2C1_Config();
    SSD1306_Config();
    SSD1306_Window_Clear();
    Lora_Reset_Pin_Config();

    SPI_Config();
    DMA_Config();
    USART_2_Config();
    DMA1_USART2_Config();

    Lora_Start_Conf();
    Lora_Config();
    Role_Config();

    SIM800L_Open_Pin_Config();
    SIM800L_Key=SIM800L_Config();

    WatchDog_Config();
    TIM3_Config();
    tim3_counter=1;


    while (1)
    {

    	  WatchDog_Feed();

  	      Main_8_Role_Driver();
  	      Lora_Value_Show();

  	     Lora_Query_Gateway();

    	  Node1_Flow();

    	  Delay(50);


          Node2_Flow();

          Delay(50);


          Node3_Flow();

          Delay(50);

              	WatchDog_Feed();

              	SIM800L_Work_Query();



              if ((tim3_counter > 0) && SIM800L_Key) {

          switch(Switch_Count) {

              case 0:

                  SIM800L_Internet_Value_Send_Float_Read(HTTPPARA_NODE3_DESIRED_TEMP_READ,&node3_write_pid_desired_temperature);
                  Switch_Count++;
                  TIM3_Reset();

                  break;

              case 1:
                  SIM800L_Internet_Value_Send_Float_Read(HTTPPARA_NODE3_KP_READ,&node3_write_pid_Kp);
                  Switch_Count++;
                  TIM3_Reset();


                  break;

              case 2:
                  SIM800L_Internet_Value_Send_Float_Read(HTTPPARA_NODE3_KI_READ,&node3_write_pid_Ki);
                  Switch_Count++;
                  TIM3_Reset();


                  break;

              case 3:
                  SIM800L_Internet_Value_Send_Float_Read(HTTPPARA_NODE3_KD_READ,&node3_write_pid_Kd);
                  Switch_Count++;
                  TIM3_Reset();

                  break;

              case 4:
                  SIM800L_Internet_Value_Send_Int_Read(HTTPPARA_GATEWAY_RELAY_READ,&gateway_write_relay_status);
                  Switch_Count++;
                  TIM3_Reset();

                  break;

              case 5:
                  SIM800L_Internet_Value_Send_Int_Read(HTTPPARA_NODE2_RELAY_HIGH_READ,&write_relay_status_high);
                  Switch_Count++;
                  TIM3_Reset();

                  break;

              case 6:
                  SIM800L_Internet_Value_Send_Int_Read(HTTPPARA_NODE2_RELAY_LOW_READ,&write_relay_status_low);
                  Switch_Count++;
                  TIM3_Reset();

                  break;

              case 7:
                  SIM800L_Internet_Value_Send_Float(HTTPPARA_NODE3_CURRENT_TEMP,&node3_read_pid_current_temperature);
                  Switch_Count++;
                  TIM3_Reset();

                  break;

              case 8:
                  SIM800L_Internet_Value_Send_Float(HTTPPARA_NODE1_MOISTURE,&node1_moisture_value);
                  Switch_Count++;
                  TIM3_Reset();

                  break;

              case 9:
                  SIM800L_Internet_Value_Send_Float(HTTPPARA_NODE1_TEMPERATURE,&node1_temperature_value);

                  Switch_Count=0;
                  TIM3_Reset();

                  break;

              default:
                  break;
          }

    	  }}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_Main_Err_Marking (uint8_t *ERR_Reg ,Err_t err) {

	*ERR_Reg |= (1 << err);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t Lora_Main_Err_Read(uint8_t *ERR_reg ,Err_t err) {

	if (*ERR_reg & (1 << err)) {

		return 1;

	}

	return 0;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Lora_Main_Err_Clean(uint8_t *ERR_reg ,Err_t err) {

	*ERR_reg &= ~(1 << err);

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


uint8_t Lora_Node1_Read() {

	WatchDog_Feed();

        Lora_Mode(MODE_STDBY);

        Lora_Buffer_Clean(TX_BUFFER);
        Lora_Buffer_Clean(RX_BUFFER);


        if (!Lora_SendData(TX_BUFFER, TX_BUFFER, 4, Lora_Gateway_Addr, Lora_Node1_Addr, 0)) {

                Lora_Main_Err_Marking(&Lora_Node1_Read_ERR,SEND_DATA_ERR);

                return 0;  }



        Lora_Mode(MODE_RXCONTINUOUS);

        while (!(Lora_IrqFlagsMask_Read(IRQ_RX_DONE))){
        	if (delay_ms(Lora_Rx_Wait_Time)) {

                Lora_Main_Err_Marking(&Lora_Node1_Read_ERR,RX_DONE_ERR);
                delay_stop();

                return 0;  }}

        delay_stop();




        Lora_Mode(MODE_STDBY);

        if (!(Lora_ReceiveData(RX_BUFFER, Lora_Data_Buf, &Lora_Main_Leng, &Lora_Sender_ADDR, &Lora_Receive_ADDR, &Lora_Process))) {

            Lora_Main_Err_Marking(&Lora_Node1_Read_ERR,RECEIVE_ERR);

        	return 0;     }



        if (Lora_Sender_ADDR != Lora_Node1_Addr) {

            Lora_Main_Err_Marking(&Lora_Node1_Read_ERR,SENDER_ADDR_ERR);

        	return 0;

        }


        if (Lora_Receive_ADDR != Lora_Gateway_Addr) {

        	Lora_Main_Err_Marking(&Lora_Node1_Read_ERR,RECEIVE_ADDR_ERR);

        	        	return 0;
        }



    read_moisture_value_integer = Lora_Data_Buf[0];
    read_moisture_value_decimal = Lora_Data_Buf[1];

    read_temperature_sign = Lora_Data_Buf[2];

    read_temperature_value_integer = Lora_Data_Buf[3];
    read_temperature_value_decimal = Lora_Data_Buf[4];


    if (read_temperature_sign == 0) {
        node1_temperature_value = read_temperature_value_integer + (0.01f * read_temperature_value_decimal);
    }

    else {
        node1_temperature_value = read_temperature_value_integer + (0.01f * read_temperature_value_decimal);
        node1_temperature_value = (-1) * node1_temperature_value;
    }

    node1_moisture_value = read_moisture_value_integer + (0.01f * read_moisture_value_decimal);


    Lora_Value_Reset();

    return 1;

}


uint8_t Lora_Node2_Write_Read() {

	WatchDog_Feed();

    	Lora_Mode(MODE_STDBY);

        Lora_Buffer_Clean(TX_BUFFER);
        Lora_Buffer_Clean(RX_BUFFER);


        Lora_Data_Buf[0]= write_relay_status_high;
        Lora_Data_Buf[1]= write_relay_status_low;



        if (!(Lora_SendData(TX_BUFFER, Lora_Data_Buf, 6, Lora_Gateway_Addr, Lora_Node2_Addr, 1))) {

        	 Lora_Main_Err_Marking(&Lora_Node2_Write_ERR, SEND_DATA_ERR);

        	                return 0;


        }

    	 Lora_Mode(MODE_RXCONTINUOUS);

    	  while (!(Lora_IrqFlagsMask_Read(IRQ_RX_DONE))){
    	        	if (delay_ms(Lora_Rx_Wait_Time)) {

    	                Lora_Main_Err_Marking(&Lora_Node2_Write_ERR, RX_DONE_ERR);
    	                delay_stop();

    	                return 0;  }}

        delay_stop();

   	 Lora_Mode(MODE_STDBY);


   	 if (!(Lora_ReceiveData(RX_BUFFER, Lora_Data_Buf, &Lora_Main_Leng, &Lora_Sender_ADDR, &Lora_Receive_ADDR, &Lora_Process))) {

   	  Lora_Main_Err_Marking(&Lora_Node2_Write_ERR, RECEIVE_ERR);

   	        	return 0;     }



   	 if (Lora_Sender_ADDR != Lora_Node2_Addr) {

   	            Lora_Main_Err_Marking(&Lora_Node2_Write_ERR, SENDER_ADDR_ERR);

   	        	return 0;

   	        }


   	if (Lora_Receive_ADDR != Lora_Gateway_Addr) {

   	       Lora_Main_Err_Marking(&Lora_Node2_Write_ERR, RECEIVE_ADDR_ERR);

   	        	        	return 0;

   	        }


   	if (Lora_Process != 2) {

   	  Lora_Main_Err_Marking(&Lora_Node2_Write_ERR, WRITE_PROCESS_ERR);

    	return 0;


   	}

    Lora_Value_Reset();



 	 Lora_Mode(MODE_STDBY);

   Lora_Buffer_Clean(TX_BUFFER);
   Lora_Buffer_Clean(RX_BUFFER);


   if (!Lora_SendData(TX_BUFFER, Lora_Data_Buf, 4, Lora_Gateway_Addr, Lora_Node2_Addr, 0)) {

   	 Lora_Main_Err_Marking(&Lora_Node2_Read_ERR, SEND_DATA_ERR);
   	 return 0;

   }


    Lora_Mode(MODE_RXCONTINUOUS);


    while (!(Lora_IrqFlagsMask_Read(IRQ_RX_DONE))){
           	if (delay_ms(Lora_Rx_Wait_Time)) {

                   Lora_Main_Err_Marking(&Lora_Node2_Read_ERR, RX_DONE_ERR);
                   delay_stop();

                   return 0;  }}


    delay_stop();

	 Lora_Mode(MODE_STDBY);


   if(!(Lora_ReceiveData(RX_BUFFER, Lora_Data_Buf, &Lora_Main_Leng, &Lora_Sender_ADDR, &Lora_Receive_ADDR, &Lora_Process))) {

   	Lora_Main_Err_Marking(&Lora_Node2_Read_ERR, RECEIVE_ERR);

   	        	return 0;
   }


   if (Lora_Sender_ADDR != Lora_Node2_Addr) {
   	Lora_Main_Err_Marking(&Lora_Node2_Read_ERR, SENDER_ADDR_ERR);
   	return 0;
   }



   if (Lora_Receive_ADDR != Lora_Gateway_Addr) {

        	Lora_Main_Err_Marking(&Lora_Node2_Read_ERR, RECEIVE_ADDR_ERR);

        	        	return 0;
        }


read_relay_status_high = Lora_Data_Buf[0];
read_relay_status_low  = Lora_Data_Buf[1];

Lora_Value_Reset();

return 1;


}




uint8_t Lora_Node3_Write_Read() {

	WatchDog_Feed();

    Lora_Mode(MODE_STDBY);

    Lora_Buffer_Clean(TX_BUFFER);
    Lora_Buffer_Clean(RX_BUFFER);

    write_pid_desired_temperature_integer = (uint8_t)node3_write_pid_desired_temperature;
    write_pid_desired_temperature_decimal = (uint8_t)(((float)node3_write_pid_desired_temperature - write_pid_desired_temperature_integer) * 100 + 0.5f);

    write_pid_Kp_integer = (uint8_t)node3_write_pid_Kp;
    write_pid_Kp_decimal = (uint8_t)(((float)node3_write_pid_Kp - write_pid_Kp_integer) * 100 + 0.5f);

    write_pid_Ki_integer = (uint8_t)node3_write_pid_Ki;
    write_pid_Ki_decimal = (uint8_t)(((float)node3_write_pid_Ki - write_pid_Ki_integer) * 100 + 0.5f);

    write_pid_Kd_integer = (uint8_t)node3_write_pid_Kd;
    write_pid_Kd_decimal = (uint8_t)(((float)node3_write_pid_Kd - write_pid_Kd_integer) * 100 + 0.5f);

    Lora_Data_Buf[0] = write_pid_desired_temperature_integer;
    Lora_Data_Buf[1] = write_pid_desired_temperature_decimal;
    Lora_Data_Buf[2] = write_pid_Kp_integer;
    Lora_Data_Buf[3] = write_pid_Kp_decimal;
    Lora_Data_Buf[4] = write_pid_Ki_integer;
    Lora_Data_Buf[5] = write_pid_Ki_decimal;
    Lora_Data_Buf[6] = write_pid_Kd_integer;
    Lora_Data_Buf[7] = write_pid_Kd_decimal;



    if (!(Lora_SendData(TX_BUFFER, Lora_Data_Buf, 12, Lora_Gateway_Addr, Lora_Node3_Addr, 1))) {
        Lora_Main_Err_Marking(&Lora_Node3_Write_ERR, SEND_DATA_ERR);
        return 0;
    }


    Lora_Mode(MODE_RXCONTINUOUS);

    while (!(Lora_IrqFlagsMask_Read(IRQ_RX_DONE))) {

        if (delay_ms(Lora_Rx_Wait_Time)) {
            Lora_Main_Err_Marking(&Lora_Node3_Write_ERR, RX_DONE_ERR);
            delay_stop();
            return 0;
        }
    }


    delay_stop();


    Lora_Mode(MODE_STDBY);

    if (!(Lora_ReceiveData(RX_BUFFER, Lora_Data_Buf, &Lora_Main_Leng, &Lora_Sender_ADDR, &Lora_Receive_ADDR, &Lora_Process))) {
        Lora_Main_Err_Marking(&Lora_Node3_Write_ERR, RECEIVE_ERR);
        return 0;
    }


    if (Lora_Sender_ADDR != Lora_Node3_Addr) {
        Lora_Main_Err_Marking(&Lora_Node3_Write_ERR, SENDER_ADDR_ERR);
        return 0;
    }


    if (Lora_Receive_ADDR != Lora_Gateway_Addr) {
        Lora_Main_Err_Marking(&Lora_Node3_Write_ERR, RECEIVE_ADDR_ERR);
        return 0;
    }

    if (Lora_Process != 2) {
        Lora_Main_Err_Marking(&Lora_Node3_Write_ERR, WRITE_PROCESS_ERR);
        return 0;
    }

    Lora_Value_Reset();



    Lora_Mode(MODE_STDBY);



       Lora_Buffer_Clean(TX_BUFFER);
       Lora_Buffer_Clean(RX_BUFFER);



       if (!Lora_SendData(TX_BUFFER, TX_BUFFER, 4, Lora_Gateway_Addr, Lora_Node3_Addr, 0)) {
           Lora_Main_Err_Marking(&Lora_Node3_Read_ERR, SEND_DATA_ERR);
           return 0;
       }


       Lora_Mode(MODE_RXCONTINUOUS);


       while (!(Lora_IrqFlagsMask_Read(IRQ_RX_DONE))) {
           if (delay_ms(Lora_Rx_Wait_Time)) {
               Lora_Main_Err_Marking(&Lora_Node3_Read_ERR, RX_DONE_ERR);
               delay_stop();
               return 0;
           }
       }

       delay_stop();


       Lora_Mode(MODE_STDBY);



       if (!(Lora_ReceiveData(RX_BUFFER, Lora_Data_Buf, &Lora_Main_Leng, &Lora_Sender_ADDR, &Lora_Receive_ADDR, &Lora_Process))) {
           Lora_Main_Err_Marking(&Lora_Node3_Read_ERR, RECEIVE_ERR);
           return 0;
       }



       if (Lora_Sender_ADDR != Lora_Node3_Addr) {
           Lora_Main_Err_Marking(&Lora_Node3_Read_ERR, SENDER_ADDR_ERR);
           return 0;
       }



       if (Lora_Receive_ADDR != Lora_Gateway_Addr) {
           Lora_Main_Err_Marking(&Lora_Node3_Read_ERR, RECEIVE_ADDR_ERR);
           return 0;
       }


       read_pid_desired_temperature_integer = Lora_Data_Buf[0];
       read_pid_desired_temperature_decimal = Lora_Data_Buf[1];

       read_pid_current_temperature_integer = Lora_Data_Buf[2];
       read_pid_current_temperature_decimal = Lora_Data_Buf[3];

       read_pid_Kp_integer = Lora_Data_Buf[4];
       read_pid_Kp_decimal = Lora_Data_Buf[5];

       read_pid_Ki_integer = Lora_Data_Buf[6];
       read_pid_Ki_decimal = Lora_Data_Buf[7];

       read_pid_Kd_integer = Lora_Data_Buf[8];
       read_pid_Kd_decimal = Lora_Data_Buf[9];

       node3_read_pid_desired_temperature = read_pid_desired_temperature_integer + (0.01f * read_pid_desired_temperature_decimal);
       node3_read_pid_current_temperature = read_pid_current_temperature_integer + (0.01f * read_pid_current_temperature_decimal);
       node3_read_pid_Kp                  = read_pid_Kp_integer + (0.01f * read_pid_Kp_decimal);
       node3_read_pid_Ki                  = read_pid_Ki_integer + (0.01f * read_pid_Ki_decimal);
       node3_read_pid_Kd                  = read_pid_Kd_integer + (0.01f * read_pid_Kd_decimal);


       Lora_Value_Reset();


       return 1;


}


void Node1_Flow() {

uint8_t Node1_Token=3;

	while (Node1_Token > 0) {


		 if (Lora_Node1_Read()) {

			 Lora_Main_Err_Clean(&Lora_Node1_Read_ERR, SEND_DATA_ERR);
			 Lora_Main_Err_Clean(&Lora_Node1_Read_ERR, RX_DONE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node1_Read_ERR, RECEIVE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node1_Read_ERR, SENDER_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node1_Read_ERR, RECEIVE_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node1_Read_ERR, FLOW_ERR);

			 break;
		 }

		 else {

			 Node1_Token--;

		 }}


	if (Node1_Token == 0) {

		Lora_Main_Err_Marking(&Lora_Node1_Read_ERR, FLOW_ERR);

	}



}


void Node2_Flow() {

uint8_t Node2_Token=3;

	while (Node2_Token > 0) {

		 if (Lora_Node2_Write_Read()) {

			 Lora_Main_Err_Clean(&Lora_Node2_Read_ERR, SEND_DATA_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Read_ERR, RX_DONE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Read_ERR, RECEIVE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Read_ERR, SENDER_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Read_ERR, RECEIVE_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Read_ERR, FLOW_ERR);

			 Lora_Main_Err_Clean(&Lora_Node2_Write_ERR, SEND_DATA_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Write_ERR, RX_DONE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Write_ERR, RECEIVE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Write_ERR, SENDER_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Write_ERR, RECEIVE_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node2_Write_ERR, WRITE_PROCESS_ERR);
			 break;

		 }


		 else {

			 Node2_Token--;

		 }



	}


	if (Node2_Token == 0) {

		Lora_Main_Err_Marking(&Lora_Node2_Read_ERR, FLOW_ERR);

	}



}




void Node3_Flow() {

uint8_t Node3_Token=3;

	while (Node3_Token > 0) {

		 if (Lora_Node3_Write_Read()) {

			 Lora_Main_Err_Clean(&Lora_Node3_Read_ERR, SEND_DATA_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Read_ERR, RX_DONE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Read_ERR, RECEIVE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Read_ERR, SENDER_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Read_ERR, RECEIVE_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Read_ERR, FLOW_ERR);

			 Lora_Main_Err_Clean(&Lora_Node3_Write_ERR, SEND_DATA_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Write_ERR, RX_DONE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Write_ERR, RECEIVE_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Write_ERR, SENDER_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Write_ERR, RECEIVE_ADDR_ERR);
			 Lora_Main_Err_Clean(&Lora_Node3_Write_ERR, WRITE_PROCESS_ERR);

break;

		 }


		 else {

			 Node3_Token--;


		 }





	}



	if (Node3_Token == 0) {

		Lora_Main_Err_Marking(&Lora_Node3_Read_ERR, FLOW_ERR);

	}



}





void Lora_Float_Show_Reg_Reset() {


	for (uint8_t x=0; x < 6; x++) {

		Lora_Float_Show_Reg[x]='\0';

	}



}
void Lora_Relay_Show_Reg_Reset() {

	for (uint8_t x=0; x < 8; x++) {

		Lora_Relay_Show_Reg[x]='\0';

		}

}


void Lora_Uint_Show_Reg_Reset() {

	for (uint8_t x=0; x < 9; x++) {

		Lora_Uint_Show_Reg[x]='\0';

		}}


void Lora_Value_Show() {

    switch (Lora_Value_Show_Counter) {

        case 0: {

            SSD1306_Window_Clear();

            Lora_Float_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_0);
            SSD1306_Float_Write_Show(&node1_temperature_value, Lora_Float_Show_Reg);
            SSD1306_Write("Node1 Sicaklik:", PAGE_0, 0);
            SSD1306_Write(Lora_Float_Show_Reg, PAGE_0, 76);

            Lora_Float_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_1);
            SSD1306_Float_Write_Show(&node1_moisture_value, Lora_Float_Show_Reg);
            SSD1306_Write("Node1 Nem: % ", PAGE_1, 0);
            SSD1306_Write(Lora_Float_Show_Reg, PAGE_1, 66);

            Lora_Relay_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_3);
            SSD1306_Relay_Write_Show(&read_relay_status_high, Lora_Relay_Show_Reg);
            SSD1306_Write("16_Role_H:", PAGE_3, 0);
            SSD1306_Write(Lora_Relay_Show_Reg, PAGE_3, 70);

            Lora_Relay_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_4);
            SSD1306_Relay_Write_Show(&read_relay_status_low, Lora_Relay_Show_Reg);
            SSD1306_Write("16_Role_L:", PAGE_4, 0);
            SSD1306_Write(Lora_Relay_Show_Reg, PAGE_4, 70);

            Lora_Relay_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_6);
            SSD1306_Relay_Write_Show(&gateway_read_relay_status, Lora_Relay_Show_Reg);
            SSD1306_Write("Gateway_Role:", PAGE_6, 0);
            SSD1306_Write(Lora_Relay_Show_Reg, PAGE_6, 76);

            Lora_Relay_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_7);
            SSD1306_Relay_Write_Show(&Lora_Error_Reg, Lora_Relay_Show_Reg);
            SSD1306_Write("Lora_Error_Reg:", PAGE_7, 0);
            SSD1306_Write(Lora_Relay_Show_Reg, PAGE_7, 76);

            Lora_Value_Show_Counter++;

            while(!(delay_ms(100)));
            delay_stop();

            break;
        }

        case 1: {

            SSD1306_Window_Clear();

            Lora_Float_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_0);
            SSD1306_Float_Write_Show(&node3_read_pid_current_temperature, Lora_Float_Show_Reg);
            SSD1306_Write("PID_Mevcut:", PAGE_0, 0);
            SSD1306_Write(Lora_Float_Show_Reg, PAGE_0, 56);

            Lora_Float_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_1);
            SSD1306_Float_Write_Show(&node3_read_pid_desired_temperature, Lora_Float_Show_Reg);
            SSD1306_Write("PID_Istenen:", PAGE_1, 0);
            SSD1306_Write(Lora_Float_Show_Reg, PAGE_1, 61);

            Lora_Float_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_2);
            SSD1306_Float_Write_Show(&node3_read_pid_Kp, Lora_Float_Show_Reg);
            SSD1306_Write("PID_Kp:", PAGE_2, 0);
            SSD1306_Write(Lora_Float_Show_Reg, PAGE_2, 36);

            Lora_Float_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_3);
            SSD1306_Float_Write_Show(&node3_read_pid_Ki, Lora_Float_Show_Reg);
            SSD1306_Write("PID_Ki:", PAGE_3, 0);
            SSD1306_Write(Lora_Float_Show_Reg, PAGE_3, 36);

            Lora_Float_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_4);
            SSD1306_Float_Write_Show(&node3_read_pid_Kd, Lora_Float_Show_Reg);
            SSD1306_Write("PID_Kd:", PAGE_4, 0);
            SSD1306_Write(Lora_Float_Show_Reg, PAGE_4, 36);

            Lora_Relay_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_5);
            SSD1306_Relay_Write_Show(&Lora_Error_Reg, Lora_Relay_Show_Reg);
            SSD1306_Write("Lora_Error_Reg:", PAGE_5, 0);
            SSD1306_Write(Lora_Relay_Show_Reg, PAGE_5, 76);

            Lora_Value_Show_Counter++;

            while(!(delay_ms(100)));
            delay_stop();

            break;
        }

        case 2: {

// casting (uint16_t*) işe yaramadığı için 8 bit yaptık

            SSD1306_Window_Clear();

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_0);
            SSD1306_Int8_Write(&Lora_Node1_Read_ERR, Lora_Uint_Show_Reg);
            SSD1306_Write("Node1_Read_ER:", PAGE_0, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_0, 70);

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_1);
            SSD1306_Int8_Write(&Lora_Node2_Read_ERR, Lora_Uint_Show_Reg);
            SSD1306_Write("Node2_Read_ER:", PAGE_1, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_1, 70);

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_2);
            SSD1306_Int8_Write(&Lora_Node2_Write_ERR, Lora_Uint_Show_Reg);
            SSD1306_Write("Node2_Writ_ER:", PAGE_2, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_2, 70);

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_3);
            SSD1306_Int8_Write(&Lora_Node3_Read_ERR, Lora_Uint_Show_Reg);
            SSD1306_Write("Node3_Read_ER:", PAGE_3, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_3, 70);

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_4);
            SSD1306_Int8_Write(&Lora_Node3_Write_ERR, Lora_Uint_Show_Reg);
            SSD1306_Write("Node3_Writ_ER:", PAGE_4, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_4, 70);

            Lora_Value_Show_Counter++;

            while(!(delay_ms(100)));
            delay_stop();

            break;
        }

        case 3: {

            SSD1306_Window_Clear();

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_0);
            SSD1306_Int_Write(&SIM800L_Config_Error, Lora_Uint_Show_Reg);
            SSD1306_Write("SIM800L_Conf_ERR:", PAGE_0, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_0, 95);

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_1);
            SSD1306_Int_Write(&SIM800L_Int_Read_Error, Lora_Uint_Show_Reg);
            SSD1306_Write("SIM800L_I_R_ERR:", PAGE_1, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_1, 95);

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_2);
            SSD1306_Int_Write(&SIM800L_Int_Send_Error, Lora_Uint_Show_Reg);
            SSD1306_Write("SIM800L_I_S_ERR:", PAGE_2, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_2, 95);

            Lora_Uint_Show_Reg_Reset();
            SSD1306_Page_Clear(PAGE_3);
            SSD1306_Int_Write(&SIM800L_Float_Read_Error, Lora_Uint_Show_Reg);
            SSD1306_Write("SIM800L_F_S_ERR:", PAGE_3, 0);
            SSD1306_Write(Lora_Uint_Show_Reg, PAGE_3, 95);

            Lora_Value_Show_Counter = 0;

            while(!(delay_ms(100)));
            delay_stop();

            break;
        }

        default: break;

    }
}




	void Main_8_Role_Driver(void) {


	    if (gateway_read_relay_status != gateway_write_relay_status) {
	        static const ROLE_NAME_t Role_Alignment[8] = {
	            ROLE_1, ROLE_2, ROLE_3, ROLE_4,
	            ROLE_5, ROLE_6, ROLE_7, ROLE_8
	        };


	        for (uint8_t x = 0; x < 8; x++) {
	            ROLE_NAME_t Available_Role = Role_Alignment[x];

	            if (gateway_write_relay_status & (1 << x)) {
	                Role_Drive(Available_Role, ROLE_SET);


	            } else {
	                Role_Drive(Available_Role, ROLE_RESET);
	            }
	        }

	        gateway_read_relay_status = gateway_write_relay_status;
	    }
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





void Lora_Main_Reset() {

	Lora_Start_Conf();
    Lora_Config();
    Lora_Mode(MODE_RXCONTINUOUS);

}


void Lora_Query_Gateway() {

	if ((SPI_Read(RegOpMode) & 0x80) != 0x80) {

	    Lora_Main_Reset();
	}
}




