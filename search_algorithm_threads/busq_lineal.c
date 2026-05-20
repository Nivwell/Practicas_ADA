/*
IMPLEMENTACION DEL ALGORITMO DE BUSQUEDA LINEAL PARALELA (CON HILOS)
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
        Pérez Ortiz Luis Angel
Autor original de la librería de tiempos: 
        Edgardo Adrián Franco Martinez
Fecha de entrega 20 de Mayo del 2026
Version: 2.0

DESCRIPCION: Este codigo implementa el algoritmo de Búsqueda Lineal (o secuencial) para localizar 
elementos dentro de un arreglo desordenado, distribuyendo la carga de trabajo entre múltiples 
hilos mediante la biblioteca Pthreads. El arreglo principal se divide en bloques (chunks) 
de tamaño equitativo. Cada hilo recorre exclusivamente su bloque asignado. Para optimizar 
el rendimiento, todos los hilos monitorean una variable global compartida; si algún hilo 
encuentra el objetivo, los demás abortan su búsqueda inmediatamente para no desperdiciar 
ciclos de CPU. El tiempo total de vida de los hilos se registra y exporta a un archivo CSV.

OBSERVACIONES: La cantidad de elementos 'n' debe coincidir con el tamaño del archivo de entrada. 
El archivo "objetivos.txt" debe existir en el mismo directorio.

Compilación: gcc busq_lineal_paralela.c tiempo.c -lpthread -o busq_lineal_paralela
Ejecución: ./busq_lineal_paralela <numero10millones.txt> <n> <hilos>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Librería vital para el manejo de hilos
#include "tiempo.h"

/* --- ESTRUCTURA PARA LOS HILOS --- */
// Pthreads solo permite pasar un (1) argumento a la función del hilo.
// Usamos un struct para empaquetar toda la información que el hilo necesita.
typedef struct {
    int *arreglo;
    int objetivo;
    int inicio;
    int fin;
    int *posicion_global; // Puntero compartido para avisar si ya se encontró
} ThreadArgs;

/* --- PROTOTIPOS --- */
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void* busquedaLinealHilo(void *arg);

