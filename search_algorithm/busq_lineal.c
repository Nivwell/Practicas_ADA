/*
IMPLEMENTACION DEL ALGORITMO DE BUSQUEDA LINEAL SECUENCIAL
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
        Pérez Ortiz Luis Angel
Autor original de la librería de tiempos: 
        Edgardo Adrián Franco Martinez
Fecha de entrega 20 de Mayo del 2026
Version: 2.0

DESCRIPCION: Este codigo implementa el algoritmo de Búsqueda Lineal (o secuencial) para encontrar 
elementos específicos dentro de un arreglo de n elementos desordenados. La búsqueda lineal funciona 
recorriendo cada elemento del arreglo desde el primer índice hasta el último, comparándolo con el 
valor objetivo. Su complejidad en el peor de los casos es O(n). El programa mide de forma aislada 
el tiempo de búsqueda para múltiples objetivos empleando la librería 'tiempo.h' y exporta los 
resultados en tiempo real a un archivo CSV.

OBSERVACIONES: La cantidad de elementos 'n' debe ser un número entero positivo y coincidir 
con el tamaño del archivo de entrada. El archivo "objetivos.txt" debe existir en el mismo directorio.

Compilación: gcc busq_lineal.c tiempo.c -o busq_lineal
Ejecución: ./busq_lineal <numeros10millones.txt> <n>
*/

#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

/* --- PROTOTIPOS --- */
int* cargarNumeros(char *nombre, int cantidad, int conComas);
int busqueda_lineal_secuencial(int *arreglo, int n, int objetivo);

/*
int main(int argc, char *argv[])
Recibe: argc - Número de argumentos recibidos (incluyendo el nombre del programa).
        *argv[] - Vector de cadenas (argv[1] es el archivo de datos desordenados, argv[2] es la cantidad 'n').
Devuelve: int (0 si el programa finaliza correctamente, 1 en caso de error).
Función: 
    1. Valida los argumentos de entrada desde la terminal.
    2. Gestiona la carga de 'n' datos desordenados desde el archivo a un arreglo dinámico en memoria.
    3. Carga un conjunto de objetivos a buscar desde "objetivos.txt".
    4. Abre (o crea) el archivo "bitacora_tiempos.csv" en modo append.
    5. Implementa el cronometraje con 'uswtime' aislando exclusivamente la función de Búsqueda Lineal.
    6. Exporta fila por fila los resultados (tiempo Real, User, Sys, posición y estado de éxito) al CSV, forzando la escritura con fflush.
    7. Limpia la memoria dinámica antes de finalizar el proceso.
*/
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

/* --- FUNCIÓN DE BÚSQUEDA TRADICIONAL --- */

/*
int busqueda_lineal_secuencial(int *arreglo, int n, int objetivo)
DESCRIPCION: Función que implementa el algoritmo de búsqueda lineal inspeccionando elemento por elemento.
Recibe: arreglo - Puntero al arreglo de enteros (no requiere estar ordenado).
        n - Número total de elementos en el arreglo.
        objetivo - El número entero que se desea localizar.
Devuelve: Un entero que representa el índice de la primera aparición del objetivo, o -1 si no existe en el arreglo.
*/
int busqueda_lineal_secuencial(int *arreglo, int n, int objetivo) {
    for (int i = 0; i < n; i++) {
        if (arreglo[i] == objetivo) {
            return i; 
        }
    }
    return -1; 
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