clear
clc
close all

%% Leer datos
data = readtable('resultados_algoritmos.csv');

t = data.n;              % tamaño de entrada
algoritmos = data(:,2:end); % todas las columnas excepto n

numAlg = width(algoritmos);

for i = 1:numAlg
    
    y = algoritmos{:,i};
    nombre = algoritmos.Properties.VariableNames{i};
    
    %% -----------------------------
    % Ajustes polinomiales
    %% -----------------------------
    
    p1 = polyfit(t,y,1);
    p2 = polyfit(t,y,2);
    p3 = polyfit(t,y,3);
    p6 = polyfit(t,y,6);
    
    f1 = polyval(p1,t);
    f2 = polyval(p2,t);
    f3 = polyval(p3,t);
    f6 = polyval(p6,t);
    
    figure
    
    plot(t,y,'ro','MarkerFaceColor','r')
    hold on
    
    plot(t,f1,'b','LineWidth',1.5)
    plot(t,f2,'g','LineWidth',1.5)
    plot(t,f3,'m','LineWidth',1.5)
    plot(t,f6,'k','LineWidth',1.5)
    
    title(['Ajuste polinomial - ',nombre])
    xlabel('Tamaño de entrada (n)')
    ylabel('Tiempo')
    
    legend('Datos','grado 1','grado 2','grado 3','grado 6')
    
    grid on
    hold off
    
    
    %% -----------------------------
    % Ajuste potencial
    %% -----------------------------
    
    p = polyfit(log(t),log(y),1);
    
    a = p(1);
    c = exp(p(2));
    
    f = c*t.^a;
    
    figure
    
    plot(t,y,'ro','MarkerFaceColor','r')
    hold on
    plot(t,f,'b','LineWidth',2)
    
    title(['Modelo potencial - ',nombre])
    xlabel('n')
    ylabel('tiempo')
    
    legend('datos','c*n^a')
    
    grid on
    hold off
    
    
    %% -----------------------------
    % Ajuste exponencial
    %% -----------------------------
    
    p = polyfit(t,log(y),1);
    
    a = p(1);
    c = exp(p(2));
    
    f = c*exp(a*t);
    
    figure
    
    plot(t,y,'ro','MarkerFaceColor','r')
    hold on
    plot(t,f,'g','LineWidth',2)
    
    title(['Modelo exponencial - ',nombre])
    xlabel('n')
    ylabel('tiempo')
    
    legend('datos','c*e^{at}')
    
    grid on
    hold off
    
    
    %% -----------------------------
    % Ajuste logarítmico
    %% -----------------------------
    
    p = polyfit(log(t),y,1);
    
    a = p(1);
    c = p(2);
    
    f = a*log(t)+c;
    
    figure
    
    plot(t,y,'ro','MarkerFaceColor','r')
    hold on
    plot(t,f,'k','LineWidth',2)
    
    title(['Modelo logaritmico - ',nombre])
    xlabel('n')
    ylabel('tiempo')
    
    legend('datos','a ln(n)+c')
    
    grid on
    hold off
    
end