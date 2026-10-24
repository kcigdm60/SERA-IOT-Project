/*
 * ssd1306_oled.c
 *
 *  Created on: Jun 17, 2026
 *      Author: kcigd
 */

#include "ssd1306_oled.h"

uint8_t empty_buffer[8]={0};
const uint8_t ASCII_Font_5x7[][5] = {

    {0x00, 0x00, 0x00, 0x00, 0x00}, // 32: Boşluk (Space)
    {0x00, 0x00, 0x4F, 0x00, 0x00}, // 33: !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 34: "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 35: #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 36: $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 37: %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 38: &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 39: '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 40: (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 41: )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // 42: *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 43: +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 44: ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 45: -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 46: .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 47: /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 48: 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 49: 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 50: 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 51: 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 52: 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 53: 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 54: 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 55: 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 56: 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 57: 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 58: :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 59: ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 60: <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 61: =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 62: >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 63: ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 64: @

    // --- BÜYÜK HARFLER ---
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 65: A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 66: B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 67: C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 68: D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 69: E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 70: F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 71: G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 72: H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 73: I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 74: J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 75: K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 76: L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 77: M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 78: N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 79: O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 80: P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 81: Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 82: R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 83: S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 84: T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 85: U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 86: V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 87: W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 88: X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 89: Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 90: Z

    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 91: [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 92: \ (Ters Taksim)
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 93: ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 94: ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 95: _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 96: `

    // --- KÜÇÜK HARFLER ---
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 97: a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 98: b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 99: c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 100: d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 101: e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 102: f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 103: g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 104: h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 105: i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 106: j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 107: k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 108: l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 109: m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 110: n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 111: o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 112: p
    {0x08, 0x14, 0x14, 0x14, 0x7C}, // 113: q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 114: r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 115: s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 116: t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 117: u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 118: v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 119: w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 120: x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 121: y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 122: z

    {0x00, 0x08, 0x36, 0x41, 0x00}, // 123: {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124: |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 125: }
    {0x02, 0x01, 0x02, 0x04, 0x02}  // 126: ~
};





void empty_buf_reset() {

	empty_buffer[0]=0;
	empty_buffer[1]=0;
	empty_buffer[2]=0;
	empty_buffer[3]=0;
	empty_buffer[4]=0;
	empty_buffer[5]=0;
	empty_buffer[6]=0;
	empty_buffer[7]=0;


}

// ekrana i2c ile veri gönderen fonk

void SSD1306_Data_Send(uint8_t control_byte,uint8_t* data_array,uint8_t data_size) {

	I2C1_Write(control_byte, data_array, data_size);

}

// ekran kontrast ayarı (max 255)

void Fndmntl_Set_Contr_Ctrl(uint8_t contrast_value) {


empty_buf_reset();

empty_buffer[0]=SET_CONTRAST_CTRL;
empty_buffer[1]=contrast_value;

SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 2);

empty_buf_reset();


}


void Fndmntl_Ent_Disp_ON (uint8_t display_status) {

	empty_buf_reset();
	empty_buffer[0]= display_status;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 1);
	empty_buf_reset();

}


void Fndmntl_Set_Nor_Inv_Disp(uint8_t Set_Nor_Inv) {

	empty_buf_reset();
	empty_buffer[0]= Set_Nor_Inv;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 1);
	empty_buf_reset();

}


// display on of durumu için

void Fndmntl_Set_Disp_ON_OFF(On_Off_t ON_OFF) {

	empty_buf_reset();
	empty_buffer[0]= ON_OFF;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 1);
	empty_buf_reset();

}




void Scrll_Cont_Hor_Scroll_Setup(Horizontal_Scroll_t horizontal_scroll,
						   Page_Adress_t start_page_addr,
						   Frame_Frequency_t frame_frequency,
						   Page_Adress_t end_page_addr)
{

	empty_buf_reset();
	empty_buffer[0]=horizontal_scroll;
	empty_buffer[1]=0x00;
	empty_buffer[2]=start_page_addr;
	empty_buffer[3]=frame_frequency;
	empty_buffer[4]=end_page_addr;
	empty_buffer[5]=0x00;
	empty_buffer[6]=0x00;

	SSD1306_Data_Send(COMMAND_SEND,empty_buffer, 7);

	empty_buf_reset();


}


void Scrll_Cont_Vert_Hor_Scroll_Setup(Vertical_Horizontal_Scroll_t vertical_horizontal_scroll,
		                        Page_Adress_t start_page_addr,
								Frame_Frequency_t frame_frequency,
								Page_Adress_t end_page_addr,
								uint8_t Vertical_scrolling_offset) {


	empty_buf_reset();
		empty_buffer[0]=vertical_horizontal_scroll;
		empty_buffer[1]=0x00;
		empty_buffer[2]=start_page_addr;
		empty_buffer[3]=frame_frequency;
		empty_buffer[4]=end_page_addr;
		empty_buffer[5]=Vertical_scrolling_offset;

		SSD1306_Data_Send(COMMAND_SEND,empty_buffer, 6);

		empty_buf_reset();



}



void Scrll_Deactivate_Scroll() {

	empty_buf_reset();
	empty_buffer[0]=0x2E;
	SSD1306_Data_Send(COMMAND_SEND,empty_buffer,1);
	empty_buf_reset();



}



void Scrll_Activate_Scroll() {

	empty_buf_reset();
	empty_buffer[0]=0x2F;
	SSD1306_Data_Send(COMMAND_SEND,empty_buffer,1);
	empty_buf_reset();



}




void Scrll_Set_Vert_Scroll_Area(uint8_t top_fixed_rows,uint8_t scrolling_rows) {

	empty_buf_reset();
	empty_buffer[0]=0xA3;
	empty_buffer[1]=top_fixed_rows;
	empty_buffer[2]=scrolling_rows;
	SSD1306_Data_Send(COMMAND_SEND,empty_buffer,3);
	empty_buf_reset();


}



void Addr_Sttng_Set_Colm_Start_Addr(uint8_t starting_adress) {

		empty_buf_reset();

		empty_buffer[0] = starting_adress & 0x0F;
		SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 1);

		empty_buf_reset();

		empty_buffer[0] = starting_adress >> 4;
		empty_buffer[0] |= (1 << 4);
		SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 1);

		empty_buf_reset();


}


void Addr_Sttng_Set_Mem_Addr_Mode(Set_Memory_Addr_Mode_t mode) {

	empty_buf_reset();

	empty_buffer[0]=0x20;
	empty_buffer[1]=mode;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,2);

	empty_buf_reset();


}


void Addr_Sttng_Set_Colmn_Addr(uint8_t start_address,uint8_t stop_address) {

	empty_buf_reset();

	empty_buffer[0]=0x21;
	empty_buffer[1]=start_address;
	empty_buffer[2]=stop_address;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,3);

	empty_buf_reset();

}


void Addr_Sttng_Set_Page_Addr(Page_Adress_t Start_Colm_Page,Page_Adress_t End_Colm_Page) {

	empty_buf_reset();

	empty_buffer[0]=0x22;
	empty_buffer[1]=Start_Colm_Page;
	empty_buffer[2]=End_Colm_Page;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,3);

	empty_buf_reset();

}


void Addr_Sttng_Set_Page_Start_Addr(Page_Adress_t GDDRAM_Page_Start_Address){

	empty_buf_reset();

	empty_buffer[0] = 0xB0 | GDDRAM_Page_Start_Address;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,1);

	empty_buf_reset();

}



void Hrwre_Set_Disp_Start_Line(uint8_t screen_scroll_y) {

	empty_buf_reset();

	empty_buffer[0]= (0x40 | screen_scroll_y);
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,1);

	empty_buf_reset();

}

void Hrwre_Set_Segment_Remap(Set_Segment_Remap_t mode) {

	empty_buf_reset();

	empty_buffer[0]= (0xA0 | mode);
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,1);

	empty_buf_reset();
}


void Hrwre_Set_Multi_Ratio(uint8_t multiplex_ratio) {

	empty_buf_reset();

	empty_buffer[0]=0xA8;
	empty_buffer[1]=multiplex_ratio;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,2);

	empty_buf_reset();

}

void Hrwre_Set_COM_Output_Scan_Direction (Set_COM_Output_Scan_t mode) {

	empty_buf_reset();

	empty_buffer[0]=mode;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,1);

	empty_buf_reset();

}


void Hrwre_Set_Disp_Offset(uint8_t vertical_offset) {

	empty_buf_reset();

	empty_buffer[0]=0xD3;
	empty_buffer[1]=vertical_offset;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer,2);

	empty_buf_reset();

}

void Hrwre_Set_COM_Pins_Hardware_Configuration (COM_Pins_Config_t Pin_conf, COM_Remap_Config_t Left_Right_remap) {

		empty_buf_reset();

	    empty_buffer[0] = 0xDA;
	    empty_buffer[1] = 0x02 | (Pin_conf << 4) | (Left_Right_remap << 5);
	    SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 2);

	    empty_buf_reset();


}


void Tim_Drive_Disp_Clck_Ratio_Freq(uint8_t divide_ratio,uint8_t Osc_Frequency) {

	empty_buf_reset();

	empty_buffer[0]=0xD5;
	empty_buffer[1]=(Osc_Frequency << 4) | (divide_ratio);
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 2);

	empty_buf_reset();

}

void Tim_Drive_Set_Pre_Charge(uint8_t phase1_period,uint8_t phase2_preiod) {

	empty_buf_reset();

	empty_buffer[0]=0xD9;
	empty_buffer[1]= (phase2_preiod << 4) | (phase1_period);
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 2);

	empty_buf_reset();

}

void Tim_Drive_Set_V_Deselect_Level(Voltage_Level_t voltage_level) {

	empty_buf_reset();

	empty_buffer[0]=0xDB;
	empty_buffer[1]=voltage_level;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 2);

	empty_buf_reset();

}



void Tim_Drive_NOP() {

	empty_buf_reset();

	empty_buffer[0] = 0xE3;
	SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 1);

	empty_buf_reset();

}



void SSD1306_Config() {

	Fndmntl_Set_Disp_ON_OFF(DISPLAY_OFF);
	Tim_Drive_Disp_Clck_Ratio_Freq(0, 8);

	Hrwre_Set_Multi_Ratio(0x3F);
	Hrwre_Set_Disp_Offset(0x00);
	Hrwre_Set_Disp_Start_Line(0x00);

	Addr_Sttng_Set_Mem_Addr_Mode(Page_Addressing_Mode);

	Hrwre_Set_Segment_Remap(NORMAL_MOD);
	Hrwre_Set_COM_Output_Scan_Direction(COM_NORMAL_MOD);
	Hrwre_Set_COM_Pins_Hardware_Configuration(COM_PINS_ALTERNATIVE, COM_REMAP_DISABLE);

	Fndmntl_Set_Contr_Ctrl(0x7F);
	Tim_Drive_Set_Pre_Charge(0x02, 0x02);
	Tim_Drive_Set_V_Deselect_Level(_0_77_MULTIPLIER);

	Fndmntl_Ent_Disp_ON(ENTIRE_DISPLAY_NORMAL);
	Fndmntl_Set_Nor_Inv_Disp(SET_NORMAL);
	Hrwre_Set_Charge_Pump();

	Fndmntl_Set_Disp_ON_OFF(DISPLAY_ON);

}




void SSD1306_Window_Clear() {

	empty_buf_reset();

    for (uint8_t page = 0; page < 8; page++) {

        Addr_Sttng_Set_Page_Start_Addr((Page_Adress_t)page);
        Addr_Sttng_Set_Colm_Start_Addr(0);

        for (uint8_t x = 0; x < 16; x++) {
            SSD1306_Data_Send(PIXEL_SEND,empty_buffer,8);
        }
    }

    empty_buf_reset();

}



void SSD1306_Write(char *str,Page_Adress_t start_page,uint8_t start_colmn) {

	char character;
	uint16_t str_ln;


	Addr_Sttng_Set_Colm_Start_Addr(start_colmn);
	Addr_Sttng_Set_Page_Start_Addr(start_page);


	str_ln=str_len(str);

	for ( uint8_t x=0 ; x < str_ln ; x++) {

		character=str[x];
		character=character-32;
		SSD1306_Data_Send(PIXEL_SEND,(uint8_t*)&ASCII_Font_5x7[(uint8_t)character][0], 5);

	}


}



void SSD1306_Page_Clear(Page_Adress_t page_clear) {


	Addr_Sttng_Set_Colm_Start_Addr(0);
	Addr_Sttng_Set_Page_Start_Addr(page_clear);

	empty_buf_reset();

	for (uint8_t x=0; x < 16 ; x++) {


		SSD1306_Data_Send(PIXEL_SEND, empty_buffer,8);

	}



}

void SSD1306_Int_Write(uint16_t *value, char *int_to_char) {

	char figures[11] = {'0','1','2','3','4','5','6','7','8','9','\0'};
	uint16_t thousands=0, hundred=0, tens=0, ones=0;

	thousands = (*value / 1000);
	hundred = (*value % 1000) / 100;
	tens = (*value % 100) / 10;
	ones = (*value % 10);

	int_to_char[0] = figures[thousands];
	int_to_char[1] = figures[hundred];
	int_to_char[2] = figures[tens];
	int_to_char[3] = figures[ones];


	if (thousands == 0) {

		if (hundred == 0) {

			if (ones == 0) {

				int_to_char[0] = figures[ones];
				int_to_char[1] = figures[10];
				int_to_char[2] = figures[10];
				int_to_char[3] = figures[10];

			}

			else {

				int_to_char[0] = figures[tens];
				int_to_char[1] = figures[ones];
				int_to_char[2] = figures[10];
				int_to_char[3] = figures[10];

			}

		}

		else {

			int_to_char[0] = figures[hundred];
			int_to_char[1] = figures[tens];
			int_to_char[2] = figures[ones];
			int_to_char[3] = figures[10];

		}

	}

	else {

		int_to_char[0] = figures[thousands];
		int_to_char[1] = figures[hundred];
		int_to_char[2] = figures[tens];
		int_to_char[3] = figures[ones];

	}

}



void SSD1306_Int8_Write(uint8_t *value, char *int_to_char) {
    char figures[11] = {'0','1','2','3','4','5','6','7','8','9','\0'};
    uint8_t hundred = 0, tens = 0, ones = 0;

    hundred = (*value / 100);
    tens = (*value % 100) / 10;
    ones = (*value % 10);

    if (hundred != 0) {
        int_to_char[0] = figures[hundred];
        int_to_char[1] = figures[tens];
        int_to_char[2] = figures[ones];
        int_to_char[3] = figures[10];
    }
    else if (tens != 0) {
        int_to_char[0] = figures[tens];
        int_to_char[1] = figures[ones];
        int_to_char[2] = figures[10];
    }
    else {
        int_to_char[0] = figures[ones];
        int_to_char[1] = figures[10];
    }
}





void SSD1306_Float_Write_Show(float *value, char *float_to_char) {

	uint8_t integer_value=0;
	uint8_t decimal_value=0;

	char figures[10] = {'0','1','2','3','4','5','6','7','8','9'};


	uint8_t int_hundred=0,int_tens=0,int_ones=0,float_tens=0,float_ones=0;



	integer_value=(uint8_t)*value;
	decimal_value = (uint8_t)((*value - integer_value) * 100 + 0.5f);

	int_hundred=(integer_value/100);
	int_tens=(integer_value%100)/10;
	int_ones=(integer_value%10);
	float_tens=(decimal_value%100)/10;
	float_ones=(decimal_value%10);


if (int_hundred != 0) {

	float_to_char[0]=figures[int_hundred];
	float_to_char[1]=figures[int_tens];
	float_to_char[2]=figures[int_ones];
	float_to_char[3]='.';
	float_to_char[4]=figures[float_tens];
	float_to_char[5]=figures[float_ones];



}



else {

	if (int_tens != 0)  {

	    float_to_char[0]=figures[int_tens];
		float_to_char[1]=figures[int_ones];
		float_to_char[2]='.';
		float_to_char[3]=figures[float_tens];
		float_to_char[4]=figures[float_ones];
		float_to_char[5]='\0';


	}


	else {

		 float_to_char[0]=figures[int_ones];
		 float_to_char[1]='.';
		 float_to_char[2]=figures[float_tens];
		 float_to_char[3]=figures[float_ones];
		 float_to_char[4]='\0';
		 float_to_char[5]='\0';



	}

}




}

void SSD1306_Relay_Write_Show(uint8_t *relay_value,char *relay_to_char) {

uint8_t y=7;

	for(uint8_t x=0; x < 8; x++) {

		if (*relay_value & (1 << x)) {

			relay_to_char[y]='1';


		}

		else {

			relay_to_char[y]='0';


		}

		y--;

	}





}



void Hrwre_Set_Charge_Pump() {

    empty_buf_reset();

    empty_buffer[0] = 0x8D;        // Charge Pump Komutu
    empty_buffer[1] = CHARGE_PUMP_ENABLE; // 0x14 (Enable) veya 0x10 (Disable)
    SSD1306_Data_Send(COMMAND_SEND, empty_buffer, 2);

    empty_buf_reset();

}






