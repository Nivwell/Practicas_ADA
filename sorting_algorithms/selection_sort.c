/*
Implementacion del algortimo de ordenamiento de seleccion
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
Autor original: 
        Edgardo Adri­an Franco Martinez
Fecha de entrega 18 de Marzo del 2025
Version: 2.0

Descripcion: Programa que implementa un algorimto de ordenamiento por seleccion
para un arreglo de numeros enteros de tamanio determinado.

Compilación: gcc selection_sort.c tiempo.c -o selection
Ejecución: ./selection 10000 < numeros10millones.txt >> resultados.csv

*/
#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

void Seleccion(int *A, int n); // Prototipo de funcion. Recibe int * direccion del arreglo A, int n tamanio del arreglo.

/*
int main(int argc, char *argv[])
Recibe: argc   - El número de argumentos pasados por la línea de comandos.
        *argv[] - El vector de argumentos (donde argv[1] es el tamaño 'n').
Devuelve: int (0 si el programa finaliza correctamente).
Función: 
    1. Procesa los parámetros de entrada para definir el tamaño del conjunto de datos.
    2. Reserva un bloque de memoria dinámica para alojar 'n' números enteros.
    3. Llena el arreglo mediante la lectura de valores desde la entrada estándar.
    4. Ejecuta el algoritmo 'Seleccion', evaluando su desempeño mediante 'uswtime'.
    5. Imprime un reporte detallado de tiempos (Real, CPU, E/S) en formato decimal 
    y científico para facilitar el análisis de complejidad.
    6. Libera la memoria utilizada para mantener la integridad de los recursos del sistema.
*/
int main(int argc, char *argv[])
{
    double utime0, stime0, wtime0, utime1, stime1, wtime1;
    int i, n, *A;

    if (argc != 2)
    {
        fprintf(stderr, "\nIndique el tamanio de n. Ejemplo %s 100\n", argv[0]);
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

    /* -------- Lectura desde stdin -------- */
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

    Seleccion(A, n);

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

    /* -------- Imprimir resultado CSV -------- */
    printf("Seleccion,%d,%.10f,%.10f,%.10f,%.10f\n",
           n,
           wall_time,
           user_time,
           sys_time,
           cpu_percent);

    free(A);

    return 0;
}


/*
void Seleccion(int *A, int n);
Descricion: Ordena los elemenotos de un arreglo A de tamanio n.
Recibe: int * direccion de un arreglo A, int n tamanio del arreglo.
Devuelve: void (No retorna valor explicito).
Observaciones: Observaciones: Funcion que ordena los valores del arreglo A de tamanio n
en razon de O(n^2)
*/
void Seleccion(int *A, int n)
{
    // Declaracion de variables
    int k, i, p, temp;
    // Se inicia el bucle que recorre todo el arreglo desde el elemento 0 hasta el elemento n-2
    for (k = 0; k <= (n - 2); k++)
    {
        // En cada iteracion, se asigna a 'p' el valor actual de 'k' (p es el elemento menor relativo a la iteracion)
        p = k;
        // Se inicializa el bucle que recorre todos los elementos desde el valor actual de 'k'+1 hasta 'n-1'
        for (i = k + 1; i <= (n - 1); i++)
        {
            // Verifica si el elemento 'i(k+1)' es menor al anterior 'p(k)'
            if (A[i] < A[p])
            {
                // Se asigna 'p' como el valor como el valor de 'i'
                p = i;
            }
        }
        // Se asigna a la variable temporal el valor de p
        temp = A[p];
        // Se intercambia el valor de k a la posicion de 'p'
        A[p] = A[k];
        // Se asigna a la posicion 'k' el elemento menor relativo a la iteracion
        A[k] = temp;
    }
}