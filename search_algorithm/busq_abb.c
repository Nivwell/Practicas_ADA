/*
IMPLEMENTACION DEL ALGORITMO DE BUSQUEDA EN ARBOL BINARIO DE BUSQUEDA (ABB)
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
        Pérez Ortiz Luis Angel
Autor original de la librería de tiempos: 
        Edgardo Adrián Franco Martinez
Fecha de entrega 20 de Mayo del 2026
Version: 2.0

DESCRIPCION: Este codigo implementa la estructura de datos de un Arbol Binario de Busqueda (ABB)
para almacenar y buscar elementos. Un ABB organiza los nodos de tal manera que, para cualquier nodo dado,
todos los elementos en su subárbol izquierdo son estrictamente menores y todos los elementos en su subárbol
derecho son estrictamente mayores. Esto permite que las operaciones de búsqueda tengan una complejidad
promedio logarítmica. El programa mide de forma precisa el tiempo de búsqueda individual para múltiples 
objetivos y exporta los resultados a un archivo CSV.

OBSERVACIONES: La cantidad de elementos 'n' debe ser acorde al numero de lineas del archivo de entrada.
El archivo "objetivos.txt" debe existir en el mismo directorio.

Compilación: gcc busq_abb.c tiempo.c -o busqueda_abb
Ejecución: ./busqueda_abb <numeros10millones.txt> <n>
*/

#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

typedef struct Nodo {
    int dato;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

/* --- PROTOTIPOS --- */
Nodo* crearNodo(int valor);
Nodo* insertar(Nodo* raiz, int valor);
Nodo* buscarABB(Nodo* raiz, int objetivo);
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void liberarArbol(Nodo* raiz); 

/*
int main(int argc, char *argv[])
Recibe: argc - Número de argumentos recibidos (incluyendo el nombre del programa).
        *argv[] - Vector de cadenas (argv[1] es el archivo de datos, argv[2] es la cantidad 'n').
Devuelve: int (0 si el programa finaliza correctamente, 1 en caso de error).
Función: 
    1. Valida los argumentos de entrada desde la terminal.
    2. Gestiona la carga de 'n' datos desordenados desde el archivo a la memoria.
    3. Construye iterativamente el Árbol Binario de Búsqueda insertando los datos.
    4. Carga un conjunto de objetivos a buscar desde "objetivos.txt".
    5. Implementa el cronometraje con 'uswtime' aislando exclusivamente la función de búsqueda.
    6. Exporta fila por fila los resultados (tiempo Real, User, Sys y estado de éxito) a un CSV.
    7. Limpia la memoria dinámica (arreglo y árbol) antes de salir.
*/
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
Nodo* buscarABB(Nodo* raiz, int objetivo)
DESCRIPCION: Función recursiva que busca un elemento específico navegando por las ramas del ABB.
Recibe: raiz - Puntero al nodo raíz del subárbol actual donde se está buscando.
        objetivo - El número entero que se desea localizar.
Devuelve: Un puntero al Nodo que contiene el valor (si lo encuentra) o NULL si no existe en el árbol.
*/
Nodo* buscarABB(Nodo* raiz, int objetivo) {
    if (raiz == NULL || raiz->dato == objetivo)
        return raiz;

    if (objetivo > raiz->dato)
        return buscarABB(raiz->der, objetivo);

    return buscarABB(raiz->izq, objetivo);
}

/*
int* cargarNumeros(char *nombre, int cantidad, int conComas)
DESCRIPCION: Función utilitaria para leer archivos de texto hacia un arreglo dinámico.
Recibe: nombre - Cadena de texto con el nombre del archivo.
        cantidad - Número total de elementos a extraer.
        conComas - Bandera entera (1 si los números están separados por comas, 0 por saltos de línea).
Devuelve: Un puntero al arreglo de enteros cargado en memoria.
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