/*
int main(int argc, char *argv[])
Recibe: argc - Número de argumentos recibidos.
        *argv[] - Vector de cadenas (argv[1] es el archivo de datos, argv[2] es 'n', argv[3] es la cantidad de hilos).
Devuelve: int (0 si el programa finaliza correctamente, 1 en caso de error).
Función: 
    1. Valida los 3 argumentos de entrada requeridos desde la terminal.
    2. Carga los datos desordenados desde el archivo de texto a la memoria dinámica.
    3. Carga el arreglo de objetivos a buscar desde "objetivos.txt".
    4. Implementa el cronometraje midiendo la creación, ejecución y destrucción concurrente de los hilos.
    5. Exporta fila por fila los resultados (tiempo Real, User, Sys, posición y cantidad de hilos) a un CSV.
    6. Limpia la memoria dinámica antes de finalizar la ejecución.
*/
int main(int argc, char *argv[]) {
    // Ahora pedimos 3 argumentos: archivo, N e hilos
    if (argc < 4) {
        printf("Uso: %s <numero10millones.txt> <n> <hilos>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    int n = atoi(argv[2]);
    int num_hilos = atoi(argv[3]);
    int n_objetivos = 20;

    double utime0, stime0, wtime0, utime1, stime1, wtime1;
    
    // PASO 1: CARGA DE DATOS SIN ORDENAR
    printf("[1/3] Cargando %d numeros sin ordenar... ", n); fflush(stdout);
    int *datos = cargarNumeros(f_sucio, n, 0);
    if (!datos) {
        printf("Error al abrir %s\n", f_sucio);
        return 1;
    }
    printf("OK\n");

    // PASO 2: CARGA DE OBJETIVOS
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);
    if (!objetivos) { 
        printf("Error: No se encontro objetivos.txt\n"); 
        free(datos);
        return 1; 
    }

    // PASO 3: BÚSQUEDA PARALELA Y ESCRITURA EN CSV
    printf("[2/3] Buscando %d objetivos usando %d hilo(s)...\n", n_objetivos, num_hilos);
    
    FILE *f_csv = fopen("bitacora_tiempos.csv", "a");
    if (!f_csv) {
        printf("[X] Error: No se pudo abrir o crear 'bitacora_tiempos.csv'\n");
        free(datos); free(objetivos);
        return 1;
    }

    // Calculamos el tamaño del bloque de datos que le toca a cada hilo
    int chunk = n / num_hilos;

    for (int j = 0; j < n_objetivos; j++) {
        int buscado = objetivos[j];
        int posicion_encontrada = -1; // -1 significa no encontrado
        
        // Arreglos para manejar los hilos y sus argumentos
        pthread_t hilos[num_hilos];
        ThreadArgs args[num_hilos];
        
        // ----------------------------------------------------
        // INICIA CRONÓMETRO: Creación, ejecución y destrucción
        // ----------------------------------------------------
        uswtime(&utime0, &stime0, &wtime0);

        // 1. Lanzar los hilos
        for (int i = 0; i < num_hilos; i++) {
            args[i].arreglo = datos;
            args[i].objetivo = buscado;
            args[i].inicio = i * chunk;
            
            // El último hilo se lleva el "residuo" si la división no es exacta
            args[i].fin = (i == num_hilos - 1) ? (n - 1) : (i * chunk + chunk - 1);
            
            args[i].posicion_global = &posicion_encontrada;
            
            pthread_create(&hilos[i], NULL, busquedaLinealHilo, &args[i]);
        }

        // 2. Esperar a que todos los hilos terminen su bloque
        for (int i = 0; i < num_hilos; i++) {
            pthread_join(hilos[i], NULL);
        }

        uswtime(&utime1, &stime1, &wtime1);
        // ----------------------------------------------------
        // TERMINA CRONÓMETRO
        // ----------------------------------------------------

        int encontrado = (posicion_encontrada != -1) ? 1 : 0;

        if (encontrado) {
            printf("  - Objetivo [%d]: Encontrado en indice %d\n", buscado, posicion_encontrada);
        } else {
            printf("  - Objetivo [%d]: No encontrado\n", buscado);
        }

        // ESCRITURA FORZADA EN EL CSV
        // Nota: Actualizamos la columna "Hilos" para que registre num_hilos en vez de un "1" quemado en el código
        fprintf(f_csv, "Busqueda Lineal Paralela,%d,%d,%d,%d,%d,%.10f,%.10f,%.10f\n", 
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
    printf("[3/3] Resultados guardados exitosamente en 'bitacora_tiempos.csv'\n");

    free(datos); 
    free(objetivos);
    return 0;
}

/* --- FUNCIÓN DEL HILO (BÚSQUEDA LINEAL FRAGMENTADA) --- */

/*
void* busquedaLinealHilo(void *arg)
DESCRIPCION: Función ejecutada concurrentemente por cada hilo. Recorre linealmente el bloque de 
memoria que le fue asignado. Se interrumpe de forma anticipada si detecta que la variable 
compartida 'posicion_global' ya fue modificada por otro hilo que haya encontrado el elemento.
Recibe: arg - Puntero genérico (void*) a la estructura ThreadArgs con los datos de configuración del bloque.
Devuelve: NULL al finalizar la ejecución del hilo.
*/
void* busquedaLinealHilo(void *arg) {
    ThreadArgs *datos = (ThreadArgs*)arg;
    
    for (int i = datos->inicio; i <= datos->fin; i++) {
        // Variable compartida: Si otro hilo ya encontró el número, 
        // abortamos la búsqueda en este hilo para ahorrar CPU.
        if (*(datos->posicion_global) != -1) {
            break;
        }
        
        if (datos->arreglo[i] == datos->objetivo) {
            *(datos->posicion_global) = i; // Guardamos la posición
            break; 
        }
    }
    return NULL; // El hilo termina exitosamente
}

/* --- FUNCIÓN DE CARGA --- */

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
    if (!f) return NULL;
    int *arreglo = malloc(cantidad * sizeof(int));
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