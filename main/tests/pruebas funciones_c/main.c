#include <stdio.h>
#include <stdlib.h>
#include "../../parse.h"

int main() {
    printf("Hello world!\n");
    const char* mijason="{\"nombre\":\"server\",\"tipo\":\"alarma\",\"data\":{\"temperatura_aire_ext\":10.2,\"temperatura_aire_int\":25.5,\"cantidad_azul\":10}}";

    const char* mijason2="{\"nombre\":\"server\",\"tipo\":\"alarma\",\"data\":{\"temperatura_aire_ext\":10.2,\"temperatura_aire_int\":25.5,\"cantidad_azul\":10,\"cantidad_rojo\":11}}";


    int a=data_size(mijason,',');

    mensaje* mimensaje=crear_men(a+1);
    avalores(mijason,',',mimensaje);



    printf("nombre:%s\n",mimensaje->fuente);
    printf("cant azul:%.3f\n",((pares*)(mimensaje->data+2))->valor);

    avalores(mijason2,',',mimensaje);
    printf("nombre:%s\n",mimensaje->fuente);

    printf("var %d:%.3f",((pares*)(mimensaje->data+3))->nombre,((pares*)(mimensaje->data+3))->valor);
    printf("\nnombre:%s\n",mimensaje->fuente);

    printf("cant azul:%.3f\n",((pares*)(mimensaje->data+2))->valor);

    char aux[255];

    ajson(aux,mimensaje);                  //livera el data
    //pares*auxa=mimensaje->data;
    printf("%s\n",aux);




    destruir_men(mimensaje);
    
	char *entrada, *salida;
	mensaje* variables_actuales;
	salida = (char*)malloc(600);
	variables_actuales = crear_men_f();
	limpiar_data(variables_actuales);
	strcpy(variables_actuales->fuente, "esp32");

	variables_actuales->tipo = 1;
	ajson(salida, variables_actuales);
	printf("max: %d\nactual: %d\n", variables_actuales->max_data,variables_actuales->num_data);
	printf("funco:%s",salida);

	if (salida != 0)free(salida);
	destruir_men(variables_actuales);



    /*typedef struct eso {
        int a;
        int b;
        int *c;
    } eso;
    eso* uno=(eso*)calloc(1,sizeof(eso));
    uno->a=1;
    uno->c=(int*)calloc(sizeof(int),uno->a);
    *(uno->c)=123;

    printf("c es:%d\n",*(uno->c));
    int *pan=uno->c;

    printf("c es:%d\n",*(pan));

    free(uno->c);
    free(uno);
    printf("c es:%d\n",*(pan));*/
    return 0;
}
