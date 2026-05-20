/*
IMPLEMENTACION DEL ALGORITMO DE BUSQUEDA DE FIBONACCI SECUENCIAL
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
        Pérez Ortiz Luis Angel
Autor original de la librería de tiempos: 
        Edgardo Adrián Franco Martinez
Fecha de entrega 20 de Mayo del 2026
Version: 2.0

DESCRIPCION: Este codigo ordena un arreglo de n elementos utilizando el algoritmo QuickSort y 
posteriormente implementa el algoritmo de Búsqueda de Fibonacci para encontrar elementos específicos. 
La búsqueda de Fibonacci es una técnica de búsqueda en arreglos ordenados que utiliza los números de 
Fibonacci para calcular los índices de partición, en lugar de dividir el arreglo a la mitad como 
lo hace la búsqueda binaria. Esto puede ser ventajoso en ciertas arquitecturas de hardware ya que 
solo requiere sumas y restas, evitando las operaciones de división. El programa mide de forma aislada 
el tiempo de búsqueda para múltiples objetivos y exporta los resultados a un archivo CSV.

OBSERVACIONES: La cantidad de elementos 'n' debe ser un número entero positivo y coincidir 
con el tamaño del archivo de entrada. El archivo "objetivos.txt" debe existir en el mismo directorio.

Compilación: gcc busq_fibonacci.c tiempo.c -o busqueda_fibonacci
Ejecución: ./busqueda_fibonacci <numeros10millones.txt> <n>
*/

#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h" // Se agrega la librería de medición

/* --- PROTOTIPOS --- */
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void QuickSort(int *arr, int p, int r);
int Pivot(int *arr, int p, int r);
void Intercambiar(int *arr, int i, int j);

// Funciones de búsqueda
int min(int x, int y);
int busquedaFibonacci(int *arreglo, int n, int objetivo);

/*
int main(int argc, char *argv[])
Recibe: argc - Número de argumentos recibidos (incluyendo el nombre del programa).
        *argv[] - Vector de cadenas (argv[1] es el archivo de datos desordenados, argv[2] es la cantidad 'n').
Devuelve: int (0 si el programa finaliza correctamente, 1 en caso de error).
Función: 
    1. Valida los argumentos de entrada desde la terminal.
    2. Gestiona la carga de 'n' datos desordenados desde el archivo a la memoria dinámica.
    3. Ordena los datos en memoria utilizando QuickSort y reporta este tiempo en terminal.
    4. Carga un conjunto de objetivos a buscar desde "objetivos.txt".
    5. Implementa el cronometraje con 'uswtime' aislando exclusivamente la función de Búsqueda de Fibonacci.
    6. Exporta fila por fila los resultados (tiempo Real, User, Sys y estado de éxito) a un CSV.
    7. Limpia la memoria dinámica antes de finalizar el proceso.
*/
int main(int argc, char *argv[]) {
    // Solo pedimos el archivo sucio y la N
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

    // 4. EJECUCIÓN DE BÚSQUEDA DE FIBONACCI Y REGISTRO EN CSV
    printf("\n[3/4] Ejecutando Busqueda Fibonacci individual...\n");
    
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
        int posicion = busquedaFibonacci(datos, n, buscado);
        uswtime(&utime1, &stime1, &wtime1);

        int encontrado = (posicion != -1) ? 1 : 0;

        // Imprimir en consola para validación visual
        if (encontrado) {
            printf("  - Objetivo [%d]: Encontrado en el indice %d\n", buscado, posicion);
        } else {
            printf("  - Objetivo [%d]: No encontrado\n", buscado);
        }

        // ESCRITURA FORZADA FILA POR FILA EN EL CSV
        fprintf(f_csv, "Busqueda Fibonacci Secuencial,%d,%d,%d,%d,1,%.10f,%.10f,%.10f\n", 
                n, 
                buscado, 
                encontrado, 
                posicion, 
                wtime1 - wtime0, 
                utime1 - utime0, 
                stime1 - stime0);
        
        fflush(f_csv);
    }

    fclose(f_csv);
    printf("\n[4/4] Resultados guardados exitosamente en 'bitacora_tiempos.csv'\n");

    free(objetivos);
    free(datos);
    return 0;
}

/* --- ALGORITMO DE BÚSQUEDA DE FIBONACCI --- */

