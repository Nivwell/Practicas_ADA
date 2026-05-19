#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

int* cargarNumeros(char *nombre, int cantidad, int conComas);
int busqueda_lineal_secuencial(int *arreglo, int n, int objetivo);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <numero10millones.txt> <n>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    int n = atoi(argv[2]);
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

    // PASO 3: BÚSQUEDA Y ESCRITURA INMEDIATA EN CSV
    printf("[2/3] Buscando %d objetivos de manera secuencial...\n", n_objetivos);
    
    FILE *f_csv = fopen("bitacora_tiempos.csv", "a");
    if (!f_csv) {
        printf("[X] Error: No se pudo abrir o crear 'bitacora_tiempos.csv'\n");
        free(datos); free(objetivos);
        return 1;
    }

    for (int j = 0; j < n_objetivos; j++) {
        int buscado = objetivos[j];
        
        // Cronómetro exclusivo para la búsqueda actual
        uswtime(&utime0, &stime0, &wtime0);
        int posicion = busqueda_lineal_secuencial(datos, n, buscado);
        uswtime(&utime1, &stime1, &wtime1);

        int encontrado = (posicion != -1) ? 1 : 0;

        // Imprimir en consola para validación visual
        if (encontrado) {
            printf("  - Objetivo [%d]: Encontrado en indice %d\n", buscado, posicion);
        } else {
            printf("  - Objetivo [%d]: No encontrado\n", buscado);
        }

        // ESCRITURA FORZADA FILA POR FILA EN EL CSV
        // Formato: Algoritmo, N, Objetivo, Encontrado, Indice, Hilos, T_Real, T_User, T_Sys
        fprintf(f_csv, "Busqueda Lineal Secuencial,%d,%d,%d,%d,1,%.10f,%.10f,%.10f\n", 
                n, 
                buscado, 
                encontrado, 
                posicion, 
                wtime1 - wtime0, 
                utime1 - utime0, 
                stime1 - stime0);
        
        fflush(f_csv); // Obliga al sistema operativo a escribir la fila en el disco duro AHORA
    }

    fclose(f_csv);
    printf("[3/3] Resultados guardados exitosamente en 'bitacora_tiempos.csv'\n");

    free(datos); 
    free(objetivos);
    return 0;
}

// BÚSQUEDA TRADICIONAL
int busqueda_lineal_secuencial(int *arreglo, int n, int objetivo) {
    for (int i = 0; i < n; i++) {
        if (arreglo[i] == objetivo) {
            return i; 
        }
    }
    return -1; 
}

// FUNCIÓN DE CARGA
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