/* OLARU Gabriel-312CB */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functii-LDI.h"
#include "thash.h"


//functia hash
int hashf(char * nume, int M)
{
	int i = 0, s = 0;
	for(i = 0 ; *(nume) !='\0' ; i++)  //citesc caracter pana la sfarsitul sirului ('\0')
	s = s + *(nume++);
	return s%M;
}

//functie de comparare nume site 
int cmpSite(void * e1, char * e2)
{
	TSite * site1 = (TSite *) e1;
	if (strcmp(site1->nume, e2) == 0) // daca sirurile sunt identice intorc 1
		return 1;
	else
		return 0;	//daca sirurile NU sunt identice intorc 0
}

//se foloseste la put ordonare dupa cheie in lista
int ordSite(void * e1, char * e2)
{
	TSite * site1 = (TSite *) e1;
	if (strcmp(site1->nume, e2) > 0) //daca sirul 1 din lista este mai mare "alfabetic" ca sirul 2 nume site 
									//intorc 1 
		return 1;
	else
		return 0;
}


void afisareSite(void * element, FILE *f_out)
{
	TSite * site = (TSite *) element;
	fprintf(f_out,"%s ",  site->ip); 
}

//functia put calculeaza codul hash corespunzator functiei hash
//(restul impartirii sumei caracterelor ce fac parte din numele site-ului-key la
// nr. max. de bucket M 
//insereaza crescator in acelasi bucket numele pag. web si adresa ip

void put(TH * h, char * nume, char * ip, int M, TFCmp fcmp)
{
	TLDI p, l, s;

	TSite * site = malloc(sizeof(TSite));
	if (site == NULL)
		return;

	site->nume = strdup(nume);
	site->ip = strdup(ip);

	int cod = h->fh(nume,M);

	if(h->v[cod] == NULL)
	{
		l = InitLDI();
		h->v[cod] = l;
	}
	s = h->v[cod];
	for(p = s; p->urm != s  && fcmp(p->urm->info,site->nume) == 0; p = p->urm);
	InsDupa(p,site);
}

//folosita la operatia remove
void  elimin(TH * h, char* ae, TFCmp fcmp, int M)
{
	int cod = h->fh(ae,M);
    TLDI p, s;
    s = h->v[cod];
    if(s == NULL)
        return ;
    if(s->urm == s)
        return ;
 //parcuge lista 
	for(p = s->urm; p != s; p = p->urm)
	{
		if( fcmp(p->info,ae) == 1) // conditia de eliminare
		{
			//elementul de eliminat va fi p 
			p->pre->urm = p->urm;
			p->urm->pre = p->pre;
			free(((TSite *)p->info)->nume);	//eliberez memoria alocata structurii TSite
			free(((TSite *)p->info)->ip);
			free(p->info); // eliminarem mem alocata celula
			free(p);
			return;
		}
	}
	return ; // nu am gasit element eliminat
}

// functia get
void * get(TH * h,char* ae, TFCmp fcmp, int M)
{
	int cod = h->fh(ae,M); // calculez indicele bucket-ului
    TLDI p, s;
    s = h->v[cod];
    if(s == NULL)
        return NULL;
    if(s->urm == s)
        return NULL;
 //parcuge lista 
  	for(p = s->urm; p != s; p = p->urm)
    {
        if( fcmp(((TSite *)p->info),ae) == 1) // conditia de identificare  
		{
			TSite * site = (TSite *)p->info;
			return site->ip;
		}
	}
	return NULL;
}

// functia print care scrie in fisierul de iesire valorile IP-urilor din hash table
void print(TH * h, TFF afiEl, FILE *f_out)
{
	TLDI s, p;
	int i;
   	for(i = 0; i < h->M; i++)
	{
       	s = h->v[i];
	   	if(s != NULL)
		{
			if (s != s->urm)
			{	
    		fprintf(f_out,"%d: ", i);
	   		for(p = s->urm; p != s; p = p->urm)
	        	afiEl(p->info, f_out);
			fprintf(f_out,"\n");
			}
		}
	 }

}

// scrie in fisierul de iesire valorile din bucket-ul cu indicele index
void print_bucket(TH * h, int index, TFF afiEl, FILE * f_out)
{
	TLDI s, p;
	int i = index;
	s = h->v[i];
	if(s == NULL || s == s->urm)
			fprintf(f_out,"%s","VIDA");
	else
	{	
	   	for(p = s->urm; p != s; p = p->urm)
	        afiEl(p->info,f_out);
	}
    fprintf(f_out,"\n");
}

//tema1 - Program principal  
int main(int argc, char* argv[])
{
    int index;
	int M;
    FILE * f_in, *f_out; //argumente 2 si 3 linie comanda
    char* line = NULL;
    char *numefis_in, *numefis_out;
    size_t len = 0;
    M = atoi(argv[1]); //argument 1

//aici se construieste o tabela hash cu M elemente
//initializez prima santinela Lista situri din vector Tabela Hash	
	TH * h = NULL;
	
//initializare tabela hash
	h = (TH *) IniTH(M, hashf);
	if(h == NULL)
		return 1;

    numefis_in = argv[2];
    numefis_out = argv[3];
    char *cmd1 = "put";
    char *cmd2 = "get";
    char *cmd3 = "remove";
    char *cmd4 = "find";
    char *cmd5 = "print";
    char *cmd6 = "print_bucket";
	char *rez_find;
	char *rez_get;
    
  //deschid fis de intrare si fis de iesire 
    f_in = fopen(numefis_in,"rt");
    if(f_in == NULL)
    	return 1;
	 f_out = fopen(numefis_out,"w+");
    if(f_out == NULL)
    	return 1;
    	
    while (getline(&line,&len,f_in) != -1 && strlen(line) != 1)
    	{
    		if (line[strlen(line) - 1] =='\n')
    			line[strlen(line) - 1] = ' '; // se adauga un spatiu la sfarsit pentru parsare corecta

  		char * cmd = strtok(line," ");
  		char * key = strtok(NULL," ");
		char * val = strtok(NULL," ");
//cautare comenzi din fisier hash.in
//comanda put
 			if (strcmp(cmd, cmd1) == 0)
			{
				if(ExistaTH(h, key, cmpSite, M) != 0)
					put(h, key, val, M, ordSite);
			}

//comanda get
			if (strcmp(cmd, cmd2) == 0)
			{
				rez_get=(char *)get(h, key, cmpSite, M);
				if(rez_get == NULL)
					fprintf(f_out, "%s\n", "NULL");
				else
					fprintf(f_out, "%s\n", rez_get);
			}

//comanda remove
 			if (strcmp(cmd, cmd3) == 0)
				elimin(h, key, cmpSite, M);
			
//comanda find
 			if (strcmp(cmd, cmd4) == 0)
		 	{
				if(ExistaTH(h, key, cmpSite, M) != 0)
					rez_find = "False";
				else
					rez_find = "True";
			fprintf(f_out, "%s\n", rez_find);
			}

//comanda print
 			if (strcmp(cmd, cmd5) == 0 )
				print(h, afisareSite, f_out); 

//comanda print_bucket
 			if (strcmp(cmd, cmd6) == 0)
 			{
 				index = atoi(key);

			 	if(index < M) //verific daca index bucket este < M in caz contarar nu fac nimic
					print_bucket(h, index, afisareSite, f_out);
    		}
    	}
	free(line);	//eliberez buffer citire din fisier intrare
	DistrTH(h);	//eliberez mem alocata tabelei de dispersie si bucket-urilor 
    fclose(f_in);	//inchid fisierele de intare si iesire
	fclose(f_out);
    return 0;
}
