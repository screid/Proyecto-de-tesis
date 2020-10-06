#ifndef oled_menu
#define oled_menu
#include "stdint.h"

typedef enum{
	MENUES,
	INTEGER,
	DOUBLE,
	CHAR,
	STRINGS
}tipo_menu;

typedef struct{
	uint8_t 	num_opciones;
	const char* nombre;
	tipo_menu 	tipo;
	menu* 		padre;
	union {
		menu* 	hijos;
		int* 	var_int;
		float* 	var_float;
		double*	var_double;
		char*	var_char;
		char*	var_string;
	};
	
}menu;

/** \brief crea un nuevo menu pricipal
 *	\param nombre del  nuevo menu
 *	\return puntero al menu creado
 */
menu* crear_menu(const char* nombre);

/** \brief crea un nuevo sub-menu
 *	\param puntero al menu padre
 *	\param nombre del nuevo sub-menu
 *	\return puntero al sub-menu creado
 */
menu* crear_sub_menu(menu* padre, const char* sub_nombre);

/**	\brief añade el sub-menu al menu padre
 *	\param puntero al menu padre que recibira el hijo
 *	\param puntero del menu hijo que sera agregado
 */
void  add_sub_menu(menu* padre, menu* hijo);

/**	\brief imprime el menu indicado el la pantalla oled
 *	\param puntero al menu que se quiere imprimir
 *	\param puntero a la pantalla oled donde se quiere desplegar el menu
 */
void  print_menu(menu* menu_actual, oled* pantalla);

/**	\brief inicia el sistema de menus
 *	\param puntero al menu principal
 *	\param puntero a la pantalla oled donde se quiere desplegar el menu
 *	\return
 */
void  inicio_menus(menu* padre, oled* pantalla);

/**	\brief elimina al menu padre entregado y a todos sus hijos
 *	\param puntero al menu principal1
 *	\return numero de menues destruidos
 */
int	  destruir_menus(menu* padre);

#endif