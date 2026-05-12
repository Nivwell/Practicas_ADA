#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

// 1. Estructura para pasar argumentos a los hilos
typedef struct {
    int *arreglo;
    int objetivo;
    int inicio;
    int fin;
    int id;
} ThreadArgs;

// 2. Variables Globales para Sincronización
int POSICION_ENCONTRADA = -1;
pthread_mutex_t cerrojo = PTHREAD_MUTEX_INITIALIZER;

// Prototipos
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void ejecutarOrdenamiento(int *arr, int n, char *nombreSalida);
void QuickSort(int *arr, int p, int r);
int Pivot(int *arr, int p, int r);
void Intercambiar(int *arr, int i, int j);
void guardarArchivo(char *nombre, int *arr, int n);
void* buscar_lineal_thread(void* args);

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s <num_threads> <sucio.txt> <ordenado.txt> <n>\n", argv[0]);
        return 1;
    }

    int NumThreads = atoi(argv[1]);
    char *f_sucio = argv[2];
    char *f_ordenado = argv[3];
    int n = atoi(argv[4]);

    double utime0, stime0, wtime0, utime1, stime1, wtime1;
    
    // 1. Carga de datos y objetivos
    printf("Cargando %d numeros...\n", n);
    int *datos = cargarNumeros(f_sucio, n, 0);
    if (!datos) return 1;

    printf("Ordenando datos...\n");
    ejecutarOrdenamiento(datos, n, f_ordenado);

    int n_objetivos = 20;
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);
    if (!objetivos) return 1;

    pthread_t *hilos = malloc(NumThreads * sizeof(pthread_t));
    ThreadArgs *t_args = malloc(NumThreads * sizeof(ThreadArgs));

    // --- INICIO DE MEDICIÓN DE RENDIMIENTO ---
    printf("\nIniciando busqueda de %d objetivos con %d hilos...\n", n_objetivos, NumThreads);
    uswtime(&utime0, &stime0, &wtime0);

    for (int j = 0; j < n_objetivos; j++) {
        int buscado = objetivos[j];
        POSICION_ENCONTRADA = -1; 

        for (int i = 0; i < NumThreads; i++) {
            t_args[i].arreglo = datos;
            t_args[i].objetivo = buscado;
            t_args[i].id = i;
            t_args[i].inicio = (i * n) / NumThreads;
            t_args[i].fin = (i == NumThreads - 1) ? n - 1 : ((i + 1) * n) / NumThreads - 1;

            pthread_create(&hilos[i], NULL, buscar_lineal_thread, (void*)&t_args[i]);
        }

        for (int i = 0; i < NumThreads; i++) {
            pthread_join(hilos[i], NULL);
        }

        if (POSICION_ENCONTRADA != -1)
            printf("[Objetivo %d] Encontrado en: %d\n", buscado, POSICION_ENCONTRADA);
        else
            printf("[Objetivo %d] No encontrado\n", buscado);
    }

    uswtime(&utime1, &stime1, &wtime1);
    // --- FIN DE MEDICIÓN ---

    // 4. Reporte Final en Pantalla
    printf("\n========================================\n");
    printf("REPORTE DE RENDIMIENTO (%d hilos)\n", NumThreads);
    printf("Real (Tiempo total): %.10f s\n", wtime1 - wtime0);
    printf("User (Tiempo en CPU): %.10f s\n", utime1 - utime0);
    printf("Sys (Tiempo E/S): %.10f s\n", stime1 - stime0);
    printf("CPU/Wall: %.2f%%\n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    printf("========================================\n");

    // ==========================================================
    // NUEVA SECCIÓN: GUARDAR EN ARCHIVO AUTOMÁTICAMENTE
    // ==========================================================
    FILE *f_tiempos = fopen("bitacora_tiempos.txt", "a"); // "a" abre para añadir al final
    if (f_tiempos != NULL) {
        fprintf(f_tiempos, "Algoritmo: Lineal con Hilos | n: %d | Hilos: %d\n", n, NumThreads);
        fprintf(f_tiempos, "Real: %.10f s\n", wtime1 - wtime0);
        fprintf(f_tiempos, "User: %.10f s\n", utime1 - utime0);
        fprintf(f_tiempos, "Sys: %.10f s\n", stime1 - stime0);
        fprintf(f_tiempos, "CPU/Wall: %.2f%%\n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
        fprintf(f_tiempos, "----------------------------------------------------\n");
        fclose(f_tiempos);
        printf("\nResultados guardados en 'bitacora_tiempos.txt'\n");
    } else {
        printf("\nError: No se pudo crear el archivo de tiempos.\n");
    }

    free(datos); free(objetivos); free(hilos); free(t_args);
    return 0;
}

// (Las funciones buscar_lineal_thread, cargarNumeros, etc., permanecen igual)
void* buscar_lineal_thread(void* args) {
    ThreadArgs* datos = (ThreadArgs*)args;
    for (int i = datos->inicio; i <= datos->fin; i++) {
        if (POSICION_ENCONTRADA != -1) pthread_exit(NULL);
        if (datos->arreglo[i] == datos->objetivo) {
            pthread_mutex_lock(&cerrojo);
            if (POSICION_ENCONTRADA == -1) POSICION_ENCONTRADA = i;
            pthread_mutex_unlock(&cerrojo);
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

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

void ejecutarOrdenamiento(int *arr, int n, char *nombreSalida) {
    QuickSort(arr, 0, n - 1);
    guardarArchivo(nombreSalida, arr, n);
}

void QuickSort(int *arr, int p, int r) {
    if (p < r) {
        int j = Pivot(arr, p, r);
        QuickSort(arr, p, j - 1);
        QuickSort(arr, j + 1, r);
    }
}

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

void Intercambiar(int *arr, int i, int j) {
    int temp = arr[j]; arr[j] = arr[i]; arr[i] = temp;
}

void guardarArchivo(char *nombre, int *arr, int n) {
    FILE *f = fopen(nombre, "w");
    if (f == NULL) return;
    for (int i = 0; i < n; i++) fprintf(f, "%d\n", arr[i]);
    fclose(f);
}