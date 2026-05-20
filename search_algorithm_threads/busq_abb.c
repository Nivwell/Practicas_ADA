/*
IMPLEMENTACION DEL ALGORITMO DE BUSQUEDA EN ARBOL BINARIO DE BUSQUEDA (ABB) CON HILOS
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
        Pérez Ortiz Luis Angel
Autor original de la librería de tiempos: 
        Edgardo Adrián Franco Martinez
Fecha de entrega 20 de Mayo del 2026
Version: 2.0

DESCRIPCION: Este codigo implementa la estructura de un Arbol Binario de Busqueda (ABB)
y evalúa el rendimiento de su búsqueda utilizando paralelismo con la biblioteca Pthreads. 
Dado que la búsqueda en un ABB tiene una complejidad logarítmica O(log n), paralelizar 
el recorrido para un solo elemento genera una sobrecarga (overhead) en el procesador. 
Por fines académicos, este programa lanza múltiples hilos, pero condiciona la lógica 
para que solo un hilo (el hilo 0) realice el recorrido iterativo. Esto permite medir 
y demostrar matemáticamente el costo de la creación y destrucción de hilos en contraste 
con el tiempo real de búsqueda. Los resultados de tiempo y rendimiento se exportan a un CSV.

OBSERVACIONES: La cantidad de elementos 'n' debe coincidir con el tamaño del archivo de 
entrada. El archivo "objetivos.txt" debe existir en el mismo directorio.

Compilación: gcc busq_abb.c tiempo.c -lpthread -o busqueda_abb_paralela
Ejecución: ./busqueda_abb_paralela <numeros10millones.txt> <n> <hilos>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tiempo.h"

typedef struct Nodo {
    int dato;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

/* --- ESTRUCTURA PARA LOS HILOS --- */
typedef struct {
    Nodo *raiz;
    int objetivo;
    int *encontrado_global;
    int id_hilo;
} ThreadArgs;

/* --- PROTOTIPOS --- */
Nodo* crearNodo(int valor);
Nodo* insertar(Nodo* raiz, int valor);
void* busquedaABBHilo(void *arg);
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void liberarArbol(Nodo* raiz); 

