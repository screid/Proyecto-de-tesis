#ifndef MI_MQTT_H_INCLUDED
#define MI_MQTT_H_INCLUDED
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_log.h"



#include "mqtt_client.h"

/*typedef struct mi_mqtt_cli{
    esp_mqtt_client_handle_t cliente;
    EventGroupHandle_t grupo_control;
};*/
extern EventGroupHandle_t EGMQTT;


esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);
void mqtt_app_start(esp_mqtt_client_handle_t* client);
int pub(esp_mqtt_client_handle_t client,const char *topico,const char *mensaje,int len,int  qos,int retain);
void recivir_in(char mensaje[],int tam);
void recivir_out(char mensaje[],int tam);


#endif // MI_MQTT_H_INCLUDED
