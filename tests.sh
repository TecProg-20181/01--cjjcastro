# Compilando o código
gcc main.c -o photopobre
# Testando o método inverter cores
cat lena/lena.ppm lena/inverter_cores.txt | ./photopobre > saida.ppm
echo "Inverter cores:"
diff lena/inverter_cores.ppm saida.ppm
# Testando o método blur 15
cat lena/lena.ppm lena/blur_15.txt | ./photopobre > saida.ppm
echo "Blur 15:"
diff saida_blur.ppm saida.ppm

cat lena/lena.ppm lena/cortar.txt | ./photopobre > saida.ppm
echo "Cortar:"
diff lena/cortar.ppm saida.ppm

cat lena/lena.ppm lena/escala_cinza.txt | ./photopobre > saida.ppm
echo "Escala cinza:"
diff lena/escala_cinza.ppm saida.ppm

cat lena/lena.ppm lena/espelhamento_vertical.txt | ./photopobre > saida.ppm
echo "Espelhamento vertical:"
diff lena/espelhamento_vertical.ppm saida.ppm

cat lena/lena.ppm lena/rotacao_90.txt | ./photopobre > saida.ppm
echo "Rotacao 90:"
diff lena/rotacao_90.ppm saida.ppm

cat lena/lena.ppm lena/sepia.txt | ./photopobre > saida.ppm
echo "Sepia:"
diff lena/sepia.ppm saida.ppm