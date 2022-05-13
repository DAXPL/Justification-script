/*
Projekt zaliczeniowy Języki programowania
Miłosz Klim - Technologie Komputerowe Semestr II
16.04.2022
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//na razie bez polskich znakow
unsigned  int documentWidth=71;
char buffor[255];
char word[255];
unsigned  int bufforCounter=0;
unsigned  int wordLetters=0;
unsigned  int wordsInLine=0;

void lineBufforDump(bool last);
void addWordToLineBuffor();
FILE *file;
FILE *fout;

int main()
{
	while(1)
	{
		printf("Podaj żądaną szerokość dokumentu:\n");
		scanf("%u",&documentWidth);
		fflush(stdin);

		documentWidth=abs(documentWidth);
		printf("Szerokosc: %d\n",documentWidth);

		if(documentWidth==0)
		{
			printf("Niepoprawne wejście\n");
		}
		else
		{
			break;
		}
		
	}
	/*
	char fileName[255];
	while(1)
	{
		printf("Podaj nazwę dokumentu:\n");
		scanf("%254[^\n]", fileName);
		break;
	}
	fflush(stdin);
	*/
	
	file = fopen("t2.txt","r");
	fout = fopen("b2.txt","w+");

	if(file == NULL || fout==NULL)
	{
		printf("No file\n");
		return 0;
	}

	bool ignore=true;
	char c;
	while((c = fgetc(file)) != EOF)
	{
		if(c!=' ' && c!='\n' && c!='\t'&& c!=-17 && c!=-69 && c!=-65)
		{
			//wykryto poprawna litere

			//tutaj wykrywam niepoprawne litery
			if(c<0 || c>127)
			{
				printf("%d\t%c\n",c,c);
				c='?';
			}
			word[wordLetters]=c;
			wordLetters++;
			ignore=false;
		}else if(ignore==false)
		{
			//koniec slowa
			ignore=true;

			unsigned int newLength = wordLetters+bufforCounter;
			if(newLength>=documentWidth-1)
			{
				lineBufforDump(false);
			}
			addWordToLineBuffor();
		}
	}
	lineBufforDump(true);
	fclose(file);
	fclose(fout);
	return 0;
}

void lineBufforDump(bool last)
{
	//printf("%s\nznakow: %d\n slow: %d\n",buffor,bufforCounter,wordsInLine);
	//nie dopisujemy - podmieniamy
	//buffor counter  wskazuje NASTEPNA POZYCJE i LICZBE znakow w linii
	//addWordToLineBuffor na koncu dodaje zawsze dodatkowa spacje - usuwamy ja
	buffor[bufforCounter-1]=0;
	bufforCounter--;

	unsigned int spacesLeft = (documentWidth - bufforCounter);
	unsigned int additionalSpacesForAll = spacesLeft/(wordsInLine-1);
	unsigned int leftoverSpaces = spacesLeft%(wordsInLine-1);
	unsigned int writenWords = 0;
	//printf("Chars in line: %d, words %d \n", bufforCounter,wordsInLine);
	//printf("Spaces left: %d, additionalSpacesForAll %d, leftoverSpaces %d \n", spacesLeft,additionalSpacesForAll,leftoverSpaces);
	
	//zamiast algorytmu ponizej
	//fputs(buffor,fout);
	//fputs("\n",fout);
//Algorytm jest git - po prostu nie chce by na razie przeszkadzał

	for(int i=0;i<bufforCounter;i++)
	{
		char ch=buffor[i];
		fputc(ch,fout);
		if(ch==' ' && last==false)
		{
			for(int j=0;j<additionalSpacesForAll;j++)
			{
				fputc(' ',fout);
			}
			if(writenWords<leftoverSpaces)
			{
				fputc(' ',fout);
			}
			writenWords++;
		}	
	}
	//fputs(buffor,fout);
	if(last==false)
	{
		fputs("\n",fout);
	}


	memset(buffor,0,sizeof(buffor));
	bufforCounter=0;
	wordsInLine=0;
}
void addWordToLineBuffor()
{
	for(int i=0;i<wordLetters;i++)
	{
		buffor[bufforCounter]=word[i];
		bufforCounter++;
	}
	buffor[bufforCounter]=' ';
	bufforCounter++;
	
	wordsInLine++;
	wordLetters=0;

	memset(word,0,sizeof(word));
}