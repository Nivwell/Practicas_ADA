% ====================================================================
% SCRIPT DE ESTIMACIÓN TEÓRICA PARA VALORES EXTREMOS (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega: 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script realiza una extrapolación teórica para 
% predecir cuánto tardaría cada algoritmo en ordenar conjuntos de datos 
% masivos (hasta 10 millones de elementos). Utiliza las funciones de 
% complejidad asintótica estimadas previamente para proyectar el 
% rendimiento y presenta los resultados en una tabla limpia. Esto evita 
% tener que ejecutar empíricamente algoritmos O(n^2) que tardarían días.
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Abrir MATLAB o GNU Octave.
% 2. Ejecutar el script. La tabla de estimaciones se imprimirá 
%    directamente en la "Command Window".
% ====================================================================

clear
clc

% ================================
% 1. Valores extremos a estimar (n)
% ================================
% Tamaños masivos: 3M, 4M, 5M, 6M y 10 Millones
n = [3000000, 4000000, 5000000, 6000000, 10000000];

% ================================
% 2. MODELOS TEÓRICOS (LOS CORRECTOS)
% ================================

% Algoritmos Cuadráticos O(n^2)
bubble    = 2e-10 * n.^2;
insertion = 1.9e-10 * n.^2;
selection = 1.8e-10 * n.^2;

% Algoritmo Intermedio ~O(n^1.5)
shell = 3e-8 * n.^1.5;

% Algoritmos Logarítmicos O(n log n)
merge = 2e-8 * n.*log(n);
quick = 1.7e-8 * n.*log(n);
heap  = 1.9e-8 * n.*log(n);
tree  = 2.1e-8 * n.*log(n);

% ================================
% 3. CREACIÓN Y VISUALIZACIÓN DE LA TABLA
% ================================
% Se transponen los vectores (usando ') para convertirlos en columnas
tabla = table(n', bubble', insertion', selection', shell', ...
              merge', quick', heap', tree');

% Asignar nombres claros a las columnas de la tabla
tabla.Properties.VariableNames = ...
{'n', 'Bubble', 'Insertion', 'Selection', 'Shell', ...
 'Merge', 'Quick', 'Heap', 'Tree'};

% Mostrar la tabla en la consola con un encabezado amigable
disp('Estimación teórica de tiempos (en segundos) para "n" masivos:')
disp('=============================================================================')
disp(tabla)