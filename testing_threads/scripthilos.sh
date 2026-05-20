#!/bin/bash

# =================================================================
# SCRIPT MAESTRO - PRÁCTICAS DE ADA
# Ejecuta algoritmos de búsqueda paralela (Pthreads) con Múltiples N
# VERSIÓN: Solo Ejecución (Sin Compilación) y Bitácora Renombrada
# =================================================================

# 1. Configuración de parámetros globales
DATOS_SUCIOS="../testing/numeros10millones.txt"

# ARREGLO DE TAMAÑOS: Las diferentes N para tus gráficas
TAMANOS=(1000 5000 10000 50000 100000 500000 1000000 5000000 10000000)

# Lista exacta de tus EJECUTABLES (ya sin la extensión .c)
EJECUTABLES=(
    "busq_lineal"
    "busq_binaria"
    "busq_abb"
    "busq_exponencial"
    "busq_fibonacci"
)

echo "================================================="
echo "  Iniciando Bateria de Pruebas Paralelas ADA     "
echo "================================================="

# Ajustar el tamaño del Stack (vital para el ABB y ordenamientos)
ulimit -s unlimited

# Limpiar bitácoras anteriores si existen para evitar mezclar datos viejos
rm -f bitacora_tiempos.csv bitacora_tiempos_threads.csv

# Verificar dependencias
if [ ! -f objetivos.txt ]; then
    echo "  [i] Copiando objetivos.txt desde /testing..."
    cp ../testing/objetivos.txt .
fi

# 2. Bucle principal: Recorrer cada ejecutable
for EJECUTABLE in "${EJECUTABLES[@]}"
do
    echo -e "\n================================================="
    echo "[*] PROCESANDO ALGORITMO: $EJECUTABLE"
    echo "================================================="
    
    # Validar si el archivo existe y es ejecutable
    if [ ! -x "./$EJECUTABLE" ]; then
        echo "  [-] ADVERTENCIA: No se encontro el ejecutable './$EJECUTABLE' o no tiene permisos."
        echo "      Saltando al siguiente..."
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
    
    echo "  [+] $EJECUTABLE terminado."
    
done

# 4. Renombrar la bitácora automáticamente al finalizar
if [ -f bitacora_tiempos.csv ]; then
    mv bitacora_tiempos.csv bitacora_tiempos_threads.csv
    echo -e "\n  [i] Archivo renombrado a 'bitacora_tiempos_threads.csv'"
fi

echo -e "\n================================================="
echo "  ¡Pruebas paralelas completadas con exito!      "
echo "  Revisa el archivo 'bitacora_tiempos_threads.csv'"
echo "================================================="