/* OLARU Gabriel-312CB */

/*  functii-tabHash.c - functii tabela hash */


#include "thash.h"

TH* IniTH(int M, TFHash fh)
{
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h) 
    {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TLDI *) calloc(M, sizeof(TLDI));
    if(!h->v) 
    {
        printf("eroare alocare vector de pointeri TLDI in hash\n");
        free(h);
        return NULL;
    }

    h->M = M;
    h->fh = fh;
    return h;
}
int  ExistaTH(TH *h, char* ae, TFCmp fcmp, int M)
//intoarce 0 daca exista valoarea in lista
{
    int cod = h->fh(ae, M);
    
    TLDI p, s;
    s = h->v[cod];
    if(s == NULL)
        return 1;
    if(s->urm == s)
        return 1;
 
    for(p = s->urm; p != s; p = p->urm)
    {
        if( fcmp(((TSite *)p->info), ae) == 1)
            return 0;
    }
    return 1;
}

void DistrTH(TH * h)
{
	TLDI s;
	int i;
 //parcurgere cu pointeri
   	for(i = 0; i < h->M; i++)
	{
       	s = h->v[i];
//daca exista elemente corespunzatoare acestui hash
//eliberam info din celula si apoi eliberam celula
	   	if(s != NULL)
		    DistrLDI(&s);
	 }
    free(h->v);
    free(h);
    h = NULL;
}


