/*
IMPLEMENTACION DEL ALGORITMO DE BUSQUEDA BINARIA PARALELA (CON HILOS)
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
        Pérez Ortiz Luis Angel
Autor original de la librería de tiempos: 
        Edgardo Adrián Franco Martinez
Fecha de entrega 20 de Mayo del 2026
Version: 2.0

DESCRIPCION: Este codigo implementa la Búsqueda Binaria sobre un arreglo ordenado, 
distribuyendo la carga de trabajo entre múltiples hilos utilizando la biblioteca Pthreads. 
El arreglo se divide en bloques (chunks) de tamaño equitativo. Como los datos están 
ordenados por QuickSort, cada hilo realiza una optimización inicial comprobando si el 
valor objetivo está dentro del rango [inicio, fin] de su bloque asignado. Si el elemento 
puede existir ahí, el hilo ejecuta la búsqueda binaria internamente; si no, el hilo 
termina de inmediato para ahorrar ciclos de CPU. El tiempo total de vida de los hilos 
se registra y exporta a un archivo CSV.

OBSERVACIONES: La cantidad de elementos 'n' debe coincidir con el tamaño del archivo de entrada. 
El archivo "objetivos.txt" debe existir en el mismo directorio.

Compilación: gcc busq_binaria_paralela.c tiempo.c -lpthread -o busq_binaria_paralela
Ejecución: ./busq_binaria_paralela <sucio.txt> <n> <hilos>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tiempo.h"

/* --- ESTRUCTURA PARA LOS HILOS --- */
typedef struct {
    int *arreglo;
    int objetivo;
    int inicio;
    int fin;
    int *posicion_global;
} ThreadArgs;

/* --- PROTOTIPOS --- */
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void QuickSort(int *arr, int p, int r);
int Pivot(int *arr, int p, int r);
void Intercambiar(int *arr, int i, int j);
void* busquedaBinariaHilo(void *arg);

