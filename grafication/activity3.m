% 1. Leer los datos
data = readtable('datos.csv');

% Ordenar datos por TiempoReal para una mejor visualización
data = sortrows(data, 'TiempoReal');

algoritmos = data.Algoritmo;
t_real = data.TiempoReal;
t_cpu = data.TiempoCPU;

% ---- GRÁFICA DE TIEMPOS ----
figure('Color', 'w', 'Position', [100 100 900 600])

% Gráfica de barras
b = bar([t_real, t_cpu], 'grouped');
set(gca, 'YScale', 'log'); % <--- CRITICO: Escala logarítmica
grid on

% Personalización
set(gca, 'XTickLabel', algoritmos, 'XTick', 1:length(algoritmos))
xtickangle(45)
ylabel('Tiempo en Segundos (Escala Logarítmica)')
title('Comparativa de Rendimiento: 10 Algoritmos (n=500,000)')
legend('Tiempo Real', 'Tiempo CPU', 'Location', 'northwest')

% Añadir etiquetas de texto sobre las barras para los más lentos
for i = 1:length(t_real)
    if t_real(i) > 1
        text(i, t_real(i), sprintf(' %.1fs', t_real(i)), ...
            'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom')
    end
end

% ---- GRÁFICA DE EFICIENCIA (CPU vs Wall) ----
figure('Color', 'w')
bar(data.CPUWall, 'FaceColor', [0.4 0.7 0.4])
set(gca, 'XTickLabel', algoritmos, 'XTick', 1:length(algoritmos))
xtickangle(45)
title('Porcentaje de Uso de CPU (CPU/Wall %)')
ylabel('% Eficiencia')
ylim([0 110])
grid on