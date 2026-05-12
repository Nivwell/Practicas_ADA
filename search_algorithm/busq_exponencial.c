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
int busquedaBinariaInterna(int *arreglo, int izquierda, int derecha, int objetivo);
int busquedaExponencial(int *arreglo, int n, int objetivo);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <sucio.txt> <ordenado.txt> <n>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    char *f_ordenado = argv[2];
    int n = atoi(argv[3]);
    int *datos = NULL;

    // 1. LÓGICA DE CACHÉ (Reutilizando tu lógica actual)
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

    // 2. CARGA DE OBJETIVOS (Archivo con comas)
    int n_objetivos = 20;
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);

    // 3. EJECUCIÓN DE BÚSQUEDA EXPONENCIAL
    if (objetivos != NULL) {
        printf("\n--- Resultados de Busqueda Exponencial ---\n");
        for (int i = 0; i < n_objetivos; i++) {
            int resultado = busquedaExponencial(datos, n, objetivos[i]);
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

/* --- ALGORITMO DE BÚSQUEDA EXPONENCIAL --- */

int busquedaExponencial(int *arreglo, int n, int objetivo) {
    // Si el elemento está en la primera posición
    if (arreglo[0] == objetivo) return 0;

    // Encontrar el rango para la búsqueda binaria incrementando i exponencialmente
    int i = 1;
    while (i < n && arreglo[i] <= objetivo) {
        i = i * 2;
    }

    // Aplicar búsqueda binaria en el rango encontrado [i/2, min(i, n-1)]
    int limite_derecho = (i < n) ? i : n - 1;
    return busquedaBinariaInterna(arreglo, i / 2, limite_derecho, objetivo);
}

// Búsqueda binaria adaptada para trabajar con rangos específicos
int busquedaBinariaInterna(int *arreglo, int izquierda, int derecha, int objetivo) {
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        if (arreglo[medio] == objetivo) return medio;
        if (arreglo[medio] < objetivo) izquierda = medio + 1;
        else derecha = medio - 1;
    }
    return -1;
}

/* --- BLOQUE DE FUNCIONES REUTILIZADAS (QuickSort y Carga) --- */

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