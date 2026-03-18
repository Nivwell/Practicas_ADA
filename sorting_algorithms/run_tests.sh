#!/bin/bash

# Tamaño de prueba
N=$1

if [ -z "$N" ]; then
    echo "Uso: ./run_tests.sh <n>"
    exit 1
fi

INPUT="numeros10millones.txt"

echo "Compilando algoritmos..."

gcc bubble_sort.c tiempo.c -o bubble_sort
gcc bubble_sort01.c tiempo.c -o bubble_sort01
gcc bubble_sort02.c tiempo.c -o bubble_sort02
gcc heap_sort.c tiempo.c -o heap_sort
gcc insertion_sort.c tiempo.c -o insertion_sort
gcc merge_sort.c tiempo.c -o merge_sort
gcc quick_sort.c tiempo.c -o quick_sort
gcc selection_sort.c tiempo.c -o selection_sort
gcc shell_sort.c tiempo.c -o shell_sort
gcc tree_sort.c TADArbol_Bin.c tiempo.c -o tree_sort

echo "Compilación terminada"

echo "Ejecutando pruebas con n=$N"

mkdir -p resultados

algoritmos=(
bubble_sort
bubble_sort01
bubble_sort02
heap_sort
insertion_sort
merge_sort
quick_sort
selection_sort
shell_sort
tree_sort
)

for alg in "${algoritmos[@]}"
do
    echo "Ejecutando $alg..."

    ./$alg $N < $INPUT

    mv salida.txt resultados/salida_$alg.txt
    cp resultados.csv resultados/resultados_$alg.csv
    rm resultados.csv
done

echo "Pruebas terminadas"