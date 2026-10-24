
/*
 * SIM800L.c
 *
 *  Created on: Sep 22, 2026
 *      Author: kcigd
 */

#include "SIM800L.h"

uint16_t total_index = 0;
uint8_t try = 0;

uint16_t SIM800L_Config_Error=0;
uint16_t SIM800L_Int_Send_Error=0;
uint16_t SIM800L_Int_Read_Error=0;
uint16_t SIM800L_Float_Read_Error=0;


void SIM800L_Open_Pin_Config() {
    // PC 13
    RCC->AHB1ENR |= (1 << 2);

    GPIOC->MODER &= ~(3 << (13*2));
    GPIOC->MODER |= (1 << (13*2));

    GPIOC->OTYPER &= ~(1 << 13);
    GPIOC->OSPEEDR |= (3 << (13*2));

    GPIOC->PUPDR &= ~(3 << (13*2));
    GPIOC->PUPDR |= (2 << (13*2));
}


void SIM800L_Open_Pin() {
    GPIOC->ODR |= (1 << 13);
    while(!(delay_ms(2000)));
    delay_stop();

    GPIOC->ODR &= ~(1 << 13);
    while(!(delay_ms(2000)));
    delay_stop();

    GPIOC->ODR |= (1 << 13);
}


void SIM800L_Close_Pin() {
    GPIOC->ODR &= ~(1 << 13);
    while(!(delay_ms(1000)));
    delay_stop();

    GPIOC->ODR |= (1 << 13);
}


uint8_t SIM800L_Send(char *AT_Command) {
    return USART_Send(AT_Command);
}

uint8_t SIM800L_Reader(char *desired_value) {
    return Usart_Reader(desired_value);
}


void SIM800L_Value_Copy(char *variable_to_copy, uint16_t starting_index, uint16_t total_data_copy) {
    uint16_t curr_index = (usart_index + starting_index) % 1024;

    for (uint16_t y = 0; y < total_data_copy; y++) {
        variable_to_copy[y] = Usart_Rx[curr_index];
        curr_index = (curr_index + 1) % 1024;
    }
}

void SIM800L_Error_Marking(uint8_t Error,uint16_t *reg) {
    *reg |= (1 << Error);
}

uint8_t SIM800L_Error_Read(uint8_t Error,uint16_t *reg) {
    if (*reg & (1 << Error)) {
        return 1;
    }
    return 0;
}

void SIM800L_Error_Clean(uint8_t Error,uint16_t *reg) {

    *reg &= ~(1 << Error);
}

void SIM800L_Int_Write(uint8_t *value, char *int_to_char) {
    char figures[11] = {'0','1','2','3','4','5','6','7','8','9','\0'};
    uint8_t hundred = (*value / 100);
    uint8_t tens = (*value % 100) / 10;
    uint8_t ones = (*value % 10);

    if (hundred == 0) {
        if (tens == 0) {
            int_to_char[0] = figures[ones];
            int_to_char[1] = '\0';
        } else {
            int_to_char[0] = figures[tens];
            int_to_char[1] = figures[ones];
            int_to_char[2] = '\0';
        }
    } else {
        int_to_char[0] = figures[hundred];
        int_to_char[1] = figures[tens];
        int_to_char[2] = figures[ones];
        int_to_char[3] = '\0';
    }
}

void SIM800L_Float_Write_Show(float *value, char *float_to_char) {
    uint8_t integer_value = (uint8_t)*value;
    uint8_t decimal_value = (uint8_t)((*value - integer_value) * 100 + 0.5f);

    char figures[10] = {'0','1','2','3','4','5','6','7','8','9'};

    uint8_t int_hundred = (integer_value / 100);
    uint8_t int_tens = (integer_value % 100) / 10;
    uint8_t int_ones = (integer_value % 10);
    uint8_t float_tens = (decimal_value % 100) / 10;
    uint8_t float_ones = (decimal_value % 10);

    if (int_hundred != 0) {
        float_to_char[0] = figures[int_hundred];
        float_to_char[1] = figures[int_tens];
        float_to_char[2] = figures[int_ones];
        float_to_char[3] = '.';
        float_to_char[4] = figures[float_tens];
        float_to_char[5] = figures[float_ones];
        float_to_char[6] = '\0';
    } else {
        if (int_tens != 0) {
            float_to_char[0] = figures[int_tens];
            float_to_char[1] = figures[int_ones];
            float_to_char[2] = '.';
            float_to_char[3] = figures[float_tens];
            float_to_char[4] = figures[float_ones];
            float_to_char[5] = '\0';
        } else {
            float_to_char[0] = figures[int_ones];
            float_to_char[1] = '.';
            float_to_char[2] = figures[float_tens];
            float_to_char[3] = figures[float_ones];
            float_to_char[4] = '\0';
        }
    }
}




