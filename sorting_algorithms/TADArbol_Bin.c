/*
Implementacion de la librería TAD Arbol Binario de Búsqueda
Autores: 
		Garcia Peñalva Saul 
		López Alvarado Daniel
		Olarte Tomas Kevin Saul
Autor original: 
		Edgardo Adri­an Franco Martinez
Fecha de entrega 18 de Marzo del 2025
Version: 2.0

DESCRIPCIÓN: TAD árbol binario de búsqueda (ABB)
Estructura de datos en la que se cumple:
- Cada nodo tiene como máximo dos hijos (izquierdo y derecho)
- Todos los nodos de la izquierda son menores al padre
- Todos los nodos de la derecha son mayores al padre

OBSERVACIONES: TAD Árbol Binario por definición es una Estructura de Datos dinámica.
La implementación del presente código se realiza mediante nodos que contienen
un elemento y están ligados a sus hijos izquierdo y derecho.

Estructura visual:
                    Raíz
                   /    \
              Hijo Izq  Hijo Der
             /    \        /    \
           ...   ...    ...   ...

COMPILACIÓN: gcc TADArbol_Bin.c TADArbol_Bin.h -o TADArbol_Bin
*/
#include "TADArbol_Bin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// DEFINICIÓN DE FUNCIONES

/***************************************************
Operaciones de construcción
***************************************************/

/*
void Initialize(arbol_binario *a)
Descripción: Inicializar árbol binario (Iniciar un árbol binario para su uso)
Recibe: arbol_binario *a (Referencia al árbol binario "a" a operar)
Devuelve:
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él,
si esto no ha pasado se ocasionará un error.
*/
void Initialize(arbol_binario *a){
    *a = NULL;
    return;
}

/*
void Destroy(arbol_binario *a)
Descripción: Destruir un árbol binario (Recibe un árbol a y lo libera completamente)
Recibe: arbol_binario *a (Referencia al árbol binario "a" a operar)
Devuelve:
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él,
si esto no ha pasado se ocasionará un error. La destrucción se realiza de forma recursiva.
*/
void Destroy(arbol_binario *a)
{
    if (*a != NULL)
    {
        if ((*a)->izq != NULL)
            Destroy(&((*a)->izq));
        if ((*a)->der != NULL)
            Destroy(&((*a)->der));
        free(*a);
        *a = NULL;
    }
    return;
}

/***************************************************
Operaciones de posicionamiento y búsqueda
***************************************************/

/*
posicion Root(arbol_binario *a)
Descripción: Recibe un árbol binario a y regresa la posición de la raíz
Recibe: arbol_binario *a (Referencia al árbol binario "a" a operar)
Devuelve: posición del nodo raíz del árbol
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él,
si esto no ha pasado se ocasionará un error.
*/
posicion Root(arbol_binario *a)
{
    return *a;
}

/*
posicion Parent(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, devuelve la posición del padre de p
Recibe: arbol_binario *a y posicion p (Referencia al árbol "a" a operar y posición válida del árbol)
Devuelve: posición del nodo padre de la posición dada
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él, p es una posición válida del árbol.
Si p es la raíz, devuelve NULL. Si no se encuentra p, se produce error.
*/
posicion Parent(arbol_binario *a, posicion p)
{
    posicion padre = NULL;
    if (*a != NULL)
    {
        if (*a == p)
            return NULL; // El nodo raíz no tiene padre
        if ((*a)->der == p || (*a)->izq == p)
            return *a;
        if ((*a)->izq != NULL)
            padre = Parent(&((*a)->izq), p);
        if ((*a)->der != NULL && padre == NULL)
            padre = Parent(&((*a)->der), p);
    }
    else
    {
        printf("ERROR: Parent(a,p) la posición dada no es válida");
        exit(1);
    }
    return padre;
}

/*
posicion RightSon(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, devuelve la posición del hijo derecho de p
Recibe: arbol_binario *a y posicion p (Referencia al árbol "a" a operar y posición válida del árbol)
Devuelve: posición del hijo derecho de la posición dada
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él, p es una posición válida del árbol,
si esto no ha pasado se ocasionará un error.
*/
posicion RightSon(arbol_binario *a, posicion p)
{
    if (!NullNode(&(*a), p))
    {
        return p->der;
    }
    else
    {
        printf("ERROR: RightSon(a,p) la posición dada no es válida");
        exit(1);
    }
}

/*
posicion LeftSon(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, devuelve la posición del hijo izquierdo de p
Recibe: arbol_binario *a y posicion p (Referencia al árbol "a" a operar y posición válida del árbol)
Devuelve: posición del hijo izquierdo de la posición dada
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él, p es una posición válida del árbol,
si esto no ha pasado se ocasionará un error.
*/
posicion LeftSon(arbol_binario *a, posicion p)
{
    if (!NullNode(&(*a), p))
    {
        return p->izq;
    }
    else
    {
        printf("ERROR: LeftSon(a,p) la posición dada no es válida");
        exit(1);
    }
}

