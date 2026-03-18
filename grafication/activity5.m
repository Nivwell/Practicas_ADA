% ====================================================================
% SCRIPT DE COMPARATIVA FINAL DE ALGORITMOS (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega: 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script genera la gráfica de comparativa global (Punto 5).
% Busca automáticamente el tamaño máximo de 'n' evaluado en las pruebas,
% extrae el Tiempo Real de cada algoritmo para ese tamaño y genera una 
% gráfica de barras ordenada de menor a mayor tiempo (del más rápido al 
% más lento). Finalmente, exporta la gráfica como 'comparativa_algoritmos.png'.
%
% OBSERVACIONES: El archivo "datos_resultados.csv" debe encontrarse en 
% el mismo directorio que este script.
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Abrir MATLAB o GNU Octave.
% 2. Navegar hasta la carpeta donde se encuentra este script.
% 3. Ejecutar el script para generar y guardar la imagen automáticamente.
% ====================================================================

% Limpieza inicial del entorno de trabajo
clc
clear
close all

% ================================
% 1. Cargar datos
% ================================
% Lee la tabla completa generada por las pruebas en C
datos = readtable('datos_resultados.csv');

% Extrae la lista única de algoritmos evaluados
algoritmos = unique(datos.Algoritmo);

% ================================
% 2. Generación de Gráfica (PUNTO 5 - COMPARATIVA FINAL)
% ================================

% Encuentra el tamaño máximo de 'n' que se probó en el experimento
max_n = max(datos.n);

% Filtra los datos para quedarse ÚNICAMENTE con las filas del tamaño máximo
idx = datos.n == max_n;

% Extrae los nombres de los algoritmos y sus tiempos reales para ese 'n' máximo
alg = datos.Algoritmo(idx);
tiempo = datos.TiempoReal(idx);

% 🔹 ORDENAR DE MENOR A MAYOR TIEMPO (CRÍTICO PARA VISUALIZACIÓN)
% Ordena los tiempos del más rápido al más lento y ajusta los nombres
[tiempo, orden] = sort(tiempo);
alg = alg(orden);

% Crear la ventana de la figura
figure

% Trazar la gráfica de barras. 
% Se usa 'categorical' para que MATLAB respete el orden que definimos 
% arriba y no los reordene alfabéticamente por defecto.
bar(categorical(alg, alg), tiempo)

% Personalización estética (Títulos y etiquetas)
% Se incluye dinámicamente el valor de max_n en el título para mayor claridad
title(['Comparativa de algoritmos (Tiempo real para n = ', num2str(max_n), ')'], 'FontSize', 14)
xlabel('Algoritmo', 'FontSize', 12)
ylabel('Tiempo (segundos)', 'FontSize', 12)

grid on
set(gca, 'FontSize', 11)

% Guardar la gráfica final automáticamente
saveas(gcf, 'comparativa_algoritmos.png')