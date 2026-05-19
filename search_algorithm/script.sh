#!/bin/bash

# =================================================================
# SCRIPT MAESTRO - PRÁCTICAS DE ADA
# Ejecuta todos los algoritmos de búsqueda secuencial
# =================================================================

# 1. Configuración de parámetros globales
# Apuntamos a la carpeta 'testing' basándonos en tu árbol de directorios
DATOS_SUCIOS="../testing/numeros10millones.txt"
N=10000000

# Lista exacta de tus archivos .c
ALGORITMOS=(
    "busq_lineal.c"
    "busq_binaria.c"
    "busq_abb.c"
    "busq_exponencial.c"
    "busq_fibonacci.c"
)

echo "================================================="
echo "  Iniciando Bateria de Pruebas ADA               "
echo "================================================="

# Ajustar el tamaño del Stack (vital para QuickSort o ABB recursivo con 10M de datos)
ulimit -s unlimited

# Limpiar bitácora CSV anterior si existe en esta carpeta
if [ -f bitacora_tiempos.csv ]; then
    rm bitacora_tiempos.csv
fi

# IMPORTANTE: Los códigos C buscan "objetivos.txt" en el directorio actual.
# Lo copiamos temporalmente desde testing si no existe aquí.
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

    # Compilar enlazando con tiempo.c
    echo "  [+] Compilando..."
    gcc "$ARCHIVO_C" tiempo.c -o "$EJECUTABLE"
    
    if [ $? -ne 0 ]; then
        echo "  [-] ERROR al compilar $ARCHIVO_C. Saltando al siguiente..."
        continue
    fi

    # 3. Ejecución Secuencial
    echo "  [>] Ejecutando $EJECUTABLE..."
    
    # Ejecutamos pasándole únicamente el archivo de datos y la N
    ./"$EJECUTABLE" "$DATOS_SUCIOS" "$N"
    
    echo "  [+] $ARCHIVO_C terminado."
    
done

echo -e "\n================================================="
echo "  ¡Todas las pruebas completadas con exito!      "
echo "  Revisa el archivo 'bitacora_tiempos.csv'       "
echo "================================================="