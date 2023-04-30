#include "pa2m.h"

#include "src/abb.h"
#include "src/abb_estructura_privada.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	size_t id;
	size_t salud;
	char *nombre_entrenador;
	char *nombre;
} pkm_para_destruir_t;

typedef struct cosa {
	int clave;
	char contenido[10];
} cosa;

int abb_comparador_enteros(void *elemento1, void *elemento2)
{
	int *numero1 = elemento1;
	int *numero2 = elemento2;
	
	return (int)(numero1 - numero2);
}

bool mostrar_elemento(void *elemento, void *extra)
{
	extra = extra; //para que no se queje el compilador, gracias -Werror -Wall
	if (elemento)
		printf("%i \n", ((cosa *)elemento)->clave);
	return true;
}

void pruebas_de_creacion_y_destruccion_del_abb()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);
	pa2m_afirmar(abb != NULL,
		     "Se puede crear un arbol binario de busqueda con exito");

	pa2m_afirmar(!abb_tamanio(abb) && abb_vacio(abb),
		     "El arbol esta vacio");

	abb_destruir(abb);
}

void pruebas_insertar_y_destruir()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);

	int numeros[] = { 5, 1, 9, 7, 2, 6, 7 };

	pa2m_afirmar(abb_insertar(abb, &numeros[0]) != NULL,
		     "Se puede insertar un elemento en un arbol vacio");

	pa2m_afirmar(abb_insertar(abb, NULL) != NULL,
		     "Se puede insertar NULL en el arbol");

	size_t i;
	for (i = 1; i < ((sizeof(numeros) / sizeof(int)) - 1); i++)
		pa2m_afirmar(abb_insertar(abb, &numeros[i]) != NULL, 
			     "Se puede insertar otro elemento en el arbol");

	pa2m_afirmar(abb_insertar(abb, &numeros[i + 1]) != NULL, 
		     "Se pueden insertar elementos repetidos en el arbol");

	pa2m_afirmar(!abb_vacio(abb) && abb_tamanio(abb) == 8,
		     "Se insertan 8 elementos, y en el arbol hay 8 elementos");

	abb_destruir(abb);
}

void pruebas_quitar_y_destruir()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);

	int numeros[] = { 5, 1, 9, 7, 2, 6, 7 };

	size_t i;
	for (i = 0; i < sizeof(numeros) / sizeof(int); i++)
		abb_insertar(abb, &numeros[i]);

	pa2m_afirmar(abb_quitar(abb, &numeros[0]) == &numeros[0], 
		     "Se puede quitar el elemento en la raiz del arbol");

	i--;
	pa2m_afirmar(!abb_vacio(abb) && abb_tamanio(abb) == (i + 1),
		     "Se quita 1 elemento, y quedan 7 elementos en el arbol");

	abb_destruir(abb);
}

void pruebas_buscar_elementos_por_comparacion()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);

	int numeros[] = { 5, 1, 9, 7, 2, 6, 7 };

	size_t i;
	for (i = 0; i < sizeof(numeros) / sizeof(int); i++)
		abb_insertar(abb, &numeros[i]);

	pa2m_afirmar(abb_buscar(abb, &numeros[0]) == &numeros[0], 
		     "Se puede encontrar el elemento en la raiz del arbol");

	pa2m_afirmar(!abb_buscar(abb, NULL), 
		     "NO se puede encontrar un elemento que no esta en el arbol");

	abb_destruir(abb);
}

void pruebas_de_operaciones_del_tda_abb()
{
	pa2m_nuevo_grupo("PRUEBAS DE CREACION Y DESTRUCCION");
	pruebas_de_creacion_y_destruccion_del_abb();

	pa2m_nuevo_grupo("PRUEBAS DE INSERTAR ELEMENTOS Y DESTRUIR ARBOL");
	pruebas_insertar_y_destruir();

	pa2m_nuevo_grupo("PRUEBAS DE QUITAR Y DESTRUIR ELEMENTOS");
	//pruebas_quitar_y_destruir();

	pa2m_nuevo_grupo("PRUEBAS DE BUSCAR ELEMENTOS POR CONDICION");
	pruebas_buscar_elementos_por_comparacion();
}

/*
-----------------------------------------------------------------------------
*/

void pruebas_de_destruir_todo_en_el_abb()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);

	abb_insertar(abb, malloc(sizeof(int)));
	abb_insertar(abb, malloc(sizeof(float)));
	abb_insertar(abb, malloc(sizeof(long)));
	abb_insertar(abb, malloc(sizeof(char)));
	abb_insertar(abb, malloc(sizeof(pkm_para_destruir_t)));

	abb_destruir_todo(abb, free);
	printf("Destruir abb con exito no debe perder memoria\n");
}

/*
-------------------------------------------------------------------------------
*/

void pruebas_del_tda_abb_con_parametros_nulos()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);

	size_t tamanio_array = 10;
	void *array[tamanio_array];

	char a = 'a';

	pa2m_afirmar(!abb_crear(NULL),
		     "Crear un abb con un comparador que no existe da error");

	pa2m_afirmar(!abb_insertar(NULL, &a),
		     "Insertar elementos a un arbol que no existe da error");

	pa2m_afirmar(!abb_quitar(NULL, &a),
		     "Quitar elementos de un arbol que no existe da error");

	pa2m_afirmar(!abb_buscar(NULL, &a),
		     "Buscar elementos en un arbol que no existe da error");

	pa2m_afirmar(abb_vacio(NULL),
		     "Un arbol que no existe, es un arbol vacio");

	pa2m_afirmar(!abb_tamanio(NULL),
		     "Un arbol que no existe, tiene 0 elementos");

	pa2m_afirmar(!abb_con_cada_elemento(NULL, INORDEN, mostrar_elemento, NULL),
		     "Iterar un arbol que no existe da error");

	pa2m_afirmar(!abb_con_cada_elemento(abb, INORDEN, NULL, NULL),
		     "Iterar un arbol con una funcion que no existe da error");

	pa2m_afirmar(!abb_recorrer(NULL, INORDEN, array, tamanio_array),
		     "Recorrer un arbol que no existe da error");

	pa2m_afirmar(!abb_recorrer(abb, INORDEN, NULL, tamanio_array),
		     "Guardar elementos en un vector que no existe da error");

	abb_destruir(abb);
}

/*
-------------------------------------------------------------------------------
*/

int main()
{
	pa2m_nuevo_grupo("----------- PRUEBAS DEL TP: TDA-ABB -----------");

	pa2m_nuevo_grupo("--- PRUEBAS DE LAS OPERACIONES DEL ARBOL ---");
	pruebas_de_operaciones_del_tda_abb();
	printf("\n");

	pa2m_nuevo_grupo("--- PRUEBAS DEL DESTRUCTOR ---");
	printf("Librerar el arbol y sus elementos no debe perder memoria");
	//pruebas_de_destruir_todo_en_el_abb();
	printf("\n");

	//pa2m_nuevo_grupo("--- PRUEBAS DE FUNCIONES CON PARAMETROS NULOS ---");
	//pruebas_del_tda_abb_con_parametros_nulos();

	return pa2m_mostrar_reporte();
}
