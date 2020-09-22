#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED
#include "datos.h"
#define _CRT_SECURE_NO_WARNINGS


    /** \brief tranforma la estructura del mensaje al formato json
     * \param bufer donde se guardara el mensaje procesado
	 * \param estructura que contiene la informacion a enviar
     * \return el tamano final del string guadado en el buffer
     */
int ajson(char* buffer,const mensaje*men);//primer nombre es el tipo de mensaje
	 /** \brief calculo de elementos en un mensaje JSON
     * \param buffer que contiene el mensaje JSON
	 * \param carracter caracteristico que dilimita cada elemento dentro del objeto data
     * \return numero de elementos en el mensaje
     */
int data_size(const char *buffer,char separador);
     /** \brief traspaso del mensaje en formato JSON a la estructura de tipo menssaje
	 * \param buffer con el mensaje en JSON
	 * \param carracter caracteristico que dilimita cada elemento dentro del objeto data
	 * \param estructura que recibira el mensaje procesado
     * \return el numero de elementos en data
     */   
int avalores(const char *buffer,char separador,mensaje* men );



#endif // PARSE_H_INCLUDED
