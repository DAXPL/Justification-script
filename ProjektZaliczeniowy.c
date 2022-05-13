/*
Projekt zaliczeniowy - Języki programowania
Miłosz Klim - Technologie Komputerowe Semestr II
7.05.2022
----------
Należy napisać program czytajacy plik tekstowy .txt
i justujący zawarty w nim tekst w zależności od podanego
parametru ilości znaków do justowania.
----------
Kiedy justujesz tekst, dodawane są odstępy między wyrazami tak, 
aby obie krawędzie wierszy były wyrównane do obu marginesów. Ostatni wiersz akapitu jest wyrównany do lewej.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "CharArray.h"

bool firstLine=true;

struct CharArray mainBuffor;
struct CharArray wordBuffor;
char *openedFileName;

unsigned short int wordsInLine=0;
unsigned short int documentWidth=0;

FILE *file;
FILE *fout;

void LineBufforDump(bool last,bool endOfParagraph);
void AddWordToLineBuffor();
bool isCharBelongToWord(char _c);
void GetDocumentWidth();
void GetDocumentName();
void OpenOutputFile();
bool isCharBelongToWord(char _c);

int main(int argc, char *argv[])
{
	CharArrayInit(&mainBuffor);
	CharArrayInit(&wordBuffor);

	GetDocumentWidth();
	GetDocumentName();
	OpenOutputFile();
	printf("Configuration complete!\n");
	printf("Starting doing stuff\n");
	bool ignore=true;
	char c;
	while(1)
	{
		c = fgetc(file);
		if(isCharBelongToWord(c))
		{
			//character which can belong to word
			AddLetter(&wordBuffor,c);
			ignore=false;
		}
		else if(ignore==false)
		{
			//space or other character suggesting end of the word
			ignore=true;
			unsigned short int newLength = wordBuffor.realLetters+mainBuffor.realLetters;
			if(newLength>=documentWidth-1)
			{
				//When next word cant fit into line, line is dumped to file, and then word is added to buffer
				LineBufforDump(false,false);
				AddWordToLineBuffor();
			}
			else
			{
				//Line is dumped when sees EOL sign
				if(c=='\n' || c=='\r' || c==EOF)
				{
					AddWordToLineBuffor();
					LineBufforDump(false,true);
				}
				else
				{
					AddWordToLineBuffor();
				}
			}			
		}
		//Finishing file
		if(c==EOF)
		{
			LineBufforDump(true,true);
			break;
		}
	}

	printf("Ending\n");
	//clearing mess
	fclose(file);
	fclose(fout);
	CharArrayClear(&mainBuffor);
	CharArrayClear(&wordBuffor);
	printf("Success\n");
	return 0;
}
void GetDocumentWidth()
{
	//getting wanted document width from user
	while(1)
	{
		printf("Enter wanted width of the document:\n");
		scanf("%u",&documentWidth);
		fflush(stdin);

		documentWidth=abs(documentWidth);
		
		if(documentWidth==0)
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
	//getting name of wanted document
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
	strncat(outputName,"_justed.txt",12);
	
	fout = fopen(outputName,"w");

	if(fout==NULL)
	{
		printf("Can't' create new file!\n");
		exit(1);
	}
	printf("Output file will be saved as %s\n",outputName);
}
void AddWordToLineBuffor()
{
	for(int i=0;i<wordBuffor.bufforCounter;i++)
	{
		AddLetter(&mainBuffor,GetLetter(&wordBuffor,i));
	}

	AddLetter(&mainBuffor,' ');
	
	wordsInLine++;

	ClearArray(&wordBuffor);
}
void LineBufforDump(bool last,bool endOfParagraph)
{
	if(mainBuffor.bufforCounter<=0)
	{
		return;
	}
	if(firstLine==false)
	{
		fputs("\n",fout);
	}
	firstLine=false;
	mainBuffor.bufforCounter--;
	mainBuffor.content[mainBuffor.bufforCounter]='\0';
	int spacesLeft = (documentWidth - mainBuffor.realLetters);
	if(spacesLeft>0 && endOfParagraph==false)
	{
		unsigned int denominator=wordsInLine-1;
		if(denominator<=0)
		{
			denominator=1;
		}
		unsigned int additionalSpacesForAll = spacesLeft/denominator;
		unsigned int leftoverSpaces = spacesLeft%denominator;
		unsigned int writenWords = 0;

		for(int i = 0;i<mainBuffor.bufforCounter;i++)
		{
			char ch=GetLetter(&mainBuffor,i);
			fputc(ch,fout);
			if(ch==' ')
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
	}
	else
	{
		fputs(mainBuffor.content,fout);
	}
	ClearArray(&mainBuffor);
	wordsInLine=0;
}
bool isCharBelongToWord(char _c)
{
	return(_c!=' ' && _c!='\n' && _c!='\r' && _c!='\t'&& _c!=-17 && _c!=-69 && _c!=-65 && _c!=EOF);
}