/*
posicion Search(arbol_binario *a, elemento e)
Descripción: Recibe un árbol binario a y un elemento e, regresa la posición que coincida exactamente con el elemento e.
Recibe: arbol_binario *a y un elemento e (Referencia al árbol "a" a operar y elemento a buscar en el árbol)
Devuelve: posición del elemento en caso de ser encontrado, si no se encuentra se devuelve NULL
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él. El elemento a buscar se compara directamente
a nivel de bytes. La búsqueda se realiza de forma recursiva.
*/
posicion Search(arbol_binario *a, elemento e)
{
    posicion p = NULL;
    if (Empty(&(*a)))
        return p;
    if (memcmp(&((*a)->e), &e, sizeof(elemento)) == 0)
        return *a;
    if ((*a)->izq != NULL)
        p = Search(&((*a)->izq), e);
    if ((*a)->der != NULL && p == NULL)
        p = Search(&((*a)->der), e);

    return p;
}

/***************************************************
Operaciones de consulta
***************************************************/

/*
boolean Empty(arbol_binario *a)
Descripción: Recibe un árbol binario a y devuelve TRUE si el árbol está vacío y FALSE en caso contrario.
Recibe: arbol_binario *a (Referencia al árbol binario "a")
Devuelve: boolean (TRUE o FALSE)
Observaciones: El usuario ha creado un árbol binario, el árbol fue correctamente inicializado.
*/
boolean Empty(arbol_binario *a)
{
    if (*a != NULL)
        return FALSE;
    else
        return TRUE;
}

/*
boolean NullNode(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, devuelve TRUE si la posición es válida en el árbol a y FALSE en caso contrario.
Recibe: arbol_binario *a y una posicion p (Referencia al árbol "a" a operar y una posición)
Devuelve: Booleano
Observaciones: La función verifica recursivamente si la posición p existe en el árbol a.
*/
boolean NullNode(arbol_binario *a, posicion p)
{
    boolean b = TRUE;
    if (*a == NULL || p == NULL)
        return TRUE;
    if (*a == p)
        return FALSE;
    if ((*a)->izq != NULL)
        b = NullNode(&((*a)->izq), p);
    if ((*a)->der != NULL && b == TRUE)
        b = NullNode(&((*a)->der), p);

    return b;
}

/*
elemento ReadNode(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, devuelve el elemento en dicha posición.
Recibe: arbol_binario *a y una posicion p (Referencia al árbol "a" a operar y posición válida en el árbol)
Devuelve: Elemento en la posición dada, si la posición es inválida se genera error.
Observaciones: El árbol a no está vacío y la posición p es una posición válida.
*/
elemento ReadNode(arbol_binario *a, posicion p)
{
    if (!NullNode(&(*a), p))
    {
        return p->e;
    }
    else
    {
        printf("ERROR: ReadNode(a,p) la posición dada no es válida");
        exit(1);
    }
}

/***************************************************
Operaciones de modificación
***************************************************/

/*
void NewRightSon(arbol_binario *a, posicion p, elemento e)
Descripción: Recibe un árbol binario a, una posición p y un elemento e; crea un nuevo hijo derecho de p con el elemento e.
Recibe: arbol_binario *a, posicion p y elemento e (Referencia al árbol "a", posición válida en el árbol y elemento a insertar)
Devuelve:
Observaciones: Si el árbol está vacío, se crea la raíz. Si p ya tiene hijo derecho, se produce error.
El usuario ha creado un árbol binario, el árbol fue correctamente inicializado.
*/
void NewRightSon(arbol_binario *a, posicion p, elemento e)
{
    if (Empty(&(*a)))
    {
        *a = malloc(sizeof(nodo));
        if (*a == NULL)
        {
            printf("ERROR: NewRightSon(a,p,e) desbordamiento del árbol");
            exit(1);
        }
        (*a)->e = e;
        (*a)->der = NULL;
        (*a)->izq = NULL;
    }
    else
    {
        if (!NullNode(&(*a), p))
        {
            if (p->der != NULL)
            {
                printf("ERROR: NewRightSon(a,p,e) p ya tiene un hijo derecho");
                exit(1);
            }
            p->der = malloc(sizeof(nodo));
            if (p->der == NULL)
            {
                printf("ERROR: NewRightSon(a,p,e) desbordamiento de memoria");
                exit(1);
            }
            p->der->e = e;
            p->der->der = NULL;
            p->der->izq = NULL;
        }
        else
        {
            printf("ERROR: NewRightSon(a,p,e) la posición dada no es válida");
            exit(1);
        }
    }
    return;
}

