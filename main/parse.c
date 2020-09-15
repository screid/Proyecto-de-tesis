#include "parse.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"


void anadir_str(char *buff,const char * add) {
    strcat(buff,"\"");
    strcat(buff,add);
    strcat(buff,"\"");
}
void anadir_flo(char *buff,const float val) {
    char aux[10];
    sprintf(aux,"%0.3f",val);
    strcat(buff,aux);
}


void anadir_par(char *buff,const pares* par) {
    anadir_str(buff,str_variables[par->nombre]);
    strcat(buff,":");
    anadir_flo(buff,par->valor);
}
void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage)
            dst++;
    }
    *dst = '\0';
}
void limpiar(char cad[]) {
    for (int i=0; i<25; i++) {
        cad[i]=0;
    }
}


int ajson(char* buffer,const mensaje*men) {



    strcpy(buffer,"{\"nombre\":\"ESP32\",\0");
    anadir_str(buffer,"tipo");
    strcat(buffer,":");
    if(men->tipo!=no_tipo){
        anadir_str(buffer,str_tipo[num_tipos()-1]);                                  //añade el tipo
    }else{
        anadir_str(buffer,str_tipo[men->tipo]);
    }
	


    //((pares*)(variables_actuales->data)+temperatura_aire_ext)->valor=lectura.temperature;


    strcat(buffer,",\"data\":{");                                                //inicio del data
    for(int indice=0; indice<men->num_data; indice++) {                                //iterar sobre cada pares y agregarlos a data
        if(((pares*)(men->data)+indice)->nombre==no_var){
            anadir_str(buffer,     str_variables[num_vars()-1]);
        }else{
            anadir_str(buffer, str_variables[((pares*)(men->data)+indice)->nombre]);  //agregar en nombre
        }


        strcat(buffer,":");
        anadir_flo(buffer,(((pares*)(men->data)+indice)->valor));                  //agregar en valor
        if(!(indice==men->num_data-1))
            strcat(buffer,",");                                    //agregar la coma a todos menos el ultimo
    }
    strcat(buffer,"}}");

    return strlen(buffer);                                                      //entregar el tamano resultante

}
int data_size(const char *buffer,char separador) {
    char* aux=strchr(buffer+1,'{')+1;                                             //despues del segundo {
    if(strchr(aux,'}')<(aux+5)) {
        return 0;                                                               //si no hay data o es nulo return 0
    }
    int num=1;
    while((aux=strchr(aux+1,separador))!=0 ) {
        num++;
    }
    return num;
}

int avalores(const char *buffer,char separador,mensaje* men ) {

    men->num_data=data_size(buffer,separador);                                  //agregar en numero de variables en data
	if(men->data==0){															//verificar que exista memoria alocada
		men->data=(pares*)calloc(men->num_data,sizeof(pares));
		men->max_data=men->num_data;
	}
    if(men->num_data>men->max_data){											//verificar que la memoria alocada alcance
        men->data=(pares*)realloc(men->data,men->num_data*sizeof(pares));
        men->max_data=men->num_data;
    }
    limpiar_data(men);															//limpiar la memoria alocada

    char *arr=(char*)malloc(strlen(buffer));
    char cadena[25];
    strcpy(arr,buffer);                                                         //el mensaje ahora esta en arr[]
    removeChar(arr,'"');                                                        // quitar todo "
    char*aux;
    int aux_tam;

//////////////////////////////////////////////nombre//////////////////////////////////////////////

    aux=strchr(arr,'{')+1;                                                  //inicio aux
    aux_tam=strchr(aux,(int)':')-aux;                                           //tamaño aux
    memcpy(men->fuente,aux,(unsigned int)(aux_tam));                                        //capturar el nombre nombre

    if(strcmp(men->fuente,"nombre")) {
        printf("fuente:%s\n",men->fuente);
        return 0;                                 //verificar
    }

    aux=aux+aux_tam+1;                                                          //despues de la ,
    aux_tam=strchr(aux,(int)',')-aux;
    memcpy(men->fuente,aux,(unsigned int)(aux_tam));                                        //capturar el nombre


//////////////////////////////////////////////tipo//////////////////////////////////////////////

    aux=aux+aux_tam+1;
    aux_tam=strchr(aux,(int)':')-aux;
    limpiar(cadena);
    memcpy(cadena,aux,(unsigned int)(aux_tam));                                             //capturar si hay tipo


    if(strcmp(cadena,"tipo"))
        return 0;                                        //verificar si existe tipo

    aux=aux+aux_tam+1;                                                          //despues del:
    aux_tam=strchr(aux,(int)',')-aux;
    limpiar(cadena);
    memcpy(cadena,aux,(unsigned int)(aux_tam));                                             //capturar el nombre del tipo
    men->tipo=get_tipo(cadena);


//////////////////////////////////////////////data//////////////////////////////////////////////

    aux=aux+aux_tam+1;
    aux_tam=strchr(aux,(int)':')-aux;
    limpiar(cadena);
    memcpy(cadena,aux,(unsigned int)(aux_tam));                                                 //capturar si hay data


    if(!strcmp(cadena,"data")) {                                                    //verificar si existe data
        aux=strchr(aux,(int)'{')+1;
        aux_tam=strchr(aux,(int)':')-aux;

        for(int i=0; i<men->num_data; i++) {
            limpiar(cadena);
            memcpy(cadena,aux,(unsigned int)(aux_tam));
            ((pares*)(men->data+i))->nombre=get_variable(cadena);

            if(((pares*)(men->data+i))->nombre==0) {
                return 0;
            }

            aux=aux+aux_tam+1;

            if(i!=men->num_data-1)
                aux_tam=strchr(aux,(int)',')-aux;                                   //nuevo final no ultimo caso
            if(i==men->num_data-1)
                aux_tam=strchr(aux,(int)'}')-aux;                                   //nuevo final ultimo caso

            limpiar(cadena);
            memcpy(cadena,aux,(unsigned int)(aux_tam));
            ((pares*)(men->data+i))->valor=(float)atof(cadena);

            aux=aux+aux_tam+1;
            aux_tam=strchr(aux,(int)':')-aux;
        }
    }
	//free(arr);
    return men->num_data;
}
