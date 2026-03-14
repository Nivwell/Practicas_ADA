/*
Implementacion del algoritmo de ordenamiento por mezcla
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
Autor original: 
        Edgardo Adri­an Franco Martinez
Fecha de entrega 18 de Marzo del 2025
Version: 2.0

Descripcion: Programa que implementa un algortimo de ordanamiento por mezcla
para un arreglo de numeros enteros de un tamanio determinado

INSTRUCCIONES DE COMPILACIÓN Y EJECUCIÓN

Compilación: gcc merge_sort.c tiempo.c -o merge
Ejecución: ./merge 10000 < numeros10millones.txt >> resultados.csv
*/
#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

void MergeSort(int *A, int p, int r);    // Prototipo de funcion. Recibe int * direccion del arreglo A, int p incio del arreglo, int r final del arreglo.
void Merge(int *A, int p, int q, int r); // Protitpo de fncion. Recibe int * direccion del arreglo A, int p inicio del arreglo, int q mitad del arreglo, int r final del arreglo.

/*
int main(int argc, char *argv[])
Recibe: argc   - El número de argumentos pasados por la línea de comandos.
        *argv[] - El vector de argumentos (argv[1] contiene el tamaño 'n').
Devuelve: int (0 si el programa finaliza correctamente).
Función:
    1. Procesa los argumentos de entrada para determinar el tamaño del arreglo 'n'.
    2. Reserva un bloque de memoria dinámica para 'n' números enteros.
    3. Carga los datos desde la entrada estándar (stdin) hacia el arreglo 'A'.
    4. Implementa el cronometraje de alta precisión mediante la función 'uswtime'
    para medir el desempeño del algoritmo MergeSort (recursivo).
    5. Calcula y despliega estadísticas de rendimiento: tiempo Real, de Usuario (CPU)
    y de Sistema (E/S), presentándolos en formato decimal y exponencial.
    6. Realiza la limpieza de recursos mediante la liberación de memoria (free).
*/
int main(int argc, char *argv[])
{
    double utime0, stime0, wtime0, utime1, stime1, wtime1;
    int i, p, n, *A;

    p = 0;

    if (argc != 2)
    {
        fprintf(stderr, "\nIndique el tamanio de n. Ejemplo %s 100\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);

    A = malloc(n * sizeof(int));
    if (A == NULL)
    {
        fprintf(stderr, "\nError al intentar reservar memoria para %d elementos\n", n);
        exit(1);
    }

    /* -------- Lectura segura desde stdin -------- */
    for (i = 0; i < n; i++)
    {
        if (scanf("%d", &A[i]) != 1)
        {
            fprintf(stderr, "Error al leer datos en la posición %d\n", i);
            free(A);
            exit(1);
        }
    }

    /* -------- Inicio medición -------- */
    uswtime(&utime0, &stime0, &wtime0);

    MergeSort(A, p, n - 1);

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
    printf("MergeSort,%d,%.10f,%.10f,%.10f,%.10f\n",
           n,
           wall_time,
           user_time,
           sys_time,
           cpu_percent);

    free(A);

    return 0;
}

/*
void MergeSort(int *A, int p, int r);
Descripcion: Ordena los elementos de un arreglo de tamanio n.
Recibe: int * direccion de un arreglo A, int p indice incial del arreglo, int r indice final del arreglo
Devuelve: void (No retorna valor explicito).
Observaciones: Observaciones: Funcion que ordena los valores del arreglo A de tamanio n
en razon de O(nlog(n))
*/
void MergeSort(int *A, int p, int r)
{
    int q;
    // Verifica que el índice inicial sea menor que el final (caso base de la recursión)
    if (p < r)
    {
        // Calcula el punto medio del subarreglo
        q = (p + r) / 2;
        // Llamadas recursivas para ordenar la primera mitad del arreglo
        MergeSort(A, p, q);
        // Llamadas recursivas para ordenar la segunda mitad del arreglo
        MergeSort(A, q + 1, r);
        // Combina ambas mitades ordenadas en un solo subarreglo ordenado
        Merge(A, p, q, r);
    }
}
/*
void Merge(int *A, int p, int q, int r);
Descripcion: Ordena los elementos de un arreglo de tamanio n.
Recibe: int * direccion de un arreglo A, int p indice incial del arreglo, int r indice final del arreglo, int q indice de mitad del arreglo
Devuelve: void (No retorna valor explicito).
Observaciones: Observaciones: Funcion que ordena los valores del arreglo A de tamanio n
en razon de O(nlog(n))
*/
void Merge(int *A, int p, int q, int r)
{
    // Declaración de variables
    int l, i, j, k, *C;
    // Calcula el tamaño del subarreglo temporal
    l = r - p + 1;
    // Inicializa índices para recorrer las dos mitades del arreglo
    i = p;
    j = q + 1;
    // Reserva memoria para el arreglo auxiliar C
    C = malloc(l * sizeof(int));
    if (C == NULL)
    {
        printf("Error al intentar reservar memoria para %d elementos. ", l);
        exit(1);
    }
    // Fusión de los dos subarreglos en C
    for (k = 0; k < l; k++)
    {
        if (i <= q && j <= r)
        {
            if (A[i] < A[j])
            {
                C[k] = A[i];
                i++;
            }
            else
            {
                C[k] = A[j];
                j++;
            }
        }
        else if (i <= q)
        {
            C[k] = A[i];
            i++;
        }
        else
        {
            C[k] = A[j];
            j++;
        }
    }
    // Copia los elementos ordenados de C de vuelta al arreglo original A

    k = p;
    for (i = 0; i < l; i++)
    {
        A[k] = C[i];
        k++;
    }
    // Libera la memoria reservada para C
    free(C);
}