% ====================================================================
% SCRIPT DE AJUSTE DE MODELOS MATEMÁTICOS Y COMPLEJIDAD (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega: 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script realiza un análisis de regresión matemática 
% (ajuste de curvas) sobre los tiempos de ejecución de cada algoritmo.
% Utiliza interpolación polinomial y transformaciones logarítmicas para 
% aproximar el comportamiento empírico de los datos a diferentes modelos 
% de complejidad asintótica (O(n), O(n^2), O(log n), exponencial, etc.).
% Genera gráficas superponiendo los datos reales con las curvas teóricas
% en escala logarítmica para facilitar la identificación del modelo 
% que mejor describe (fitea) a cada algoritmo de ordenamiento.
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Asegurarse de tener el archivo "datos_resultados.csv".
% 2. Ejecutar el script. Las imágenes se guardarán automáticamente
%    con el prefijo "ajuste_".
% ====================================================================

% Limpieza inicial del entorno de trabajo
clc
clear
close all

% ================================
% 1. Cargar y preparar datos
% ================================
data = readtable('datos_resultados.csv');

% Obtener la lista de algoritmos evaluados
algoritmos = unique(data.Algoritmo);

% Bucle para analizar y graficar cada algoritmo
for i = 1:length(algoritmos)

    nombre = algoritmos{i};

    % Filtrar los datos correspondientes al algoritmo actual
    idx = strcmp(data.Algoritmo, nombre);

    t = data.n(idx);          % Variable independiente (Tamaño n)
    y = data.TiempoReal(idx); % Variable dependiente (Tiempo en segundos)

    % 🔹 ORDENAR
    % Garantiza que los datos de 'n' estén ordenados de menor a mayor
    % para que las líneas de las curvas se dibujen correctamente
    [t, orden] = sort(t);
    y = y(orden);

    % =========================
    % AJUSTES POLINOMIALES (O(n), O(n^2), O(n^3), O(n^6))
    % =========================
    p1 = polyfit(t, y, 1); % Lineal
    p2 = polyfit(t, y, 2); % Cuadrático
    p3 = polyfit(t, y, 3); % Cúbico
    p6 = polyfit(t, y, 6); % Grado 6 (Sobreaajuste/Overfitting)

    % Evaluar los polinomios obtenidos
    f1 = polyval(p1, t);
    f2 = polyval(p2, t);
    f3 = polyval(p3, t);
    f6 = polyval(p6, t);

    % =========================
    % AJUSTE POTENCIAL (y = a*t^b) -> O(n^c)
    % =========================
    % Se linealiza aplicando logaritmos naturales: ln(y) = ln(a) + b*ln(t)
    p = polyfit(log(t), log(y), 1);
    fpot = exp(p(2)) * t.^p(1);

    % =========================
    % AJUSTE EXPONENCIAL (y = a*e^(bt)) -> O(2^n)
    % =========================
    % Se linealiza aplicando logaritmo a 'y': ln(y) = ln(a) + b*t
    p = polyfit(t, log(y), 1);
    fexp = exp(p(2)) * exp(p(1)*t);

    % =========================
    % AJUSTE LOGARÍTMICO (y = a*ln(t) + b) -> O(log n)
    % =========================
    p = polyfit(log(t), y, 1);
    flog = p(1)*log(t) + p(2);

    % =========================
    % 2. GRÁFICA MEJORADA DE MODELOS
    % =========================
    figure

    % Datos empíricos (reales) más visibles
    plot(t, y, 'o', 'MarkerSize', 7, 'MarkerFaceColor', 'r', 'MarkerEdgeColor', 'k')
    hold on

    % Curvas teóricas (Mejor estilo de trazado)
    plot(t, f2, 'g', 'LineWidth', 2)    % Grado 2 (Principal referente O(n^2))
    plot(t, fpot, 'c', 'LineWidth', 2)  % Potencial

    plot(t, f1, '--b', 'LineWidth', 1)  % Lineal O(n)
    plot(t, f3, '--m', 'LineWidth', 1)  % Cúbico O(n^3)
    plot(t, f6, ':k', 'LineWidth', 1)   % Polinomio grado 6

    plot(t, flog, '--', 'LineWidth', 2, 'Color', [0 0.4 0.8]) % Logarítmico

    % ⚠️ Exponencial más tenue (no dominante, rara vez se alcanza en estos algoritmos)
    plot(t, fexp, 'Color', [0.9 0.9 0], 'LineWidth', 1)

    % 🔥 ESCALA LOGARÍTMICA (CLAVE)
    % Permite diferenciar el crecimiento de las curvas en magnitudes extremas
    set(gca, 'YScale', 'log')

    % Personalización estética
    title(['Ajuste de modelos - ', strrep(nombre,'_',' ')], 'FontSize', 14)
    xlabel('Tamaño del problema (n)', 'FontSize', 12)
    ylabel('Tiempo (segundos)', 'FontSize', 12)

    legend('Datos', 'grado 2', 'potencial', ...
           'grado 1', 'grado 3', 'grado 6', ...
           'logarítmico', 'exponencial', ...
           'Location', 'northwest')

    grid on
    set(gca, 'FontSize', 11)

    % Guardar gráfica
    saveas(gcf, strcat('ajuste_', nombre, '.png'))

    hold off

end