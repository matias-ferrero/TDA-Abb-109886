#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define COMPARACION 0

struct vector {
	void **vector;
	size_t tope;
	size_t posicion;
};


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

nodo_abb_t *buscar_nodo_por_elemento(abb_t *arbol, nodo_abb_t *nodo,
				     void *elemento)
{
	if (!nodo)
		return NULL;

	if (arbol->comparador(nodo->elemento, elemento) < COMPARACION)
		nodo = buscar_nodo_por_elemento(arbol,
							  nodo->izquierda,
						 	  elemento);
	else if (arbol->comparador(nodo->elemento, elemento) > COMPARACION)
		nodo = buscar_nodo_por_elemento(arbol, nodo->derecha,
						 	  elemento);

	return nodo;
}

nodo_abb_t *extraer_predecesor_inorden(nodo_abb_t *predecesor_inorden,
				       void **elemento)
{
	if (!predecesor_inorden->derecha) {
		*elemento = predecesor_inorden->elemento;
		nodo_abb_t *izq_de_predecesor_inorden = predecesor_inorden->izquierda;
		free(predecesor_inorden);
		return izq_de_predecesor_inorden;
	}

	predecesor_inorden->derecha = 
		extraer_predecesor_inorden(predecesor_inorden->derecha,
	 				     elemento);
	return predecesor_inorden;
}

nodo_abb_t *abb_quitar_nodo(nodo_abb_t *nodo)
{
	if (!nodo->derecha && !nodo->izquierda) {
		void *elemento_derecho = NULL;
		nodo->izquierda = extraer_predecesor_inorden(nodo->izquierda,
							     &elemento_derecho);
		nodo->elemento = elemento_derecho;
		return nodo;
	} else if (!nodo->derecha) {
		free(nodo);
		return nodo->izquierda;
	}
	free(nodo);
	return nodo->derecha;
}

abb_t *destruir_nodos(abb_t *arbol, nodo_abb_t *nodo,
		      void (*destructor)(void *))
{
	if (nodo->izquierda != NULL)
		arbol = destruir_nodos(arbol, nodo->izquierda, destructor);

	if (nodo->derecha != NULL)
		arbol = destruir_nodos(arbol, nodo->derecha, destructor);

	if (destructor != NULL)
		destructor(nodo->elemento);

	free(nodo);
	arbol->tamanio--;
	
	return arbol;
}

bool abb_recorrer_inorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
			   void *aux, size_t *contador)
{
	if (!nodo)
		return true;

	if (!abb_recorrer_inorden(nodo->izquierda, funcion, aux, contador))
		return false;

	(*contador)++;
	if (!funcion(nodo->elemento, aux))
		return false;

	return abb_recorrer_inorden(nodo->derecha, funcion, aux, contador);
}

bool abb_recorrer_preorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
			   void *aux, size_t *contador)
{
	if (!nodo)
		return true;

	(*contador)++;
	if (!funcion(nodo->elemento, aux))
		return false;

	if (!abb_recorrer_preorden(nodo->izquierda, funcion, aux, contador))
		return false;

	return abb_recorrer_preorden(nodo->derecha, funcion, aux, contador);
}

bool abb_recorrer_postorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
			   void *aux, size_t *contador)
{
	if (!nodo)
		return true;

	if (!abb_recorrer_postorden(nodo->izquierda, funcion, aux, contador))
		return false;

	if (!abb_recorrer_postorden(nodo->derecha, funcion, aux, contador))
		return false;

	(*contador)++;

	return funcion(nodo->elemento, aux);
}

bool rellenar_vector(void * elemento, void *aux)
{
	return true;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador)
		return NULL;

	abb_t *arbol = calloc(1, sizeof(abb_t));
	if (!arbol)
		return NULL;

	arbol->comparador = comparador;
	return arbol;
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
	arbol->tamanio++;

	return arbol;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol))
		return NULL;

	nodo_abb_t *nodo_a_quitar = buscar_nodo_por_elemento(arbol,
							     arbol->nodo_raiz,
							     elemento);
	if (!nodo_a_quitar)
		return NULL;

	void *elemento_extraido = nodo_a_quitar->elemento;
	nodo_a_quitar = abb_quitar_nodo(nodo_a_quitar);

	return elemento_extraido;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol))
		return NULL;

	nodo_abb_t *nodo = buscar_nodo_por_elemento(arbol, arbol->nodo_raiz,
						    elemento);
	return nodo->elemento;
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

	arbol = destruir_nodos(arbol, arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion || abb_vacio(arbol))
		return 0;

	size_t *contador = 0;

	if (recorrido == INORDEN)
		abb_recorrer_inorden(arbol->nodo_raiz, funcion, aux, contador);
	else if (recorrido == PREORDEN)
		abb_recorrer_preorden(arbol->nodo_raiz, funcion, aux, contador);
	else
		abb_recorrer_postorden(arbol->nodo_raiz, funcion, aux, contador);

	return 0;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !array || abb_vacio(arbol) || !tamanio_array)
		return 0;

	struct vector vector;
	vector.vector = array;
	vector.tope = tamanio_array;
	vector.posicion = 0;

	return abb_con_cada_elemento(arbol, recorrido, rellenar_vector, vector.vector);
}