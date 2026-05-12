#include <stdio.h>
#include <stdlib.h>

// Estructura del Nodo para el ABB
typedef struct Nodo {
    int dato;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

// Prototipos
Nodo* crearNodo(int valor);
Nodo* insertar(Nodo* raiz, int valor);
Nodo* buscarABB(Nodo* raiz, int objetivo);
int* cargarNumeros(char *nombre, int cantidad, int conComas);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <sucio.txt> <n>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    int n = atoi(argv[2]);
    int n_objetivos = 20;
    Nodo* raiz = NULL;

    // 1. Cargar los datos desordenados en un arreglo temporal
    printf("Cargando %d numeros desde el archivo sucio...\n", n);
    int *datos_sucios = cargarNumeros(f_sucio, n, 0);
    if (!datos_sucios) return 1;

    // 2. Construir el ABB
    // Al estar desordenados, el árbol tenderá a estar más balanceado solo
    printf("Construyendo el Arbol Binario de Busqueda...\n");
    for (int i = 0; i < n; i++) {
        raiz = insertar(raiz, datos_sucios[i]);
    }

    // 3. Cargar los números que queremos buscar
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);

    // 4. Ejecutar la Búsqueda en el Árbol
    if (objetivos != NULL && raiz != NULL) {
        printf("\n--- Resultados de Busqueda ABB ---\n");
        for (int i = 0; i < n_objetivos; i++) {
            Nodo* resultado = buscarABB(raiz, objetivos[i]);
            if (resultado != NULL) {
                printf("[ENCONTRADO] %d esta en el arbol.\n", objetivos[i]);
            } else {
                printf("[NO ENCONTRADO] %d no existe.\n", objetivos[i]);
            }
        }
        free(objetivos);
    }

    // Limpieza de memoria del arreglo (el árbol requiere una función aparte para liberarse)
    free(datos_sucios);
    return 0;
}

/* --- IMPLEMENTACIÓN DE FUNCIONES --- */

Nodo* crearNodo(int valor) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return NULL;
    nuevo->dato = valor;
    nuevo->izq = nuevo->der = NULL;
    return nuevo;
}

Nodo* insertar(Nodo* raiz, int valor) {
    if (raiz == NULL) return crearNodo(valor);

    if (valor < raiz->dato)
        raiz->izq = insertar(raiz->izq, valor);
    else if (valor > raiz->dato)
        raiz->der = insertar(raiz->der, valor);

    return raiz;
}

Nodo* buscarABB(Nodo* raiz, int objetivo) {
    if (raiz == NULL || raiz->dato == objetivo)
        return raiz;

    if (objetivo > raiz->dato)
        return buscarABB(raiz->der, objetivo);

    return buscarABB(raiz->izq, objetivo);
}

int* cargarNumeros(char *nombre, int cantidad, int conComas) {
    FILE *f = fopen(nombre, "r");
    if (f == NULL) {
        printf("Error: No se pudo abrir %s\n", nombre);
        return NULL;
    }
    int *arreglo = (int *)malloc(cantidad * sizeof(int));
    for (int i = 0; i < cantidad; i++) {
        if (conComas) fscanf(f, " %d ,", &arreglo[i]);
        else fscanf(f, "%d", &arreglo[i]);
    }
    fclose(f);
    return arreglo;
}