#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define PUNTO_COMPARACION 0

struct vector_recorrer_abb {
	void **vec;
	size_t tope;
	size_t cantidad;
};

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

nodo_abb_t *insertar_nodo(abb_t *arbol, nodo_abb_t *nodo_a_insertar,
			  nodo_abb_t *nodo_actual)
{
	if (!nodo_actual)
		return nodo_a_insertar;

	int comparacion = arbol->comparador(nodo_a_insertar->elemento,
					    nodo_actual->elemento);
	if (comparacion <= PUNTO_COMPARACION)
		nodo_actual->izquierda = insertar_nodo(arbol, nodo_a_insertar,
						       nodo_actual->izquierda);
	else
		nodo_actual->derecha = insertar_nodo(arbol, nodo_a_insertar,
						     nodo_actual->derecha);
	return nodo_actual;
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

nodo_abb_t *extraer_predecesor_inorden(nodo_abb_t *nodo,
				       void **predecesor_inorden)
{
	if (!nodo->derecha) {
		*predecesor_inorden = nodo->elemento;
		nodo_abb_t *izquierda_de_predecesor_inorden = nodo->izquierda;
		free(nodo);
		return izquierda_de_predecesor_inorden;
	}

	nodo->derecha =
		extraer_predecesor_inorden(nodo->derecha, predecesor_inorden);
	return nodo;
}

nodo_abb_t *abb_quitar_nodo(abb_t *arbol, nodo_abb_t *nodo, void *elemento,
			    void **elemento_extraido)
{
	if (!nodo)
		return NULL;

	int comparacion = arbol->comparador(elemento, nodo->elemento);
	if (!comparacion) {
		nodo_abb_t *izq = nodo->izquierda;
		nodo_abb_t *der = nodo->derecha;
		*elemento_extraido = nodo->elemento;
		arbol->tamanio--;

		if (nodo->derecha != NULL && nodo->izquierda != NULL) {
			void *elemento_derecho = NULL;
			nodo->izquierda = extraer_predecesor_inorden(
				nodo->izquierda, &elemento_derecho);
			nodo->elemento = elemento_derecho;
			return nodo;
		}

		free(nodo);
		if (!izq)
			return der;

		return izq;
	} else if (comparacion < PUNTO_COMPARACION)
		nodo->izquierda = abb_quitar_nodo(arbol, nodo->izquierda,
						  elemento, elemento_extraido);
	else
		nodo->derecha = abb_quitar_nodo(arbol, nodo->derecha, elemento,
						elemento_extraido);
	return nodo;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol))
		return NULL;

	void *elemento_extraido = NULL;
	arbol->nodo_raiz = abb_quitar_nodo(arbol, arbol->nodo_raiz, elemento,
					   &elemento_extraido);
	return elemento_extraido;
}

nodo_abb_t *buscar_nodo_por_elemento(abb_t *arbol, nodo_abb_t *nodo,
				     void *elemento)
{
	if (!nodo)
		return NULL;

	int comparacion = arbol->comparador(elemento, nodo->elemento);
	if (comparacion < PUNTO_COMPARACION)
		nodo = buscar_nodo_por_elemento(arbol, nodo->izquierda,
						elemento);
	else if (comparacion > PUNTO_COMPARACION)
		nodo = buscar_nodo_por_elemento(arbol, nodo->derecha, elemento);
	return nodo;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol))
		return NULL;

	nodo_abb_t *nodo =
		buscar_nodo_por_elemento(arbol, arbol->nodo_raiz, elemento);
	if (!nodo)
		return NULL;

	return nodo->elemento;
}

bool abb_vacio(abb_t *arbol)
{
	return abb_tamanio(arbol) == 0;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;

	return arbol->tamanio;
}

void destruir_nodos(abb_t *arbol, nodo_abb_t *nodo, void (*destructor)(void *))
{
	if (!nodo)
		return;

	destruir_nodos(arbol, nodo->izquierda, destructor);
	destruir_nodos(arbol, nodo->derecha, destructor);

	if (destructor != NULL)
		destructor(nodo->elemento);

	free(nodo);
	arbol->tamanio--;
}

void abb_destruir(abb_t *arbol)
{
	abb_destruir_todo(arbol, NULL);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;

	destruir_nodos(arbol, arbol->nodo_raiz, destructor);
	free(arbol);
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

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion || abb_vacio(arbol))
		return 0;

	size_t contador = 0;

	if (recorrido == INORDEN)
		abb_recorrer_inorden(arbol->nodo_raiz, funcion, aux, &contador);
	else if (recorrido == PREORDEN)
		abb_recorrer_preorden(arbol->nodo_raiz, funcion, aux,
				      &contador);
	else
		abb_recorrer_postorden(arbol->nodo_raiz, funcion, aux,
				       &contador);

	return contador;
}

bool rellenar_vector(void *elemento, void *aux)
{
	struct vector_recorrer_abb *vector = aux;
	if (vector->cantidad >= vector->tope)
		return false;

	vector->vec[vector->cantidad] = elemento;
	vector->cantidad++;
	return vector->cantidad < vector->tope;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !array || !tamanio_array)
		return 0;

	struct vector_recorrer_abb vector;
	vector.vec = array;
	vector.tope = tamanio_array;
	vector.cantidad = 0;
	return abb_con_cada_elemento(arbol, recorrido, rellenar_vector,
				     &vector);
}