#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "biblioteca.h"

int main(int argc, char *argv[])
{
	Instalatie I;

	I=Montaj(argv[1],argv[2],argv[3]);//crearea instalatiei
	afisare(I,argv[4]);//executarea cerintelor si scrierea in fisier a rezultatelor
	return 0;
}