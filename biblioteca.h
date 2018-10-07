#ifndef MY_LIBRARY
#define MY_LIBRARY

typedef struct led//structura pentru leduri
{
	char *led;//numele ledului
	char *culoare;//culoarea ledului
	int intensitate;//intensitatea ledului
	
}Led;
typedef struct rezistente//structura pentru rezistente
{
	char *nume;//numlele de rezistentei
	int scadere_intensitate;//valoarea cu care rezistenta scade intensitatea unui led
}Rezistenta;

typedef struct instalatie//structura instalatie ce cuprinde:rezistentele,ledurile si cerintele
{
	Led *leduri;
	Rezistenta *rezistor;
	int nr_rezistente;//nuamrul de rezistente din instalatie
	int nr_leduri;//nuamrul de leduri din instalatie
	int nr_cerinte;//numarul de cerinte
	int vector_cerinte[4];//vectorul in care se retin valorile cerintele:1 pentru executa si 0 pentru nu se executa
	int intensitate_de_echilibru;//intensitatea la care trenuie aduse intensitatile ledurilor
	int dif_de_intensitate;//diferenta maxima de intensitate ceruta

	
}Instalatie;

Instalatie Montaj(char *nume1,char *nume2,char *nume3);//functie de creare a instalatiei
Instalatie ordonare_rezistente(Instalatie I);//functie pentru ordonarea rezistentelor in ordine descrescatoare
void cerinta_1(Instalatie I,char *nume_fisier);//functie pentru cerinta 1
void cerinta_2(Instalatie I,char *nume_fisier);//functie pentru cerinta 2
void cerinta_3(Instalatie I,char *nume_fisier);;//functie pentru cerinta 3
void cerinta_4(Instalatie I,char *nume_fisier);;//functie pentru cerinta 4
void afisare(Instalatie I,char *nume_fisier);//functie de afisare si executare a cerintelor
void aranjament(Instalatie I,int start,int finish,int *nr);//functie aflarea numarului de permutari a ledurilor in functie de conditiile impuse
int verificare_ordine(Instalatie I);//functie ce verifica daca o permutare verifica conditia impusa
Instalatie swap(Instalatie I,int start,int finish);//functie ce schimba pozitia a doua elemente


#endif