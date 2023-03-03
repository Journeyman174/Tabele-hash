/* OLARU Gabriel-312CB */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "functii-LDI.h"

#ifndef _TAB_HASH_
#define _TAB_HASH_

typedef int (*TFElem)(void*);     /* functie prelucrare element */
typedef int (*TFCmp)(void*, char*); /* functie de comparare doua elemente */
typedef void (*TFF)(void*,FILE*);     /* functie afisare/eliberare un element */
typedef void (*TF)(void*);  
typedef int (*TFHash)(char*,int);

typedef struct
{
    int M;
    TFHash fh;
    TLDI *v;
} TH;

typedef struct 
{	
    char* nume;
    char* ip;
} TSite;


/* functii tabela hash */
TH* IniTH(int M, TFHash fh);
int InsTH(TH*a, void*ae, TFCmp f);
int ExistaTH(TH *h, char* ae, TFCmp fcmp, int M);
void DistrTH(TH *aa);
#endif