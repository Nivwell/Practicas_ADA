#!/bin/bash

INPUT_FILE="numeros10millones.txt"
OUTPUT_FILE="resultados_algoritmos.csv"

# tamaño solicitado
n=500000

# ejecutables ordenados aproximadamente de más rápido a más lento
algoritmos=(
quick_sort
merge_sort
heap_sort
shell_sort
tree_sort
insertion_sort
selection_sort
bubble_sort02
bubble_sort01
bubble_sort
)

# encabezado CSV
echo "Algoritmo,n,TiempoReal,TiempoCPU,TiempoES,CPUWall" > $OUTPUT_FILE

for alg in "${algoritmos[@]}"
do
    echo "Ejecutando $alg con n=$n"

    result=$(head -n $n $INPUT_FILE | ./$alg $n)

    real=$(echo "$result" | grep "real" | head -1 | awk '{print $4}')
    cpu=$(echo "$result" | grep "user" | head -1 | awk '{print $5}')
    es=$(echo "$result" | grep "sys" | head -1 | awk '{print $5}')
    cpuwall=$(echo "$result" | grep "CPU/Wall" | head -1 | awk '{print $2}')

    echo "$alg,$n,$real,$cpu,$es,$cpuwall" >> $OUTPUT_FILE
done

# ordenar resultados por tiempo real (más rápido primero)
sort -t',' -k3 -n $OUTPUT_FILE -o $OUTPUT_FILE

echo "Pruebas terminadas. Resultados guardados en $OUTPUT_FILE"