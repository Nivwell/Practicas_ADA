% ====================================================================
% SCRIPT DE VISUALIZACIÓN BÁSICA: TIEMPOS Y EFICIENCIA (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega: 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script genera gráficas de barras para comparar el 
% rendimiento general de los algoritmos de ordenamiento. 
% Genera dos figuras:
% 1. Una gráfica de barras agrupadas (Tiempo Real vs Tiempo CPU) en 
%    escala logarítmica para poder visualizar simultáneamente algoritmos 
%    ultrarrápidos y algoritmos muy lentos.
% 2. Una gráfica de barras simples que muestra la eficiencia de uso del 
%    procesador (Porcentaje CPU/Wall).
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Asegurarse de tener el archivo "datos.csv" en la misma carpeta.
% 2. Ejecutar el script. Se abrirán dos ventanas con las gráficas.
% ====================================================================

% Limpieza recomendada (opcional, pero buena práctica)
clc
clear
close all

% ================================
% 1. Lectura y preparación de datos
% ================================
% Carga los datos del archivo CSV generado previamente
data = readtable('datos.csv');

% Ordenar datos por TiempoReal (de menor a mayor) para una mejor 
% visualización (escalera visual) en la gráfica.
data = sortrows(data, 'TiempoReal');

% Extracción de columnas para facilitar el manejo de variables
algoritmos = data.Algoritmo;
t_real = data.TiempoReal;
t_cpu = data.TiempoCPU;

% ================================
% 2. GRÁFICA DE TIEMPOS (REAL VS CPU)
% ================================
% Crea una ventana de fondo blanco con proporciones específicas
figure('Color', 'w', 'Position', [100 100 900 600])

% Gráfica de barras agrupadas: compara ambos tiempos lado a lado
b = bar([t_real, t_cpu], 'grouped');

% <--- CRÍTICO: Escala logarítmica --->
% Sin esto, los algoritmos O(n log n) no se verían al lado de los O(n^2)
set(gca, 'YScale', 'log'); 
grid on

% Personalización estética de ejes y títulos
set(gca, 'XTickLabel', algoritmos, 'XTick', 1:length(algoritmos))
xtickangle(45) % Inclina los nombres para que no se encimen
ylabel('Tiempo en Segundos (Escala Logarítmica)')
title('Comparativa de Rendimiento: 10 Algoritmos (n=500,000)')
legend('Tiempo Real', 'Tiempo CPU', 'Location', 'northwest')

% Añadir etiquetas de texto dinámicas sobre las barras
% (Solo se imprime el texto para aquellos algoritmos que tardan más de 1 segundo)
for i = 1:length(t_real)
    if t_real(i) > 1
        text(i, t_real(i), sprintf(' %.1fs', t_real(i)), ...
            'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom')
    end
end

% ================================
% 3. GRÁFICA DE EFICIENCIA (CPU vs Wall)
% ================================
% Crea una segunda ventana para el análisis de uso del procesador
figure('Color', 'w')

% Gráfica de barras simple en tono verde
bar(data.CPUWall, 'FaceColor', [0.4 0.7 0.4])

% Personalización estética
set(gca, 'XTickLabel', algoritmos, 'XTick', 1:length(algoritmos))
xtickangle(45)
title('Porcentaje de Uso de CPU (CPU/Wall %)')
ylabel('% Eficiencia')
ylim([0 110]) % Se fija el límite en 110% para mantener proporción visual
grid on