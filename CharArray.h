#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//I suggest to increase this value for bigger files and/or computers with more RAM
#define BASE_ARRAY_SIZE_INCREMENT 16

struct CharArray
{
    //some letters like ŁŹŻĆÓ are coded by use of two chars
    //thats why i track chars count and real letters count
    //lastLetterFlag help me tracking real letters
    unsigned short int bufforCounter;
    unsigned short int realLetters;
    bool lastLetterFlag;

    //i have some troubles getting real current max content size, so for now i using that variable
    int arraySize;

    //
    char* content;
};

void CharArrayInit(struct CharArray *array);
void ExpandCharArray(struct CharArray *array);
void CharArrayClear(struct CharArray *array);
void PutLetter(struct CharArray *array,char c, int where);
void AddLetter(struct CharArray *array,char c);
char GetLetter(struct CharArray *array, int where);
void ClearArray(struct CharArray *array);

void CharArrayInit(struct CharArray *array)
{
    array->arraySize = BASE_ARRAY_SIZE_INCREMENT;
    array->content = calloc(array->arraySize, sizeof(char));

    if(array->content == NULL)
    {
        printf("Cant calloc memory!");
        exit(1);
    }
    ClearArray(array);
}
void ExpandCharArray(struct CharArray *array)
{
    
    array->arraySize +=BASE_ARRAY_SIZE_INCREMENT;
    //printf("New array size %d\n",array->arraySize);
    char* realocContent = realloc(array->content, array->arraySize * sizeof(char));
    if(realocContent==NULL)
    {
        printf("Cant realloc array!\n");
        exit(1);
    }else
    {
        array->content = realocContent;
    }
}
void CharArrayClear(struct CharArray *array)
{
    //printf("Freeing array memory");
    free(array->content);
}
void PutLetter(struct CharArray *array,char c, int where)
{
    array->content[where]=c;
}
void AddLetter(struct CharArray *array,char c)
{
    if(array->bufforCounter >= array->arraySize)
    {
        //printf("Potrzeba wiecej pamieci %d z %d\n",array->bufforCounter,array->arraySize);
        ExpandCharArray(array);
    }

    array->content[array->bufforCounter]=c;
    array->bufforCounter++;

    if((c<0 || c>127)&&array->lastLetterFlag==false)
    {
        array->lastLetterFlag=true;
    }else
    {
        array->lastLetterFlag=false;
        array->realLetters++;
    }
}
char GetLetter(struct CharArray *array, int where)
{
    return array->content[where];
}
void ClearArray(struct CharArray *array)
{
    memset(array->content,0,sizeof(array->content));
    array->bufforCounter = 0;
    array->realLetters = 0;
    array->lastLetterFlag = false;
}