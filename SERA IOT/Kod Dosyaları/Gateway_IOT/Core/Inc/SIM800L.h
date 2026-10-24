/*
 * SIM800L.h
 *
 *  Created on: Sep 22, 2026
 *      Author: kcigd
 */

#ifndef INC_SIM800L_H_
#define INC_SIM800L_H_

#include "main.h"
#include "gateway_main_conf_func.h"
#include "ssd1306_oled.h"

extern uint16_t SIM800L_Config_Error;
extern uint16_t SIM800L_Int_Send_Error;
extern uint16_t SIM800L_Int_Read_Error;
extern uint16_t SIM800L_Float_Read_Error;


#define SIM800L_FLOAT_READ_FAIL   (-9999.0f)

#define HTTPPARA_GATEWAY_RELAY        "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/gateway-relay/data\"\r\n"
#define HTTPPARA_NODE1_MOISTURE       "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node1-moisture-value/data\"\r\n"
#define HTTPPARA_NODE1_TEMPERATURE    "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node1-temperature-value/data\"\r\n"
#define HTTPPARA_NODE2_RELAY_HIGH     "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node2-relay-high/data\"\r\n"
#define HTTPPARA_NODE2_RELAY_LOW      "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node2-relay-low/data\"\r\n"
#define HTTPPARA_NODE3_CURRENT_TEMP   "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-current-temp/data\"\r\n"
#define HTTPPARA_NODE3_DESIRED_TEMP   "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-desired-temp/data\"\r\n"
#define HTTPPARA_NODE3_KD             "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-kd/data\"\r\n"
#define HTTPPARA_NODE3_KI             "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-ki/data\"\r\n"
#define HTTPPARA_NODE3_KP             "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-kp/data\"\r\n"


#define HTTPPARA_GATEWAY_RELAY_READ       "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/gateway-relay/data?limit=1\"\r\n"
#define HTTPPARA_NODE1_MOISTURE_READ      "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node1-moisture-value/data?limit=1\"\r\n"
#define HTTPPARA_NODE1_TEMPERATURE_READ   "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node1-temperature-value/data?limit=1\"\r\n"
#define HTTPPARA_NODE2_RELAY_HIGH_READ    "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node2-relay-high/data?limit=1\"\r\n"
#define HTTPPARA_NODE2_RELAY_LOW_READ     "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node2-relay-low/data?limit=1\"\r\n"
#define HTTPPARA_NODE3_CURRENT_TEMP_READ  "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/feeds/node3-current-temp/data?limit=1\"\r\n" 
#define HTTPPARA_NODE3_DESIRED_TEMP_READ  "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-desired-temp/data?limit=1\"\r\n"
#define HTTPPARA_NODE3_KD_READ            "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-kd/data?limit=1\"\r\n"
#define HTTPPARA_NODE3_KI_READ            "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-ki/data?limit=1\"\r\n"
#define HTTPPARA_NODE3_KP_READ            "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/v2/kcigdm60/groups/gateway-iot/feeds/node3-kp/data?limit=1\"\r\n"
#define HTTPPARA_CONTENT_JSON             "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n"
#define HTTPPARA_USERDATA_AIO_KEY         "AT+HTTPPARA=\"USERDATA\",\"X-AIO-Key:**************************"


extern uint16_t total_index;
extern uint8_t try;


typedef enum {

	SIM800L_ATE0_ERR,
    SIM800L_AT_ERR,
    SIM800L_CPIN_ERR,
    SIM800L_CREG_ERR,
    SIM800L_CGAAT_ERR,
    SIM800L_SAPBR_TYPE_ERR,
    SIM800L_SAPBR_APN_ERR,
    SIM800L_SAPBR_ENABLE_ERR,
    SIM800L_CONFIG_ERR,
	SIM800L_HTTP_INIT_ERR,
	SIM800L_HTTP_CID_ERR,
	SIM800L_SAPBR_QUERY_ERR

} SIM800L_Config_Error_t;


typedef enum {

    SIM800L_HTTPPARA_ERR,
	SIM800L_HTTPPARA_CONTENT_JSON_ERR,
	SIM800L_HTTPPARA_USERDATA_AIO_KEY_ERR,
	SIM800L_HTTPARA_DOWLOAD_ERR,
	SIM800L_HTTPARA_SEND_DATA_ERR,
	SIM800L_HTTPARA_COMPLATE_ERR

}SIM800L_Error_t;



