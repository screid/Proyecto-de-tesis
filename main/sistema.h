#ifndef SISTEMA_H_INCLUDED
#define SISTEMA_H_INCLUDED


#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/err.h"
#include "lwip/sys.h"


#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"


#include "mqtt_client.h"
#include "wifi.h"
#include "mi_mqtt.h"

#include "parse.h"
#include "datos.h"

#include "sensores.h"
#include "actuadores.h"




#define fin_sist BIT0
#define m_hum BIT1
#define m_tem BIT2
#define m_luz BIT3






extern TaskHandle_t h_hum ;
extern TaskHandle_t h_luz ;
extern TaskHandle_t h_tem ;
extern TaskHandle_t h_log ;
extern TaskHandle_t h_esc ;
extern TaskHandle_t dht11 ;
extern EventGroupHandle_t sist;

    /** \brief 
     * \param 
	 * \param 
     * \return 
     */
void control_luz(void* parametros);
void control_humedad(void* parametros);
void control_temperatura(void* parametros);
void loger(void* parametros);
void escucha(void* parametros);
void sensardht11(void* parametros);
char* get_entrada();
char* get_salida();



void sist_init();


#endif // SISTEMA_H_INCLUDED
