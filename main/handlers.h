#if !defined(handlers)
#define handlers
#define button GPIO_NUM_36 
#define pina GPIO_NUM_34
#define pinb GPIO_NUM_39
#define CW 0b01001011
#define CCW 0b10000111
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"


extern EventGroupHandle_t enc_ready;


void iniciar_handlers(void * guardar);

void botton_h(void *puntero);

void encoder(void *ignorar);

 int get_value();


#endif