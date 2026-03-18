% ====================================================================
% SCRIPT DE COMPARATIVA TEÓRICA DE COMPLEJIDAD ASINTÓTICA (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega: 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script genera una gráfica teórica que modela el 
% comportamiento asintótico (Big-O) de los algoritmos de ordenamiento 
% implementados. Utiliza funciones matemáticas puras (n^2, n^1.5, n*log(n)) 
% multiplicadas por constantes de escala arbitrarias para separar visualmente 
% las curvas. Esta gráfica sirve como "Baseline" o referencia teórica 
% para comparar contra los resultados empíricos obtenidos en C.
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Abrir MATLAB o GNU Octave.
% 2. Ejecutar el script. La gráfica resultante se guardará como 
%    'comparativa_teorica.png'.
% ====================================================================

clc
clear
close all

% ================================
% 1. Rango del tamaño del problema (n)
% ================================
% Genera 300 puntos equiespaciados entre 1,000 y 500,000
n = linspace(1000, 500000, 300);

% ================================
% 2. MODELOS MATEMÁTICOS TEÓRICOS
% ================================
% Nota: Los coeficientes (ej. 2e-10) son factores de escala arbitrarios 
% para que las curvas se distingan visualmente en la gráfica sin encimarse, 
% respetando siempre su tasa de crecimiento asintótico.

% Modelos Cuadráticos O(n^2) - Algoritmos elementales
bubble    = 2e-10 * n.^2;
insertion = 1.9e-10 * n.^2;
selection = 1.8e-10 * n.^2;

% Modelo Intermedio O(n^1.5) - Promedio/Peor caso común para Shell
shell = 3e-8 * n.^1.5;

% Modelos Logarítmicos O(n log n) - Algoritmos divide y vencerás / estructuras avanzadas
merge = 2e-8 * n.*log(n);
quick = 1.7e-8 * n.*log(n);
heap  = 1.9e-8 * n.*log(n);
tree  = 2.1e-8 * n.*log(n);

% ================================
% 3. GRÁFICA Y ESTILOS
% ================================
figure('Color', 'w', 'Position', [100 100 800 600])
hold on

% 🔴 Trazado de Cuadráticos (Rojos)
plot(n, bubble, 'r', 'LineWidth', 2)
plot(n, insertion, 'Color', [0.8 0 0], 'LineWidth', 2)
plot(n, selection, 'Color', [0.6 0 0], 'LineWidth', 2)

% 🟡 Trazado de Intermedio (Naranja)
plot(n, shell, 'Color', [0.9 0.6 0], 'LineWidth', 2)

% 🔵 Trazado de O(n log n) (Azules)
plot(n, merge, 'b', 'LineWidth', 2)
plot(n, quick, 'Color', [0 0.4 1], 'LineWidth', 2)
plot(n, heap, 'Color', [0 0.2 0.8], 'LineWidth', 2)
plot(n, tree, 'Color', [0 0 0.5], 'LineWidth', 2)

% 🔥 ESCALA LOGARÍTMICA (MUY IMPORTANTE)
% Permite visualizar la enorme brecha que se abre entre O(n^2) y O(n log n) a medida que 'n' crece.
set(gca, 'YScale', 'log')

% ================================
% 4. FORMATO Y ETIQUETAS
% ================================
legend({'Bubble O(n^2)', 'Insertion O(n^2)', 'Selection O(n^2)', ...
        'Shell ~O(n^{1.5})', ...
        'Merge O(n log n)', 'Quick O(n log n)', ...
        'Heap O(n log n)', 'Tree O(n log n)'}, ...
        'Location', 'northwest')

xlabel('Tamaño del problema (n)', 'FontSize', 12)
ylabel('Tiempo estimado (escala logarítmica)', 'FontSize', 12)
title('Comparativa Teórica de Funciones de Complejidad Asintótica', 'FontSize', 14)

grid on
set(gca, 'FontSize', 11)

hold off

% Guardar la gráfica final automáticamente
saveas(gcf, 'comparativa_teorica.png')