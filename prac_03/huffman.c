/*
IMPLEMENTACION DEL ALGORITMO VORAZ DE CODIFICACION DE HUFFMAN
Autores: 
        Garcia Peñalva Saul 
        López Alvarado Daniel
        Olarte Tomas Kevin Saul
        Pérez Ortiz Luis Angel
Autor original de la librería de tiempos: 
        Edgardo Adrián Franco Martinez
Fecha: 24 de Mayo del 2026
Version: 2.0

DESCRIPCION: Este codigo implementa el algoritmo de codificación de Huffman para la compresión 
y descompresión de archivos sin pérdida de información. Utiliza un enfoque voraz (greedy) apoyado 
por una Cola de Prioridad (Min-Heap) para construir un árbol binario óptimo de prefijos de longitud 
variable. El programa opera a nivel de bits para empaquetar la información comprimida de forma 
eficiente, superando las limitaciones de escritura por bytes de C estándar. Adicionalmente, mide 
de forma precisa el tiempo de ejecución (Wall time, CPU User, CPU Sys) y exporta estas estadísticas 
a un archivo de bitácora para su análisis de complejidad asintótica.

OBSERVACIONES: Los archivos resultantes de la compresión son "codificacion.dat" (binario puro) y 
"frecuencias.txt" (metadatos necesarios para reconstruir el árbol). El registro acumulativo de 
tiempos se anexa automáticamente en "estadisticas_huffman.txt".

Compilación: gcc huffman.c tiempo.o -o huffman
Ejecución compresión: ./huffman -c <archivo>
Ejecución descompresión: ./huffman -d <archivo.dat> <archivo_salida>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tiempo.h" 

#define PESOBIT(bpos) (1 << (bpos))
#define CONSULTARBIT(var,bpos) ((*(unsigned*)&(var) & PESOBIT(bpos)) ? 1 : 0)
#define PONE_1(var,bpos) (*(unsigned*)&(var) |= PESOBIT(bpos))
#define PONE_0(var,bpos) (*(unsigned*)&(var) &= ~(PESOBIT(bpos)))
#define CAMBIA(var,bpos) (*(unsigned*)&(var) ^= PESOBIT(bpos))

typedef struct Nodo {
    unsigned char dato;
    unsigned int frecuencia;
    struct Nodo *izq, *der;
} Nodo;

typedef struct MinHeap {
    unsigned int tamano;
    unsigned int capacidad;
    Nodo **arreglo;
} MinHeap;

/*
Nodo* crear_nodo(unsigned char dato, unsigned int frecuencia)
DESCRIPCION: Función que instancia un nuevo nodo en memoria dinámica para el árbol de Huffman.
Recibe: dato - El carácter o byte (0-255) a almacenar en la hoja.
        frecuencia - La cantidad de veces que aparece dicho byte en el archivo.
Devuelve: Un puntero al nuevo Nodo inicializado.
*/
Nodo* crear_nodo(unsigned char dato, unsigned int frecuencia) {
    Nodo* nuevo_nodo = (Nodo*)malloc(sizeof(Nodo));
    nuevo_nodo->dato = dato;
    nuevo_nodo->frecuencia = frecuencia;
    nuevo_nodo->izq = nuevo_nodo->der = NULL;
    return nuevo_nodo;
}

/*
MinHeap* crear_min_heap(unsigned int capacidad)
DESCRIPCION: Función que aloja memoria para la estructura de la Cola de Prioridad (Min-Heap).
Recibe: capacidad - Número máximo de nodos que podrá almacenar el arreglo (típicamente 256).
Devuelve: Un puntero a la nueva estructura MinHeap lista para usarse.
*/
MinHeap* crear_min_heap(unsigned int capacidad) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->tamano = 0;
    minHeap->capacidad = capacidad;
    minHeap->arreglo = (Nodo**)malloc(minHeap->capacidad * sizeof(Nodo*));
    return minHeap;
}

/*
void intercambiar_nodos(Nodo** a, Nodo** b)
DESCRIPCION: Función utilitaria para intercambiar las posiciones de dos punteros a nodo dentro del heap.
Recibe: a, b - Dobles punteros apuntando a las direcciones de memoria de los nodos a intercambiar.
Devuelve: void.
*/
void intercambiar_nodos(Nodo** a, Nodo** b) {
    Nodo* t = *a;
    *a = *b;
    *b = t;
}

