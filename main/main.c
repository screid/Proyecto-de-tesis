/*
	app_main es el punto de entrada del esp32, sist_init es el punto de entrada para la aplicacion desarollada
*/
#include <stdio.h>
#include <string.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"


#include "sistema.h"

#include "actuadores.h"
#include "sensores.h"

#include "handlers.h"


#include "oled_i2c.h"















void app_main() {
	ESP_LOGI("main","inicio");
	/*
	const i2c_port_t i2c_luz=I2C_NUM_0;	
	i2c_master_init(i2c_luz,400000);
	ads1115_t adc =ads1115_config(i2c_luz, 0b1001000 );
	ads1115_set_mux(&adc,ADS1115_MUX_0_GND);
	while(1){
		double val= ads1115_get_voltage(&adc);
		ESP_LOGI("lectura","voltaje= %f",val);
		vTaskDelay(20 / portTICK_PERIOD_MS);
	} */
	/*												//prueba de los dht11 exito
	dht dh1 =DHT11_init(16);
	dht dh2 =DHT11_init(17);
	dht dh3 =DHT11_init(18);
	dht dh4 =DHT11_init(5);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	while(1){
		DHT11_read(&dh1);
		DHT11_read(&dh2);
		DHT11_read(&dh3);
		DHT11_read(&dh4);
		ESP_LOGI("lectura","temp/hum: %d/%d : %d/%d : %d/%d : %d/%d",dh1.temperature,dh1.humidity,dh2.temperature,dh2.humidity,dh3.temperature,dh3.humidity,dh4.temperature,dh4.humidity);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}*/
	
   //sist_init();
   /*int i=0;
	iniciar_handlers(&i);
	while(1){
		ESP_LOGI("main","esperando...");
		vTaskDelay(500 / portTICK_PERIOD_MS);
		ESP_LOGI("main","%d",i);
	}*/
	/*
	unsigned int boton=0;
	int pos=0;
	iniciar_handlers(&boton);
	gpio_pad_select_gpio(13);
	gpio_set_direction(13, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(12);
	gpio_set_direction(12, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(26);
	gpio_set_direction(26, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(25);
	gpio_set_direction(25, GPIO_MODE_OUTPUT);

	while(1){
		pos+=get_value();
		if(pos<0){
			pos=3;
		}
		pos=pos%4;
		switch(pos){
			case 0 :
				gpio_set_level(13,boton);
				break;
			case 1 :
				gpio_set_level(12,boton);
				break;
			case 2 :
				gpio_set_level(26,boton);
				break;
			case 3 :
				gpio_set_level(25,boton);
				break;
			default :
				ESP_LOGE("case","pos no valida: %d",pos);
			break;			
		}
		
		
		
		vTaskDelay(50 / portTICK_PERIOD_MS);
		
	}*/
	/*
	char r,g,b;
	tira_led leds = leds_init(CONFIG_led_clk,CONFIG_led_dat);
	while(1){
		esp_fill_random(&r,sizeof(r));
		esp_fill_random(&g,sizeof(g));
		esp_fill_random(&b,sizeof(b));
		set_color(r,g,b,&leds);
		ESP_LOGI("colores","r:%c g:%c b:%c",r,g,b);
		vTaskDelay(50 / portTICK_PERIOD_MS);
		set_color(0,0,0,&leds);
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
	*/
	const i2c_port_t i2c=I2C_NUM_0;	
	i2c_master_init(i2c,400000);
	oled ole={i2c,oled1,{""}};
	oled_init(&ole);
	oled_res(&ole);
	
	
	for (char c=32;c<126;c++){
		enviar_caracter_oled(&ole, c);
		ESP_LOGI("car","	%c",c);
		vTaskDelay(1000/ portTICK_PERIOD_MS);
	}

	
	vTaskDelay(100000 / portTICK_PERIOD_MS);
	
	
}
























