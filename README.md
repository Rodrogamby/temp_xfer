## TE2009B Simulación del control de una placa caliente

###### Autor: Rodrigo Gamboa
###### Profesor: Dr. Alberto Bortoni
#### Descripción de los archivos
- **build.sh** -> Script que compila el programa en un archivo de salida "sim.out"
- **run.sh** -> Script que utiliza "sim.out" para generar gráficos.
- **_script.gnu** -> Template de script de Gnuplot para gráficos (usado por run.sh).
- **main.cpp** -> Instanciación de clases, loop principal y guardado de datos.
- **elements.cpp** -> Definiciones de las clases (Burner, Heatplate, ControlPolicy & derivados)
- **combo.cpp** -> Funciones útiles (imprimir matrices, log, convolution)
- **combo.h** -> Header file de combo.cpp

Para poder hacer sentido de la simulación debemos asumir que la placa se encuentra en un lugar con mucho viento a cierta temperatura constante, es decir, no está calentando nada más que a sí misma.
De igual forma, se considera que el material calentado "eléctricamente" tiene de alguna forma una muy buena conductividad térmica con el aire, por lo que pierde su temperatura igual de rápido que como la obtiene.

#### Controlador BangBang
Su histéresis fue de +- 2.5 grados en la simulación (temperatura ambiente 15grados, temperatura objetivo 24 grados, temperatura del material caliente 60grados), a diferencia del +- 1 programado. Esto sucede porque si bien la temperatura baja casi inmediatamente con el apagado del calentador, esta tarda un poco más en regresar cuando vuelve a encenderse. El periodo de oscilaciones es mostrado en la simulación.

#### Análisis del nuevo controlador
Este controlador tuvo una histéresis "real" de +-1.7grados, funciona de manera similar al BangBang, sin embargo la histéresis programada es "dinámica" de acuerdo a la temperatura ambiente y la pico del calentador. De igual forma, el calentador se enciende de nuevo aunque no se haya retrocedido hasta la temperatura de referencia (tiene un pequeño offset). El periodo de oscilaciones es mostrado en la simulación.

#### Ejecución del programa
Para poder compilar y ejecutar el programa, se requiere:
- **GCC** (preinstalado en GNU/Linux)
- **Gnuplot +5.4** (instalable desde el package manager)

Compilar y ejecutar simulación con
	`./build.sh && ./run.sh`

Archivos de salida:
- **a.dat**
- **b.dat**
- **z.dat**
- **heatplate.gif**
- **simulation.png**

#### Nota
*Para hacer más rápida la visualización se ajustaron los tiempos hacia la mitad, es decir, dead time a 1.5s y tiempo de carga a 2.5s.*
