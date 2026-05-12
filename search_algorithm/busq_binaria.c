#include <stdio.h>
#include <stdlib.h>

/* --- PROTOTIPOS --- */
int* cargarNumeros(char *nombre, int cantidad, int conComas);
void ejecutarOrdenamiento(int *arr, int n, char *nombreSalida);
void QuickSort(int *arr, int p, int r);
int Pivot(int *arr, int p, int r);
void Intercambiar(int *arr, int i, int j);
void guardarArchivo(char *nombre, int *arr, int n);
int busquedaBinaria(int *arreglo, int n, int objetivo);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <sucio.txt> <ordenado.txt> <n>\n", argv[0]);
        return 1;
    }

    char *f_sucio = argv[1];
    char *f_ordenado = argv[2];
    int n = atoi(argv[3]);
    int *datos = NULL;

    // 1. LÓGICA DE CACHÉ: Reutilizar archivo si ya está ordenado
    FILE *test = fopen(f_ordenado, "r");
    if (test != NULL) {
        fclose(test);
        printf("¡Caché encontrado! Cargando datos ordenados...\n");
        datos = cargarNumeros(f_ordenado, n, 0);
    } else {
        printf("Caché no encontrado. Cargando archivo original...\n");
        datos = cargarNumeros(f_sucio, n, 0);
        if (datos != NULL) {
            // 2. ORDENAMIENTO: Necesario para la búsqueda binaria
            ejecutarOrdenamiento(datos, n, f_ordenado);
        }
    }

    if (datos == NULL) return 1;

    // 3. CARGA DE OBJETIVOS (Archivo con comas)
    int n_objetivos = 20;
    int *objetivos = cargarNumeros("objetivos.txt", n_objetivos, 1);

    // 4. EJECUCIÓN DE BÚSQUEDA BINARIA
    if (objetivos != NULL) {
        printf("\n--- Resultados de Busqueda Binaria ---\n");
        for (int i = 0; i < n_objetivos; i++) {
            int resultado = busquedaBinaria(datos, n, objetivos[i]);
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

/* --- FUNCIÓN DE BÚSQUEDA BINARIA --- */
int busquedaBinaria(int *arreglo, int n, int objetivo) {
    int izquierda = 0;
    int derecha = n - 1;

    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        if (arreglo[medio] == objetivo)
            return medio; // Encontrado

        if (arreglo[medio] < objetivo)
            izquierda = medio + 1; // Buscar en la mitad derecha
        else
            derecha = medio - 1; // Buscar en la mitad izquierda
    }
    return -1; // No encontrado
}

/* --- BLOQUE DE FUNCIONES REUTILIZADAS --- */

int* cargarNumeros(char *nombre, int cantidad, int conComas) {
    FILE *f = fopen(nombre, "r");
    if (f == NULL) return NULL;
    int *arreglo = (int *)malloc(cantidad * sizeof(int));
    for (int i = 0; i < cantidad; i++) {
        if (conComas) fscanf(f, " %d ,", &arreglo[i]); // Formato para objetivos
        else fscanf(f, "%d", &arreglo[i]);
    }
    fclose(f);
    return arreglo;
}

void ejecutarOrdenamiento(int *arr, int n, char *nombreSalida) {
    printf("Ordenando con QuickSort para busqueda binaria...\n");
    QuickSort(arr, 0, n - 1); //
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
    FILE *f = fopen(nombre, "w"); // Sobrescribe el caché
    if (f == NULL) return;
    for (int i = 0; i < n; i++) fprintf(f, "%d\n", arr[i]);
    fclose(f);
}