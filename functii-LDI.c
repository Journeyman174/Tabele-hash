/* OLARU Gabriel-312CB */

/*--- functii-LDI.c ---
      operatii lista generica dublu inlantuita, circulara, cu santinela ---*/

#include "functii-LDI.h"
#include "thash.h"
TLDI InitLDI()
    /* -> lista vida, cu info din santinela specifica aplicatiei */
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
    if (!aux) return NULL;

	aux->pre = aux->urm = aux;
    aux->info = NULL;

	return aux;     /* rezultatul este adresa santinelei sau NULL */
}

int InsDupa(TLDI a, void* x)  /* inserare dupa celula cu adresa a -> 1/0 */
{
	
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if(!aux) return 0;
	aux->info = x;
	aux->pre = a;
	aux->urm = a->urm;
	a->urm->pre = aux;
	a->urm = aux;
    return 1;
}
void ResetLDI(TLDI s)
     /* transforma lista in lista vida */
{
	TLDI p = s->urm, aux;
	while(p != s)
	{
		aux = p;
		p = p->urm;
		free(((TSite *)aux->info)->nume);
		free(((TSite *)aux->info)->ip);
		free(aux->info);
		free(aux);
	}
	s->urm = s->pre = s;
}

void DistrLDI(TLDI* as)
     /* elibereaza tot spatiul ocupat de lista, inclusiv santinela */
{
	ResetLDI(*as);
	free(*as);
	*as = NULL;
}