typedef enum {


	SIM800L_HTTPPARA_READ_ERR,
	SIM800L_HTTPACTION_READ_ERR=3,
	SIM800L_HTTPARA_COMPLATE_READ_ERR=4,
	SIM800L_READ_FAIL,


}SIM800L_Read_Error_t;




#define SIM800_CMD_AT               "AT\r\n"
#define SIM800_CMD_CPIN             "AT+CPIN?\r\n"
#define SIM800_CMD_CREG             "AT+CREG?\r\n"
#define SIM800_CMD_CGATT            "AT+CGATT=1\r\n"
#define SIM800_CMD_SAPBR_TYPE       "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n"
#define SIM800_CMD_SAPBR_APN        "AT+SAPBR=3,1,\"APN\",\"internet\"\r\n"
#define SIM800_CMD_SAPBR_ENABLE     "AT+SAPBR=1,1\r\n"

#define SIM800_CMD_HTTP_INIT        "AT+HTTPINIT\r\n"
#define SIM800_CMD_HTTP_CID         "AT+HTTPPARA=\"CID\",1\r\n"
#define SIM800_CMD_HTTP_CONTENT_JSON "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n"
#define SIM800_CMD_HTTP_TERM        "AT+HTTPTERM\r\n"
#define SIM800_CMD_HTTP_READ        "AT+HTTPREAD\r\n"





#define SIM800_CMD_HTTPACTION_POST  "AT+HTTPACTION=1\r\n"
#define SIM800_CMD_HTTPACTION_GET   "AT+HTTPACTION=0\r\n"
#define SIM800_CMD_HTTPDATA_12B     "AT+HTTPDATA="
#define SIM800_CMD_HTTPDATA_12B2    ",5000\r\n"

#define SIM800_RESP_OK              "\r\nOK\r\n"
#define SIM800_RESP_ERROR           "\r\nERROR"
#define SIM800_RESP_CPIN_READY      "\r\n+CPIN: READY"
#define SIM800_RESP_DOWNLOAD        "\r\nDOWNLOAD"
#define SIM800_RESP_ACTION          "+HTTPACTION:"
#define SIM800_RESP_READ            "+HTTPREAD:"
#define SIM800_RESP_COMPLATE        "+HTTPACTION: 1,200"
#define SIM800_RESP_COMPLATE_READ    "+HTTPACTION: 0,200"

#define SIM800_CMD_SAPBR_QUERY       "AT+SAPBR=2,1\r\n"
#define SIM800_RESP_SAPBR_CONNECTED  "+SAPBR: 1,1"


uint8_t SIM800L_Config(void);
void SIM800L_Open_Pin_Config(void);
void SIM800L_Open_Pin(void);
void SIM800L_Close_Pin(void);
uint8_t SIM800L_Send(char *AT_Command);
uint8_t SIM800L_Reader(char *desired_value);
void SIM800L_Value_Copy(char *variable_to_copy, uint16_t starting_index, uint16_t total_data_copy);


uint8_t SIM800L_Get_Value_Int(void);
float SIM800L_Get_Value_Float(void);

void SIM800L_Error_Marking(uint8_t Error,uint16_t *reg);
uint8_t SIM800L_Error_Read(uint8_t Error,uint16_t *reg);
void SIM800L_Error_Clean(uint8_t Error,uint16_t *reg);


void SIM800L_Internet_Value_Send_Int(char *destination, uint8_t *value);
void SIM800L_Internet_Value_Send_Float(char *destination, float *value);

void SIM800L_Internet_Value_Send_Int_Read(char *destination,uint8_t *reg);
void SIM800L_Internet_Value_Send_Float_Read(char *destination, float *reg);
float SIM800L_Get_Value_Float(void);
void SIM800L_Work_Query();

void SDD_SHOW();
void Delay(volatile uint16_t ms);
void SIM800L_Int_Write(uint8_t *value, char *int_to_char);
void SIM800L_Float_Write_Show(float *value, char *float_to_char);
void SIM800L_16Int_Write(uint16_t *value, char *int_to_char);
void SIM800L_Clear_Rx_Buffer(void);
uint8_t SIM800L_Check_Network_Status(void);
#endif /* INC_SIM800L_H_ */
