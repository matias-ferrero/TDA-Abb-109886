#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

abb_t *abb_crear(abb_comparador comparador)
{
	return calloc(1, sizeof(abb_t));
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	return arbol;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	return elemento;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	return elemento;
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol || !abb_tamanio(arbol))
		return true;

	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;

	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol)
		return;

	abb_destruir_todo(arbol, NULL);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion)
		return 0;

	return 0;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	return 0;
}