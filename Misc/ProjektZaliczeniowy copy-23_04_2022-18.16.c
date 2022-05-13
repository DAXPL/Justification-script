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

#define WORD_BUFFOR_LEN 1024
#define LINE_BUFFOR_LEN 1024
#define FILE_OUTPUT_NAME "b1.txt"
//na razie bez polskich znakow

char buffor[LINE_BUFFOR_LEN];
char word[WORD_BUFFOR_LEN];
char *openedFileName;

unsigned short int bufforCounter=0;
unsigned short int wordLetters=0;
unsigned short int wordsInLine=0;
unsigned short int documentWidth=0;

void LineBufforDump(bool last);
void AddWordToLineBuffor();
void AddCharToWord(char _c);
bool isCharBelongToWord(char _c);
void GetDocumentWidth();
void GetDocumentName();
void OpenOutputFile();

FILE *file;
FILE *fout;

int main(int argc, char *argv[])
{
	GetDocumentWidth();
	GetDocumentName();
	OpenOutputFile();
	
	bool ignore=true;
	char c;
	while((c = fgetc(file)) != EOF)
	{
		if(isCharBelongToWord(c))
		{
			AddCharToWord(c);
			ignore=false;
		}
		else if(ignore==false)
		{
			//wykryto spację lub inny znak sugerujący koniec słowa
			ignore=true;

			unsigned int newLength = wordLetters+bufforCounter;
			if(newLength>=documentWidth-1)
			{
				LineBufforDump(false);
			}
			AddWordToLineBuffor();
		}
	}
	printf("Last dump\n");
	LineBufforDump(true);

	printf("Ending\n");
	fclose(file);
	fclose(fout);

	printf("Success\n");
	return 0;
}
void GetDocumentWidth()
{
	while(1)
	{
		printf("Enter wanted width of the document:\n");
		scanf("%u",&documentWidth);
		fflush(stdin);

		documentWidth=abs(documentWidth);
		
		if(documentWidth==0 || documentWidth>WORD_BUFFOR_LEN)
		{
			printf("Incorrect document width\n");
			printf("Try again:\n");
		}
		else
		{
			break;
		}
	}
}
void GetDocumentName()
{
	while(1)
	{
		printf("Enter name of the document:\n");
		char *filename;
		scanf("%s",filename);
		file = fopen(filename,"r");
		if(file==NULL)
		{
			printf("Document doesnt exist!\n");
		}
		else
		{
			openedFileName=filename;
			break;
		}
	}
}
void OpenOutputFile()
{
	char *outputName;
	strcpy(outputName,openedFileName);
	outputName[strlen(outputName)-4]='\0';
	printf("%s\n",outputName);
	strncat(outputName,"_justed.txt",12);
	
	printf("%s\n",outputName);
	fout = fopen(outputName,"w+");

	if(fout==NULL)
	{
		printf("Can't' create new file!\n");
		exit(1);
	}
}
void AddCharToWord(char _c)
{
	//wykryto znak który należy do aktualnie tworzonego słowa
	//zostanie on spisany do buforu słowa

	if(_c<0 || _c>127)
	{
		//tutaj wykrywam niepoprawne litery
		printf("%d\t%c\n",_c,_c);
		_c='?';
	}

	word[wordLetters]=_c;
	wordLetters++;
}
void AddWordToLineBuffor()
{
	for(int i=0;i<wordLetters;i++,bufforCounter++)
	{
		buffor[bufforCounter]=word[i];
	}

	buffor[bufforCounter]=' ';
	bufforCounter++;
	
	wordsInLine++;
	wordLetters=0;

	memset(word,0,sizeof(word));
		
	//co jeżeli słowo jest większe od buffora albo samej linii?
}
void LineBufforDump(bool last)
{
	//nie dopisujemy - podmieniamy
	//buffor counter  wskazuje NASTEPNA POZYCJE i LICZBE znakow w linii
	//addWordToLineBuffor na koncu dodaje zawsze dodatkowa spacje - usuwamy ja
	printf("Dumping\n");
	if(bufforCounter<=0)
	{
		printf("Empty\n");
		return;
	}
	bufforCounter--;
	buffor[bufforCounter]='\0';

	unsigned int spacesLeft = (documentWidth - bufforCounter);
	unsigned int additionalSpacesForAll = spacesLeft/(wordsInLine-1);
	unsigned int leftoverSpaces = spacesLeft%(wordsInLine-1);
	unsigned int writenWords = 0;

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
	if(last==false)
	{
		fputs("\n",fout);
	}

	printf("Cleaning after dump\n");
	memset(buffor,0,sizeof(buffor));
	bufforCounter=0;
	wordsInLine=0;
}
bool isCharBelongToWord(char _c)
{
	return(_c!=' ' && _c!='\n' && _c!='\t'&& _c!=-17 && _c!=-69 && _c!=-65);
}