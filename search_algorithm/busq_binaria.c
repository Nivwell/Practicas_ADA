#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

/* --- PROTOTIPOS --- */
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void QuickSort(int *arr, int p, int r);
int Pivot(int *arr, int p, int r);
void Intercambiar(int *arr, int i, int j);
int busquedaBinaria(int *arreglo, int n, int objetivo);

int main(int argc, char *argv[]) {
    // Ahora solo pedimos el archivo sucio y la N
    if (argc < 3) {
        printf("Uso: %s <sucio.txt> <n>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    int n = atoi(argv[2]);
    int *datos = NULL;

    // Variables para medir tiempos
    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    // 1. CARGA DE DATOS DESORDENADOS
    printf("[1/4] Cargando %d numeros originales...\n", n);
    datos = cargarNumeros(f_sucio, n, 0);
    if (datos == NULL) {
        printf("[X] Error al cargar el archivo %s\n", f_sucio);
        return 1;
    }

    // 2. ORDENAMIENTO EN MEMORIA DIRECTA
    // Medimos cuánto le cuesta al CPU ordenar todo el arreglo antes de buscar
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

    // 4. EJECUCIÓN DE BÚSQUEDA BINARIA Y REGISTRO EN CSV
    printf("\n[3/4] Ejecutando Busqueda Binaria individual...\n");
    
    FILE *f_csv = fopen("bitacora_tiempos.csv", "a");
    if (!f_csv) {
        printf("[X] Error: No se pudo abrir o crear 'bitacora_tiempos.csv'\n");
        free(datos); free(objetivos);
        return 1;
    }

    for (int i = 0; i < n_objetivos; i++) {
        int buscado = objetivos[i];
        
        // Cronómetro exclusivo para la búsqueda actual
        uswtime(&utime0, &stime0, &wtime0);
        int posicion = busquedaBinaria(datos, n, buscado);
        uswtime(&utime1, &stime1, &wtime1);

        int encontrado = (posicion != -1) ? 1 : 0;

        // Imprimir en consola para validación visual
        if (encontrado) {
            printf("  - Objetivo [%d]: Encontrado en el indice %d\n", buscado, posicion);
        } else {
            printf("  - Objetivo [%d]: No encontrado\n", buscado);
        }

        // ESCRITURA FORZADA FILA POR FILA EN EL CSV
        // Formato: Algoritmo, N, Objetivo, Encontrado, Indice, Hilos, T_Real, T_User, T_Sys
        fprintf(f_csv, "Busqueda Binaria Secuencial,%d,%d,%d,%d,1,%.10f,%.10f,%.10f\n", 
                n, 
                buscado, 
                encontrado, 
                posicion, 
                wtime1 - wtime0, 
                utime1 - utime0, 
                stime1 - stime0);
        
        fflush(f_csv); // Obligamos a escribir al disco duro de inmediato
    }

    fclose(f_csv);
    printf("\n[4/4] Resultados guardados exitosamente en 'bitacora_tiempos.csv'\n");

    free(objetivos);
    free(datos);
    return 0;
}

/* --- FUNCIÓN DE BÚSQUEDA BINARIA --- */
int busquedaBinaria(int *arreglo, int n, int objetivo) {
    int izquierda = 0;
    int derecha = n - 1;

    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        if (arreglo[medio] == objetivo)
            return medio; // Encontrado

        if (arreglo[medio] < objetivo)
            izquierda = medio + 1; // Buscar en la mitad derecha
        else
            derecha = medio - 1; // Buscar en la mitad izquierda
    }
    return -1; // No encontrado
}

/* --- BLOQUE DE FUNCIONES --- */

int* cargarNumeros(char *nombre, int cantidad, int conComas) {
    FILE *f = fopen(nombre, "r");
    if (f == NULL) return NULL;
    int *arreglo = (int *)malloc(cantidad * sizeof(int));
    for (int i = 0; i < cantidad; i++) {
        if (conComas) fscanf(f, " %d ,", &arreglo[i]); // Formato para objetivos
        else fscanf(f, "%d", &arreglo[i]);
    }
    fclose(f);
    return arreglo;
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
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}