/*
int min(int x, int y)
DESCRIPCION: Función auxiliar simple que compara dos números enteros.
Recibe: x - Primer número entero.
        y - Segundo número entero.
Devuelve: El número que sea menor de los dos.
*/
int min(int x, int y) { return (x <= y) ? x : y; }

/*
int busquedaFibonacci(int *arreglo, int n, int objetivo)
DESCRIPCION: Función que implementa el algoritmo de búsqueda basada en la secuencia de Fibonacci.
Recibe: arreglo - Puntero al arreglo de enteros previamente ordenado.
        n - Número total de elementos en el arreglo.
        objetivo - El número entero que se desea localizar.
Devuelve: Un entero que representa el índice donde se encontró el objetivo, o -1 si no existe en el arreglo.
*/
int busquedaFibonacci(int *arreglo, int n, int objetivo) {
    // Inicializar números de Fibonacci
    int fibM2 = 0;          // (m-2)-ésimo número de Fibonacci
    int fibM1 = 1;          // (m-1)-ésimo número de Fibonacci
    int fibM = fibM2 + fibM1; // m-ésimo número de Fibonacci

    // fibM almacenará el número de Fibonacci más pequeño mayor o igual a n
    while (fibM < n) {
        fibM2 = fibM1;
        fibM1 = fibM;
        fibM = fibM2 + fibM1;
    }

    // Marca el rango eliminado desde el principio
    int offset = -1;

    while (fibM > 1) {
        // Verificar si fibM2 es una posición válida
        int i = min(offset + fibM2, n - 1);

        // Si el objetivo es mayor que el valor en el índice i, corta el arreglo desde offset hasta i
        if (arreglo[i] < objetivo) {
            fibM = fibM1;
            fibM1 = fibM2;
            fibM2 = fibM - fibM1;
            offset = i;
        }
        // Si el objetivo es menor, corta el arreglo después de i+1
        else if (arreglo[i] > objetivo) {
            fibM = fibM2;
            fibM1 = fibM1 - fibM2;
            fibM2 = fibM - fibM1;
        }
        // Elemento encontrado
        else return i;
    }

    // Comparar el último elemento con el objetivo
    if (fibM1 && arreglo[offset + 1] == objetivo) return offset + 1;

    return -1;
}

/* --- BLOQUE DE FUNCIONES REUTILIZADAS --- */

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
    if (f == NULL) return NULL;
    int *arreglo = (int *)malloc(cantidad * sizeof(int));
    for (int i = 0; i < cantidad; i++) {
        if (conComas) fscanf(f, " %d ,", &arreglo[i]);
        else fscanf(f, "%d", &arreglo[i]);
    }
    fclose(f);
    return arreglo;
}

/*
void QuickSort(int *arr, int p, int r)
DESCRIPCION: Función que divide el arreglo en subarreglos más pequeños alrededor de un pivote y 
luego ordena esos subarreglos de manera recursiva hasta que todo el arreglo está ordenado de menor a mayor.
Recibe: arr - Puntero al arreglo de enteros a ordenar.
        p - Índice inicial (límite inferior) del subarreglo.
        r - Índice final (límite superior) del subarreglo.
Devuelve: void (No retorna valor explicito).
*/
void QuickSort(int *arr, int p, int r) {
    if (p < r) {
        int j = Pivot(arr, p, r);
        QuickSort(arr, p, j - 1);
        QuickSort(arr, j + 1, r);
    }
}

/*
int Pivot(int *arr, int p, int r)
DESCRIPCION: Función auxiliar de QuickSort que selecciona un pivote y particiona el arreglo, 
colocando los elementos menores a la izquierda del pivote y los mayores a la derecha.
Recibe: arr - Puntero al arreglo de enteros.
        p - Índice inicial para iniciar la evaluación.
        r - Índice final para la evaluación.
Devuelve: Un entero que representa el índice final de partición donde fue ubicado el pivote.
*/
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

/*
void Intercambiar(int *arr, int i, int j)
DESCRIPCION: Función utilitaria para realizar el intercambio (swap) de dos elementos dentro del arreglo.
Recibe: arr - Puntero al arreglo de enteros.
        i - Índice de la posición del primer elemento.
        j - Índice de la posición del segundo elemento.
Devuelve: void (No retorna valor explicito).
*/
void Intercambiar(int *arr, int i, int j) {
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}