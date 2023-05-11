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
	void *elemento;
} cosa;

int abb_comparador_enteros(void *elemento1, void *elemento2)
{
	int *numero1 = elemento1;
	int *numero2 = elemento2;

	return (int)(numero1 - numero2);
}

cosa *crear_cosa(int clave)
{
	cosa *c = (cosa *)malloc(sizeof(cosa));
	if (c)
		c->clave = clave;
	return c;
}

int comparar_cosas(void *c1, void *c2)
{
	cosa *cosa1 = c1;
	cosa *cosa2 = c2;
	return cosa1->clave - cosa2->clave;
}

bool leer_elemento(void *elemento, void *extra)
{
	extra = extra; //para que no se queje el compilador, gracias -Werror -Wall
	int *numero1 = elemento;
	if (elemento)
		printf("%i ", *numero1);
	return true;
}

bool mostrar_hasta_5(void *elemento, void *extra)
{
	extra = extra; //para que no se queje el compilador, gracias -Werror -Wall
	if (elemento) {
		printf("%i ", ((cosa *)elemento)->clave);
		if (((cosa *)elemento)->clave == 5)
			return false;
	}
	return true;
}

void destruir_cosa(cosa *c)
{
	free(c);
}

void destructor_de_cosas(void *c)
{
	cosa *cosa = c;
	if (!cosa)
		return;

	free(cosa->elemento);
	destruir_cosa(c);
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

	for (size_t i = 1; i < ((sizeof(numeros) / sizeof(int)) - 1); i++)
		pa2m_afirmar(abb_insertar(abb, &numeros[i]) != NULL,
			     "Se puede insertar otro elemento en el arbol");

	pa2m_afirmar(abb_insertar(abb, &numeros[7]) != NULL,
		     "Se pueden insertar elementos repetidos en el arbol");

	pa2m_afirmar(!abb_vacio(abb) && abb_tamanio(abb) == 8,
		     "Se insertan 8 elementos, y en el arbol hay 8 elementos");

	abb_destruir(abb);
}

void pruebas_quitar_y_destruir()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);

	pa2m_afirmar(!abb_quitar(abb, NULL),
		     "No se pueden quitar elementos de una arbol vacio");

	int numeros[] = { 5, 3, 1, 7, 4, 7, 6 };

	for (size_t i = 0; i < sizeof(numeros) / sizeof(int); i++)
		abb_insertar(abb, &numeros[i]);

	pa2m_afirmar(abb_quitar(abb, &numeros[1]) == &numeros[1],
		     "Se puede quitar un elemento del arbol con dos hijos");

	pa2m_afirmar(abb_quitar(abb, &numeros[2]) == &numeros[2],
		     "Se puede quitar un elemento con un hijo derecho");

	pa2m_afirmar(abb_quitar(abb, &numeros[3]) == &numeros[3],
		     "Se puede quitar un elemento con un hijo izquierdo");

	pa2m_afirmar(abb_quitar(abb, &numeros[4]) == &numeros[4],
		     "Se puede quitar un elemento sin hijos");

	pa2m_afirmar(abb_quitar(abb, &numeros[5]) == &numeros[5],
		     "Se pueden quitar elementos repetidos de un arbol");

	pa2m_afirmar(abb_quitar(abb, &numeros[0]) == &numeros[0],
		     "Se puede quitar el elemento en la raiz del arbol");

	pa2m_afirmar(!abb_vacio(abb) && abb_tamanio(abb) == 1,
		     "Se quitan 6 elementos y el tamanio del arbol es 1");

	abb_destruir(abb);
}

void pruebas_buscar_elementos_por_comparacion()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);

	pa2m_afirmar(!abb_buscar(abb, NULL),
		     "No se pueden encontrar elementos en una arbol vacio");

	int numeros[] = { 5, 3, 1, 7, 4, 7, 6 };
	//int vec_buscar[] = { 5, 3, 1, 7, 4, 7, 6 };

	size_t i;
	for (i = 0; i < sizeof(numeros) / sizeof(int); i++)
		abb_insertar(abb, &numeros[i]);

	for (i = 0; i < sizeof(numeros) / sizeof(int); i++)
		pa2m_afirmar(abb_buscar(abb, &numeros[i]) == &numeros[i],
			     "Se puede encontrar el elemento en el arbol");

	pa2m_afirmar(
		!abb_buscar(abb, NULL),
		"NO se puede encontrar un elemento que no esta en el arbol");

	abb_destruir(abb);
}