/*
void min_heapify(MinHeap* minHeap, int idx)
DESCRIPCION: Función recursiva que mantiene la propiedad matemática del Min-Heap (el nodo padre 
            siempre tiene menor frecuencia que sus hijos) reordenando los nodos de arriba hacia abajo.
Recibe: minHeap - Puntero a la Cola de Prioridad.
        idx - Índice actual desde donde se evaluará y hundirá el nodo si es necesario.
Devuelve: void.
*/
void min_heapify(MinHeap* minHeap, int idx) {
    int mas_pequeno = idx;
    int izq = 2 * idx + 1;
    int der = 2 * idx + 2;

    if (izq < minHeap->tamano && minHeap->arreglo[izq]->frecuencia < minHeap->arreglo[mas_pequeno]->frecuencia)
        mas_pequeno = izq;

    if (der < minHeap->tamano && minHeap->arreglo[der]->frecuencia < minHeap->arreglo[mas_pequeno]->frecuencia)
        mas_pequeno = der;

    if (mas_pequeno != idx) {
        intercambiar_nodos(&minHeap->arreglo[mas_pequeno], &minHeap->arreglo[idx]);
        min_heapify(minHeap, mas_pequeno);
    }
}

/*
Nodo* extraer_minimo(MinHeap* minHeap)
DESCRIPCION: Retira el nodo con la frecuencia más baja (la raíz del heap) en tiempo O(log N).
Recibe: minHeap - Puntero a la Cola de Prioridad activa.
Devuelve: Un puntero al Nodo extraído.
*/
Nodo* extraer_minimo(MinHeap* minHeap) {
    Nodo* temp = minHeap->arreglo[0];
    minHeap->arreglo[0] = minHeap->arreglo[minHeap->tamano - 1];
    --minHeap->tamano;
    min_heapify(minHeap, 0);
    return temp;
}

