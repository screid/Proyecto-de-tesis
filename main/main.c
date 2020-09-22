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
	
   sist_init();



}
























