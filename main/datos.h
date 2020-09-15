#ifndef DATOS_H_INCLUDED
#define DATOS_H_INCLUDED

typedef enum variables{
    no_var=-1,
    temperatura_aire_ext=0,
	temperatura_aire_int=1,
	temperatura_suelo=2,
	temperatura_del_agua,
	humedad_aire_externa,
	humedad_aire_interna,
	humedad_suelo,
	Luz_externa,
	Luz_interna,



	Bomba_agua_humedad,
	Bomba_agua_temperatura,
	calefactor,

	cantidad_rojo,
	cantidad_azul,
	cantidad_verde,

	corriente


}variables;
typedef enum tipo{
    no_tipo=-1,
    estado,
    alarma,
    error,
	setpoint

}tipo;

typedef struct pares{
    variables nombre;
    float valor;
}pares;
typedef struct mensaje{
    char fuente[15];
    tipo tipo;
    int num_data;
    int max_data;
    pares *data;
}mensaje;
extern const char * const str_variables[];
extern const char * const str_tipo[];
tipo get_tipo(const char*);
variables get_variable(const char*);
mensaje* crear_men(int num_data);
mensaje* crear_men_f();
void destruir_men(mensaje*);
void limpiar_data(mensaje*men);
void copiar_men(mensaje* dest,const mensaje* fuente);
int num_vars();
int num_tipos();
const char * nombre_tipo(int tipo);
const char * nombre_variable(int var);

#endif // DATOS_H_INCLUDED
