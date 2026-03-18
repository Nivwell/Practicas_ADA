#!/bin/bash

INPUT_FILE="numeros10millones.txt"

# Tamaños solicitados
sizes=(1000 2000 3000 4000 5000 8000 10000 50000 100000 150000 200000 300000 400000 500000 600000 700000 800000 900000 1000000 1500000 2000000)

# Ejecutables
algoritmos=(
heap_sort
merge_sort
quick_sort
tree_sort
)

for alg in "${algoritmos[@]}"
do
    # 1. Definimos un archivo de salida único para cada algoritmo
    OUTPUT_FILE="resultados_${alg}.csv"

    # Verificar si el ejecutable existe
    if [ ! -f "./$alg" ]; then
        echo "Error: No se encuentra el ejecutable ./$alg"
        continue
    fi

    # 2. Creamos el encabezado para este archivo específico
    echo "Algoritmo,n,TiempoReal,TiempoCPU,TiempoES,CPUWall" > "$OUTPUT_FILE"

    echo "Iniciando pruebas para: $alg"

    for n in "${sizes[@]}"
    do
        echo "  -> Ejecutando con n=$n"

        # 3. Tomamos los números y añadimos la salida al archivo correspondiente
        head -n $n $INPUT_FILE | ./$alg $n >> "$OUTPUT_FILE"
    done

    echo "Finalizado: Resultados de $alg guardados en $OUTPUT_FILE"
    echo "-----------------------------------------------"
done

echo "Todas las pruebas han terminado."