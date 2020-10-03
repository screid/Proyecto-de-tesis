#ifndef oled_i2c
#define oled_i2c
#include "driver/i2c.h"
#include "stdint.h"
#define oled1 0x3c

extern const char caracteres[];

typedef enum{
	COMMAND						=0x00,
	DATA_CONTINUE				=0x40,
	DISPLAY_OFF					=0xae,
	DISPLAY_ON					=0xaf,
	MEMORY_ADDR_MODE			=0x20,
	SET_COLUMN_ADDR				=0x21,
	SET_PAGE_ADDR				=0x22,
	SET_CONTRAST_CONTROL		=0x81,
	NORMAL_DISPLAY				=0xa6,
	INVERT_DISPLAY				=0xa7,
	SET_DISPLAY_CLOCK_DIV_RATIO	=0xd5,
	SET_MULTIPLEX_RATIO			=0xa8,
	SET_DISPLAY_OFFSET			=0xd3,
	SET_START_LINE				=0x40,
	CHARGE_PUMP					=0x8d,
	SET_SEGMENT_REMAP			=0xa0,
	COM_SCAN_DIR_DEC			=0xc0,
	SET_COM_PINS				=0xda,
	SET_PRECHARGE_PERIOD		=0xd9,
	SET_VCOM_DESELECT			=0xdb,
	DISPLAY_ALL_ON_RESUME		=0Xa4,
	SET_PAGE_0					=0xb0,
	SET_PAGE_1					=0xb1,
	SET_PAGE_2					=0xb2,
	SET_PAGE_3					=0xb3,
	SET_PAGE_4					=0xb4,
	SET_PAGE_5					=0xb5,
	SET_PAGE_6					=0xb6,
	SET_PAGE_7					=0xb7
}comandos;

typedef struct{
	i2c_port_t i2c;
	uint8_t direccion;
	
	char data[8][16];
}oled;



/**	\brief enviar un comando al oled atravez del i2c
 *	\param direccion del oled
 *	\param i2c ya configurado
 */
void send_comand(oled* ole,comandos comando);
void oled_init(oled* ole);
void enviar_col(oled* ole,uint8_t data);
void oled_res(oled* ole);
const char* a_bitmap(char caracter);
void enviar_caracter_oled(oled* ole, char character);


















#endif