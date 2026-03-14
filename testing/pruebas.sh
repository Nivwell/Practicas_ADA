#!/bin/bash

INPUT_FILE="numeros10millones.txt"
OUTPUT_FILE="datos.csv"

# tamaños solicitados
sizes=(1000 2000 3000 4000 5000 8000 10000 50000 100000 150000 200000 300000 400000 500000 600000 700000 800000 900000 1000000 1500000 2000000)

# ejecutables
algoritmos=(
bubble_sort
bubble_sort01
bubble_sort02
insertion_sort
selection_sort
shell_sort
heap_sort
merge_sort
quick_sort
tree_sort
)

# encabezado CSV
echo "Algoritmo,n,TiempoReal,TiempoCPU,TiempoES,CPUWall" > $OUTPUT_FILE

for alg in "${algoritmos[@]}"
do
    for n in "${sizes[@]}"
    do
        echo "Ejecutando $alg con n=$n"

        result=$(head -n $n $INPUT_FILE | ./$alg $n)

        real=$(echo "$result" | grep "real" | awk '{print $4}')
        cpu=$(echo "$result" | grep "user" | awk '{print $5}')
        es=$(echo "$result" | grep "sys" | awk '{print $5}')
        cpuwall=$(echo "$result" | grep "CPU/Wall" | awk '{print $2}')

        echo "$alg,$n,$real,$cpu,$es,$cpuwall" >> $OUTPUT_FILE
    done
done

echo "Pruebas terminadas. Resultados guardados en $OUTPUT_FILE"