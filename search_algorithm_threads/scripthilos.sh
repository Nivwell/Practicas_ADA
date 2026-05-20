#!/bin/bash

# =================================================================
# SCRIPT MAESTRO - PRÁCTICAS DE ADA
# Ejecuta algoritmos de búsqueda paralela (Pthreads) con Múltiples N
# =================================================================

# 1. Configuración de parámetros globales
DATOS_SUCIOS="../testing/numeros10millones.txt"

# ARREGLO DE TAMAÑOS: Las diferentes N para tus gráficas
TAMANOS=(1000 5000 10000 50000 100000 500000 1000000 5000000 10000000)

# Lista exacta de tus archivos .c
ALGORITMOS=(
    "busq_lineal.c"
    "busq_binaria.c"
    "busq_abb.c"
    "busq_exponencial.c"
    "busq_fibonacci.c"
)

echo "================================================="
echo "  Iniciando Bateria de Pruebas Paralelas ADA     "
echo "================================================="

# Ajustar el tamaño del Stack (vital para el ABB y ordenamientos)
ulimit -s unlimited

# Limpiar bitácora CSV anterior si existe
if [ -f bitacora_tiempos.csv ]; then
    rm bitacora_tiempos.csv
fi

# Verificar dependencias
if [ ! -f objetivos.txt ]; then
    echo "  [i] Copiando objetivos.txt desde /testing..."
    cp ../testing/objetivos.txt .
fi

# 2. Bucle principal: Recorrer cada algoritmo
for ARCHIVO_C in "${ALGORITMOS[@]}"
do
    echo -e "\n================================================="
    echo "[*] PROCESANDO ALGORITMO: $ARCHIVO_C"
    echo "================================================="
    
    # Quitarle la extensión .c para nombrar el ejecutable
    EJECUTABLE="${ARCHIVO_C%.c}"

    # Compilar enlazando con la librería de tiempos y PTHREADS
    echo "  [+] Compilando con -lpthread..."
    gcc "$ARCHIVO_C" tiempo.c -lpthread -o "$EJECUTABLE"
    
    if [ $? -ne 0 ]; then
        echo "  [-] ERROR al compilar $ARCHIVO_C. Saltando al siguiente..."
        continue
    fi

    # 3. Bucle doble: Por tamaño N y luego por cantidad de hilos
    echo "  [>] Iniciando ejecuciones masivas..."
    for N in "${TAMANOS[@]}"
    do
        echo "      -> Pruebas para N = $N"
        
        for HILOS in 1 2 4 8
        do
            echo "         ~ Ejecutando con $HILOS hilo(s)..."
            
            # Ejecución pasando: Archivo, N y Número de Hilos
            ./"$EJECUTABLE" "$DATOS_SUCIOS" "$N" "$HILOS"
            
        done
    done
    
    echo "  [+] $ARCHIVO_C terminado."
    
done

echo -e "\n================================================="
echo "  ¡Pruebas paralelas completadas con exito!      "
echo "  Revisa el archivo 'bitacora_tiempos.csv'       "
echo "================================================="