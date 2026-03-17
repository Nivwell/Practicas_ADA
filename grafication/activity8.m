clear
clc
close all

% ================================
% Rango de n
% ================================
n = linspace(1000,500000,300);

% ================================
% MODELOS (SOLO TUS ALGORITMOS)
% ================================

% O(n^2)
bubble    = 2e-10 * n.^2;
insertion = 1.9e-10 * n.^2;
selection = 1.8e-10 * n.^2;

% Intermedio
shell = 3e-8 * n.^1.5;

% O(n log n)
merge = 2e-8 * n.*log(n);
quick = 1.7e-8 * n.*log(n);
heap  = 1.9e-8 * n.*log(n);
tree  = 2.1e-8 * n.*log(n);

% ================================
% GRAFICA
% ================================
figure
hold on

% 🔴 Cuadráticos
plot(n,bubble,'r','LineWidth',2)
plot(n,insertion,'Color',[0.8 0 0],'LineWidth',2)
plot(n,selection,'Color',[0.6 0 0],'LineWidth',2)

% 🟡 Intermedio
plot(n,shell,'Color',[0.9 0.6 0],'LineWidth',2)

% 🔵 n log n
plot(n,merge,'b','LineWidth',2)
plot(n,quick,'Color',[0 0.4 1],'LineWidth',2)
plot(n,heap,'Color',[0 0.2 0.8],'LineWidth',2)
plot(n,tree,'Color',[0 0 0.5],'LineWidth',2)

% 🔥 ESCALA LOG (MUY IMPORTANTE)
set(gca,'YScale','log')

% ================================
% FORMATO
% ================================
legend({'Bubble O(n^2)','Insertion O(n^2)','Selection O(n^2)',...
        'Shell ~O(n^{1.5})',...
        'Merge O(n log n)','Quick O(n log n)',...
        'Heap O(n log n)','Tree O(n log n)'},...
        'Location','northwest')

xlabel('Tamaño del problema (n)','FontSize',12)
ylabel('Tiempo estimado (escala log)','FontSize',12)

title('Comparativa de funciones de complejidad seleccionadas','FontSize',14)

grid on
set(gca,'FontSize',11)

hold off