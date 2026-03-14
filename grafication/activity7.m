clear
clc
close all

data = readtable('resultados_algoritmos.csv');

t = data.n;
algoritmos = data(:,2:end);

numAlg = width(algoritmos);

for i=1:numAlg

    y = algoritmos{:,i};
    nombre = algoritmos.Properties.VariableNames{i};

    % POLINOMIOS
    p1 = polyfit(t,y,1);
    p2 = polyfit(t,y,2);
    p3 = polyfit(t,y,3);
    p6 = polyfit(t,y,6);

    f1 = polyval(p1,t);
    f2 = polyval(p2,t);
    f3 = polyval(p3,t);
    f6 = polyval(p6,t);

    % POTENCIAL
    p = polyfit(log(t),log(y),1);
    a = p(1);
    c = exp(p(2));
    fpot = c*t.^a;

    % EXPONENCIAL
    p = polyfit(t,log(y),1);
    a = p(1);
    c = exp(p(2));
    fexp = c*exp(a*t);

    % LOGARITMICO
    p = polyfit(log(t),y,1);
    flog = p(1)*log(t)+p(2);

    % GRAFICA COMPARATIVA
    figure

    plot(t,y,'ro','MarkerFaceColor','r')
    hold on

    plot(t,f1,'b','LineWidth',1.5)
    plot(t,f2,'g','LineWidth',1.5)
    plot(t,f3,'m','LineWidth',1.5)
    plot(t,f6,'k','LineWidth',1.5)

    plot(t,fpot,'c','LineWidth',2)
    plot(t,fexp,'y','LineWidth',2)
    plot(t,flog,'--','LineWidth',2)

    title(['Comparativa de aproximaciones - ',nombre])
    xlabel('n')
    ylabel('Tiempo')

    legend('Datos','grado1','grado2','grado3','grado6',...
           'potencial','exponencial','logaritmico')

    grid on
    hold off

ends