/*
void NewLeftSon(arbol_binario *a, posicion p, elemento e)
Descripción: Recibe un árbol binario a, una posición p y un elemento e; crea un nuevo hijo izquierdo de p con el elemento e.
Recibe: arbol_binario *a, posicion p y elemento e (Referencia al árbol "a", posición válida en el árbol y elemento a insertar)
Devuelve:
Observaciones: Si el árbol está vacío, se crea la raíz. Si p ya tiene hijo izquierdo, se produce error.
El usuario ha creado un árbol binario, el árbol fue correctamente inicializado.
*/
void NewLeftSon(arbol_binario *a, posicion p, elemento e)
{
    if (Empty(&(*a)))
    {
        *a = malloc(sizeof(nodo));
        if (*a == NULL)
        {
            printf("ERROR: NewLeftSon(a,p,e) desbordamiento de memoria");
            exit(1);
        }
        (*a)->e = e;
        (*a)->der = NULL;
        (*a)->izq = NULL;
    }
    else
    {
        if (!NullNode(&(*a), p))
        {
            if (p->izq != NULL)
            {
                printf("ERROR: NewLeftSon(a,p,e) p ya tiene un hijo izquierdo");
                exit(1);
            }
            p->izq = malloc(sizeof(nodo));
            if (p->izq == NULL)
            {
                printf("ERROR: NewLeftSon(a,p,e) desbordamiento de memoria");
                exit(1);
            }
            p->izq->e = e;
            p->izq->der = NULL;
            p->izq->izq = NULL;
        }
        else
        {
            printf("ERROR: NewLeftSon(a,p,e) la posición dada no es válida");
            exit(1);
        }
    }
    return;
}

/*
void DeleteRightSon(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, elimina completamente el subárbol derecho de p.
Recibe: arbol_binario *a y posicion p (Referencia al árbol "a" a operar y posición válida del árbol)
Devuelve:
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él, p es una posición válida del árbol,
si esto no ha pasado se ocasionará un error. La eliminación se realiza recursivamente.
*/
void DeleteRightSon(arbol_binario *a, posicion p)
{
    if (!NullNode(&(*a), p))
    {
        Destroy(&(p->der));
        p->der = NULL;
    }
    else
    {
        printf("ERROR: DeleteRightSon(a,p) la posición dada no es válida");
        exit(1);
    }
    return;
}

/*
void DeleteLeftSon(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, elimina completamente el subárbol izquierdo de p.
Recibe: arbol_binario *a y posicion p (Referencia al árbol "a" a operar y posición válida del árbol)
Devuelve:
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él, p es una posición válida del árbol,
si esto no ha pasado se ocasionará un error. La eliminación se realiza recursivamente.
*/
void DeleteLeftSon(arbol_binario *a, posicion p)
{
    if (!NullNode(&(*a), p))
    {
        Destroy(&(p->izq));
        p->izq = NULL;
    }
    else
    {
        printf("ERROR: DeleteLeftSon(a,p) la posición dada no es válida");
        exit(1);
    }
    return;
}

/*
void DeleteNode(arbol_binario *a, posicion p)
Descripción: Recibe un árbol binario a y una posición p, elimina el nodo en la posición p y todos sus descendientes.
Recibe: arbol_binario *a y posicion p (Referencia al árbol "a" a operar y posición válida del árbol)
Devuelve:
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él, p es una posición válida del árbol,
si esto no ha pasado se ocasionará un error. Si p es la raíz, se destruye todo el árbol.
*/
void DeleteNode(arbol_binario *a, posicion p)
{
    if (!NullNode(&(*a), p))
    {
        if (*a == p)
        { // Es el nodo raíz
            Destroy(a);
            return;
        }

        posicion padre = Parent(a, p);
        if (padre != NULL)
        {
            if (padre->der == p)
                padre->der = NULL;
            else if (padre->izq == p)
                padre->izq = NULL;
        }

        posicion temp = p;
        Destroy(&temp);
    }
    else
    {
        printf("ERROR: DeleteNode(a,p) la posición dada no es válida");
        exit(1);
    }
    return;
}

/*
void ReplaceNode(arbol_binario *a, posicion p, elemento e)
Descripción: Recibe un árbol binario a, una posición p y un elemento e, el elemento en la posición p será sustituido por e
Recibe: arbol_binario *a, posicion p y elemento e (Referencia al árbol "a", posición válida en el árbol y elemento que reemplazará al existente en p)
Devuelve:
Observaciones: El usuario ha creado un árbol binario y a tiene la referencia a él, p es una posición válida del árbol,
si esto no ha pasado se ocasionará un error.
*/
void ReplaceNode(arbol_binario *a, posicion p, elemento e)
{
    if (!NullNode(&(*a), p))
    {
        p->e = e;
    }
    else
    {
        printf("ERROR: ReplaceNode(a,p,e) la posición dada no es válida");
        exit(1);
    }
    return;
}
