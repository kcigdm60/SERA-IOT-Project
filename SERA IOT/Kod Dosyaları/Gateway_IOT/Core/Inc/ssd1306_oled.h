/*
 * ssd1306_oled.h
 *
 *  Created on: Jun 17, 2026
 *      Author: kcigd
 */

#ifndef INC_SSD1306_OLED_H_
#define INC_SSD1306_OLED_H_

#include "main.h"


extern uint8_t empty_buffer[8];
extern uint16_t str_len(const char *string);
extern uint8_t Lora_Error_Reg;
extern char Usart_Rx[1024];

extern const uint8_t ASCII_Font_5x7[][5];





/////////////////////////// FUNDAMENTAL COMMAND TABLE /////////////////////////


///////////////////////// Slave Adress  ////////////////////////////

#define SLAVE_ADDR               0x3C

///////////////////////// Control Byte /////////////////////////////

#define COMMAND_SEND             0x00
#define PIXEL_SEND               0x40

////////////////////////// Set Contrast Control  ///////////////////

#define SET_CONTRAST_CTRL 	     0x81

/////////////////////////  Entire Display Control   ////////////////

#define ENTIRE_DISPLAY_NORMAL       0xA4
#define ENTIRE_DISPLAY_ALL_ON       0xA5

////////////////////////  Set Normal / Inverse Display /////////////

#define SET_NORMAL 					0xA6
#define SET_INVERSE					0xA7

////////////////////////  Set Display ON/OFF  //////////////////////

#define CHARGE_PUMP_ENABLE   		0x14


////////////////////////  SCROLLING COMMAND TABLE  ////////////////////////


//////////////////////// Horizontal Scroll ////////////////////////

///////////////// PART 1 ////////////////

typedef enum {

	Right_Horizontal_Scroll=0x26,
	Left_Horizontal_Scroll=0x27

}Horizontal_Scroll_t;


typedef enum {

	PAGE_0 = 0x00,
	PAGE_1 = 0x01,
	PAGE_2 = 0x02,
	PAGE_3 = 0x03,
	PAGE_4 = 0x04,
	PAGE_5 = 0x05,
	PAGE_6 = 0x06,
	PAGE_7 = 0x07


}Page_Adress_t;


typedef enum {

	FRAMES_5   = 0x00,
    FRAMES_64  = 0x01,
	FRAMES_128 = 0x02,
	FRAMES_256 = 0x03,
	FRAMES_3   = 0x04,
    FRAMES_4   = 0x05,
	FRAMES_25  = 0x06,
	FRAMES_2   = 0x07


}Frame_Frequency_t;

typedef enum {

DISPLAY_OFF=0xAE,
DISPLAY_ON=0xAF

}On_Off_t;





typedef enum {

	Vertical_Right_Horizontal_Scroll = 0x29,
	Vertical_Left_Horizontal_Scroll = 0x2A,


}Vertical_Horizontal_Scroll_t;



typedef enum {

	Horizontal_Addressing_Mode = 0x00,
	Vertical_Addressing_Mode= 0x01,
	Page_Addressing_Mode = 0x02,


}Set_Memory_Addr_Mode_t;




typedef enum {

	NORMAL_MOD=0x00,
	MIRROR_MOD=0x01


}Set_Segment_Remap_t;


typedef enum {

	COM_NORMAL_MOD=0xC0,
	COM_INVRTED_MODE= 0xC8

}Set_COM_Output_Scan_t;

typedef enum {

    COM_PINS_SEQUENTIAL  = 0x00,
    COM_PINS_ALTERNATIVE = 0x01

} COM_Pins_Config_t;


typedef enum {

    COM_REMAP_DISABLE = 0x00,
    COM_REMAP_ENABLE  = 0x01

} COM_Remap_Config_t;


typedef enum {

	_0_65_MULTIPLIER=0x00,
	_0_77_MULTIPLIER=0x20,
	_0_83_MULTIPLIER=0x30

}Voltage_Level_t;












extern void I2C1_Write(uint8_t control_byte,uint8_t* data_array,uint8_t data_size);

void SSD1306_Data_Send(uint8_t control_byte,uint8_t* data_array,uint8_t data_size);

void empty_buf_reset();

void Fndmntl_Set_Contr_Ctrl(uint8_t contrast_value);
void Fndmntl_Ent_Disp_ON (uint8_t display_status);
void Fndmntl_Set_Nor_Inv_Disp(uint8_t Set_Nor_Inv);
void Fndmntl_Set_Disp_ON_OFF(On_Off_t ON_OFF);



void Scrll_Cont_Hor_Scroll_Setup(Horizontal_Scroll_t horizontal_scroll,
						   Page_Adress_t start_page_addr,
						   Frame_Frequency_t frame_frequency,
						   Page_Adress_t end_page_addr);
void Scrll_Cont_Vert_Hor_Scroll_Setup(Vertical_Horizontal_Scroll_t vertical_horizontal_scroll,
		                        Page_Adress_t start_page_addr,
								Frame_Frequency_t frame_frequency,
								Page_Adress_t end_page_addr,
								uint8_t Vertical_scrolling_offset);
void Scrll_Deactivate_Scroll();
void Scrll_Activate_Scroll();
void Scrll_Set_Vert_Scroll_Area(uint8_t top_fixed_rows,uint8_t scrolling_rows);



void Addr_Sttng_Set_Colm_Start_Addr(uint8_t starting_adress);
void Addr_Sttng_Set_Mem_Addr_Mode(Set_Memory_Addr_Mode_t mode);
void Addr_Sttng_Set_Colmn_Addr(uint8_t start_address,uint8_t stop_address);
void Addr_Sttng_Set_Page_Addr(Page_Adress_t Start_Colm_Page,Page_Adress_t End_Colm_Page);
void Addr_Sttng_Set_Page_Start_Addr(Page_Adress_t GDDRAM_Page_Start_Address);



void Hrwre_Set_Disp_Start_Line(uint8_t screen_scroll_y);
void Hrwre_Set_Charge_Pump();
void Hrwre_Set_Segment_Remap(Set_Segment_Remap_t mode);
void Hrwre_Set_Multi_Ratio(uint8_t multiplex_ratio);
void Hrwre_Set_COM_Output_Scan_Direction (Set_COM_Output_Scan_t mode);
void Hrwre_Set_Disp_Offset(uint8_t vertical_offset);
void Hrwre_Set_COM_Pins_Hardware_Configuration (COM_Pins_Config_t Pin_conf, COM_Remap_Config_t Left_Right_remap);


void Tim_Drive_Disp_Clck_Ratio_Freq(uint8_t divide_ratio,uint8_t Osc_Frequency);
void Tim_Drive_Set_Pre_Charge(uint8_t phase1_period,uint8_t phase2_preiod);
void Tim_Drive_Set_V_Deselect_Level(Voltage_Level_t voltage_level);
void Tim_Drive_NOP();


void SSD1306_Config();

void SSD1306_Window_Clear();
void SSD1306_Write(char *str,Page_Adress_t start_page,uint8_t start_colmn);
void SSD1306_Page_Clear(Page_Adress_t page_clear);

void SSD1306_Int_Write(uint16_t *value, char *int_to_char);
void SSD1306_Float_Write_Show(float *value, char *float_to_char);
void SSD1306_Relay_Write_Show(uint8_t *relay_value,char *relay_to_char);
void SSD1306_Int8_Write(uint8_t *value, char *int_to_char);

#endif /* INC_SSD1306_OLED_H_ */
