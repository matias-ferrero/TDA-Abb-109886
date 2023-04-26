#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

#define COMPARACION 0

nodo_abb_t *insertar_nodo(abb_t *arbol, nodo_abb_t *nodo_a_insertar,
			      nodo_abb_t *nodo_actual)
{
	if (!nodo_actual)
		return nodo_a_insertar;

	if (arbol->comparador(nodo_a_insertar->elemento, nodo_actual->elemento)
	    <= COMPARACION)
		nodo_actual->izquierda = insertar_nodo(arbol,
						       nodo_a_insertar,
						       nodo_actual->izquierda);
	else
		nodo_actual->derecha = insertar_nodo(arbol,
						     nodo_a_insertar,
						     nodo_actual->izquierda);

	return nodo_actual;
}

abb_t *abb_crear(abb_comparador comparador)
{
	return calloc(1, sizeof(abb_t));
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	nodo_abb_t *nodo = calloc(1, sizeof(nodo_abb_t));
	if (!nodo)
		return NULL;

	nodo->elemento = elemento;
	arbol->nodo_raiz = insertar_nodo(arbol, nodo, arbol->nodo_raiz);

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