uint8_t SIM800L_Config(void) {

    uint16_t lag = 50;
    uint16_t lag_try = 50;

    SIM800L_Open_Pin();

     uint8_t try = 5;
     uint8_t ok = 0;
     uint16_t current_lag_try=0;

    while (try > 0) {


        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send("ATE0\r\n");

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_ATE0_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_ATE0_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_AT);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_AT_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_AT_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////

        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_CPIN);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_CPIN_READY)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_CPIN_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_CPIN_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////

        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_CREG);

        while(current_lag_try > 0) {
            if (SIM800L_Reader("\r\n+CREG: 0,1")) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_CREG_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_CREG_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_CGATT);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_CGAAT_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_CGAAT_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_SAPBR_TYPE);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_SAPBR_TYPE_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_SAPBR_TYPE_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_SAPBR_APN);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_SAPBR_APN_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_SAPBR_APN_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_SAPBR_ENABLE);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_SAPBR_ENABLE_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_SAPBR_ENABLE_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_HTTP_INIT);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTP_INIT_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTP_INIT_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_HTTP_CID);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTP_CID_ERR, &SIM800L_Config_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTP_CID_ERR, &SIM800L_Config_Error);
            try--;
            continue;
        }

       ////////////////////////////////////////////////////////////////////////////////////////// // burası

        WatchDog_Feed();

              ok = 0;
              current_lag_try = lag_try;
              SIM800L_Clear_Rx_Buffer();
              SIM800L_Send(SIM800_CMD_SAPBR_QUERY);

              while(current_lag_try > 0) {
                  if (SIM800L_Reader(SIM800_RESP_SAPBR_CONNECTED)) {
                      ok = 1;
                      break;
                  }
                  Delay(lag);
                  current_lag_try--;
              }

              if (ok == 1) {
                  SIM800L_Error_Clean(SIM800L_SAPBR_QUERY_ERR, &SIM800L_Config_Error);
              } else {
                  SIM800L_Error_Marking(SIM800L_SAPBR_QUERY_ERR, &SIM800L_Config_Error);
                  try--;
                  continue;
              }




        break;
    }

    if (try == 0) {
        SIM800L_Error_Marking(SIM800L_CONFIG_ERR, &SIM800L_Config_Error);


        return 0;
    }
    else {
        SIM800L_Error_Clean(SIM800L_AT_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_CPIN_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_CREG_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_CGAAT_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_SAPBR_TYPE_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_SAPBR_APN_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_SAPBR_ENABLE_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_HTTP_INIT_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_HTTP_CID_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_HTTPPARA_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_SAPBR_QUERY_ERR, &SIM800L_Config_Error);
        SIM800L_Error_Clean(SIM800L_CONFIG_ERR, &SIM800L_Config_Error);


        return 1;
    }
}

