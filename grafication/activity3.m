% ====================================================================
% SCRIPT DE VISUALIZACIÓN Y ANÁLISIS DE RENDIMIENTO (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script procesa el archivo "datos.csv" generado por 
% las pruebas de rendimiento de los algoritmos de ordenamiento en C.
% Extrae las métricas de tiempo (Tiempo Real y Tiempo CPU) y eficiencia 
% (CPU/Wall), ordenándolas de manera ascendente para una mejor visualización. 
% Genera dos gráficas profesionales:
% 1. Gráfica de barras agrupadas en escala logarítmica (crucial para 
%    comparar algoritmos ultrarrápidos O(n log n) contra los lentos O(n^2)).
% 2. Gráfica de barras mostrando el aprovechamiento del procesador (%).
%
% OBSERVACIONES: El archivo "datos.csv" debe encontrarse en el mismo 
% directorio (carpeta) que este script antes de su ejecución.
%
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Abrir MATLAB o GNU Octave.
% 2. Navegar hasta la carpeta donde se encuentra este script y "datos.csv".
% 3. Ejecutar el script presionando "Run" o escribiendo el nombre del 
%    archivo en la "Command Window".
% ====================================================================

% 1. Leer los datos desde el archivo CSV
data = readtable('datos.csv');

% 2. Ordenar datos por TiempoReal (de menor a mayor) para una mejor visualización
data = sortrows(data, 'TiempoReal');

% Extraer columnas a variables individuales para facilitar el trazado
algoritmos = data.Algoritmo;
t_real = data.TiempoReal;
t_cpu = data.TiempoCPU;

% ====================================================================
% ---- GRÁFICA 1: COMPARATIVA DE TIEMPOS (ESCALA LOGARÍTMICA) ----
% ====================================================================
figure('Color', 'w', 'Position', [100 100 900 600])

% Crear gráfica de barras agrupadas (Compara Real vs CPU)
b = bar([t_real, t_cpu], 'grouped');

% Ajustar la escala a logarítmica (CRÍTICO para ver diferencias extremas)
set(gca, 'YScale', 'log'); 
grid on

% Personalización de ejes, etiquetas y títulos
set(gca, 'XTickLabel', algoritmos, 'XTick', 1:length(algoritmos))
xtickangle(45)
ylabel('Tiempo en Segundos (Escala Logarítmica)')
title('Comparativa de Rendimiento: 10 Algoritmos (n=500,000)')
legend('Tiempo Real', 'Tiempo CPU', 'Location', 'northwest')

% Añadir etiquetas de texto sobre las barras para los algoritmos más lentos
for i = 1:length(t_real)
    if t_real(i) > 1
        text(i, t_real(i), sprintf(' %.1fs', t_real(i)), ...
            'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom')
    end
end

% ====================================================================
% ---- GRÁFICA 2: EFICIENCIA DEL PROCESADOR (CPU vs Wall) ----
% ====================================================================
figure('Color', 'w')

% Crear gráfica de barras simple para el porcentaje de eficiencia
bar(data.CPUWall, 'FaceColor', [0.4 0.7 0.4])

% Personalización de ejes, etiquetas y títulos
set(gca, 'XTickLabel', algoritmos, 'XTick', 1:length(algoritmos))
xtickangle(45)
title('Porcentaje de Uso de CPU (CPU/Wall %)')
ylabel('% Eficiencia')
ylim([0 110]) % Limitar el eje Y de 0% a 110% por estética visual
grid on