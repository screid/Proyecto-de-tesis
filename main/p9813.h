#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
typedef struct tira_led{
	int clock_pin;
	int data_pin;
	char red;
	char green;
	char blue;
}tira_led;

tira_led leds_init(int clock_pin,int data_pin);
void ck(tira_led* led);
void send_char(char data,tira_led* led);
void set_color(char red,char green,char blue,tira_led* led);