/*
int main(int argc, char *argv[])
Recibe: argc - Número de argumentos recibidos (incluyendo el nombre del programa).
        *argv[] - Vector de cadenas (argv[1] es el archivo de datos desordenados, argv[2] es 'n', argv[3] es la cantidad de hilos).
Devuelve: int (0 si el programa finaliza correctamente, 1 en caso de error).
Función: 
    1. Valida los 3 argumentos de entrada requeridos desde la terminal.
    2. Gestiona la carga de 'n' datos desordenados desde el archivo a la memoria dinámica.
    3. Ordena los datos en memoria utilizando QuickSort.
    4. Carga un conjunto de objetivos a buscar desde "objetivos.txt".
    5. Implementa el cronometraje con 'uswtime' midiendo la creación, ejecución y destrucción de los hilos.
    6. Exporta fila por fila los resultados (tiempo Real, User, Sys, posición y cantidad de hilos) a un CSV.
    7. Limpia la memoria dinámica antes de finalizar el proceso.
*/
int main(int argc, char *argv[]) {
    // Pedimos 3 argumentos: archivo sucio, N e hilos
    if (argc < 4) {
        printf("Uso: %s <sucio.txt> <n> <hilos>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    int n = atoi(argv[2]);
    int num_hilos = atoi(argv[3]);
    int *datos = NULL;

    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    // 1. CARGA DE DATOS DESORDENADOS
    printf("[1/4] Cargando %d numeros originales...\n", n);
    datos = cargarNumeros(f_sucio, n, 0);
    if (datos == NULL) {
        printf("[X] Error al cargar el archivo %s\n", f_sucio);
        return 1;
    }

    // 2. ORDENAMIENTO EN MEMORIA DIRECTA
    printf("[2/4] Ordenando %d elementos con QuickSort...\n", n);
    uswtime(&utime0, &stime0, &wtime0);
    
    QuickSort(datos, 0, n - 1);
    
    uswtime(&utime1, &stime1, &wtime1);
    printf("      -> Tiempo de ordenamiento Real: %.10f s\n", wtime1 - wtime0);

    // 3. CARGA DE OBJETIVOS
    int n_objetivos = 20;
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);
    if (!objetivos) {
        printf("[X] Error: No se encontro objetivos.txt\n");
        free(datos);
        return 1;
    }

    // 4. EJECUCIÓN DE BÚSQUEDA BINARIA PARALELA
    printf("\n[3/4] Ejecutando Busqueda Binaria usando %d hilo(s)...\n", num_hilos);
    
    FILE *f_csv = fopen("bitacora_tiempos.csv", "a");
    if (!f_csv) {
        printf("[X] Error: No se pudo abrir o crear 'bitacora_tiempos.csv'\n");
        free(datos); free(objetivos);
        return 1;
    }

    int chunk = n / num_hilos;

    for (int j = 0; j < n_objetivos; j++) {
        int buscado = objetivos[j];
        int posicion_encontrada = -1;
        
        pthread_t hilos[num_hilos];
        ThreadArgs args[num_hilos];

        // ----------------------------------------------------
        // INICIA CRONÓMETRO
        // ----------------------------------------------------
        uswtime(&utime0, &stime0, &wtime0);

        for (int i = 0; i < num_hilos; i++) {
            args[i].arreglo = datos;
            args[i].objetivo = buscado;
            args[i].inicio = i * chunk;
            args[i].fin = (i == num_hilos - 1) ? (n - 1) : (i * chunk + chunk - 1);
            args[i].posicion_global = &posicion_encontrada;
            
            pthread_create(&hilos[i], NULL, busquedaBinariaHilo, &args[i]);
        }

        for (int i = 0; i < num_hilos; i++) {
            pthread_join(hilos[i], NULL);
        }

        uswtime(&utime1, &stime1, &wtime1);
        // ----------------------------------------------------
        // TERMINA CRONÓMETRO
        // ----------------------------------------------------

        int encontrado = (posicion_encontrada != -1) ? 1 : 0;

        if (encontrado) {
            printf("  - Objetivo [%d]: Encontrado en el indice %d\n", buscado, posicion_encontrada);
        } else {
            printf("  - Objetivo [%d]: No encontrado\n", buscado);
        }

        fprintf(f_csv, "Busqueda Binaria Paralela,%d,%d,%d,%d,%d,%.10f,%.10f,%.10f\n", 
                n, 
                buscado, 
                encontrado, 
                posicion_encontrada, 
                num_hilos,
                wtime1 - wtime0, 
                utime1 - utime0, 
                stime1 - stime0);
        
        fflush(f_csv);
    }

    fclose(f_csv);
    printf("\n[4/4] Resultados guardados exitosamente en 'bitacora_tiempos.csv'\n");

    free(objetivos);
    free(datos);
    return 0;
}

/* --- FUNCIÓN DEL HILO (BÚSQUEDA BINARIA FRAGMENTADA) --- */

/*
void* busquedaBinariaHilo(void *arg)
DESCRIPCION: Función ejecutada por cada hilo. Comprueba de forma inteligente si el elemento a buscar 
tiene posibilidad de estar en el bloque asignado al hilo. De ser así, ejecuta una búsqueda binaria local.
Recibe: arg - Puntero genérico (void*) a la estructura ThreadArgs con los datos del bloque asignado.
Devuelve: NULL al finalizar la ejecución del hilo.
*/
void* busquedaBinariaHilo(void *arg) {
    ThreadArgs *datos = (ThreadArgs*)arg;
    
    // Optimización: Como el arreglo está ordenado, el hilo revisa si el número 
    // tiene posibilidad matemática de estar en su bloque antes de buscar.
    if (datos->objetivo >= datos->arreglo[datos->inicio] && 
        datos->objetivo <= datos->arreglo[datos->fin]) {
        
        int izquierda = datos->inicio;
        int derecha = datos->fin;

        while (izquierda <= derecha) {
            // Si otro hilo ya lo encontró (poco probable por la optimización, pero seguro)
            if (*(datos->posicion_global) != -1) break;

            int medio = izquierda + (derecha - izquierda) / 2;

            if (datos->arreglo[medio] == datos->objetivo) {
                *(datos->posicion_global) = medio; // Encontrado
                break;
            }

            if (datos->arreglo[medio] < datos->objetivo)
                izquierda = medio + 1;
            else
                derecha = medio - 1;
        }
    }
    
    return NULL;
}

/* --- BLOQUE DE FUNCIONES --- */

/*
int* cargarNumeros(char *nombre, int cantidad, int conComas)
DESCRIPCION: Función utilitaria para leer archivos de texto y cargar sus valores en un arreglo dinámico.
Recibe: nombre - Cadena de texto con el nombre del archivo a leer.
        cantidad - Número total de elementos a extraer del archivo.
        conComas - Bandera entera (1 si los números están separados por comas, 0 por saltos de línea/espacios).
Devuelve: Un puntero al arreglo de enteros cargado en memoria, o NULL en caso de error.
*/
int* cargarNumeros(char *nombre, int cantidad, int conComas) {
    FILE *f = fopen(nombre, "r");
    if (f == NULL) return NULL;
    int *arreglo = (int *)malloc(cantidad * sizeof(int));
    for (int i = 0; i < cantidad; i++) {
        if (conComas) fscanf(f, " %d ,", &arreglo[i]);
        else fscanf(f, "%d", &arreglo[i]);
    }
    fclose(f);
    return arreglo;
}

/*
void QuickSort(int *arr, int p, int r)
DESCRIPCION: Función que divide el arreglo en subarreglos más pequeños alrededor de un pivote y 
luego ordena esos subarreglos de manera recursiva hasta que todo el arreglo está ordenado de menor a mayor.
Recibe: arr - Puntero al arreglo de enteros a ordenar.
        p - Índice inicial (límite inferior) del subarreglo.
        r - Índice final (límite superior) del subarreglo.
Devuelve: void (No retorna valor explicito).
*/
void QuickSort(int *arr, int p, int r) {
    if (p < r) {
        int j = Pivot(arr, p, r);
        QuickSort(arr, p, j - 1);
        QuickSort(arr, j + 1, r);
    }
}

/*
int Pivot(int *arr, int p, int r)
DESCRIPCION: Función auxiliar de QuickSort que selecciona un pivote y particiona el arreglo, 
colocando los elementos menores a la izquierda del pivote y los mayores a la derecha.
Recibe: arr - Puntero al arreglo de enteros.
        p - Índice inicial para iniciar la evaluación.
        r - Índice final para la evaluación.
Devuelve: Un entero que representa el índice final de partición donde fue ubicado el pivote.
*/
int Pivot(int *arr, int p, int r) {
    int piv = arr[p], i = p + 1, j = r;
    while (i <= j) {
        while (i <= r && arr[i] <= piv) i++;
        while (arr[j] > piv) j--;
        if (i < j) Intercambiar(arr, i, j);
    }
    Intercambiar(arr, p, j);
    return j;
}

/*
void Intercambiar(int *arr, int i, int j)
DESCRIPCION: Función utilitaria para realizar el intercambio (swap) de dos elementos dentro del arreglo.
Recibe: arr - Puntero al arreglo de enteros.
        i - Índice de la posición del primer elemento.
        j - Índice de la posición del segundo elemento.
Devuelve: void (No retorna valor explicito).
*/
void Intercambiar(int *arr, int i, int j) {
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}