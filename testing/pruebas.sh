#!/bin/bash

INPUT_FILE="numeros10millones.txt"
OUTPUT_FILE="datos.csv"

# Tamaños solicitados
sizes=(1000 2000 3000 4000 5000 8000 10000 50000 100000 150000 200000 300000 400000 500000 600000 700000 800000 900000 1000000 1500000 2000000)

# Ejecutables (Asegúrate de que los nombres coincidan con tus archivos compilados)
algoritmos=(
merge_sort

quick_sort

insertion_sort

selection_sort

shell_sort

heap_sort

tree_sort
)

# Encabezado CSV
echo "Algoritmo,n,TiempoReal,TiempoCPU,TiempoES,CPUWall" > $OUTPUT_FILE

for alg in "${algoritmos[@]}"
do
    # Verificar si el ejecutable existe para evitar errores
    if [ ! -f "./$alg" ]; then
        echo "Error: No se encuentra el ejecutable ./$alg"
        continue
    fi

    for n in "${sizes[@]}"
    do
        echo "Ejecutando $alg con n=$n"

        # 1. Tomamos n números del archivo
        # 2. Se los pasamos al programa
        # 3. Guardamos la salida (que ya es una línea CSV) en el archivo final
        head -n $n $INPUT_FILE | ./$alg $n >> $OUTPUT_FILE
    done
done

echo "Pruebas terminadas. Resultados guardados en $OUTPUT_FILE"