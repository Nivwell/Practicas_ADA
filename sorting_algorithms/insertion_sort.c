/*
Algoritmo de ordenamiento insertion_sort.
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
Autor original: 
        Edgardo Adri­an Franco Martinez
Fecha de entrega 18 de Marzo del 2025
Version: 2.0

Este programa implementa el algoritmo de ordenamiento insertion_sort.

Compilacion: gcc insertion_sort.c tiempo.c -o insertion_sort
Ejecucion: ./insertion_sort 1000 < numeros10millones.txt >> resultados.csv

*/

#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

#define TRUE 1
#define FALSE 0

void Insercion(int *A, int n);

/*
int main(int num_arg, char *argv[])
Recibe: num_arg - Cantidad de argumentos pasados desde la terminal.
        *argv[] - Vector de cadenas que contiene los argumentos (espera el tamaño 'n').
Devuelve: int (0 al finalizar la ejecución).
Función: 
    1. Valida que el usuario proporcione el tamaño del arreglo como parámetro.
    2. Reserva memoria dinámica para un arreglo de enteros de tamaño 'n'.
    3. Lee los elementos de la entrada estándar y los almacena en el arreglo 'A'.
    4. Ejecuta el algoritmo de 'Insercion' (Insertion Sort) cronometrando el proceso 
    con la función 'uswtime'.
    5. Despliega el arreglo resultante y un análisis detallado del rendimiento 
    del sistema (tiempo real, tiempo de CPU y tiempo de entrada/salida).
    6. Libera la memoria dinámica para evitar fugas (memory leaks).
*/
int main(int num_arg, char *argv[])
{
    int i, n;
    double utime0, stime0, wtime0, utime1, stime1, wtime1;
    int *A;

    if (num_arg != 2)
    {
        fprintf(stderr, "\nIndique el tamaño de datos a ordenar. Ejemplo: %s 100\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);

    if (n <= 0)
    {
        fprintf(stderr, "\nError: El tamaño del arreglo debe ser un número entero positivo.\n");
        exit(1);
    }

    A = malloc(n * sizeof(int));
    if (A == NULL)
    {
        fprintf(stderr, "\nError al intentar reservar memoria para %d elementos\n", n);
        exit(1);
    }

    for (i = 0; i < n; i++)
    {
        if (scanf("%d", &A[i]) != 1)
        {
            fprintf(stderr, "Error al leer los datos de entrada en la posición %d\n", i);
            free(A);
            exit(1);
        }
    }

    /* -------- Inicio medición -------- */
    uswtime(&utime0, &stime0, &wtime0);

    Insercion(A, n);

    /* -------- Imprimir arreglo ordenado -------- */
    for (i = 0; i < n; i++)
    {
        printf("%d\n", A[i]);
    }

    /* -------- Fin medición -------- */
    uswtime(&utime1, &stime1, &wtime1);

    double wall_time = wtime1 - wtime0;
    double user_time = utime1 - utime0;
    double sys_time  = stime1 - stime0;
    double cpu_time  = user_time + sys_time;
    
    double cpu_percent = (wall_time > 0.0) ? (100.0 * cpu_time / wall_time) : 0.0;

    /* -------- Imprimir resultado en formato CSV -------- */
    printf("InsertionSort,%d,%.10f,%.10f,%.10f,%.10f\n",
           n,
           wall_time,
           user_time,
           sys_time,
           cpu_percent);

    free(A);

    return 0;
}

/*
void Insercion(int *A, int n)
Recibe: *A - Puntero al arreglo de enteros que se desea ordenar.
        n  - Número total de elementos presentes en el arreglo.
Devuelve: void
Función: 
    Ordena los elementos del arreglo de forma ascendente mediante el método 
    de inserción directa. El algoritmo divide virtualmente el arreglo en una 
    parte ordenada y otra desordenada; en cada paso, toma un elemento de la 
    parte desordenada y lo "inserta" en la posición correcta dentro de la 
    sección ordenada, desplazando los elementos mayores hacia la derecha.
*/
void Insercion(int *A, int n)
{
    int i, j, temp;

    for(i = 0; i < n-1; i++){
        j = i;
        temp = A[i];
        while(j>0 && temp < A[j-1]){
            A[j] = A[j-1];
            j--;
        } 
        A[j] = temp;
    }
}