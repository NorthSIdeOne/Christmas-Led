all:
	gcc instalatie.c biblioteca.c -o leduri
build:
	./leduri Rezistente.in Leduri.in Cerinte.in Rezultate.out
clear:
	rm leduri