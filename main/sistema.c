#include "sistema.h"


#define prop 0.1



static const char *TAG = "sistema";

																		//colores del led

esp_mqtt_client_handle_t client;										//cliente MQTT
SemaphoreHandle_t varsruct,mut_setpoints;								//mutex
char *entrada,*salida;													//topicos MQTT
mensaje *variables_actuales,*set_points;								//variables de control y setpoints

TaskHandle_t h_hum =0;
TaskHandle_t h_luz =0;
TaskHandle_t h_tem =0;
TaskHandle_t h_log =0;
TaskHandle_t h_esc =0;
TaskHandle_t dht11 =0;
EventGroupHandle_t sist;												//control del sistema

const i2c_port_t i2c_luz=I2C_NUM_0;										//puerto del i2c
ads1115_t adc1,adc2;													//adc's
dht dht1,dht2,dht3,dhte;




void control_luz(void* parametros) {
	char red=0,green=0,blue=0;	
    const char *ctag = "control_luz";
    ESP_LOGI(ctag, "iniciado");
	tira_led leds=leds_init(CONFIG_led_clk,CONFIG_led_dat);
	float set=0.0;
	lectura_luz nueva_lectura1 ,nueva_lectura2;
    nueva_lectura1.direccion=MAX44009_ADDRESS1;
    //nueva_lectura2.direccion=MAX44009_ADDRESS2;
    nueva_lectura1.i2c_port=i2c_luz;
    //nueva_lectura2.i2c_port=i2c_luz;
	init_max(nueva_lectura1);
    while((xEventGroupGetBits(sist)&fin_sist)!=0) {                     //loop hasta salir
        
        /*
                leer sensor de luz
        */
        
        //xTaskCreate(&task_max44009_read_ambient_light, "max44009_read_ambient_light",  2048, &nueva_lectura1, 11, NULL);
        //xTaskCreate(&task_max44009_read_ambient_light, "max44009_read_ambient_light",  2048, &nueva_lectura2, 11, NULL);
		//ESP_LOGI(TAG,"inicio lectura");
		task_max44009_read_ambient_light(&nueva_lectura1);
		//ESP_LOGD(TAG, "lecturas en dirs:\ndir: %d valor: %.3f \ndir: %d valor: %.3f \n",nueva_lectura1.direccion,nueva_lectura1.valor,nueva_lectura2.direccion,nueva_lectura2.valor);
		/*
				almacenar valor
		*/
        if(xSemaphoreTake( varsruct, ( TickType_t ) 10 ) == pdTRUE) {
            ESP_LOGD(TAG, "mutex tomado");
            ((pares*)(variables_actuales->data)+Luz_interna)->valor=nueva_lectura1.valor;
            //((pares*)(variables_actuales->data)+Luz_externa)->valor=nueva_lectura2.valor;
            xSemaphoreGive( varsruct );
        } else {
            ESP_LOGE(TAG, "problemas con el mutex de variables");
        }
		/*
				control
		*/
		if(xSemaphoreTake(mut_setpoints,(TickType_t)10)==pdTRUE){
			set=((pares*)(set_points->data)+Luz_interna)->valor;
			xSemaphoreGive(mut_setpoints);
		}else{
            ESP_LOGE(TAG, "problemas con el mutex de set points");
        }
		set=200;
		if(set!=0 && nueva_lectura1.valor!=set){
			//ESP_LOGI(TAG,"control proporcional");
			
			float diff=(set - nueva_lectura1.valor)*(prop);
			
			if(red+diff>=255){
				red=255;
			}else{
			red=red+(char)(diff);			//nuevos valores a los led
			}
			green=red;
			blue=red;
			ESP_LOGI(TAG,"valor de rojo: %d\ndif: %.3f",red,set - nueva_lectura1.valor);
			set_color(red,green,blue,&leds);
		
		}
		
		
		
    }
    vTaskDelete(0);
}
void control_humedad(void* parametros) {
    const char *ctag = "control_humedad";
    ESP_LOGI(ctag, "iniciado");
    while((xEventGroupGetBits(sist)&fin_sist)!=0) {                                 //loop hasta salir
        vTaskDelay(20/portTICK_PERIOD_MS);
        ESP_LOGI(ctag, "humedad(lectura voltaje 1 actual): %f\n",ads1115_get_voltage(&adc1));
		ESP_LOGI(ctag, "humedad(lectura voltaje 2 actual): %f\n",ads1115_get_voltage(&adc2));
        if((xEventGroupGetBits(sist)&m_hum)!=0) {
            if(xSemaphoreTake( varsruct, ( TickType_t ) 10 ) == pdTRUE) {           //modo manual

                gpio_set_level(CONFIG_pin_bomba_irrigacion, ((pares*)(variables_actuales->data)+humedad_suelo)->valor);

                xSemaphoreGive( varsruct );
                vTaskDelay(CONFIG_hum_periodo*1000/portTICK_PERIOD_MS);
            } else {
                ESP_LOGE(TAG, "problemas con el mutex");
            }
        } else if(xSemaphoreTake( varsruct, ( TickType_t ) 10 ) == pdTRUE) {        //modo automatico
            float estado=0;
            //              control         determinar estado pára siguiente periodo



            //              fin del control

            ((pares*)(variables_actuales->data)+humedad_suelo)->valor=estado;
            xSemaphoreGive( varsruct );
            vTaskDelay(CONFIG_hum_periodo*1000/portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(0);
}
void control_temperatura(void* parametros) {
    const char *ctag = "control_temperatura";
    ESP_LOGI(ctag, "iniciado");
    while((xEventGroupGetBits(sist)&fin_sist)!=0) {                             //loop hasta salir
        if(xSemaphoreTake( varsruct, ( TickType_t ) 10 ) == pdTRUE) {
            ESP_LOGI(ctag, "temp: %d\n",(int)((pares*)(variables_actuales->data+temperatura_aire_ext))->valor);
            xSemaphoreGive( varsruct );
            vTaskDelay(1000/portTICK_PERIOD_MS);
        }else{
        }
    }
    vTaskDelete(0);
}
void loger(void* parametros) {
    const char *ctag = "loger";
    ESP_LOGI(ctag, "iniciado cada %d segundos",CONFIG_periodo_loggeo);
    while((xEventGroupGetBits(sist)&fin_sist)!=0) {


        if(xSemaphoreTake( varsruct, ( TickType_t ) 10 ) == pdTRUE) {
            ESP_LOGI(ctag, "enviando status\n");
            strcpy(variables_actuales->fuente,CONFIG_nombre);
            variables_actuales->tipo=estado;
            ajson(salida,variables_actuales);
            xSemaphoreGive( varsruct );

            pub(client,CONFIG_topico_out,salida,strlen(salida),2,0);


            //CONFIG_periodo_loggeo
            vTaskDelay(CONFIG_periodo_loggeo*1000/portTICK_PERIOD_MS);
        }else{
        }

    }
    vTaskDelete(0);
}
void escucha(void* parametros) {
	/*
		areglar funcion escucha
			leer el tipo y discriminar
				setpoints
				manual
	*/
    const char *TAG = "escucha";	
	mensaje nuevo_mensaje;
	nuevo_mensaje.tipo=error;
	nuevo_mensaje.data=0;
	nuevo_mensaje.num_data=0;
	nuevo_mensaje.max_data=0;
	
	avalores(entrada,',',&nuevo_mensaje);
	ESP_LOGI(TAG,"paso a valores num:%d",nuevo_mensaje.tipo);
	char bufer[CONFIG_buf_in];
	char enviado=0;

	
	strcpy(nuevo_mensaje.fuente,CONFIG_nombre);
	/*	TIPOS
		    no_tipo=-1,
			estado,
			alarma,
			error,
			consultar
	*/
	switch (nuevo_mensaje.tipo){
		case no_tipo	:
			ESP_LOGI(TAG, "Mensaje de tipo:%s",nombre_tipo(nuevo_mensaje.tipo));
			ajson(bufer,&nuevo_mensaje);
			pub(client,CONFIG_topico_out,bufer,strlen(bufer),2,0);

			
			break;
		case estado		:
			ESP_LOGI(TAG, "Mensaje de tipo:%s",nombre_tipo(nuevo_mensaje.tipo));
			while(enviado==0){
				if(xSemaphoreTake( varsruct, ( TickType_t ) 10 ) == pdTRUE) {
					strcpy(variables_actuales->fuente,CONFIG_nombre);
					variables_actuales->tipo=estado;
					ajson(bufer,variables_actuales);
					xSemaphoreGive( varsruct );
					pub(client,CONFIG_topico_out,bufer,strlen(bufer),2,0);
					enviado=1;
				}
			}
			break;
			
			
		case alarma		:
			ESP_LOGI(TAG, "Mensaje de tipo:%s",nombre_tipo(nuevo_mensaje.tipo));
			
			break;
		case error		:
			ESP_LOGI(TAG, "Mensaje de tipo:%s",nombre_tipo(nuevo_mensaje.tipo));
			ajson(bufer,&nuevo_mensaje);
			pub(client,CONFIG_topico_out,bufer,strlen(bufer),2,0);			
			break;

			break;
		case setpoint	:
			ESP_LOGI(TAG, "Mensaje de tipo:%s",nombre_tipo(nuevo_mensaje.tipo));
			enviado=0;
			while(enviado==0){
				if(xSemaphoreTake( mut_setpoints, ( TickType_t ) 10 ) == pdTRUE) {
					
					copiar_men(set_points,&nuevo_mensaje);
					ajson(bufer,set_points);
					xSemaphoreGive( mut_setpoints );
					pub(client,CONFIG_topico_out,bufer,strlen(bufer),2,0);
					enviado=1;
				}
			}
			
			
			break;
		default			:
			ESP_LOGI(TAG, "Mensaje sin tipo conocido:%d",nuevo_mensaje.tipo);
			ajson(bufer,&nuevo_mensaje);
			pub(client,CONFIG_topico_out,bufer,strlen(bufer),2,0);

			break;
	}


    
    vTaskDelete(0);
}
void sensardht11(void* parametros) {									//areglar integrar todos los dht11
    while((xEventGroupGetBits(sist)&fin_sist)!=0) {

        vTaskDelay(1000/portTICK_PERIOD_MS);
        ESP_LOGD(TAG, "inicio lectura dht11");
        DHT11_read(&dhte);
        while(dhte.status<0)DHT11_read(&dhte);
        
        if(xSemaphoreTake( varsruct, ( TickType_t ) 10 ) == pdTRUE) {
            ESP_LOGD(TAG, "mutex tomado");
            ((pares*)(variables_actuales->data)+temperatura_aire_ext)->valor=dhte.temperature;
            ((pares*)(variables_actuales->data)+humedad_aire_externa)->valor=dhte.humidity;
            xSemaphoreGive( varsruct );
        } else {
            ESP_LOGE(TAG, "problemas con el mutex");
        }


    }
}


void sist_init() {
    //                          crear buffers
    entrada=(char*)malloc(CONFIG_buf_in);
    salida=(char*)malloc(600);
    variables_actuales=crear_men_f();
    set_points=crear_men_f();
    varsruct=xSemaphoreCreateMutex();
    mut_setpoints=xSemaphoreCreateMutex();
    sist=xEventGroupCreate();
    xEventGroupSetBits(sist,fin_sist);

    //                           configuracion de pines del relay
    gpio_pad_select_gpio(CONFIG_pin_bomba_irrigacion);
    gpio_set_direction(CONFIG_pin_bomba_irrigacion, GPIO_MODE_OUTPUT);          //pin bomba de irrigacion
    gpio_pad_select_gpio(CONFIG_pin_bomba_circulacion);
    gpio_set_direction(CONFIG_pin_bomba_circulacion, GPIO_MODE_OUTPUT);         //pin bomba de circulacion
    gpio_pad_select_gpio(CONFIG_pin_calefactor);
    gpio_set_direction(CONFIG_pin_calefactor, GPIO_MODE_OUTPUT);                //pin del calefactor
    //gpio_pad_select_gpio(CONFIG_pin_¿?);
    //gpio_set_direction(CONFIG_pin_¿?, GPIO_MODE_OUTPUT);        				//salida disponible del grupo relays


	//							inciar dht's
	dhte=DHT11_init(CONFIG_pin_externo);
	dht1=DHT11_init(CONFIG_pin1_interno);
	dht2=DHT11_init(CONFIG_pin2_interno);
	dht3=DHT11_init(CONFIG_pin3_interno);



    if((varsruct==0)&(mut_setpoints==0)) {
        ESP_LOGE(TAG, "no se pudo crear algun mutex");
    }

    //                          iniciar i2c

    i2c_master_init(i2c_luz,400000);

    //                          iniciar wifi
    wifi_init_sta();
	
	//							iniciar adc's
	adc1 =ads1115_config(i2c_luz, 0b1001000 );									//conectado a gnd 72
	adc2 =ads1115_config(i2c_luz, 0b1001001 );									//conectado a vcc 73

    //                      establecer comunicacion mqtt

    mqtt_app_start(&client);


    //                          obtener setpoints
    limpiar_data(variables_actuales);
    limpiar_data(set_points);


    {                                                           //prueba del pub


        strcpy(variables_actuales->fuente,CONFIG_nombre);

        variables_actuales->tipo=4;
        ajson(salida,variables_actuales);

        pub(client,CONFIG_topico_out,salida,strlen(salida),2,0);
    }



    //                          agregar setpoints





    //                          iniciar funciones
	
	//xTaskCreate(&task_max44009_read_ambient_light, "max44009_read_ambient_light",  2048, &nueva_lectura2, 11, NULL);
		
	
    xTaskCreate(&control_humedad,    "C_HUMEDAD",CONFIG_task_stack,0,8,&h_hum);
    xTaskCreate(&control_luz,        "C_LUZ",CONFIG_task_stack,0,11,&h_luz);
    xTaskCreate(&control_temperatura,"C_TEMPERATURA",CONFIG_task_stack,0,7,&h_tem);
    xTaskCreate(&loger,              "LOGGER_MQTT",CONFIG_task_stack,0,1,&h_log);
    xTaskCreate(&sensardht11,        "SENSORES_DHT11",CONFIG_task_stack,0,9,&dht11);

    //                          borrar buffers
    while((xEventGroupGetBits(sist)&fin_sist)!=0) {                             //esperra a salir
        vTaskDelay(60*1000/portTICK_PERIOD_MS);
        ESP_LOGD(TAG, "\nesperando bit salida\n");
    }


    free(entrada);
    if(salida!=0)
        free(salida);
    destruir_men(variables_actuales);
    vSemaphoreDelete(varsruct);
    vSemaphoreDelete(mut_setpoints);
    ESP_LOGI(TAG, "\n\n mutex+buffers eliminados\n\n");




}

char* get_entrada() {
    return entrada;
}
char* get_salida() {
    return salida;
}
