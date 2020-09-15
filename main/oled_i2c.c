#include "oled_i2c.h"
#include "esp_err.h"
#include "esp_log.h"




void send_comand(i2c_port_t i2c,uint8_t direccion,comandos comando){
	i2c_cmd_handle_t cmd;
	esp_err_t ret;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,COMMAND | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,comando | I2C_MASTER_WRITE,1);
	i2c_master_stop(cmd); 
	ret = i2c_master_cmd_begin(i2c, cmd, 10/portTICK_PERIOD_MS); 
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de codigo: %s" ,esp_err_to_name(ret));
	}
}
void oled_init(i2c_port_t i2c,uint8_t direccion){
    send_comand(i2c,direccion,DISPLAY_OFF);
    send_comand(i2c,direccion,SET_DISPLAY_CLOCK_DIV_RATIO);
    send_comand(i2c,direccion,0x80);
    send_comand(i2c,direccion,SET_MULTIPLEX_RATIO);
    send_comand(i2c,direccion,0x3F);
    send_comand(i2c,direccion,SET_DISPLAY_OFFSET);
    send_comand(i2c,direccion,0x0);
    send_comand(i2c,direccion,SET_START_LINE | 0x0);
    send_comand(i2c,direccion,CHARGE_PUMP);
    send_comand(i2c,direccion,0x14);
    send_comand(i2c,direccion,MEMORY_ADDR_MODE);
    send_comand(i2c,direccion,0x00);
    send_comand(i2c,direccion,SET_SEGMENT_REMAP | 0x1);
    send_comand(i2c,direccion,COM_SCAN_DIR_DEC);
    send_comand(i2c,direccion,SET_COM_PINS);
    send_comand(i2c,direccion,0x12);
    send_comand(i2c,direccion,SET_CONTRAST_CONTROL);
    send_comand(i2c,direccion,0xCF);
    send_comand(i2c,direccion,SET_PRECHARGE_PERIOD);
    send_comand(i2c,direccion,0xF1);
    send_comand(i2c,direccion,SET_VCOM_DESELECT);
    send_comand(i2c,direccion,0x40);
    send_comand(i2c,direccion,DISPLAY_ALL_ON_RESUME);
    send_comand(i2c,direccion,NORMAL_DISPLAY);
    send_comand(i2c,direccion,DISPLAY_ON);
	
	
}