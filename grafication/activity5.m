clc
clear
close all

% ================================
% Cargar datos
% ================================
datos = readtable('datos_resultados.csv');

algoritmos = unique(datos.Algoritmo);

% ================================
% PUNTO 5 - COMPARATIVA FINAL
% ================================

max_n = max(datos.n);

idx = datos.n == max_n;

alg = datos.Algoritmo(idx);
tiempo = datos.TiempoReal(idx);

% Ordenar de menor a mayor (mejor visualmente)
[tiempo, orden] = sort(tiempo);
alg = alg(orden);

figure

bar(categorical(alg), tiempo)

title('Comparativa de algoritmos (Tiempo real)','FontSize',14)
xlabel('Algoritmo','FontSize',12)
ylabel('Tiempo (segundos)','FontSize',12)

grid on
set(gca,'FontSize',11)

saveas(gcf,'comparativa_algoritmos.png')