/*
int main(int argc, char *argv[])
Recibe: argc - Número de argumentos recibidos.
        *argv[] - Vector de cadenas (argv[1] es el archivo de datos, argv[2] es 'n', argv[3] es la cantidad de hilos).
Devuelve: int (0 si el programa finaliza correctamente, 1 en caso de error).
Función: 
    1. Valida los 3 argumentos de entrada requeridos desde la terminal.
    2. Carga los datos desordenados desde el archivo de texto a la memoria dinámica.
    3. Construye el Árbol Binario de Búsqueda insertando los datos secuencialmente.
    4. Carga el arreglo de objetivos a buscar.
    5. Implementa el cronometraje abarcando el ciclo de vida completo de los hilos (creación, ejecución y destrucción).
    6. Exporta los resultados (tiempo Real, User, Sys y estado de búsqueda) al archivo CSV.
    7. Limpia la memoria dinámica liberando el arreglo y el árbol completo.
*/
int main(int argc, char *argv[]) {
    // Recibimos 3 argumentos: archivo, N e hilos
    if (argc < 4) {
        printf("Uso: %s <numero10millones.txt> <n> <hilos>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    int n = atoi(argv[2]);
    int num_hilos = atoi(argv[3]);
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

    // PASO 4: BÚSQUEDA PARALELA Y ESCRITURA EN CSV
    printf("[3/4] Buscando %d objetivos usando %d hilo(s)...\n", n_objetivos, num_hilos);
    
    FILE *f_csv = fopen("bitacora_tiempos.csv", "a");
    if (!f_csv) {
        printf("[X] Error: No se pudo abrir o crear 'bitacora_tiempos.csv'\n");
        free(datos_sucios); free(objetivos); liberarArbol(raiz);
        return 1;
    }

    for (int j = 0; j < n_objetivos; j++) {
        int buscado = objetivos[j];
        int status_encontrado = 0; // 0 = No, 1 = Sí
        
        pthread_t hilos[num_hilos];
        ThreadArgs args[num_hilos];
        
        // ----------------------------------------------------
        // INICIA CRONÓMETRO: Creación, ejecución y destrucción
        // ----------------------------------------------------
        uswtime(&utime0, &stime0, &wtime0);

        // 1. Lanzar los hilos
        for (int i = 0; i < num_hilos; i++) {
            args[i].raiz = raiz;
            args[i].objetivo = buscado;
            args[i].encontrado_global = &status_encontrado;
            args[i].id_hilo = i; 
            
            pthread_create(&hilos[i], NULL, busquedaABBHilo, &args[i]);
        }

        // 2. Esperar a que los hilos terminen
        for (int i = 0; i < num_hilos; i++) {
            pthread_join(hilos[i], NULL);
        }

        uswtime(&utime1, &stime1, &wtime1);
        // ----------------------------------------------------
        // TERMINA CRONÓMETRO
        // ----------------------------------------------------

        // En un ABB no hay índice lineal, usamos -1 como bandera de posición
        int posicion = -1; 

        if (status_encontrado) {
            printf("  - Objetivo [%d]: Encontrado en el arbol\n", buscado);
        } else {
            printf("  - Objetivo [%d]: No encontrado\n", buscado);
        }

        // ESCRITURA FORZADA EN EL CSV
        fprintf(f_csv, "Busqueda ABB Paralela,%d,%d,%d,%d,%d,%.10f,%.10f,%.10f\n", 
                n, 
                buscado, 
                status_encontrado, 
                posicion, 
                num_hilos,
                wtime1 - wtime0, 
                utime1 - utime0, 
                stime1 - stime0);
        
        fflush(f_csv); 
    }

    fclose(f_csv);
    printf("[4/4] Resultados guardados exitosamente en 'bitacora_tiempos.csv'\n");

    // Limpieza de memoria
    free(objetivos);
    free(datos_sucios);
    liberarArbol(raiz); 
    
    return 0;
}

/* --- FUNCIÓN DEL HILO (BÚSQUEDA ABB ITERATIVA) --- */

/*
void* busquedaABBHilo(void *arg)
DESCRIPCION: Función ejecutada por cada hilo creado. Evalúa si el hilo es el designado (hilo 0) 
para recorrer el árbol de manera iterativa, evitando el desbordamiento de pila (Stack Overflow). 
Los hilos restantes finalizan su ejecución inmediatamente.
Recibe: arg - Puntero genérico (void*) a la estructura ThreadArgs con los datos de configuración.
Devuelve: NULL al finalizar la ejecución del hilo.
*/
void* busquedaABBHilo(void *arg) {
    ThreadArgs *datos = (ThreadArgs*)arg;
    
    // Solo el Hilo 0 hace el recorrido lógico.
    // Los demás hilos mueren instantáneamente para no violar la complejidad del ABB.
    if (datos->id_hilo == 0) {
        Nodo* actual = datos->raiz;
        
        // Búsqueda iterativa (Evita Stack Overflow en Pthreads)
        while (actual != NULL) {
            if (actual->dato == datos->objetivo) {
                *(datos->encontrado_global) = 1;
                break;
            }
            if (datos->objetivo > actual->dato)
                actual = actual->der;
            else
                actual = actual->izq;
        }
    }
    
    return NULL;
}

/* --- IMPLEMENTACIÓN DE FUNCIONES RESTANTES --- */

/*
Nodo* crearNodo(int valor)
DESCRIPCION: Función que instancia un nuevo nodo en memoria para el árbol.
Recibe: valor - Número entero que será asignado al campo de datos del nodo.
Devuelve: Un puntero al nuevo Nodo creado.
*/
Nodo* crearNodo(int valor) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return NULL;
    nuevo->dato = valor;
    nuevo->izq = nuevo->der = NULL;
    return nuevo;
}

/*
Nodo* insertar(Nodo* raiz, int valor)
DESCRIPCION: Función que acomoda un nuevo valor respetando las reglas lógicas del ABB.
Recibe: raiz - Puntero al nodo raíz del subárbol actual.
        valor - Número entero que se desea insertar en el árbol.
Devuelve: Puntero a la raíz del subárbol después de la inserción.
*/
Nodo* insertar(Nodo* raiz, int valor) {
    if (raiz == NULL) return crearNodo(valor);

    if (valor < raiz->dato)
        raiz->izq = insertar(raiz->izq, valor);
    else if (valor > raiz->dato)
        raiz->der = insertar(raiz->der, valor);

    return raiz;
}

/*
int* cargarNumeros(char *nombre, int cantidad, int conComas)
DESCRIPCION: Función utilitaria para leer archivos de texto hacia un arreglo dinámico.
Recibe: nombre - Cadena de texto con el nombre del archivo.
        cantidad - Número total de elementos a extraer.
        conComas - Bandera entera (1 si están separados por comas, 0 por saltos de línea).
Devuelve: Un puntero al arreglo de enteros cargado en memoria, o NULL en caso de error.
*/
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

/*
void liberarArbol(Nodo* raiz)
DESCRIPCION: Función que realiza un recorrido en postorden para prevenir fugas de memoria.
Recibe: raiz - Puntero a la raíz del árbol que será destruido.
Devuelve: void (No retorna valor explicito).
*/
void liberarArbol(Nodo* raiz) {
    if (raiz == NULL) return;
    liberarArbol(raiz->izq);
    liberarArbol(raiz->der);
    free(raiz);
}