void pruebas_abb_iterador_interno()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);
	int numeros[] = { 5, 1, 9, 7, 2, 6, 7 };

	pa2m_afirmar(!abb_con_cada_elemento(abb, INORDEN, leer_elemento, NULL),
		     "No se pueden leer elementos en un arbol vacio");

	for (size_t i = 0; i < sizeof(numeros) / sizeof(int); i++)
		abb_insertar(abb, &numeros[i]);

	size_t recorridos =
		abb_con_cada_elemento(abb, INORDEN, leer_elemento, NULL);
	printf("\n");
	pa2m_afirmar(recorridos == abb_tamanio(abb),
		     "Se pueden leer los elementos guardados ascendentemente");

	recorridos = abb_con_cada_elemento(abb, PREORDEN, leer_elemento, NULL);
	printf("\n");
	pa2m_afirmar(recorridos == abb_tamanio(abb),
		     "Se puede leer una copia del arbol");

	recorridos = abb_con_cada_elemento(abb, POSTORDEN, leer_elemento, NULL);
	printf("\n");
	pa2m_afirmar(recorridos == abb_tamanio(abb),
		     "Se pueden leer los elementos de forma postorden");

	printf("\nElimino un elemento y vuelvo a iterar el arbol\n");
	abb_quitar(abb, &numeros[1]);

	recorridos = abb_con_cada_elemento(abb, INORDEN, leer_elemento, NULL);
	printf("\n");
	pa2m_afirmar(recorridos == abb_tamanio(abb),
		     "Se pueden leer los elementos guardados ascendentemente");

	recorridos = abb_con_cada_elemento(abb, PREORDEN, leer_elemento, NULL);
	printf("\n");
	pa2m_afirmar(recorridos == abb_tamanio(abb),
		     "Se puede leer una copia del arbol");

	recorridos = abb_con_cada_elemento(abb, POSTORDEN, leer_elemento, NULL);
	printf("\n");
	pa2m_afirmar(recorridos == abb_tamanio(abb),
		     "Se pueden leer los elementos de forma postorden");

	printf("\nElimino otro elemento y vuelvo a iterar el arbol\n");
	abb_quitar(abb, &numeros[2]);

	abb_destruir(abb);
}

void pruebas_abb_recorrer_guardar_en_vector()
{
	abb_t *abb = abb_crear(abb_comparador_enteros);
	int numeros[] = { 5, 1, 9, 7, 2, 6, 7 };
	size_t tamanio_array = sizeof(numeros) / sizeof(int);

	pa2m_afirmar(!abb_recorrer(abb, INORDEN, (void **)numeros,
				   tamanio_array),
		     "No se puede recorrer un arbol vacio");

	for (size_t i = 0; i < tamanio_array; i++)
		abb_insertar(abb, &numeros[i]);

	size_t recorridos =
		abb_con_cada_elemento(abb, INORDEN, leer_elemento, NULL);
	printf("Los elementos deberian estar guardados INORDEN\n");
	for (size_t i = 0; i < recorridos; i++)
		printf("La posicion %zu es: %i\n", i, numeros[i]);

	abb_destruir(abb);
}

void pruebas_de_operaciones_del_tda_abb()
{
	pa2m_nuevo_grupo("PRUEBAS DE CREACION Y DESTRUCCION");
	//pruebas_de_creacion_y_destruccion_del_abb();

	pa2m_nuevo_grupo("PRUEBAS DE INSERTAR ELEMENTOS Y DESTRUIR ARBOL");
	//pruebas_insertar_y_destruir();

	pa2m_nuevo_grupo("PRUEBAS DE QUITAR Y DESTRUIR ELEMENTOS");
	//pruebas_quitar_y_destruir();

	pa2m_nuevo_grupo("PRUEBAS DE BUSCAR ELEMENTOS POR CONDICION");
	//pruebas_buscar_elementos_por_comparacion();

	pa2m_nuevo_grupo("PRUEBAS DEL ITERADOR INTERNO");
	pruebas_abb_iterador_interno();

	pa2m_nuevo_grupo("PRUEBAS DE RECORRER Y GUARDAR ELEMENTOS EN VECTOR");
	pruebas_abb_recorrer_guardar_en_vector();
}

/*
-----------------------------------------------------------------------------
*/

void pruebas_de_destruir_todo_en_el_abb()
{
	abb_t *abb = abb_crear(comparar_cosas);

	cosa *c1 = crear_cosa(1);
	cosa *c2 = crear_cosa(2);
	cosa *c3 = crear_cosa(3);
	cosa *c4 = crear_cosa(4);

	c1->elemento = malloc(sizeof(int));
	c2->elemento = malloc(sizeof(float));
	c3->elemento = malloc(sizeof(char));
	c4->elemento = malloc(sizeof(pkm_para_destruir_t));

	abb_insertar(abb, c4);
	abb_insertar(abb, c2);
	abb_insertar(abb, c1);
	abb_insertar(abb, c3);

	abb_destruir_todo(abb, destructor_de_cosas);
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

	pa2m_afirmar(!abb_con_cada_elemento(NULL, INORDEN, leer_elemento, NULL),
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
