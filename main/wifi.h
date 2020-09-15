#ifndef WIFI_H_INCLUDED
#define WIFI_H_INCLUDED
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

void wifi_init_sta();
esp_err_t event_handler(void *ctx, system_event_t *event);  //static?
EventGroupHandle_t s_wifi_event_group; //static?







#endif // WIFI_H_INCLUDED
