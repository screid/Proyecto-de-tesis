#include "p9813.h"


void ck(tira_led* led){
	
	vTaskDelay(pdMS_TO_TICKS(1));
	gpio_set_level(led->clock_pin, 1);
	vTaskDelay(pdMS_TO_TICKS(1));
	gpio_set_level(led->clock_pin, 0);


}
void send_char(char data, tira_led* led)
{
	
    // Send one bit at a time, starting with the MSB
	//ESP_LOGI("send","pin data:  %d \npin clock: %d\n datos:          %d",led->data_pin,led->clock_pin,(int)data);
    for (char i=0; i<8; i++)
    {
        // If MSB is 1, write one and clock it, else write 0 and clock
        if ((data & 0x80) != 0)
            gpio_set_level(led->data_pin, 1);
        else
            gpio_set_level(led->data_pin, 0);
        ck(led);

        // Advance to the next bit to send
        data <<= 1;
    }
}

  

void set_color(char red,char green,char blue, tira_led* led)


{

  char preamble=0b11000000;
  preamble|=((blue>>2)&0b110000)|((green>>4)&0b1100)|((red>>6));
  

 
  
  
  send_char(preamble,led);
  send_char(blue,led);
  send_char(green,led);
  send_char(red,led);
  
  send_char(0,led);
  send_char(0,led);
  send_char(0,led);
  send_char(0,led);


}
tira_led leds_init(int clock_pin,int data_pin){

	gpio_pad_select_gpio(clock_pin);
	gpio_set_direction(clock_pin, GPIO_MODE_OUTPUT);

	gpio_pad_select_gpio(data_pin);
	gpio_set_direction(data_pin, GPIO_MODE_OUTPUT);

	tira_led led={.clock_pin=clock_pin,.data_pin=data_pin};
	send_char(0,&led);											//iniciar apagado
	send_char(0,&led);
	send_char(0,&led);
	send_char(0,&led);
	send_char(0,&led);
	send_char(0,&led);
	send_char(0,&led);
	send_char(0,&led);

	return led;
}
