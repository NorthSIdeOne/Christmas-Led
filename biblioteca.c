#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "biblioteca.h"

Instalatie Montaj(char *nume1,char *nume2,char *nume3)
{
	FILE *f;
	Instalatie I;



//								Citire din Rezistente.in
//---------------------------------------------------------------------------------
	if((f=fopen(nume1,"r"))==NULL)
	{
		printf("Nu s-a putut deschide\n");
		exit(1);
	}
	
	int i;

	fscanf(f,"%d",&I.nr_rezistente);//citire numarul de rezistente


	I.rezistor=malloc(sizeof(Rezistenta)*I.nr_rezistente);//alocare memorie rezistentelor


	for(i=0;i<I.nr_rezistente;i++)
	{
		fscanf(f,"%ms",&I.rezistor[i].nume);//citire eticheta rezistentei
		fscanf(f,"%d",&I.rezistor[i].scadere_intensitate);//citire valoare cu care scade curentul cu rezistenta respectiva
	}
	fclose(f);
	
//---------------------------------------------------------------------------------
//								Citire din Leduri.in

	if((f=fopen(nume2,"r"))==NULL)
	{
		printf("Nu s-a putut deschide\n");
		exit(1);
	}



			fscanf(f,"%d",&I.nr_leduri);//citire numarul de leduri
			fscanf(f,"%d",&I.intensitate_de_echilibru);//citire intensitatea la care trebuie aduse ledurile
			fscanf(f,"%d",&I.dif_de_intensitate);//diferenta de intensitate acceptata

//---------------------------------------------------------------------------------

	I.leduri=malloc(sizeof(Led)*I.nr_leduri);//alocare memorie pentru leduri


//---------------------------------------------------------------------------------


	for(i=0;i<I.nr_leduri;i++)
	{
		fscanf(f,"%ms",&I.leduri[i].led);//citire numele ledului
		fscanf(f,"%ms",&I.leduri[i].culoare);//citire culoarea ledului
		fscanf(f,"%d",&I.leduri[i].intensitate);//citire intensitatea ledului
	}

	fclose(f);
//---------------------------------------------------------------------------------
//								Citire Cerinte.in

		
			if((f=fopen(nume3,"r"))==NULL)
				{
					printf("Nu s-a putut deschide\n");
					exit(1);
				}


			fscanf(f,"%d",&I.vector_cerinte[0]);
			fscanf(f,"%d",&I.vector_cerinte[1]);
			fscanf(f,"%d",&I.vector_cerinte[2]);
			fscanf(f,"%d",&I.vector_cerinte[3]);

			fclose(f);
//---------------------------------------------------------------------------------


		
		return I;	
}

