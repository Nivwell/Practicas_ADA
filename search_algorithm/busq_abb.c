#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

typedef struct Nodo {
    int dato;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

Nodo* crearNodo(int valor);
Nodo* insertar(Nodo* raiz, int valor);
Nodo* buscarABB(Nodo* raiz, int objetivo);
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void liberarArbol(Nodo* raiz); 

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <numero10millones.txt> <n>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    int n = atoi(argv[2]);
    int n_objetivos = 20;
    Nodo* raiz = NULL;

    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    // PASO 1: CARGA DE DATOS
    printf("[1/4] Cargando %d numeros desde el archivo... ", n); fflush(stdout);
    int *datos_sucios = cargarNumeros(f_sucio, n, 0);
    if (!datos_sucios) return 1;
    printf("OK\n");

    // PASO 2: CONSTRUCCIÓN DEL ABB
    printf("[2/4] Construyendo el Arbol Binario de Busqueda... "); fflush(stdout);
    for (int i = 0; i < n; i++) {
        raiz = insertar(raiz, datos_sucios[i]);
    }
    printf("OK\n");

    // PASO 3: CARGA DE OBJETIVOS
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);
    if (!objetivos) {
        printf("Error: No se encontro objetivos.txt\n");
        liberarArbol(raiz);
        free(datos_sucios);
        return 1;
    }

    // PASO 4: BÚSQUEDA INDIVIDUAL Y ESCRITURA EN CSV
    printf("[3/4] Buscando %d objetivos en el ABB y registrando tiempos...\n", n_objetivos);
    
    FILE *f_csv = fopen("bitacora_tiempos.csv", "a");
    if (!f_csv) {
        printf("[X] Error: No se pudo abrir o crear 'bitacora_tiempos.csv'\n");
        free(datos_sucios); free(objetivos); liberarArbol(raiz);
        return 1;
    }

    for (int j = 0; j < n_objetivos; j++) {
        int buscado = objetivos[j];
        
        // Cronómetro exclusivo para la búsqueda actual en el ABB
        uswtime(&utime0, &stime0, &wtime0);
        Nodo* resultado = buscarABB(raiz, buscado);
        uswtime(&utime1, &stime1, &wtime1);

        int encontrado = (resultado != NULL) ? 1 : 0;
        int posicion = -1; // En un ABB no hay índice lineal, usamos -1 como bandera

        // Imprimir en consola para validación visual
        if (encontrado) {
            printf("  - Objetivo [%d]: Encontrado en el arbol\n", buscado);
        } else {
            printf("  - Objetivo [%d]: No encontrado\n", buscado);
        }

        // ESCRITURA FORZADA FILA POR FILA EN EL CSV
        // Formato: Algoritmo, N, Objetivo, Encontrado, Indice, Hilos, T_Real, T_User, T_Sys
        fprintf(f_csv, "Busqueda ABB Secuencial,%d,%d,%d,%d,1,%.10f,%.10f,%.10f\n", 
                n, 
                buscado, 
                encontrado, 
                posicion, 
                wtime1 - wtime0, 
                utime1 - utime0, 
                stime1 - stime0);
        
        fflush(f_csv); // Obliga al SO a escribir en disco AHORA
    }

    fclose(f_csv);
    printf("[4/4] Resultados guardados exitosamente en 'bitacora_tiempos.csv'\n");

    // Limpieza de memoria
    free(objetivos);
    free(datos_sucios);
    liberarArbol(raiz); 
    
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
    if (f == NULL) return NULL;
    
    int *arreglo = (int *)malloc(cantidad * sizeof(int));
    if (!arreglo) {
        fclose(f);
        return NULL;
    }
    
    for (int i = 0; i < cantidad; i++) {
        if (conComas) {
            if (fscanf(f, " %d ,", &arreglo[i]) != 1) break;
        } else {
            if (fscanf(f, "%d", &arreglo[i]) != 1) break;
        }
    }
    fclose(f);
    return arreglo;
}

void liberarArbol(Nodo* raiz) {
    if (raiz == NULL) return;
    liberarArbol(raiz->izq);
    liberarArbol(raiz->der);
    free(raiz);
}