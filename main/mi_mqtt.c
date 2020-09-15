#include "mi_mqtt.h"
#include "string.h"
#include "parse.h"
#include "sistema.h"
#include "freertos/semphr.h"



static const char *TAG = "mqtt";
EventGroupHandle_t EGMQTT;

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
    esp_mqtt_client_handle_t client = event->client;
    // your_context_t *context = event->context;
    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        xEventGroupSetBits(EGMQTT,BIT0);
        esp_mqtt_client_subscribe(client, CONFIG_topico_in, 2);
        esp_mqtt_client_subscribe(client, CONFIG_topico_out, 2);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        xEventGroupClearBits(EGMQTT,BIT0);

        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        {


            //char top[event->topic_len];
            //char mem[event->data_len];
            //memcpy(top,event->topic,event->topic_len);
            if(!strcmp(event->topic,CONFIG_topico_in)) {                                 //topico in
                //memcpy(mem,event->data,event->data_len);
                recivir_in(event->data,event->data_len);
            }
            if(!strcmp(event->topic,CONFIG_topico_out)) {                                 //topico out
                //memcpy(mem,event->data,event->data_len);
                recivir_out(event->data,event->data_len);
            }
        }



        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}




void mqtt_app_start(esp_mqtt_client_handle_t* client) {

    EGMQTT=xEventGroupCreate();
    if(EGMQTT==0) {
        printf("no se greo grupo EGMQTT");
    }
    int len=strlen(CONFIG_mensaje_ltw);

    esp_mqtt_client_config_t mqtt_cfg = {
        //.uri = "//192.168.0.20:1883",
        .host=CONFIG_ip_broker,
        .port=CONFIG_puero_mqtt,
        .event_handle = mqtt_event_handler,
        .username = CONFIG_usuario_mqtt,
        .password = CONFIG_contrasena_mqtt,
        // .user_context = (void *)your_context
        .lwt_topic= CONFIG_topico_ltw, //pointer to the LWT message topic
        .lwt_msg= CONFIG_mensaje_ltw,//pointer to the LWT message
        .lwt_msg_len= len,//length of the LWT message, required if lwt_msg is not null-terminated
        .lwt_qos=2, //quality of service for the LWT message
        .lwt_retain=1, //
        .keepalive= 20
    };

#if CONFIG_BROKER_URL_FROM_STDIN
    char line[128];

    if (strcmp(mqtt_cfg.uri, "FROM_STDIN") == 0) {
        int count = 0;
        printf("Please enter url of mqtt broker\n");
        while (count < 128) {
            int c = fgetc(stdin);
            if (c == '\n') {
                line[count] = '\0';
                break;
            } else if (c > 0 && c < 127) {
                line[count] = c;
                ++count;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        mqtt_cfg.uri = line;
        printf("Broker url: %s\n", line);
    } else {
        ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
        abort();
    }
#endif // CONFIG_BROKER_URL_FROM_STDIN

    *client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(*client);
}
int pub(esp_mqtt_client_handle_t client,const char *topico,const char *mensaje,int len,int  qos,int retain) {
    xEventGroupWaitBits(EGMQTT,BIT0,pdFALSE,pdTRUE,100);
    return esp_mqtt_client_publish(client,topico, mensaje, len, qos, retain);
}
void recivir_in(char mensaje[],int tam) {
    /*
		pasarlo el mensaje al bufer
		ejecutar funcion de escucha
    */
    ESP_LOGI(TAG,"mensaje en fun recibir_in: %.*s",tam,mensaje);
	sprintf(get_entrada(),"%.*s",tam,mensaje);
	xTaskCreate(&escucha,            "PARSER_MQTT_IN",CONFIG_task_stack,0,3,&h_esc);
    

}
void recivir_out(char mensaje[],int tam) {


    ESP_LOGI(TAG,"mensaje enviado en topico de salida \n%.*s\n",tam,mensaje);
}