void SIM800L_Internet_Value_Send_Int(char *destination, uint8_t *value) {

    uint8_t y = 0;
    uint8_t x = 0;
    volatile uint16_t lag = 25;
    uint16_t lag_try = 100;

    char send_data[50];
    char data[15];

    for(x = 0; x < 50; x++) { send_data[x] = '\0'; }
    for(x = 0; x < 15; x++) { data[x] = '\0'; }

    send_data[0] = '{';
    send_data[1] = '"';
    send_data[2] = 'v';
    send_data[3] = 'a';
    send_data[4] = 'l';
    send_data[5] = 'u';
    send_data[6] = 'e';
    send_data[7] = '"';
    send_data[8] = ':';
    send_data[9] = ' ';

    y = 10;

    SIM800L_Int_Write(value, data);

    for (x = 0; data[x] != '\0'; x++) {
        send_data[y] = data[x];
        y++;
    }

    send_data[y] = '}';
    y++;
    send_data[y] = '\0';

    //////////////////////////////////////////////////////////////////////////////////////////
    char send_data_len_tim[32];
    uint8_t value_len = 0;

    for(x = 0; x < 32; x++) { send_data_len_tim[x] = '\0'; }
    for(x = 0; x < 15; x++) { data[x] = '\0'; }

    value_len = str_len(send_data);
    SIM800L_Int_Write(&value_len, data);

    send_data_len_tim[0]  = 'A';
    send_data_len_tim[1]  = 'T';
    send_data_len_tim[2]  = '+';
    send_data_len_tim[3]  = 'H';
    send_data_len_tim[4]  = 'T';
    send_data_len_tim[5]  = 'T';
    send_data_len_tim[6]  = 'P';
    send_data_len_tim[7]  = 'D';
    send_data_len_tim[8]  = 'A';
    send_data_len_tim[9]  = 'T';
    send_data_len_tim[10] = 'A';
    send_data_len_tim[11] = '=';

    y = 12;

    for(x = 0; data[x] != '\0'; x++) {
        send_data_len_tim[y] = data[x];
        y++;
    }

    send_data_len_tim[y++] = ',';
    send_data_len_tim[y++] = '1';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '\r';
    send_data_len_tim[y++] = '\n';
    send_data_len_tim[y]   = '\0';

    //////////////////////////////////////////////////////////////////////////////////////////
    uint8_t try = 5;
    uint8_t ok = 0;

    while(try > 0) {

        WatchDog_Feed();

        ok = 0;
        uint16_t current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(destination);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_CONTENT_JSON);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_USERDATA_AIO_KEY);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(send_data_len_tim);

        while(current_lag_try > 0) {
            if (SIM800L_Reader("\r\nDOWNLOAD")) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPARA_DOWLOAD_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPARA_DOWLOAD_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(send_data);
        Delay(lag);

        SIM800L_Send(SIM800_CMD_HTTPACTION_POST);

        ok = 0;
        current_lag_try = lag_try;
        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPARA_SEND_DATA_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPARA_SEND_DATA_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        {
            uint8_t wait = 100;
            uint8_t action_ok = 0;

            while (wait > 0) {
                WatchDog_Feed();

                if (SIM800L_Reader(SIM800_RESP_COMPLATE)) {
                    action_ok = 1;
                    break;
                }
                Delay(100);
                wait--;
            }

            if (!action_ok) {
                SIM800L_Error_Marking(SIM800L_HTTPARA_COMPLATE_ERR, &SIM800L_Int_Send_Error);
                try--;
                continue;
            } else {
                SIM800L_Error_Clean(SIM800L_HTTPARA_COMPLATE_ERR, &SIM800L_Int_Send_Error);
            }
        }

        break;
    }
}




