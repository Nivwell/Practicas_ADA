#ifndef __TADArbol_Bin_H
#define __TADArbol_Bin_H
/*
Implementacion de la librería TAD Arbol Binario de Búsqueda
Autor Original: Edgardo Adrián Franco Martínez  Mayo 2021
Autor: 
		Garcia Peñalva Saul 
		López Alvarado Daniel
		Olarte Tomas Kevin Saul
Version: 1.1 Marzo 2026

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

#define TRUE 1
#define FALSE 0

typedef unsigned char boolean;

typedef struct elemento{
    long num;
}elemento;

//Estructura de un NODO de árbol binario
typedef struct nodo{
    elemento e;
    //Relación con hijo izquierdo
    struct nodo* izq;
    //Relación con hijo derecho
    struct nodo* der;
}nodo;

//Arbol binario como un apuntador a nodo (raíz)
typedef nodo* arbol_binario;

//Posición como un apuntador a nodo
typedef nodo* posicion;

/***************************************************
Operaciones de construcción
***************************************************/

//Efecto: Recibe un árbol binario a y lo inicializa para su trabajo normal.
void Initialize(arbol_binario *a);

//Efecto: Recibe un árbol binario a y lo libera completamente de memoria.
void Destroy(arbol_binario *a);

/***************************************************
Operaciones de posicionamiento y búsqueda
***************************************************/

//Efecto: Recibe un árbol binario a y retorna la posición del nodo raíz.
posicion Root(arbol_binario *a);

//Efecto: Recibe un árbol binario a y una posición p, devuelve la posición del nodo padre de p.
//Requerimientos: La posición p es válida y no es la raíz.
posicion Parent(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, devuelve la posición del hijo derecho de p.
//Requerimientos: La posición p es válida.
posicion RightSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, devuelve la posición del hijo izquierdo de p.
//Requerimientos: La posición p es válida.
posicion LeftSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y un elemento e, devuelve la posición del primer nodo que coincida exactamente con e.
posicion Search(arbol_binario *a, elemento e);

/***************************************************
Operaciones de consulta
***************************************************/

//Efecto: Recibe un árbol binario a y devuelve TRUE si el árbol está vacío, FALSE en caso contrario.
boolean Empty(arbol_binario *a);

//Efecto: Recibe un árbol binario a y una posición p, devuelve TRUE si p es una posición válida en el árbol, FALSE en caso contrario.
boolean NullNode(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, devuelve el elemento almacenado en la posición p.
//Requerimientos: La posición p es válida.
elemento ReadNode(arbol_binario *a, posicion p);

/***************************************************
Operaciones de modificación
***************************************************/

//Efecto: Recibe un árbol binario a, una posición p y un elemento e; crea un nuevo hijo derecho de p con el elemento e.
//Requerimientos: Si el árbol está vacío, se crea la raíz. Si p ya tiene hijo derecho, se produce error.
void NewRightSon(arbol_binario *a, posicion p, elemento e);

//Efecto: Recibe un árbol binario a, una posición p y un elemento e; crea un nuevo hijo izquierdo de p con el elemento e.
//Requerimientos: Si el árbol está vacío, se crea la raíz. Si p ya tiene hijo izquierdo, se produce error.
void NewLeftSon(arbol_binario *a, posicion p, elemento e);

//Efecto: Recibe un árbol binario a y una posición p, elimina completamente el subárbol derecho de p.
//Requerimientos: La posición p es válida.
void DeleteRightSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, elimina completamente el subárbol izquierdo de p.
//Requerimientos: La posición p es válida.
void DeleteLeftSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, elimina el nodo en la posición p y todos sus descendientes.
//Requerimientos: La posición p es válida.
void DeleteNode(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a, una posición p y un elemento e, sustituye el elemento en la posición p por e.
//Requerimientos: La posición p es válida.
void ReplaceNode(arbol_binario *a, posicion p, elemento e);

#endif    // __TADABB_H
