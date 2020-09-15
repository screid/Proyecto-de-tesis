#ifndef oled_i2c
#define oled_i2c
#include "driver/i2c.h"



typedef enum{
	COMMAND						=0x00,
	DATA_CONTINUE				=0x40,
	DISPLAY_OFF					=0xae,
	DISPLAY_ON					=0xaf,
	MEMORY_ADDR_MODE			=0x20,
	SET_COLUMN_ADDR				=0x21,
	ET_PAGE_ADDR				=0x22,
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
	DISPLAY_ALL_ON_RESUME		=0Xa4
}comandos;



/**	\brief enviar un comando al oled atravez del i2c
 *	\param direccion del oled
 *	\param i2c ya configurado
 */
void send_comand(i2c_port_t i2c,uint8_t direccion,comandos comando);
void oled_init(i2c_port_t i2c,uint8_t direccion);




















#endif