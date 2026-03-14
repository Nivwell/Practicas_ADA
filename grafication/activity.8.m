clear
clc
close all

% rango de n para mostrar la grafica
n = linspace(1000,500000,200);

%% MODELOS SELECCIONADOS (ejemplo)

% Burbuja (ejemplo cuadratico)
burbuja = 2e-10 * n.^2;

% Insercion
insertion = 1.9e-10 * n.^2;

% Seleccion
selection = 1.8e-10 * n.^2;

% Shell (ejemplo potencial)
shell = 3e-8 * n.^1.5;

% Merge
merge = 2e-8 * n.*log(n);

% Quick
quick = 1.7e-8 * n.*log(n);

% Heap
heap = 1.9e-8 * n.*log(n);

% Radix
radix = 8e-7 * n;

% Counting
counting = 7e-7 * n;

% Bucket
bucket = 6e-7 * n;

%% GRAFICA COMPARATIVA

figure

plot(n,burbuja,'LineWidth',2)
hold on
plot(n,insertion,'LineWidth',2)
plot(n,selection,'LineWidth',2)
plot(n,shell,'LineWidth',2)
plot(n,merge,'LineWidth',2)
plot(n,quick,'LineWidth',2)
plot(n,heap,'LineWidth',2)
plot(n,radix,'LineWidth',2)
plot(n,counting,'LineWidth',2)
plot(n,bucket,'LineWidth',2)

legend('Burbuja','Insertion','Selection','Shell','Merge','Quick','Heap','Radix','Counting','Bucket')

xlabel('n')
ylabel('Tiempo estimado')

title('Comparativa de complejidad temporal de los algoritmos')

grid on