void SIM800L_Internet_Value_Send_Float(char *destination, float *value) {

    WatchDog_Feed();

    uint8_t y = 0;
    uint8_t x = 0;
    volatile uint16_t lag = 25;
    uint16_t lag_try = 100;

    char send_data[50];
    char data[15];

    for(x = 0; x < 50; x++) { send_data[x] = '\0'; }
    for(x = 0; x < 15; x++) { data[x] = '\0'; }

    send_data[0] = '{';
    send_data[1] = '"';
    send_data[2] = 'v';
    send_data[3] = 'a';
    send_data[4] = 'l';
    send_data[5] = 'u';
    send_data[6] = 'e';
    send_data[7] = '"';
    send_data[8] = ':';
    send_data[9] = ' ';

    y = 10;

    SIM800L_Float_Write_Show(value, data);

    for (x = 0; data[x] != '\0'; x++) {
        send_data[y] = data[x];
        y++;
    }

    send_data[y] = '}';
    y++;
    send_data[y] = '\0';

    WatchDog_Feed();

    //////////////////////////////////////////////////////////////////////////////////////////
    char send_data_len_tim[32];
    uint8_t value_len = 0;

    for(x = 0; x < 32; x++) { send_data_len_tim[x] = '\0'; }
    for(x = 0; x < 15; x++) { data[x] = '\0'; }

    value_len = str_len(send_data);
    SIM800L_Int_Write(&value_len, data);

    send_data_len_tim[0]  = 'A';
    send_data_len_tim[1]  = 'T';
    send_data_len_tim[2]  = '+';
    send_data_len_tim[3]  = 'H';
    send_data_len_tim[4]  = 'T';
    send_data_len_tim[5]  = 'T';
    send_data_len_tim[6]  = 'P';
    send_data_len_tim[7]  = 'D';
    send_data_len_tim[8]  = 'A';
    send_data_len_tim[9]  = 'T';
    send_data_len_tim[10] = 'A';
    send_data_len_tim[11] = '=';

    y = 12;

    for(x = 0; data[x] != '\0'; x++) {
        send_data_len_tim[y] = data[x];
        y++;
    }

    send_data_len_tim[y++] = ',';
    send_data_len_tim[y++] = '1';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '0';
    send_data_len_tim[y++] = '\r';
    send_data_len_tim[y++] = '\n';
    send_data_len_tim[y]   = '\0';

    //////////////////////////////////////////////////////////////////////////////////////////
    WatchDog_Feed();

    uint8_t try = 5;
    uint8_t ok = 0;

    while(try > 0) {

        WatchDog_Feed();

        ok = 0;
        uint16_t current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(destination);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_CONTENT_JSON);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_USERDATA_AIO_KEY);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(send_data_len_tim);

        while(current_lag_try > 0) {
            if (SIM800L_Reader("\r\nDOWNLOAD")) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPARA_DOWLOAD_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPARA_DOWLOAD_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(send_data);
        Delay(lag);

        SIM800L_Send(SIM800_CMD_HTTPACTION_POST);

        ok = 0;
        current_lag_try = lag_try;
        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPARA_SEND_DATA_ERR, &SIM800L_Int_Send_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPARA_SEND_DATA_ERR, &SIM800L_Int_Send_Error);
            try--;
            continue;
        }

        {
            uint8_t wait = 110;
            uint8_t action_ok = 0;

            while (wait > 0) {
                WatchDog_Feed();

                if (SIM800L_Reader(SIM800_RESP_COMPLATE)) {
                    action_ok = 1;
                    break;
                }
                Delay(100);
                wait--;
            }

            if (!action_ok) {
                SIM800L_Error_Marking(SIM800L_HTTPARA_COMPLATE_ERR, &SIM800L_Int_Send_Error);
                try--;
                continue;
            } else {
                SIM800L_Error_Clean(SIM800L_HTTPARA_COMPLATE_ERR, &SIM800L_Int_Send_Error);
            }
        }

        break;
    }
}


void SIM800L_Internet_Value_Send_Int_Read(char *destination, uint8_t *reg) {

    uint8_t value = 0;
    uint8_t try = 10;
    uint16_t lag = 25;
    uint16_t lag_try = 100;
    uint8_t ok = 0;

    while(try > 0) {

        WatchDog_Feed();

        ok = 0;
        uint16_t current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(destination);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_READ_ERR, &SIM800L_Int_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_READ_ERR, &SIM800L_Int_Read_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_CONTENT_JSON);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Int_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Int_Read_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_USERDATA_AIO_KEY);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Read_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_HTTPACTION_GET);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Read_Error);
            try--;
            continue;
        }

        {
            uint8_t wait = 150;
            uint8_t action_ok = 0;

            WatchDog_Feed();
            while (wait > 0) {
                WatchDog_Feed();

                if (SIM800L_Reader(SIM800_RESP_COMPLATE_READ)) {
                    action_ok = 1;
                    break;
                }
                Delay(100);
                wait--;
            }

            if (!action_ok) {
                SIM800L_Error_Marking(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Read_Error);
                try--;
                continue;
            } else {
                SIM800L_Error_Clean(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Int_Read_Error);
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_HTTP_READ);

        {
            uint16_t current_lag_try = lag_try;
            value = 0;

            while (current_lag_try > 0) {
                WatchDog_Feed();
                value = SIM800L_Get_Value_Int();
                if (value != 0) {
                    break;
                }
                Delay(lag);
                current_lag_try--;
            }
        }

        *reg = value;
        break;
    }
}