void cerinta_1(Instalatie I,char *nume_fisier)
{
	FILE *f;
	if((f=fopen(nume_fisier,"a"))==NULL)
	{
		printf("Nu s-a putut deschide\n");
		exit(1);
	}

	int max; //diferenta maxima dintre intensitati
	int nr=0;//numarul de aparitie a maximului
	int i;

//----------------------------------Initializarea maximului cu prima diferenta---------------------------------------------------------------------------------------------------

	if(I.leduri[0].intensitate-I.leduri[1].intensitate>=0)//daca diferenta este pozitiva ,max va fi diferenta celor doua intensitati
		{	
			max=I.leduri[0].intensitate-I.leduri[1].intensitate;
		 	nr++;//nuamrul de aparitie
		 }

	else //daca diferenta e negativa,max va fi diferenta dintre cele doua intensitati cu semn schimbat
		{	
			max=I.leduri[1].intensitate-I.leduri[0].intensitate;
			nr++;//nuamrul de aparitie
		}


//----------------------------------Aflarea maximului si numarul de aparitii pentru celelalte leduri---------------------------------------------------------------------------------------------------

	for(i=1;i<I.nr_leduri-1;i++)
	{
		if(I.leduri[i].intensitate-I.leduri[i+1].intensitate>max) //daca diferenta e pozitiva si mai mare ca max se adauga in max iar numarul de aparitie devine 1
				{
					max=I.leduri[i].intensitate-I.leduri[i+1].intensitate;
					nr=1;
				}
		else 
			if(I.leduri[i+1].intensitate-I.leduri[i].intensitate>max)//daca diferenta e pozitiva si mai mare ca max se adauga in max iar numarul de aparitie devine 1
			{
				max=I.leduri[i+1].intensitate-I.leduri[i].intensitate;
				nr=1;
			}
			else
				if(I.leduri[i].intensitate-I.leduri[i+1].intensitate==max||I.leduri[i+1].intensitate-I.leduri[i].intensitate==max) //daca diferenta este egala cu max,se incrementeaza numarul de aparitie
				{
					nr++;
				}

	}


//----------------------------------Scrierea in fisier a maximului---------------------------------------------------------------------------------------------------

	fprintf(f,"%d ",max);
	fprintf(f,"%d\n",nr);
	fclose(f);

}
Instalatie ordonare_rezistente(Instalatie I)//functie ce ordoneaza descrescator rezistentele pentru a putea folosi o metoda de tip Greedy
{
	int i,j;
	int aux;

//----------------------------------Ordonarea rezistentelor in ordine descrescatoare---------------------------------------------------------------------------------------------------

	for(i=0;i<I.nr_rezistente;i++)
	{
		for(j=i;j<I.nr_rezistente;j++)
		{
				if(I.rezistor[i].scadere_intensitate< I.rezistor[j].scadere_intensitate)
					{
						aux=I.rezistor[i].scadere_intensitate;
						I.rezistor[i].scadere_intensitate=I.rezistor[j].scadere_intensitate;
						I.rezistor[j].scadere_intensitate=aux;
					}			
		}
	}

//----------------------------------Returnarez I---------------------------------------------------------------------------------------------------------------------------------------

	return I;
}
void cerinta_3(Instalatie I,char *nume_fisier)
{	 FILE *f;

//----------------------------------Ordonarea rezistentelor in ordine descrescatoare-----------------------------------------------------------------------------------------------------------
	int i,j;
	int nr=0;//variabila ce o sa contina numarul minim de rezistente folosite
	int aux=0;//Memorez valoarea intesitatii temporar a ledurilor
	I=ordonare_rezistente(I);//ordoneaza descrescator rezistentele

//----------------------------------Cautarea numrului minim de rezistente ce pot fi eliminate---------------------------------------------------------------------------------------------------
	
	for(i=0;i<I.nr_leduri;i++)
	{	j=0;//indexul rezistentelor
		aux=I.leduri[i].intensitate;//stochez valoarea fiecarui led in aux
		while(aux>I.intensitate_de_echilibru)
		{	
			if(aux-I.rezistor[j].scadere_intensitate>=I.intensitate_de_echilibru)//daca cea mai maire rezistenta scazut din auxiliar este mai mare sau egala cu intensitatea ceruta il numar
			{
				nr++;//incrementez numarul de rezistente folosite 
				aux=aux-I.rezistor[j].scadere_intensitate;//scad din auxiliar valoarea cu care se modifica intensitatea dupa aplicarea rezistentei curente
			}
			else//daca nu este trec la urmatoarea rezistenta
			{
				j=j+1;//daca valoarea rezistentei curente este prea mare incrementez  indicele rezistentei pentru a folosi una mai mica
				if(j==I.nr_rezistente)//verific daca "j" a iesit din intervalul folosit,adica inseamna ca nu am rezistente disponibile pentru a aduce intensitatea la o intensitate de echilibru
				{
					nr=-1;//valoarea -1 inseamna ca nu am rezsistente necesare pentru aducerea intensitatii la valoarea ceruta
					i=I.nr_leduri;//conditie de iesire din for()
					aux=I.intensitate_de_echilibru;//conditie de iesire din while()
				}

			}

			

		}

	}
	if((f=fopen(nume_fisier,"a"))==NULL)
	{
		printf("Nu s-a putut deschide\n");
		exit(1);
	}

//----------------------------------Scrierea numarului minim de rezistente---------------------------------------------------------------------------------------------------
	
	fprintf(f,"%d",nr);//afisez nuamrul minim de rezistente in Rezultate.out
	fprintf(f,"\n");
	fclose(f);
	
}

void afisare(Instalatie I,char *nume_fisier)
{
	FILE *f;
	if((f=fopen(nume_fisier,"w"))==NULL) //deschid fisierul cu "w" pentru a sterge ce a ramas in fisierul Rezultate.out
	{
		printf("Nu s-a putut deschide\n");
		exit(1);
	}

	fclose(f);

	if((f=fopen(nume_fisier,"a"))==NULL)
	{
		printf("Nu s-a putut deschide\n");
		exit(1);
	}
//----------------------------------Cerinta 1---------------------------------------------------------------------------------------------------
	if(I.vector_cerinte[0]==1)//daca vectorul de cerinte inseamna ca cerinta este ceruta si va fi executata
	{
		cerinta_1(I,nume_fisier);
	}

//----------------------------------Cerinta 2---------------------------------------------------------------------------------------------------

	if(I.vector_cerinte[1]==1)
	{
		cerinta_2(I,nume_fisier);
	}

//----------------------------------Cerinta 3---------------------------------------------------------------------------------------------------

	if(I.vector_cerinte[2]==1)
	{
		cerinta_3(I,nume_fisier);
	}
//----------------------------------Cerinta 4---------------------------------------------------------------------------------------------------


	if(I.vector_cerinte[3]==1)
	{
		cerinta_4(I,nume_fisier);
	}
}

