% =========================================================================
% ANALISIS DE COMPLEJIDAD - PRACTICA 2 (BUSQUEDAS)
% =========================================================================

clc; clear; close all;

% 1. Cargar el archivo de la bitácora (ajustando la ruta a tu estructura)
archivo_csv = '../search_algorithm/bitacora_tiempos.csv';

% Como el CSV no tiene encabezados de columnas, los forzamos
opts = detectImportOptions(archivo_csv, 'ReadVariableNames', false);
T = readtable(archivo_csv, opts);
T.Properties.VariableNames = {'Algoritmo', 'N', 'Objetivo', 'Encontrado', 'Indice', 'Hilos', 'T_Real', 'T_User', 'T_Sys'};

% 2. Configurar los algoritmos y sus cotas teóricas para aplicar polyfit
algoritmos = {
    'Busqueda Lineal Secuencial', 'O(n)';
    'Busqueda Binaria Secuencial', 'O(log n)';
    'Busqueda ABB Secuencial', 'O(log n)';
    'Busqueda Exponencial Secuencial', 'O(log n)';
    'Busqueda Fibonacci Secuencial', 'O(log n)'
};

% Valores N para las predicciones del Punto 9
N_predicciones = [50000000, 100000000, 500000000, 1000000000, 5000000000];

% Crear una figura grande para mostrar todos los subplots (Puntos 6 y 8)
figure('Name', 'Comparativa Teórica vs Experimental', 'NumberTitle', 'off', 'Position', [100, 100, 1200, 800]);

for i = 1:size(algoritmos, 1)
    nombre_alg = algoritmos{i, 1};
    cota_teorica = algoritmos{i, 2};
    
    % Filtrar la tabla para obtener solo las filas del algoritmo actual
    datos_alg = T(strcmp(T.Algoritmo, nombre_alg), :);
    
    if isempty(datos_alg)
        continue; % Si aún no corres algún algoritmo, lo salta sin fallar
    end
    
    % Obtener los tamaños de problema (N) únicos
    N_unicos = unique(datos_alg.N);
    tiempos_promedio = zeros(length(N_unicos), 1);
    
    % 3. Calcular el T_Real promedio por cada N (Punto 5)
    for j = 1:length(N_unicos)
        % Extraer los 20 tiempos de la N actual y promediar
        tiempos_promedio(j) = mean(datos_alg.T_Real(datos_alg.N == N_unicos(j)));
    end
    
    % 4. Aproximación matemática usando polyfit (Punto 7)
    if strcmp(cota_teorica, 'O(n)')
        % Ajuste lineal puro: f(n) = x1*n + x2
        p = polyfit(N_unicos, tiempos_promedio, 1);
        curva_ajuste = p(1) * N_unicos + p(2);
        eq_str = sprintf('T(n) = %.2e * n + %.2e', p(1), p(2));
    else
        % Ajuste logarítmico: f(n) = x1*ln(n) + x2
        p = polyfit(log(N_unicos), tiempos_promedio, 1);
        curva_ajuste = p(1) * log(N_unicos) + p(2);
        eq_str = sprintf('T(n) = %.2e * ln(n) + %.2e', p(1), p(2));
    end
    
    % 5. Graficar comportamiento (Puntos 6 y 8)
    subplot(2, 3, i);
    scatter(N_unicos, tiempos_promedio, 50, 'b', 'filled', 'MarkerFaceAlpha', 0.6); hold on;
    plot(N_unicos, curva_ajuste, '-r', 'LineWidth', 2);
    
    title(nombre_alg, 'Interpreter', 'none');
    xlabel('Tamaño del arreglo (N)');
    ylabel('Tiempo Promedio Real (s)');
    legend('Promedio Experimental', ['Ajuste Teórico ', cota_teorica], 'Location', 'northwest');
    grid on;
    
    % 6. Realizar predicciones a futuro e imprimir en consola (Puntos 9 y 10)
    fprintf('\n======================================================\n');
    fprintf('  %s\n', nombre_alg);
    fprintf('  Cota teórica: %s | Ecuación: %s\n', cota_teorica, eq_str);
    fprintf('------------------------------------------------------\n');
    
    for k = 1:length(N_predicciones)
        n_futuro = N_predicciones(k);
        if strcmp(cota_teorica, 'O(n)')
            t_pred = p(1) * n_futuro + p(2);
        else
            t_pred = p(1) * log(n_futuro) + p(2);
        end
        fprintf('  N = %10d  --->  Tiempo estimado: %.6f seg\n', n_futuro, t_pred);
    end
end % <--- EL CICLO FOR TERMINA AQUÍ

% --- EXTRA PARA EL PUNTO 6: GRÁFICA COMPARATIVA GLOBAL ---
figure('Name', 'Comparativa Global de Algoritmos');
gscatter(T.N, T.T_Real, T.Algoritmo);
title('Comparativa de Tiempos Promedio (Los 5 Algoritmos)');
xlabel('Tamaño del arreglo (N)'); ylabel('Tiempo Promedio Real (s)');
grid on;