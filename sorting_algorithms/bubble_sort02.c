/*
Algoritmo de ordenamiento. Burbuja.
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
Autor original: 
        Edgardo Adri­an Franco Martinez
Fecha de entrega 18 de Marzo del 2025
Version: 2.0

Compilación:
Linux: gcc bubble_sort02.c tiempo.c -o bubble_sort02
Ejecutar como: ./bubble_sort02 10000000 < numeros10millones.txt >> salida.csv

Este programa implementa el algoritmo de ordenamiento Burbuja optimizado.
*/

#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

#define TRUE 1
#define FALSE 0

void AlgoritmoBurbujaOpt2(int *arregloA, int n);

/*
int main(int num_arg, char *argv[])
Recibe: num_arg - Número de argumentos recibidos por la línea de comandos.
        *argv[] - Arreglo de cadenas con los parámetros (espera el tamaño 'n').
Devuelve: int (0 si la ejecución finaliza correctamente).
Función: 
    1. Valida la recepción del tamaño del arreglo como argumento de entrada.
    2. Reserva dinámicamente el bloque de memoria necesario para 'n' enteros.
    3. Pobla el arreglo mediante la lectura de datos (redirigidos o manuales).
    4. Ejecuta el 'AlgoritmoBurbujaOpt2' bajo un marco de medición temporal.
    5. Utiliza 'uswtime' para obtener métricas de tiempo real, sistema y usuario.
    6. Presenta los resultados del ordenamiento y el análisis de rendimiento de la CPU.
    7. Garantiza la liberación de memoria (free) antes del cierre del proceso.
*/
int main(int num_arg, char *argv[])
{
    int i, n;
    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    if (num_arg != 2)
    {
        fprintf(stderr, "Indique el tamaño de datos a ordenar. Ejemplo: %s 100\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);

    int *arregloA = malloc(n * sizeof(int));
    if (arregloA == NULL)
    {
        fprintf(stderr, "Error al intentar reservar memoria para %d elementos\n", n);
        exit(1);
    }

    /* -------- Leer datos desde stdin -------- */
    for (i = 0; i < n; i++)
    {
        if (scanf("%d", &arregloA[i]) != 1)
        {
            fprintf(stderr, "Error al leer datos en la posición %d\n", i);
            free(arregloA);
            exit(1);
        }
    }

    /* -------- Inicio medición -------- */
    uswtime(&utime0, &stime0, &wtime0);

    AlgoritmoBurbujaOpt2(arregloA, n);

    /* -------- Fin medición -------- */
    uswtime(&utime1, &stime1, &wtime1);

    /* -------- Cálculos seguros -------- */
    double wall_time = wtime1 - wtime0;
    double user_time = utime1 - utime0;
    double sys_time  = stime1 - stime0;
    double cpu_time  = user_time + sys_time;
    
    // Evita división por cero si el tiempo es extremadamente rápido
    double cpu_percent = (wall_time > 0.0) ? (100.0 * cpu_time / wall_time) : 0.0;

    /* -------- Imprimir resultado CSV -------- */
    printf("BurbujaOpt2,%d,%.10f,%.10f,%.10f,%.10f\n",
           n,
           wall_time,
           user_time,
           sys_time,
           cpu_percent);

    free(arregloA);
    return 0;
}

/*
void AlgoritmoBurbujaOpt2(int *arregloA, int n)
Recibe: *arregloA - Puntero al arreglo de enteros.
        n         - Tamaño del arreglo.
Devuelve: void
Función: 
    Ordena el arreglo mediante el método de burbuja optimizado con una bandera 
    de control ('cambios'). Esta variante reduce significativamente el tiempo 
    de ejecución en arreglos que ya están parcial o totalmente ordenados, 
    deteniendo el proceso si en una pasada completa no se realizó ningún intercambio.
*/
void AlgoritmoBurbujaOpt2(int *arregloA, int n){

    int i = 0;
    int j, aux;
    int cambios = TRUE;
    
    while(i<= n-2 && cambios != FALSE){
        cambios = FALSE;

        for(j = 0; j < (n-2)-i; j++){
            if(arregloA[j] < arregloA[j+1]){
                aux = arregloA[j];
                arregloA[j] = arregloA[j+1];
                arregloA[j+1] = aux;
                cambios = TRUE;
            }
        }
        i = i+1;
    }
}