void cerinta_4(Instalatie I,char *nume_fisier)
{	
	FILE *f;
//----------------------------------Apelarea functie de permutari-------------------------------------------------------------------------------

	int nr=0;//variabila pentru stocarea numarului de moduri in care pot fi aranjate ledurile in conditiile date
	
	if((f=fopen(nume_fisier,"a"))==NULL)
	{
		printf("Nu s-a deschis\n");
		exit(1);
	}
	
	aranjament(I,0,I.nr_leduri,&nr);//functia pentru gasirea numarului de moduri in care pot fi aranjate ledurile in conditiile date
	fprintf(f,"%d\n",nr);//se scrie in fisier numarul d


	fclose(f);
	

}
int verificare_ordine(Instalatie I)//functie ce verifica daca doua leduri invecinate au aceeasi culoare si daca diferenta lor depaseste intensitatea impusa
{
	int i;
	int k=0;

//----------------------------------Verificarea culorii si a diferentei de intensitate----------------------------------------------------------

	for(i=0;i<I.nr_leduri-1;i++)
	{
		if(strcmp(I.leduri[i].culoare,I.leduri[i+1].culoare)!=0)//compara culoarea a doua leduri invecinate
		{		

			if(abs(I.leduri[i].intensitate-I.leduri[i+1].intensitate)<=I.dif_de_intensitate)//compara diferenta dintre ledurile invecinate
			{
				k++;//daca sunt indeplinite conditiile se incrementeaza k
				

			}
		}
		

	}
	
	
	if(k==I.nr_leduri-1)//daca k este egal cu numarul de leduri -1 inseamna ca toate ledurile indeplinesc conditia pentru ca sunt nr_leduri -1 verificari 
	{
		return 1;//se returneaza 1 pentru o verificare reusita
	}
	else
		return 0;//se returneaza 0 pentru esuarea verificarii
}
void aranjament(Instalatie I,int start,int finish,int *nr)//functie de backtracking in care se afla in cate moduri se pot aranja ledurile astfel incat sa indeplineasca si conditia
{
	
	int i;
//----------------------------------Verificarea unei permuatri----------------------------------------------------------

	if(start==finish)//daca startul ajunge sa fie egal cu sfarsitul inseamna ca s-a gasit o permuatre
	{ 

		if(verificare_ordine(I)==1)//se verifia daca combinatia indeplineste conditia ceruta
		{
			*nr=*nr+1;//daca combinatia gasita trece verificarea,nr va fi incrementat
		}
   }
//----------------------------------Cautarea unei permutari--------------------------------------------------------------------

	else//daca start!=finish
	{
		for(i=start;i<finish;i++)//incepem de la start
			{		I=swap(I,start,i);//schimbam ordinea elementelor.Schimbam pozitia ledului ce se afla pe start cu cea de pe i
					
					aranjament(I,start+1,finish,nr);//apelam functia recursiv si trimitem start+1 pentru a trece la urmatorul element

					I=swap(I,i,start); //schimbam elementele intre ele la pozitia initiala pentru a incerca noi permutari de la pozitia de start

			 
			}
		}
	
}
Instalatie swap(Instalatie I,int start,int finish)//functie in care sunt schimabte pozitiile a  2 elemente intre ele
{
//----------------------------------Schimbarea pozitie a doua elemente----------------------------------------------------------

	Led aux;
	aux=I.leduri[start];
	I.leduri[start]=I.leduri[finish];
	I.leduri[finish]=aux;

	return I;
	
}

void cerinta_2(Instalatie I,char *nume_fisier) //functie care afiseaza cate leduri trebuie scoase pentru a aveam leduri cu intesitatile in ordine descrescatoare
{	FILE *f;

	if((f=fopen(nume_fisier,"a"))==NULL)
	{
		printf("Nu s-a putut deschide\n");
		exit(1);
	}
	int *s,i,j,max=0;
	int valoarea_finala=0;
//----------------------------------Initializarea vectorului cu lungimile subsirurilor------------------------------------------------------------------

	s=malloc(sizeof(int)*I.nr_leduri);//alocam un vector in care sunt trecute numarul maxim de subsiruri
	for(i=0;i<I.nr_leduri;i++)
	{
		s[i]=1;
	}
//----------------------------------Gasirea si memorarea lungimilor de subsiruri descrescatoare----------------------------------------------------------

	for(i=1;i<I.nr_leduri;i++)
	{
		for(j=0;j<i;j++)
		{
			if(I.leduri[i].intensitate<I.leduri[j].intensitate&&s[i]<s[j]+1)//verificam nuamrul de subsiruri descrescatoare si le adaugam
				s[i]=s[j]+1;
		}
	}
//----------------------------------Cautarea celel mai mari lungim de subsir descrescator----------------------------------------------------------------

	for(i=0;i<I.nr_leduri;i++)
	{
		if(max<s[i])//cautam in vectorul temporar ce contine numarul de subsiruri descrescatoare
			max=s[i];
	}
//----------------------------------Afisarea numarului de rezistente ce trebuie eliminate----------------------------------------------------------------

	valoarea_finala=I.nr_leduri-max;//aceasta variabila contine numarul de leduri ce trebuie scoase.Se scade din nuamrul total de leduri,nr de elemente ce contine cel mai lung subsir de elemente descrescatoare
	free(s);
	fprintf(f,"%d\n",valoarea_finala);//se scrie in fisier numarul de rezistente ce trebuie eliminate
	fclose(f);
}
	

