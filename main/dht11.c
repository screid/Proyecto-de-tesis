/*
 * MIT License
 *
 * Copyright (c) 2018 Michele Biondi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht11.h"




static int _waitOrTimeout(uint16_t microSeconds, int level,dht* sensor) {
    int micros_ticks = 0;
    while(gpio_get_level(sensor->pin) == level) {
        if(micros_ticks++ > microSeconds)
            return DHT11_TIMEOUT_ERROR;
        ets_delay_us(1);
    }
    return micros_ticks;
}

static int _checkCRC(uint8_t data[]) {
    if(data[4] == (data[0] + data[1] + data[2] + data[3]))
        return DHT11_OK;
    else
        return DHT11_CRC_ERROR;
}

static void _sendStartSignal(dht* sensor) {
    gpio_set_direction(sensor->pin, GPIO_MODE_OUTPUT);
    gpio_set_level(sensor->pin, 0);
    ets_delay_us(20 * 1000);
    gpio_set_level(sensor->pin, 1);
    ets_delay_us(40);
    gpio_set_direction(sensor->pin, GPIO_MODE_INPUT);
}

static int _checkResponse(dht* sensor) {
    /* Wait for next step ~80us*/
    if(_waitOrTimeout(80, 0, sensor) == DHT11_TIMEOUT_ERROR)
        return DHT11_TIMEOUT_ERROR;

    /* Wait for next step ~80us*/
    if(_waitOrTimeout(80, 1, sensor) == DHT11_TIMEOUT_ERROR)
        return DHT11_TIMEOUT_ERROR;

    return DHT11_OK;
}

void _timeoutError(dht* sensor) {
    sensor->status=DHT11_TIMEOUT_ERROR;
	sensor->temperature=-1;
    sensor->humidity=-1;
}

void _crcError(dht* sensor) {
    sensor->status=DHT11_CRC_ERROR;
	sensor->temperature=-1;
    sensor->humidity=-1;   
}

dht DHT11_init(gpio_num_t gpio_num) {
    /* Wait 1 seconds to make the device pass its initial unstable status */
	//agregado en la tarea misma
	dht nuevo;
	nuevo.pin=gpio_num;
	nuevo.iniciado=1;
	nuevo.status=DHT11_OK;
	nuevo.temperature=-1;
	nuevo.last_read_time=-2000000;
	return nuevo;
}

void DHT11_read(dht* sensor) {
    /* Tried to sense too son since last read (dht11 needs ~1 seconds to make a new read) */
    if(esp_timer_get_time() - 1500000 < sensor->last_read_time) {
		sensor->status=DHT11_muy_rapido;
        return ;
    }

    sensor->last_read_time = esp_timer_get_time();

    uint8_t data[5] = {0,0,0,0,0};

    _sendStartSignal(sensor);

    if(_checkResponse(sensor) == DHT11_TIMEOUT_ERROR){
        _timeoutError(sensor);
		return;
	}
	
    /* Read response */
    for(int i = 0; i < 40; i++) {
        /* Initial data */
        if(_waitOrTimeout(50, 0, sensor) == DHT11_TIMEOUT_ERROR){
			_timeoutError(sensor);
			return ;
		}

        if(_waitOrTimeout(70, 1, sensor) > 28) {
            /* Bit received was a 1 */
            data[i/8] |= (1 << (7-(i%8)));
        }
    }

    if(_checkCRC(data) != DHT11_CRC_ERROR) {
        sensor->status = DHT11_OK;
        sensor->temperature = data[2];
        sensor->humidity = data[0];
        return ;
    } else {
		_crcError(sensor);
        return;
    }
}
