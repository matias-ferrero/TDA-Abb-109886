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

int comparador(void *elemento1, void *elemento2)
{
	return 0;
}

bool comparador_elementos(void *elemento1, void *elemento2)
{
	return false;
}

void pruebas_de_creacion_y_destruccion_del_abb()
{
	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL,
		     "Se puede crear un arbol binario de busqueda con exito");

	pa2m_afirmar(!abb_tamanio(abb) && abb_vacio(abb),
		     "El arbol esta vacio");

	abb_destruir(abb);
}

void pruebas_insertar_y_destruir()
{
	abb_t *abb = abb_crear(comparador);

	void *elemento1 = (void *)0x1234;

	pa2m_afirmar(abb_insertar(abb, &elemento1) != NULL,
		     "Se puede insertar al final de una abb vacia");

	pa2m_afirmar(abb_insertar(abb, NULL) != NULL,
		     "Se puede insertar NULL al final de la abb");

	abb_destruir(abb);
}

void pruebas_de_operaciones_del_tda_abb()
{
	pa2m_nuevo_grupo("PRUEBAS DE CREACION Y DESTRUCCION");
	pruebas_de_creacion_y_destruccion_del_abb();

	//pa2m_nuevo_grupo("PRUEBAS DE INSERTAR ELEMENTOS Y DESTRUIR ARBOL");
	//pruebas_insertar_y_destruir();

	//pa2m_nuevo_grupo("PRUEBAS DE LEER UNA abb");
	//pruebas_leer_abb();
}

/*
-----------------------------------------------------------------------------
*/

void pruebas_de_destruir_todo_en_el_abb()
{
	abb_t *abb = abb_crear(comparador);

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
	abb_t *abb = abb_crear(comparador);

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

	pa2m_afirmar(!abb_vacio(NULL),
		     "Un arbol que no existe, es un arbol vacio");

	pa2m_afirmar(!abb_tamanio(NULL),
		     "Un arbol que no existe, tiene 0 elementos");

	pa2m_afirmar(!abb_con_cada_elemento(NULL, INORDEN, comparador_elementos, NULL),
		     "Iterar un arbol que no existe da error");

	pa2m_afirmar(!abb_con_cada_elemento(abb, INORDEN, NULL, NULL),
		     "Iterar un arbol con una funcion que no existe da error");

	pa2m_afirmar(!abb_recorrer(NULL, INORDEN, array, tamanio_array),
		     "Recorrer un arbol que no existe da error");

	pa2m_afirmar(!abb_recorrer(abb, INORDEN, NULL, tamanio_array),
		     "Guardar elementos en un vector que no existe da error");
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

	pa2m_nuevo_grupo("--- PRUEBAS DE FUNCIONES CON PARAMETROS NULOS ---");
	pruebas_del_tda_abb_con_parametros_nulos();

	return pa2m_mostrar_reporte();
}
