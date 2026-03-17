clear
clc

% ================================
% Valores a estimar
% ================================
n = [3000000 4000000 5000000 6000000 10000000];

% ================================
% MODELOS (LOS CORRECTOS)
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
% TABLA
% ================================
tabla = table(n',bubble',insertion',selection',shell',...
              merge',quick',heap',tree');

tabla.Properties.VariableNames = ...
{'n','Bubble','Insertion','Selection','Shell',...
 'Merge','Quick','Heap','Tree'};

disp(tabla)