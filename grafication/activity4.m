% ====================================================================
% SCRIPT DE GENERACIÓN DE GRÁFICAS DE COMPLEJIDAD (MATLAB)
% Autores: 
%         Garcia Peñalva Saul 
%         López Alvarado Daniel
%         Olarte Tomas Kevin Saul
% Fecha de entrega 18 de Marzo del 2025
% Version: 2.0
% 
% DESCRIPCION: Este script automatiza la generación de gráficas de 
% rendimiento para cada algoritmo evaluado. Lee el archivo general de 
% resultados ("datos_resultados.csv"), separa la información por algoritmo, 
% ordena estrictamente los datos respecto al tamaño del problema (n) y 
% traza las curvas de crecimiento (Tiempo Real, CPU y E/S vs n). 
% Finalmente, exporta cada gráfica automáticamente como una imagen .png 
% lista para ser incrustada en el reporte final.
%
% OBSERVACIONES: El archivo "datos_resultados.csv" debe encontrarse en 
% el mismo directorio que este script.
%
% ====================================================================
% INSTRUCCIONES DE EJECUCIÓN
% ====================================================================
% 1. Abrir MATLAB o GNU Octave.
% 2. Navegar hasta la carpeta donde se encuentra este script.
% 3. Ejecutar el script presionando "Run" o desde la "Command Window".
% ====================================================================

% Limpieza inicial del entorno de trabajo
clc
clear
close all

% ================================
% 1. Cargar datos
% ================================
% Lee la tabla completa generada por el script de bash en Linux
datos = readtable('datos_resultados.csv');

% Extrae una lista de los nombres de los algoritmos sin repetirlos
algoritmos = unique(datos.Algoritmo);

% ================================
% 2. Generación de Gráficas (PUNTO 4 - MEJORADO)
% ================================

% Bucle para procesar y graficar cada algoritmo individualmente
for i = 1:length(algoritmos)

    nombre = algoritmos{i};

    % Filtrar las filas que corresponden únicamente al algoritmo actual
    idx = strcmp(datos.Algoritmo, nombre);

    % Extraer las columnas correspondientes a ese algoritmo
    n = datos.n(idx);
    tiempo_real = datos.TiempoReal(idx);
    tiempo_cpu = datos.TiempoCPU(idx);
    tiempo_es = datos.TiempoES(idx);

    % 🔹 ORDENAR POR n (IMPORTANTÍSIMO)
    % Garantiza que la línea de la gráfica fluya de izquierda a derecha 
    % sin devolverse, en caso de que el CSV no esté ordenado por tamaño.
    [n, orden] = sort(n);
    tiempo_real = tiempo_real(orden);
    tiempo_cpu  = tiempo_cpu(orden);
    tiempo_es   = tiempo_es(orden);

    % Crear una nueva ventana de figura para el algoritmo actual
    figure

    % Trazar las 3 líneas de tiempo con distintos marcadores
    plot(n, tiempo_real, '-o', 'LineWidth', 2)
    hold on % Mantiene la gráfica activa para superponer las siguientes líneas
    plot(n, tiempo_cpu, '-s', 'LineWidth', 2)
    plot(n, tiempo_es, '-^', 'LineWidth', 2)

    % Personalización estética (Títulos, etiquetas y leyenda)
    % Nota: strrep elimina los guiones bajos (ej. "bubble_sort" -> "bubble sort")
    title(['Ordenamiento ', strrep(nombre,'_',' ')],'FontSize',14)
    xlabel('Tamaño del problema (n)','FontSize',12)
    ylabel('Tiempo (segundos)','FontSize',12)

    legend('Tiempo real','Tiempo CPU','Tiempo E/S','Location','northwest')

    grid on
    set(gca,'FontSize',11)

    % Guardar gráfica automáticamente en la carpeta actual
    nombre_archivo = strcat('grafica_',nombre,'.png');
    saveas(gcf,nombre_archivo)

end