clc
clear
close all

% ================================
% Cargar datos
% ================================
datos = readtable('datos.csv');

algoritmos = unique(datos.Algoritmo);

% ================================
% PUNTO 4
% Graficar comportamiento temporal
% ================================

for i = 1:length(algoritmos)

    nombre = algoritmos{i};

    idx = strcmp(datos.Algoritmo, nombre);

    n = datos.n(idx);
    tiempo_real = datos.TiempoReal(idx);
    tiempo_cpu = datos.TiempoCPU(idx);
    tiempo_es = datos.TiempoES(idx);

    figure

    scatter(n, tiempo_real,60,'filled')
    hold on
    scatter(n, tiempo_cpu,60,'filled')
    scatter(n, tiempo_es,60,'filled')

    title(['Ordenamiento ', nombre])
    xlabel('Tamaño del problema (n)')
    ylabel('Tiempo (segundos)')

    legend('Tiempo real','Tiempo CPU','Tiempo E/S')

    grid on

    % Guardar gráfica
    nombre_archivo = strcat('grafica_',nombre,'.png');
    saveas(gcf,nombre_archivo)

end


% ================================
% PUNTO 5
% Comparativa de los algoritmos
% (solo Tiempo Real)
% ================================

max_n = max(datos.n);

idx = datos.n == max_n;

alg = datos.Algoritmo(idx);
tiempo = datos.TiempoReal(idx);

figure

bar(categorical(alg), tiempo)

title('Comparativa de algoritmos de ordenamiento (Tiempo real)')
xlabel('Algoritmo')
ylabel('Tiempo real (segundos)')

grid on

saveas(gcf,'comparativa_algoritmos.png')


% ================================
% EXTRA (Muy útil para reporte)
% Curvas completas comparativas
% ================================

figure
hold on

for i = 1:length(algoritmos)

    nombre = algoritmos{i};

    idx = strcmp(datos.Algoritmo,nombre);

    plot(datos.n(idx),datos.TiempoReal(idx),'LineWidth',2)

end

legend(algoritmos,'Location','northwest')

xlabel('Tamaño del problema (n)')
ylabel('Tiempo real (segundos)')

title('Comparativa temporal de algoritmos')

grid on

saveas(gcf,'comparativa_curvas.png')