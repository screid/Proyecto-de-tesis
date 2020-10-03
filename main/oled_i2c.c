#include "oled_i2c.h"
#include "esp_err.h"
#include "esp_log.h"




void send_comand(oled *ole,comandos comando){
	i2c_cmd_handle_t cmd;
	esp_err_t ret;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole->direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,COMMAND ,1);
	i2c_master_write_byte(cmd,comando ,1);
	i2c_master_stop(cmd); 
	ret = i2c_master_cmd_begin(ole->i2c, cmd, 10/portTICK_PERIOD_MS); 
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de codigo: %s" ,esp_err_to_name(ret));
	}
}
void oled_init(oled *ole){
    send_comand(ole,DISPLAY_OFF);
    send_comand(ole,SET_DISPLAY_CLOCK_DIV_RATIO);
    send_comand(ole,0x80);
    send_comand(ole,SET_MULTIPLEX_RATIO);
    send_comand(ole,0x3F);
    send_comand(ole,SET_DISPLAY_OFFSET);
    send_comand(ole,0x0);
    send_comand(ole,SET_START_LINE | 0x0);
    send_comand(ole,CHARGE_PUMP);
    send_comand(ole,0x14);
    send_comand(ole,MEMORY_ADDR_MODE);
    send_comand(ole,0x00);
    send_comand(ole,SET_SEGMENT_REMAP | 0x0);
    send_comand(ole,COM_SCAN_DIR_DEC);
    send_comand(ole,SET_COM_PINS);
    send_comand(ole,0x12);
    send_comand(ole,SET_CONTRAST_CONTROL);
    send_comand(ole,0xCF);
    send_comand(ole,SET_PRECHARGE_PERIOD);
    send_comand(ole,0xF1);
    send_comand(ole,SET_VCOM_DESELECT);
    send_comand(ole,0x40);
    send_comand(ole,DISPLAY_ALL_ON_RESUME);
    send_comand(ole,NORMAL_DISPLAY);
    send_comand(ole,DISPLAY_ON);
}

void set_col_addr(oled ole,uint8_t inicio,uint8_t final){
	if((inicio>127)|(inicio>final)|(final>127)){
		ESP_LOGE("OLED","columna erronea: inicio %u : fin: %u",inicio,final);
		return;
	}
	i2c_cmd_handle_t cmd;
	esp_err_t ret;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole.direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,COMMAND ,1);
	i2c_master_write_byte(cmd,inicio ,1);					//inicio
	i2c_master_write_byte(cmd,final ,1);					//fin
	i2c_master_stop(cmd); 
	ret = i2c_master_cmd_begin(ole.i2c, cmd, 10/portTICK_PERIOD_MS); 
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de codigo: %s" ,esp_err_to_name(ret));
	}
}//revisar pag20
void set_page_addr(oled ole,uint8_t inicio,uint8_t final){
	if((inicio>127)|(inicio>final)|(final>127)){
		ESP_LOGE("OLED","fila erronea: inicio %u : fin: %u",inicio,final);
		return;
	}
	i2c_cmd_handle_t cmd;
	esp_err_t ret;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole.direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,COMMAND,1);
	i2c_master_write_byte(cmd,inicio ,1);					//inicio
	i2c_master_write_byte(cmd,final ,1);					//fin
	i2c_master_stop(cmd); 
	ret = i2c_master_cmd_begin(ole.i2c, cmd, 10/portTICK_PERIOD_MS); 
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de codigo: %s" ,esp_err_to_name(ret));
	}
}//revisar pag20
void enviar_col(oled *ole,uint8_t data){
	i2c_cmd_handle_t cmd;
	esp_err_t ret;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole->direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,DATA_CONTINUE ,1);
	i2c_master_write_byte(cmd,data ,1);					//enviar byte
	i2c_master_stop(cmd); 
	ret = i2c_master_cmd_begin(ole->i2c, cmd, 10/portTICK_PERIOD_MS); 
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de codigo: %s" ,esp_err_to_name(ret));
	}
	
}

void oled_res(oled* ole){
	i2c_cmd_handle_t cmd;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole->direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,DATA_CONTINUE ,1);
	for(int i=0; i<8*128;i++){
		
		i2c_master_write_byte(cmd,0x00 ,1);					//enviar byte		
	}
	i2c_master_stop(cmd); 
	esp_err_t ret;
	ret = i2c_master_cmd_begin(ole->i2c, cmd, 10/portTICK_PERIOD_MS); 
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de codigo: %s" ,esp_err_to_name(ret));
	}
	i2c_cmd_link_delete(cmd);
	
	
	
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole->direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,COMMAND ,1);
	i2c_master_write_byte(cmd,SET_COLUMN_ADDR ,1);
	i2c_master_write_byte(cmd,0 ,1);
	i2c_master_write_byte(cmd,127 ,1);
	i2c_master_stop(cmd); 
	
	ret = i2c_master_cmd_begin(ole->i2c, cmd, 10/portTICK_PERIOD_MS); 
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio col de codigo: %s" ,esp_err_to_name(ret));
	}
	i2c_cmd_link_delete(cmd);
	
	
	
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole->direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,COMMAND ,1);
	i2c_master_write_byte(cmd,SET_PAGE_ADDR ,1);
	i2c_master_write_byte(cmd,0 ,1);
	i2c_master_write_byte(cmd,7 ,1);
	i2c_master_stop(cmd); 
	
	
	
	ret = i2c_master_cmd_begin(ole->i2c, cmd, 10/portTICK_PERIOD_MS); 
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de page codigo: %s" ,esp_err_to_name(ret));
	}
	i2c_cmd_link_delete(cmd);
}
void print_bits(char a){
    printf("0b");
    for(int i=0; i<8;i++){
        printf("%u",((a>>i)&0x1));
    }
    printf("\n");
}
void enviar_caracter_oled(oled* ole, char character){
	const char* ch = a_bitmap(character);
	char nuevo_caracter[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
		nuevo_caracter[7-i]|=(((*(ch+j))>>i)&0b1)<<j;
		}
	}
	i2c_cmd_handle_t cmd;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(ole->direccion<<1) | I2C_MASTER_WRITE,1);
	i2c_master_write_byte(cmd,DATA_CONTINUE ,1);
	for(unsigned int i=0; i<8;i++){
		
		i2c_master_write_byte(cmd,nuevo_caracter[i],1);
		//print_bits(nuevo_caracter[i]);
		//ESP_LOGI("char","bit:%u",*((char*)(ch+i)));
	}
	i2c_master_stop(cmd); 
	esp_err_t ret;
	ret = i2c_master_cmd_begin(ole->i2c, cmd, 10/portTICK_PERIOD_MS); 
	if (ret != ESP_OK) {
		ESP_LOGE("OLED", "fallo envio de codigo: %s" ,esp_err_to_name(ret));
	}
	i2c_cmd_link_delete(cmd);
}



