/*
void insertar_min_heap(MinHeap* minHeap, Nodo* minHeapNodo)
DESCRIPCION: Inserta un nuevo nodo en el heap y flota su posición hacia arriba hasta restaurar 
            la propiedad de orden del montículo.
Recibe: minHeap - Puntero a la Cola de Prioridad activa.
        minHeapNodo - El puntero al Nodo que se desea agregar.
Devuelve: void.
*/
void insertar_min_heap(MinHeap* minHeap, Nodo* minHeapNodo) {
    ++minHeap->tamano;
    int i = minHeap->tamano - 1;
    while (i && minHeapNodo->frecuencia < minHeap->arreglo[(i - 1) / 2]->frecuencia) {
        minHeap->arreglo[i] = minHeap->arreglo[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->arreglo[i] = minHeapNodo;
}

/*
int es_hoja(Nodo* raiz)
DESCRIPCION: Verifica si un nodo no tiene descendencia (es decir, es una hoja que contiene un símbolo).
Recibe: raiz - Puntero al nodo a evaluar.
Devuelve: 1 (verdadero) si es hoja, 0 (falso) en caso contrario.
*/
int es_hoja(Nodo* raiz) {
    return !(raiz->izq) && !(raiz->der);
}

/*
Nodo* construir_arbol_huffman(unsigned char datos[], unsigned int frecuencias[], int tamano)
DESCRIPCION: Ejecuta la parte central del algoritmo voraz. Extrae iterativamente los dos nodos 
            de menor frecuencia del Min-Heap y los fusiona bajo un nuevo nodo padre ('$'), 
            repitiendo el proceso hasta consolidar el árbol óptimo completo.
Recibe: datos[] - Arreglo con los caracteres/bytes únicos encontrados en el archivo.
        frecuencias[] - Arreglo con las frecuencias correspondientes de cada carácter.
        tamano - La cantidad total de símbolos únicos.
Devuelve: Puntero al nodo raíz del árbol de Huffman final.
*/
Nodo* construir_arbol_huffman(unsigned char datos[], unsigned int frecuencias[], int tamano) {
    Nodo *izq, *der, *top;
    MinHeap* minHeap = crear_min_heap(tamano);
    
    for (int i = 0; i < tamano; ++i)
        minHeap->arreglo[i] = crear_nodo(datos[i], frecuencias[i]);
    
    minHeap->tamano = tamano;
    for (int i = (minHeap->tamano - 2) / 2; i >= 0; --i)
        min_heapify(minHeap, i);

    while (minHeap->tamano != 1) {
        izq = extraer_minimo(minHeap);
        der = extraer_minimo(minHeap);
        top = crear_nodo('$', izq->frecuencia + der->frecuencia);
        top->izq = izq;
        top->der = der;
        insertar_min_heap(minHeap, top);
    }
    return extraer_minimo(minHeap);
}

char tabla_codigos[256][256];

/*
void generar_codigos_recursivo(Nodo* raiz, int arreglo[], int tope)
DESCRIPCION: Recorre exhaustivamente el árbol de Huffman. Al descender por la izquierda anota un '0' 
            y por la derecha un '1'. Al llegar a un nodo hoja, guarda la cadena binaria generada 
            en una matriz global para su acceso inmediato en O(1) durante la compresión.
Recibe: raiz - Puntero al nodo actual del recorrido.
        arreglo[] - Arreglo temporal que actúa como buffer acumulando los bits (0s y 1s).
        tope - Nivel actual de profundidad en el árbol.
Devuelve: void.
*/
void generar_codigos_recursivo(Nodo* raiz, int arreglo[], int tope) {
    if (raiz->izq) {
        arreglo[tope] = 0;
        generar_codigos_recursivo(raiz->izq, arreglo, tope + 1);
    }
    if (raiz->der) {
        arreglo[tope] = 1;
        generar_codigos_recursivo(raiz->der, arreglo, tope + 1);
    }
    if (es_hoja(raiz)) {
        for (int i = 0; i < tope; ++i) {
            tabla_codigos[raiz->dato][i] = arreglo[i] ? '1' : '0';
        }
        tabla_codigos[raiz->dato][tope] = '\0';
    }
}

/*
void guardar_reporte(const char* operacion, const char* archivo, long original, long final, double real, double user, double sys, double cpu)
DESCRIPCION: Anexa los resultados de compresión/descompresión y las métricas detalladas 
            de tiempo a un archivo de texto secuencial ("estadisticas_huffman.txt").
Recibe: operacion - Cadena indicando el proceso ("COMPRESIÓN" o "DESCOMPRESIÓN").
        archivo - Nombre del archivo de destino o fuente.
        original - Tamaño original en bytes (0 para modo descompresión).
        final - Tamaño resultante en bytes.
        real, user, sys - Tiempos obtenidos mediante la librería del Prof. Edgardo Franco.
        cpu - Porcentaje de utilización del procesador.
Devuelve: void.
*/
void guardar_reporte(const char* operacion, const char* archivo, long original, long final, double real, double user, double sys, double cpu) {
    FILE *f_reporte = fopen("estadisticas_huffman.txt", "a");
    if (f_reporte) {
        fprintf(f_reporte, "[%s] Archivo: %s\n", operacion, archivo);
        if (original > 0) {
            fprintf(f_reporte, "  Tamano original: %ld bytes\n", original);
            fprintf(f_reporte, "  Tamano final:    %ld bytes\n", final);
            fprintf(f_reporte, "  Tasa compresion: %.2f%%\n", (1.0 - ((double)final / original)) * 100.0);
        } else {
            fprintf(f_reporte, "  Bytes decodificados: %ld bytes\n", final);
        }
        fprintf(f_reporte, "  Tiempo Real: %.6f s | CPU: %.6f s | E/S: %.6f s | Uso CPU: %.2f%%\n", real, user, sys, cpu);
        fprintf(f_reporte, "--------------------------------------------------\n");
        fclose(f_reporte);
    } else {
        printf("Advertencia: No se pudo escribir en la bitácora estadisticas_huffman.txt\n");
    }
}

/*
void comprimir_archivo(const char* archivo_entrada)
DESCRIPCION: Función coordinadora que engloba el flujo de lectura, conteo de frecuencias, 
            creación del árbol óptimo y escritura de la codificación compactada a nivel de bits.
Recibe: archivo_entrada - Ruta/nombre del archivo a comprimir.
Devuelve: void.
*/
void comprimir_archivo(const char* archivo_entrada) {
    double utime0, stime0, wtime0, utime1, stime1, wtime1;
    uswtime(&utime0, &stime0, &wtime0);

    FILE* in = fopen(archivo_entrada, "rb");
    if (!in) { printf("Error al abrir %s\n", archivo_entrada); return; }

    unsigned int frecuencias_globales[256] = {0};
    unsigned char byte;
    long total_bytes = 0;

    // Conteo lineal O(N) del tamaño del archivo original
    while (fread(&byte, 1, 1, in) == 1) {
        frecuencias_globales[byte]++;
        total_bytes++;
    }

    unsigned char datos_activos[256];
    unsigned int frecs_activas[256];
    int caracteres_unicos = 0;

    for (int i = 0; i < 256; i++) {
        if (frecuencias_globales[i] > 0) {
            datos_activos[caracteres_unicos] = (unsigned char)i;
            frecs_activas[caracteres_unicos] = frecuencias_globales[i];
            caracteres_unicos++;
        }
    }

    Nodo* raiz = construir_arbol_huffman(datos_activos, frecs_activas, caracteres_unicos);
    int arreglo_aux[256], tope = 0;
    memset(tabla_codigos, 0, sizeof(tabla_codigos));
    generar_codigos_recursivo(raiz, arreglo_aux, tope);

    // Exportación de la tabla de reconstrucción
    FILE* f_frecs = fopen("frecuencias.txt", "w");
    fprintf(f_frecs, "%ld\n", total_bytes); 
    for (int i = 0; i < 256; i++) {
        if (frecuencias_globales[i] > 0) {
            fprintf(f_frecs, "%d %u\n", i, frecuencias_globales[i]);
        }
    }
    fclose(f_frecs);

    // Escritura binaria de datos compactados
    FILE* out = fopen("codificacion.dat", "wb");
    rewind(in); 
    
    unsigned char buffer = 0;
    int bit_pos = 7; 
    long bits_totales_comprimidos = 0;

    while (fread(&byte, 1, 1, in) == 1) {
        char* codigo = tabla_codigos[byte];
        for (int i = 0; codigo[i] != '\0'; i++) {
            if (codigo[i] == '1') {
                PONE_1(buffer, bit_pos);
            } else {
                PONE_0(buffer, bit_pos);
            }
            bit_pos--;
            bits_totales_comprimidos++;

            if (bit_pos < 0) {
                fwrite(&buffer, 1, 1, out);
                buffer = 0;
                bit_pos = 7;
            }
        }
    }
    if (bit_pos < 7) {
        fwrite(&buffer, 1, 1, out);
    }

    fclose(in);
    fclose(out);

    uswtime(&utime1, &stime1, &wtime1);
    double real = wtime1 - wtime0;
    double user = utime1 - utime0;
    double sys = stime1 - stime0;
    double porcentaje_cpu = real > 0.0 ? ((user + sys) / real) * 100.0 : 0.0;

    long bytes_comprimidos = (bits_totales_comprimidos / 8) + (bits_totales_comprimidos % 8 != 0 ? 1 : 0);
    
    printf("\n--- REPORTE DE COMPRESIÓN ---\n");
    printf("Tamaño original: %ld bytes\n", total_bytes);
    printf("Tamaño comprimido: %ld bytes\n", bytes_comprimidos);
    printf("\n--- RENDIMIENTO TEMPORAL ---\n");
    printf("Tiempo Real (Wall time): %.6f s\n", real);
    printf("Tiempo de CPU (User):    %.6f s\n", user);
    printf("Tiempo de E/S (System):  %.6f s\n", sys);
    printf("Porcentaje CPU/Wall:     %.2f %%\n", porcentaje_cpu);

    guardar_reporte("COMPRESIÓN", archivo_entrada, total_bytes, bytes_comprimidos, real, user, sys, porcentaje_cpu);
}

/*
void descomprimir_archivo(const char* arch_codificado, const char* arch_frecuencias, const char* arch_salida)
DESCRIPCION: Reconstruye el árbol de Huffman leyendo el archivo de texto de metadatos, y posteriormente 
            navega la estructura bit a bit interpretando el documento binario para recuperar los datos.
Recibe: arch_codificado - Ruta del archivo binario comprimido (.dat).
        arch_frecuencias - Ruta del archivo con la tabla de pesos para reconstruir el árbol.
        arch_salida - Nombre con el que se guardará el documento original restaurado.
Devuelve: void.
*/
void descomprimir_archivo(const char* arch_codificado, const char* arch_frecuencias, const char* arch_salida) {
    double utime0, stime0, wtime0, utime1, stime1, wtime1;
    uswtime(&utime0, &stime0, &wtime0);

    FILE* f_frecs = fopen(arch_frecuencias, "r");
    if (!f_frecs) { printf("Error: No se encuentra frecuencias.txt\n"); return; }

    long total_bytes_originales;
    fscanf(f_frecs, "%ld", &total_bytes_originales);

    unsigned int frecuencias_globales[256] = {0};
    int caracter;
    unsigned int frec;
    while (fscanf(f_frecs, "%d %u", &caracter, &frec) != EOF) {
        frecuencias_globales[caracter] = frec;
    }
    fclose(f_frecs);

    unsigned char datos_activos[256];
    unsigned int frecs_activas[256];
    int caracteres_unicos = 0;

    for (int i = 0; i < 256; i++) {
        if (frecuencias_globales[i] > 0) {
            datos_activos[caracteres_unicos] = (unsigned char)i;
            frecs_activas[caracteres_unicos] = frecuencias_globales[i];
            caracteres_unicos++;
        }
    }
    Nodo* raiz = construir_arbol_huffman(datos_activos, frecs_activas, caracteres_unicos);

    FILE* in = fopen(arch_codificado, "rb");
    FILE* out = fopen(arch_salida, "wb");
    
    Nodo* actual = raiz;
    unsigned char buffer;
    long bytes_decodificados = 0;

    while (fread(&buffer, 1, 1, in) == 1 && bytes_decodificados < total_bytes_originales) {
        for (int i = 7; i >= 0; i--) {
            int bit = CONSULTARBIT(buffer, i);
            
            if (bit == 0) actual = actual->izq;
            else actual = actual->der;

            if (es_hoja(actual)) {
                fwrite(&(actual->dato), 1, 1, out);
                bytes_decodificados++;
                actual = raiz;
                if (bytes_decodificados == total_bytes_originales) break;
            }
        }
    }

    fclose(in);
    fclose(out);

    uswtime(&utime1, &stime1, &wtime1);
    double real = wtime1 - wtime0;
    double user = utime1 - utime0;
    double sys = stime1 - stime0;
    double porcentaje_cpu = real > 0.0 ? ((user + sys) / real) * 100.0 : 0.0;

    printf("\n--- REPORTE DE DESCOMPRESIÓN ---\n");
    printf("Bytes decodificados: %ld bytes\n", bytes_decodificados);
    printf("\n--- RENDIMIENTO TEMPORAL ---\n");
    printf("Tiempo Real (Wall time): %.6f s\n", real);
    printf("Tiempo de CPU (User):    %.6f s\n", user);
    printf("Tiempo de E/S (System):  %.6f s\n", sys);
    printf("Porcentaje CPU/Wall:     %.2f %%\n", porcentaje_cpu);

    guardar_reporte("DESCOMPRESIÓN", arch_codificado, 0, bytes_decodificados, real, user, sys, porcentaje_cpu);
}

/*
int main(int argc, char *argv[])
Recibe: argc - Número de argumentos recibidos (incluyendo el ejecutable).
        *argv[] - Vector de cadenas apuntando a las banderas ingresadas en terminal.
Devuelve: 0 si la ejecución finaliza de manera satisfactoria, 1 si existió error en los argumentos.
Función: 
    1. Valida el número mínimo de argumentos exigidos.
    2. Compara el segundo vector ingresado para activar la función de compresión "-c" o descompresión "-d".
    3. Delega el procesamiento del archivo hacia el sub-módulo apropiado o reporta un error de sintaxis.
*/
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso:\n");
        printf("  Comprimir: %s -c <archivo>\n", argv[0]);
        printf("  Descomprimir: %s -d <archivo.dat> <salida>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        comprimir_archivo(argv[2]);
    } else if (strcmp(argv[1], "-d") == 0 && argc == 4) {
        descomprimir_archivo(argv[2], "frecuencias.txt", argv[3]);
    } else {
        printf("Comando no reconocido.\n");
    }

    return 0;
}