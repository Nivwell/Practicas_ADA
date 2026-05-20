#!/bin/bash

# =================================================================
# SCRIPT MAESTRO - PRÁCTICAS DE ADA
# Ejecuta todos los algoritmos de búsqueda secuencial (Múltiples N)
# VERSIÓN: Solo Ejecución (Sin Compilación)
# =================================================================

# 1. Configuración de parámetros globales
DATOS_SUCIOS="../testing/numeros10millones.txt"

# ARREGLO DE TAMAÑOS: Aquí defines todas las N que quieres graficar
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
echo "  Iniciando Bateria de Pruebas ADA (Varias N)    "
echo "================================================="

# Ajustar el tamaño del Stack (vital para QuickSort o ABB recursivo con 10M de datos)
ulimit -s unlimited

# Limpiar bitácora CSV anterior si existe en esta carpeta
if [ -f bitacora_tiempos.csv ]; then
    rm bitacora_tiempos.csv
fi

# IMPORTANTE: Los códigos C buscan "objetivos.txt" en el directorio actual.
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

    # 3. Ejecución Secuencial con bucle de tamaños
    echo "  [>] Iniciando ejecuciones..."
    
    for N in "${TAMANOS[@]}"
    do
        echo "      -> Corriendo con N = $N"
        # Ejecutamos pasándole el archivo de datos y la N actual del ciclo
        ./"$EJECUTABLE" "$DATOS_SUCIOS" "$N"
    done
    
    echo "  [+] $EJECUTABLE terminado."
    
done

echo -e "\n================================================="
echo "  ¡Todas las pruebas completadas con exito!      "
echo "  Revisa el archivo 'bitacora_tiempos.csv'       "
echo "================================================="