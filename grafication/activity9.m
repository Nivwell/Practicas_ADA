clear
clc

n = [3000000 4000000 5000000 6000000 10000000];

%% MODELOS 

burbuja = 2e-10 * n.^2;
insertion = 1.9e-10 * n.^2;
selection = 1.8e-10 * n.^2;

shell = 3e-8 * n.^1.5;

merge = 2e-8 * n.*log(n);
quick = 1.7e-8 * n.*log(n);
heap = 1.9e-8 * n.*log(n);

radix = 8e-7 * n;
counting = 7e-7 * n;
bucket = 6e-7 * n;

%% TABLA DE RESULTADOS

tabla = table(n',burbuja',insertion',selection',shell',merge',quick',heap',radix',counting',bucket');

tabla.Properties.VariableNames = {'n','Burbuja','Insertion','Selection','Shell','Merge','Quick','Heap','Radix','Counting','Bucket'};

disp(tabla)