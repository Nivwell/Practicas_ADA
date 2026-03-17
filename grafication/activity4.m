clc
clear
close all

% ================================
% Cargar datos
% ================================
datos = readtable('datos_resultados.csv');

algoritmos = unique(datos.Algoritmo);

% ================================
% PUNTO 4 - MEJORADO
% ================================

for i = 1:length(algoritmos)

    nombre = algoritmos{i};

    idx = strcmp(datos.Algoritmo, nombre);

    n = datos.n(idx);
    tiempo_real = datos.TiempoReal(idx);
    tiempo_cpu = datos.TiempoCPU(idx);
    tiempo_es = datos.TiempoES(idx);

    % 🔹 ORDENAR POR n (IMPORTANTÍSIMO)
    [n, orden] = sort(n);
    tiempo_real = tiempo_real(orden);
    tiempo_cpu = tiempo_cpu(orden);
    tiempo_es = tiempo_es(orden);

    figure

    plot(n, tiempo_real, '-o', 'LineWidth', 2)
    hold on
    plot(n, tiempo_cpu, '-s', 'LineWidth', 2)
    plot(n, tiempo_es, '-^', 'LineWidth', 2)

    title(['Ordenamiento ', strrep(nombre,'_',' ')],'FontSize',14)
    xlabel('Tamaño del problema (n)','FontSize',12)
    ylabel('Tiempo (segundos)','FontSize',12)

    legend('Tiempo real','Tiempo CPU','Tiempo E/S','Location','northwest')

    grid on
    set(gca,'FontSize',11)

    % Guardar gráfica
    nombre_archivo = strcat('grafica_',nombre,'.png');
    saveas(gcf,nombre_archivo)

end