#include "datos.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>



const char * const str_variables[]= {

    "temperatura_aire_ext",
    "temperatura_aire_int",
    "temperatura_suelo",
    "temperatura_del_agua",
    "humedad_aire_externa",
    "humedad_aire_interna",
    "humedad_suelo",
    "Luz_externa",
    "Luz_interna",



    "Bomba_agua_humedad",
    "Bomba_agua_temperatura",
    "calefactor",

    "cantidad_rojo",
    "cantidad_azul",
    "cantidad_verde",

    "corriente",
    "no_var"
};
const char * const str_tipo[]= {

    "estado",		//0
    "alarma",		//1
    "error",		//2
	"setpoint",		//3
    "no_tipo"		//4
};

int  num_vars(){
    return sizeof(str_variables) / sizeof(str_variables[0]);
}
int  num_tipos(){
    return sizeof(str_tipo) / sizeof(str_tipo[0]);
}

tipo get_tipo(const char* stipo) {
    for(int i=0; i< sizeof(str_tipo) / sizeof(str_tipo[0]); i++) {
        if(!strcmp(stipo,str_tipo[i])) {
            return (tipo)i;
        }
    }
    return no_tipo;
}
variables get_variable(const char*svar) {
    for(int i=0; i< sizeof(str_variables) / sizeof(str_variables[0]); i++) {
        if(!strcmp(svar,str_variables[i])) {
            return (variables)i;
        }
    }

    return no_var;
}

mensaje* crear_men(int num_data){
////////////////////////////pares////////////////////////////
    pares* pp=(pares*)calloc(num_data,sizeof(pares));
    //printf("bites alocados:%d",num_data*sizeof(pares));
////////////////////////////mensaje////////////////////////////
    mensaje* me=(mensaje*)calloc(1,sizeof(mensaje));
    me->data=pp;
    me->num_data=0;
    me->max_data=num_data;
    return me;
}
mensaje* crear_men_f(){
	int i= num_vars()-1;
	printf("num:%d",i);
	mensaje* listo=crear_men(i);
	for(int j=0;j<i;j++){
        (((pares*)(listo->data))+j)->nombre=j;
        (((pares*)(listo->data))+j)->valor=0;
	}
	listo->num_data=listo->max_data;
    return listo;
}
void copiar_men(mensaje* dest,const mensaje* fuente){
	/*
	typedef struct mensaje{
    char fuente[15];
    tipo tipo;
    int num_data;
    int max_data;
    pares *data;
}mensaje;

    variables nombre;
    float valor;
	
*/
	
	strcpy(dest->fuente,fuente->fuente);
	dest->tipo=fuente->tipo;
	 if(dest->max_data<fuente->max_data ){
		 if(dest->max_data==0){
			 dest->data=(pares*)calloc(fuente->num_data,sizeof(pares));
		}else{
			 dest->data=(pares*)realloc(dest->data,sizeof(pares)*fuente->max_data);
		}
		dest->max_data=fuente->max_data;
		dest->num_data=fuente->num_data;
	 }
	dest->num_data=fuente->num_data;
	
	
	for(int i=0;i<fuente->num_data;i++){
		((pares*)(dest->data)+i)->nombre=((pares*)(fuente->data)+i)->nombre;
		((pares*)(dest->data)+i)->valor =((pares*)(fuente->data)+i)->valor;
	}
}

void destruir_men(mensaje*men){

	pares* dat = men->data;
    free((dat));

    free(men);

}
void limpiar_data(mensaje*men){
    for (int i=0;i<men->max_data;i++){
        //(((pares*)(men->data))+i)->nombre=0;
        (((pares*)(men->data))+i)->valor=0;
    }
}

const char * nombre_tipo(int tipo){
	if(tipo<0)return str_tipo[num_tipos()-1];
	return str_tipo[tipo];
}
const char * nombre_variable(int var){
	if(var<0)return str_variables[num_vars()-1];
	return str_variables[var];
	
}










