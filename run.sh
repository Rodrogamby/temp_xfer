echo "[1] Generando datos..."
cp _script.gnu script.gnu &&
./sim.o &&
echo "[2] Generando visuales..."
gnuplot "script.gnu"
echo "[3] Terminado."
echo "---------"
