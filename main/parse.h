#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED
#include "datos.h"
#define _CRT_SECURE_NO_WARNINGS



int ajson(char* buffer,const mensaje*men);//primer nombre es el tipo de mensaje
int data_size(const char *buffer,char separador);
int avalores(const char *buffer,char separador,mensaje* men );



#endif // PARSE_H_INCLUDED
