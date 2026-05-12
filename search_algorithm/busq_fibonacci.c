#include <stdio.h>
#include <stdlib.h>

/* --- PROTOTIPOS --- */
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void ejecutarOrdenamiento(int *arr, int n, char *nombreSalida);
void QuickSort(int *arr, int p, int r);
int Pivot(int *arr, int p, int r);
void Intercambiar(int *arr, int i, int j);
void guardarArchivo(char *nombre, int *arr, int n);

// Funciones de búsqueda
int min(int x, int y);
int busquedaFibonacci(int *arreglo, int n, int objetivo);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <sucio.txt> <ordenado.txt> <n>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    char *f_ordenado = argv[2];
    int n = atoi(argv[3]);
    int *datos = NULL;

    // 1. LÓGICA DE CACHÉ
    FILE *test = fopen(f_ordenado, "r");
    if (test != NULL) {
        fclose(test);
        printf("¡Caché encontrado! Cargando datos ordenados...\n");
        datos = cargarNumeros(f_ordenado, n, 0);
    } else {
        printf("Caché no encontrado. Cargando y ordenando...\n");
        datos = cargarNumeros(f_sucio, n, 0);
        if (datos != NULL) {
            ejecutarOrdenamiento(datos, n, f_ordenado);
        }
    }

    if (datos == NULL) return 1;

    // 2. CARGA DE OBJETIVOS
    int n_objetivos = 20;
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);

    // 3. EJECUCIÓN DE BÚSQUEDA DE FIBONACCI
    if (objetivos != NULL) {
        printf("\n--- Resultados de Busqueda Fibonacci ---\n");
        for (int i = 0; i < n_objetivos; i++) {
            int resultado = busquedaFibonacci(datos, n, objetivos[i]);
            if (resultado != -1) {
                printf("[OK] %d encontrado en el indice %d\n", objetivos[i], resultado);
            } else {
                printf("[X]  %d no encontrado\n", objetivos[i]);
            }
        }
        free(objetivos);
    }

    free(datos);
    return 0;
}

/* --- ALGORITMO DE BÚSQUEDA DE FIBONACCI --- */

int min(int x, int y) { return (x <= y) ? x : y; }

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
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}

void guardarArchivo(char *nombre, int *arr, int n) {
    FILE *f = fopen(nombre, "w");
    if (f == NULL) return;
    for (int i = 0; i < n; i++) fprintf(f, "%d\n", arr[i]);
    fclose(f);
}