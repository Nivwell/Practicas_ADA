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
Linux: gcc bubble_sort01.c tiempo.c -o bubble_sort01
Ejecutar como bubble_sort01 _ < numeros10millones.txt > salida.txt

Este programa implementa el algoritmo de ordenamiento Burbuja Optimizado.
*/

#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

void AlgoritmoBurbujaOpt1(int *arregloA, int n);

/*
int main(int num_arg, char *argv[])
Recibe: num_arg - Cantidad de argumentos recibidos por consola.
        *argv[] - Vector de cadenas que contiene los argumentos (ej. el tamaño 'n').
Devuelve: int (0 si el proceso finaliza correctamente).
Función: 
    1. Controla la entrada de parámetros desde la línea de comandos.
    2. Gestiona la memoria dinámica para el almacenamiento del arreglo de datos.
    3. Captura la entrada de datos del usuario o redireccionamiento de archivos.
    4. Implementa el cronometraje de precisión mediante 'uswtime' para evaluar 
    el desempeño del 'AlgoritmoBurbujaOpt1'.
    5. Despliega el arreglo ordenado y las métricas de tiempo (Real, User, Sys).
    6. Libera los recursos de memoria antes de finalizar.
*/
int main(int num_arg, char *argv[])
{
    int i, n;
    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    if (num_arg != 2)
    {
        fprintf(stderr, "Indique el tamaño de datos a ordenar\n");
        exit(1);
    }

    n = atoi(argv[1]);

    int *arregloA = malloc(n * sizeof(int));
    if (arregloA == NULL)
    {
        fprintf(stderr, "Error al intentar reservar memoria para %d elementos\n", n);
        exit(1);
    }

    /* ---------- Leer datos desde stdin ---------- */
    for (i = 0; i < n; i++)
    {
        if (scanf("%d", &arregloA[i]) != 1)
        {
            fprintf(stderr, "Error al leer datos\n");
            free(arregloA);
            exit(1);
        }
    }

    /* ---------- Inicio medición ---------- */
    uswtime(&utime0, &stime0, &wtime0);

    AlgoritmoBurbujaOpt1(arregloA, n);

    /* ---------- Fin medición ---------- */
    uswtime(&utime1, &stime1, &wtime1);

    /* ---------- Imprimir resultado en CSV ---------- */
    printf("Burbuja,%d,%.10f,%.10f,%.10f,%.10f\n",
           n,
           wtime1 - wtime0,
           utime1 - utime0,
           stime1 - stime0,
           100.0 * (utime1 - utime0 + stime1 - stime0) /
           (wtime1 - wtime0));

    free(arregloA);
    return 0;
}


/*
void AlgoritmoBurbujaOpt1(int *arregloA, int n)
Recibe: *arregloA - Puntero al arreglo de enteros a ordenar.
        n         - Cantidad total de elementos en el arreglo.
Devuelve: void
Función: 
    Realiza el ordenamiento de los elementos del arreglo mediante una versión 
    modificada del método de burbuja. Utiliza dos ciclos anidados para comparar 
    y permutar elementos adyacentes si el actual es mayor al siguiente, logrando 
    así que los valores menores se desplacen hacia el inicio del arreglo.
*/
void AlgoritmoBurbujaOpt1(int *arregloA, int n)
{
	int i, j, aux;
	
	for(i=0;i<n-2; i++){
		for(j=0; j<(n-2)-1;j++){
			if(arregloA[j]>arregloA[j+1]){
				aux = arregloA[j];
				arregloA[j] = arregloA[j+1];
				arregloA[j+1] = aux;
			}
		}
	}
}