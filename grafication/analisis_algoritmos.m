% ====================================================================
% SCRIPT DE REGRESIÓN INDIVIDUAL POR TIPO DE MODELO (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega: 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script lee los resultados de los algoritmos y genera 
% gráficas individuales para 4 tipos de ajustes matemáticos: Polinomial, 
% Potencial, Exponencial y Logarítmico. Es útil para analizar a 
% profundidad la curva de crecimiento de cada algoritmo por separado.
% Las gráficas se exportan automáticamente para evitar la saturación 
% de ventanas en el entorno de trabajo.
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Asegurarse de tener el archivo "resultados_algoritmos.csv".
% 2. Ejecutar el script. Revisa tu carpeta para ver las imágenes generadas.
% ====================================================================

clear
clc
close all

%% 1. Leer datos
data = readtable('resultados_algoritmos.csv');

t = data.n;              % Variable independiente: tamaño de entrada
algoritmos = data(:, 2:end); % Extrae todas las columnas excepto 'n'

numAlg = width(algoritmos);

for i = 1:numAlg
    
    y = algoritmos{:, i};
    nombre_raw = algoritmos.Properties.VariableNames{i};
    nombre = strrep(nombre_raw, '_', ' '); % Limpia guiones bajos para los títulos
    
    % 🔹 ORDENAR (Protección por si el CSV viene desordenado)
    [t_ordenado, orden] = sort(t);
    y_ordenado = y(orden);
    
    % Reasignamos para usar las variables ordenadas en los ajustes
    t_actual = t_ordenado;
    y_actual = y_ordenado;
    
    %% -----------------------------
    % AJUSTES POLINOMIALES
    %% -----------------------------
    p1 = polyfit(t_actual, y_actual, 1);
    p2 = polyfit(t_actual, y_actual, 2);
    p3 = polyfit(t_actual, y_actual, 3);
    p6 = polyfit(t_actual, y_actual, 6);
    
    f1 = polyval(p1, t_actual);
    f2 = polyval(p2, t_actual);
    f3 = polyval(p3, t_actual);
    f6 = polyval(p6, t_actual);
    
    figure('Color', 'w')
    
    plot(t_actual, y_actual, 'ro', 'MarkerFaceColor', 'r')
    hold on
    
    plot(t_actual, f1, 'b', 'LineWidth', 1.5)
    plot(t_actual, f2, 'g', 'LineWidth', 1.5)
    plot(t_actual, f3, 'm', 'LineWidth', 1.5)
    plot(t_actual, f6, 'k', 'LineWidth', 1.5)
    
    title(['Ajuste polinomial - ', nombre], 'FontSize', 12)
    xlabel('Tamaño de entrada (n)')
    ylabel('Tiempo (s)')
    legend('Datos', 'grado 1', 'grado 2', 'grado 3', 'grado 6', 'Location', 'northwest')
    
    grid on
    hold off
    saveas(gcf, strcat(nombre_raw, '_1_Polinomial.png'))
    
    %% -----------------------------
    % AJUSTE POTENCIAL: y = c * x^a
    %% -----------------------------
    % Linealización: ln(y) = a*ln(t) + ln(c)
    p = polyfit(log(t_actual), log(y_actual), 1);
    
    a = p(1);
    c = exp(p(2));
    
    f = c * t_actual.^a;
    
    figure('Color', 'w')
    
    plot(t_actual, y_actual, 'ro', 'MarkerFaceColor', 'r')
    hold on
    plot(t_actual, f, 'b', 'LineWidth', 2)
    
    title(['Modelo potencial - ', nombre], 'FontSize', 12)
    xlabel('n')
    ylabel('tiempo (s)')
    legend('datos', 'c*n^a', 'Location', 'northwest')
    
    grid on
    hold off
    saveas(gcf, strcat(nombre_raw, '_2_Potencial.png'))
    
    %% -----------------------------
    % AJUSTE EXPONENCIAL: y = c * e^(at)
    %% -----------------------------
    % Linealización: ln(y) = a*t + ln(c)
    p = polyfit(t_actual, log(y_actual), 1);
    
    a = p(1);
    c = exp(p(2));
    
    f = c * exp(a * t_actual);
    
    figure('Color', 'w')
    
    plot(t_actual, y_actual, 'ro', 'MarkerFaceColor', 'r')
    hold on
    plot(t_actual, f, 'g', 'LineWidth', 2)
    
    title(['Modelo exponencial - ', nombre], 'FontSize', 12)
    xlabel('n')
    ylabel('tiempo (s)')
    legend('datos', 'c*e^{at}', 'Location', 'northwest')
    
    grid on
    hold off
    saveas(gcf, strcat(nombre_raw, '_3_Exponencial.png'))
    
    %% -----------------------------
    % AJUSTE LOGARÍTMICO: y = a * ln(t) + c
    %% -----------------------------
    p = polyfit(log(t_actual), y_actual, 1);
    
    a = p(1);
    c = p(2);
    
    f = a * log(t_actual) + c;
    
    figure('Color', 'w')
    
    plot(t_actual, y_actual, 'ro', 'MarkerFaceColor', 'r')
    hold on
    plot(t_actual, f, 'k', 'LineWidth', 2)
    
    title(['Modelo logarítmico - ', nombre], 'FontSize', 12)
    xlabel('n')
    ylabel('tiempo (s)')
    legend('datos', 'a ln(n)+c', 'Location', 'northwest')
    
    grid on
    hold off
    saveas(gcf, strcat(nombre_raw, '_4_Logaritmico.png'))
    
end