void SIM800L_Internet_Value_Send_Float_Read(char *destination, float *reg) {

    float value = 0.0f;
    uint8_t try = 10;
    uint16_t lag = 25;
    uint16_t lag_try = 100;
    uint8_t ok = 0;

    while(try > 0) {

        WatchDog_Feed();

        ok = 0;
        uint16_t current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(destination);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_READ_ERR, &SIM800L_Float_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_READ_ERR, &SIM800L_Float_Read_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_CONTENT_JSON);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }



        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Float_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_CONTENT_JSON_ERR, &SIM800L_Float_Read_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(HTTPPARA_USERDATA_AIO_KEY);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Float_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR, &SIM800L_Float_Read_Error);
            try--;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        WatchDog_Feed();

        ok = 0;
        current_lag_try = lag_try;
        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_HTTPACTION_GET);

        while(current_lag_try > 0) {
            if (SIM800L_Reader(SIM800_RESP_OK)) {
                ok = 1;
                break;
            }
            Delay(lag);
            current_lag_try--;
        }

        if (ok == 1) {
            SIM800L_Error_Clean(SIM800L_HTTPACTION_READ_ERR, &SIM800L_Float_Read_Error);
        } else {
            SIM800L_Error_Marking(SIM800L_HTTPACTION_READ_ERR, &SIM800L_Float_Read_Error);
            try--;
            continue;
        }

        {
            WatchDog_Feed();

            uint8_t wait = 150;
            uint8_t action_ok = 0;

            while (wait > 0) {
                WatchDog_Feed();

                if (SIM800L_Reader(SIM800_RESP_COMPLATE_READ)) {
                    action_ok = 1;
                    break;
                }
                Delay(100);
                wait--;
            }

            if (!action_ok) {
                SIM800L_Error_Marking(SIM800L_HTTPARA_COMPLATE_READ_ERR, &SIM800L_Float_Read_Error);
                try--;
                continue;
            } else {
                SIM800L_Error_Clean(SIM800L_HTTPARA_COMPLATE_READ_ERR, &SIM800L_Float_Read_Error);
            }
        }

        WatchDog_Feed();
        //////////////////////////////////////////////////////////////////////////////////////////////////////////

        SIM800L_Clear_Rx_Buffer();
        SIM800L_Send(SIM800_CMD_HTTP_READ);

        {
            uint16_t current_lag_try = lag_try;
            uint8_t read_ok = 0;
            value = SIM800L_FLOAT_READ_FAIL;

            while (current_lag_try > 0) {
                WatchDog_Feed();
                value = SIM800L_Get_Value_Float();
                if (value != SIM800L_FLOAT_READ_FAIL) {
                    read_ok = 1;
                    break;
                }
                Delay(lag);
                current_lag_try--;
            }

            if (!read_ok) {
                SIM800L_Error_Marking(SIM800L_READ_FAIL, &SIM800L_Float_Read_Error);
                try--;
                continue;
            } else {
                SIM800L_Error_Clean(SIM800L_READ_FAIL, &SIM800L_Float_Read_Error);
            }
        }

        *reg = value;
        break;
    }
}





void SIM800L_16Int_Write(uint16_t *value, char *int_to_char) {
    char figures[11] = {'0','1','2','3','4','5','6','7','8','9','\0'};
    uint16_t val = *value;

    uint8_t thousands = val / 1000;
    uint8_t hundreds = (val % 1000) / 100;
    uint8_t tens = (val % 100) / 10;
    uint8_t ones = val % 10;

    uint8_t idx = 0;
    if (thousands > 0) int_to_char[idx++] = figures[thousands];
    if (thousands > 0 || hundreds > 0) int_to_char[idx++] = figures[hundreds];
    if (thousands > 0 || hundreds > 0 || tens > 0) int_to_char[idx++] = figures[tens];
    int_to_char[idx++] = figures[ones];
    int_to_char[idx] = '\0';
}



void SIM800L_Clear_Rx_Buffer(void) {

	uint16_t x=0;

	while(x < 1024) {

		Usart_Rx[x]='\0';
		x++;

	}
	Usart_Flag_Clean(IDLE);

	usart_index=0;
	coming_data_len=0;

}


void Delay(volatile uint16_t ms) {

	while(!(delay_ms(ms)));
	delay_stop();



}



// DİREK YAPAY ZEKA İLE YAPILDI

