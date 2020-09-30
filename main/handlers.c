#include "handlers.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "stdint.h"


const char* tag ="interruptores";
EventGroupHandle_t enc_ready;
unsigned int enc_pos;

void iniciar_handlers(void * guardar){
	
	
	enc_ready=xEventGroupCreate();
	gpio_config_t io_conf;
    //eneable interrupt
    io_conf.intr_type = 0;
    //set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1ULL<<button)|(1ULL<<pina)|(1ULL<<pinb);
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
	
	
	esp_err_t err;
	err=gpio_set_intr_type(button,GPIO_INTR_ANYEDGE);
	if(err!=ESP_OK )ESP_LOGE(tag,"parametro incorrecto en inio de interruptores");
	
	err=gpio_set_intr_type(pina,GPIO_INTR_ANYEDGE);
	if(err!=ESP_OK )ESP_LOGE(tag,"parametro incorrecto en inio de interruptores");
	
	err=gpio_set_intr_type(pinb,GPIO_INTR_ANYEDGE);
	if(err!=ESP_OK )ESP_LOGE(tag,"parametro incorrecto en inio de interruptores");
	
	err=gpio_install_isr_service(0);
	if(err!=ESP_OK )ESP_LOGE(tag,"error en la instalacion del driver");
	
	
	err=gpio_isr_handler_add(button, botton_h,guardar);
	if(err!=ESP_OK )ESP_LOGE(tag,"error al anadir");
	gpio_intr_enable(button);
	
	err=gpio_isr_handler_add(pina, encoder,0);
	if(err!=ESP_OK )ESP_LOGE(tag,"error al anadir");
	gpio_intr_enable(pina);
	
	err=gpio_isr_handler_add(pinb, encoder,0);
	if(err!=ESP_OK )ESP_LOGE(tag,"error al anadir");
	gpio_intr_enable(pinb);
	
	
	


}

void botton_h(void *puntero){
	//ESP_LOGI(tag,"cambio de estado a: %d",gpio_get_level(button));
	*((int*)puntero)=gpio_get_level(button);
}

void encoder(void *ignorar){
	
	char b,a;
	
	b=gpio_get_level(pinb);
	a=gpio_get_level(pina);
	
	
	enc_pos=((enc_pos<<2)&0xff)|(b<<1)|(a<<0);
	
	
	if(enc_pos==CW) {
		xEventGroupClearBitsFromISR(enc_ready,0xff);
		xEventGroupSetBitsFromISR(enc_ready,0b10000001,pdFALSE );
	}else if(enc_pos==CCW) {
		xEventGroupClearBitsFromISR(enc_ready,0xff);
		xEventGroupSetBitsFromISR(enc_ready,0b10000010,pdFALSE );
	}
	
}
 
 int get_value(){
	unsigned int correcto;
	correcto=xEventGroupGetBits(enc_ready);
	
	if((correcto==0b10000001)){
		xEventGroupClearBitsFromISR(enc_ready,0xff);
		ESP_LOGI("encoder","giro: RELOJ");
		return 1;
	}else if(correcto==0b10000010){
		xEventGroupClearBitsFromISR(enc_ready,0xff);
		ESP_LOGI("encoder","giro: CONTRARELOJ");
		return -1;
	}else{
		ESP_LOGE("encoder","valor incorrecto");
		return 0;
	}
}