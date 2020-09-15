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
	/*ESP_LOGI("main","inicio");
	const i2c_port_t i2c_luz=I2C_NUM_0;	
	i2c_master_init(i2c_luz,400000);
	ads1115_t adc =ads1115_config(i2c_luz, 0b1001000 );
	while(1){
		double val= ads1115_get_voltage(&adc);
		ESP_LOGI("lectura","voltaje= %f",val);
		vTaskDelay(20 / portTICK_PERIOD_MS);
	}*/
    sist_init();



}
























