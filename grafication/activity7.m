clc
clear
close all

data = readtable('datos_resultados.csv');

algoritmos = unique(data.Algoritmo);

for i = 1:length(algoritmos)

    nombre = algoritmos{i};

    idx = strcmp(data.Algoritmo, nombre);

    t = data.n(idx);
    y = data.TiempoReal(idx);

    % 🔹 ORDENAR
    [t, orden] = sort(t);
    y = y(orden);

    % =========================
    % POLINOMIOS
    % =========================
    p1 = polyfit(t,y,1);
    p2 = polyfit(t,y,2);
    p3 = polyfit(t,y,3);
    p6 = polyfit(t,y,6);

    f1 = polyval(p1,t);
    f2 = polyval(p2,t);
    f3 = polyval(p3,t);
    f6 = polyval(p6,t);

    % =========================
    % POTENCIAL
    % =========================
    p = polyfit(log(t),log(y),1);
    fpot = exp(p(2)) * t.^p(1);

    % =========================
    % EXPONENCIAL
    % =========================
    p = polyfit(t,log(y),1);
    fexp = exp(p(2)) * exp(p(1)*t);

    % =========================
    % LOGARÍTMICO
    % =========================
    p = polyfit(log(t),y,1);
    flog = p(1)*log(t)+p(2);

    % =========================
    % GRAFICA MEJORADA
    % =========================
    figure

    % Datos reales (más visibles)
    plot(t,y,'o','MarkerSize',7,'MarkerFaceColor','r','MarkerEdgeColor','k')
    hold on

    % Curvas (mejor estilo)
    plot(t,f2,'g','LineWidth',2) % grado 2 (principal)
    plot(t,fpot,'c','LineWidth',2)

    plot(t,f1,'--b','LineWidth',1)
    plot(t,f3,'--m','LineWidth',1)
    plot(t,f6,':k','LineWidth',1)

    plot(t,flog,'--','LineWidth',2,'Color',[0 0.4 0.8])

    % ⚠️ Exponencial más tenue (no dominante)
    plot(t,fexp,'Color',[0.9 0.9 0],'LineWidth',1)

    % 🔥 ESCALA LOGARÍTMICA (CLAVE)
    set(gca,'YScale','log')

    title(['Ajuste de modelos - ', strrep(nombre,'_',' ')],'FontSize',14)
    xlabel('Tamaño del problema (n)','FontSize',12)
    ylabel('Tiempo (segundos)','FontSize',12)

    legend('Datos','grado 2','potencial',...
           'grado 1','grado 3','grado 6',...
           'logarítmico','exponencial',...
           'Location','northwest')

    grid on
    set(gca,'FontSize',11)

    saveas(gcf, strcat('ajuste_',nombre,'.png'))

    hold off

end