uint8_t SIM800L_Get_Value_Int(void) {

    // Adafruit IO cevabi:  +HTTPREAD: 250 \r\n {"id":"..","value":"25","feed_id":..}
    char target[] = "\"value\":";
    uint16_t target_len = str_len(target);
    uint16_t start = 0;
    uint16_t k = 0;
    uint16_t pos = 0;
    uint16_t result = 0;
    uint8_t found = 0;
    uint8_t digit_count = 0;

    // 1. "value": ifadesini ara.
    //    Usart_Rx DMA'nin dairesel tamponu: cevap 1023'ten 0'a tasmis olabilir,
    //    o yuzden her karaktere (indeks & 1023) ile erisiyoruz.
    for (start = 0; start < 1024; start++) {

        for (k = 0; k < target_len; k++) {
            if (Usart_Rx[(start + k) & 1023] != target[k]) {
                break;
            }
        }

        if (k == target_len) {
            found = 1;
            pos = (start + target_len) & 1023;   // "value": ifadesinden hemen sonraki karakter
            break;
        }
    }

    if (!found) {
        return 0;
    }

    // 2. Bosluk ve acilis tirnagini ('"') atla
    k = 0;
    while ((Usart_Rx[pos] == ' ' || Usart_Rx[pos] == '"') && k < 4) {
        pos = (pos + 1) & 1023;
        k++;
    }

    // 3. Rakamlari oku (char -> int), rakam disi karakterde (", '.', ',' ...) dur
    while (Usart_Rx[pos] >= '0' && Usart_Rx[pos] <= '9' && digit_count < 5) {
        result = (result * 10) + (Usart_Rx[pos] - '0');
        digit_count++;
        pos = (pos + 1) & 1023;
    }

    if (digit_count == 0) {
        return 0;
    }

    if (result > 255) {
        result = 255;   // uint8_t sinirini asma
    }

    return (uint8_t)result;
}




// DİREK YAPAY ZEKA İLE YAPILDI

float SIM800L_Get_Value_Float(void) {

    // Adafruit IO cevabi:  +HTTPREAD: 250 \r\n {"id":"..","value":"25.75","feed_id":..}
    char target[] = "\"value\":";
    uint16_t target_len = str_len(target);
    uint16_t start = 0;
    uint16_t k = 0;
    uint16_t pos = 0;
    uint8_t found = 0;
    uint8_t negative = 0;
    uint8_t digit_count = 0;
    float result = 0.0f;
    float scale = 0.1f;

    // 1. "value": ifadesini ara (dairesel tampon, indeks & 1023)
    for (start = 0; start < 1024; start++) {

        for (k = 0; k < target_len; k++) {
            if (Usart_Rx[(start + k) & 1023] != target[k]) {
                break;
            }
        }

        if (k == target_len) {
            found = 1;
            pos = (start + target_len) & 1023;
            break;
        }
    }

    if (!found) {
        return SIM800L_FLOAT_READ_FAIL;
    }

    // 2. Bosluk ve acilis tirnagini atla
    k = 0;
    while ((Usart_Rx[pos] == 0x20 || Usart_Rx[pos] == 0x22) && k < 4) {
        pos = (pos + 1) & 1023;
        k++;
    }

    // 3. Eksi isareti
    if (Usart_Rx[pos] == 0x2D) {
        negative = 1;
        pos = (pos + 1) & 1023;
    }

    // 4. Tam kisim (char -> sayi)
    while (Usart_Rx[pos] >= 0x30 && Usart_Rx[pos] <= 0x39 && digit_count < 9) {
        result = (result * 10.0f) + (float)(Usart_Rx[pos] - 0x30);
        digit_count++;
        pos = (pos + 1) & 1023;
    }

    // 5. Ondalik kisim (.)
    if (Usart_Rx[pos] == 0x2E) {
        pos = (pos + 1) & 1023;

        while (Usart_Rx[pos] >= 0x30 && Usart_Rx[pos] <= 0x39 && digit_count < 9) {
            result += (float)(Usart_Rx[pos] - 0x30) * scale;
            scale /= 10.0f;
            digit_count++;
            pos = (pos + 1) & 1023;
        }
    }

    if (digit_count == 0) {
        return SIM800L_FLOAT_READ_FAIL;
    }

    if (negative) {
        result = -result;
    }

    return result;
}




void SIM800L_Work_Query() {

	uint16_t try=100;
	uint16_t lag=30;
	uint8_t ok=0;

	    WatchDog_Feed();
	    SIM800L_Clear_Rx_Buffer();
	    SIM800L_Send(SIM800_CMD_SAPBR_QUERY);

	    while(try > 0) {
	        if (SIM800L_Reader(SIM800_RESP_SAPBR_CONNECTED)) {
	            ok = 1;
	            break;
	        }
	        Delay(lag);
	        try--;
	    }

	    SIM800L_Key=ok;
	    WatchDog_Feed();

	if (!ok) {

	SIM800L_Key=SIM800L_Config();

	}





}





