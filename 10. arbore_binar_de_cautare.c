#include <stdio.h>
#include <malloc.h>

//CHEILE IN FISIER NU SE PUN IN ORDINE CRESCATOARE(TOATE NODURILE SE VOR CREA CA FII DREAPTA)
//ARBORE ECHILIBRAT - DIFERENTA MAXIM UN NIVEL INTRE SUBARBORI
//hw: functia de stergere

typedef struct {
	int cod;
	char* titlu;
	int nrAutori;
	float pret;
}carte;

typedef struct {
	carte info; //16B
	struct nodArbore* stanga, * dreapta; //8B
}nodArbore;

nodArbore* creareNod(carte c, nodArbore* stanga, nodArbore* dreapta) {
	nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));

	nou->info.cod = c.cod;
	nou->info.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->info.titlu, c.titlu);
	nou->info.nrAutori = c.nrAutori;
	nou->info.pret = c.pret;

	nou->dreapta = stanga;
	nou->stanga = dreapta;

	return nou;
}

nodArbore* inserareNod(nodArbore* radacina, carte c) {
	if (radacina != NULL) {
		if (c.cod < radacina->info.cod) {
			radacina->stanga = inserareNod(radacina->stanga, c);
			return radacina;
		}else
			if (c.cod > radacina->info.cod) {
				radacina->dreapta = inserareNod(radacina->dreapta, c);
				return radacina;
			}
			else {
				return radacina; //cand sunt egale
			}
	}
	else {
		return creareNod(c, NULL, NULL);//cream nod radacina cu functia de mai sus
	}
}

//R-S-D - preordine
void preordine(nodArbore* radacina) {
	if (radacina != NULL) {
		printf("\nCod=%d, Titlu=%s, Numar autori=%d, Pret=%5.2f\n", radacina->info.cod, radacina->info.titlu, radacina->info.nrAutori, radacina->info.pret);
		preordine(radacina->stanga);
		preordine(radacina->dreapta);
	}
}
//S-R-D - cheile sortate crescator
void inordine(nodArbore* radacina) {
	if (radacina != NULL) {
		inordine(radacina->stanga);
		printf("\nCod=%d, Titlu=%s, Numar autori=%d, Pret=%5.2f\n", radacina->info.cod, radacina->info.titlu, radacina->info.nrAutori, radacina->info.pret);
		inordine(radacina->dreapta);
	}
}
//S-D-R
void postordine(nodArbore* radacina) {
	if (radacina != NULL) {
		postordine(radacina->stanga);
		postordine(radacina->dreapta);
		printf("\nCod=%d, Titlu=%s, Numar autori=%d, Pret=%5.2f\n", radacina->info.cod, radacina->info.titlu, radacina->info.nrAutori, radacina->info.pret);
	}
}

void dezalocare(nodArbore* radacina) {
	//alegem care varianta ni se pare mai ok
	//preordine
	/*if (radacina != NULL) {
		nodArbore* st = radacina->stanga;
		nodArbore* dr = radacina->dreapta;
		free(radacina->info.titlu);
		free(radacina);
		dezalocare(st);
		dezalocare(dr);
	}*/
	//postordine
	if (radacina != NULL) {
		dezalocare(radacina->stanga);
		dezalocare(radacina->dreapta);
		free(radacina->info.titlu);
		free(radacina);
	}
}

void numarCartiAutori(nodArbore* radacina, int* nr, int prag) {
	if (radacina != NULL) {
		if (radacina->info.nrAutori > prag) {
			(*nr)++;
		}
		numarCartiAutori(radacina->stanga, nr, prag);
		numarCartiAutori(radacina->dreapta, nr, prag);
	}
}

void salvareCartiVector(nodArbore* radacina, carte* vect, int* nr, float pretPrag) {
	if (radacina != NULL) {
		//conditie pentru nod frunza - radacina->stanga==NULL && radacina->dreapta==NULL && ...
		if (radacina->info.pret > pretPrag) {
			vect[*nr].cod = radacina->info.cod;
			vect[*nr].titlu = (char*)malloc((strlen(radacina->info.titlu) + 1) * sizeof(char));
			strcpy(vect[*nr].titlu, radacina->info.titlu);
			vect[*nr].nrAutori = radacina->info.nrAutori;
			vect[*nr].pret = radacina->info.pret;
			(*nr)++;
		}
		salvareCartiVector(radacina->stanga, vect, nr, pretPrag);
		salvareCartiVector(radacina->dreapta, vect, nr, pretPrag);
	}
}

//LA TESTUL 2 NE DA SA FACEM FUNCTIA MAXIM
int maxim(int a, int b) {
	int max = a;
	if (max < b) {
		max = b;
		return max;
	}
}

int inaltimeArbore(nodArbore* radacina) {
	if (radacina != NULL) {
		return 1 + maxim(inaltimeArbore(radacina->stanga), inaltimeArbore(radacina->dreapta));
	}
	else {
		return 0;
	}
}

void main() {
	nodArbore* radacina = NULL;
	carte c;
	int nrCarti;
	char buffer[30];

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrCarti);
	for (int i = 0;i < nrCarti;i++) {
		fscanf(f, "%d", &c.cod);
		fscanf(f, "%s", buffer);
		c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.titlu, buffer);
		fscanf(f, "%d", &c.nrAutori);
		fscanf(f, "%f", &c.pret);
		radacina = inserareNod(radacina, c);
		free(c.titlu);
	}
	fclose(f);

	printf("\n------Preordine-------\n");
	preordine(radacina);
	printf("\n------Inordine-------\n");
	inordine(radacina);
	printf("\n------Postordine-------\n");
	postordine(radacina);

	printf("\n------Carti peste 2 autori-------\n");
	int nr = 0;
	numarCartiAutori(radacina, &nr, 2);
	printf("\nNumar carti autori=%d\n", nr);

	printf("\n------Salvare in vector-------\n");
	carte* vect = (carte*)malloc(nrCarti * sizeof(carte));
	int nrC = 0;
	salvareCartiVector(radacina, vect, &nrC, 110);
	for (int i = 0;i < nrC;i++) {
		printf("\nCod=%d, Titlu=%s, Numar autori=%d, Pret=%5.2f\n", vect[i].cod, vect[i].titlu, vect[i].nrAutori, vect[i].pret);
	}
	for (int i = 0;i < nrC;i++) {
		free(vect[i].titlu);
	}
	free(vect);
	int inaltime = inaltimeArbore(radacina);
	printf("\n------Inaltime arbore=%d-------\n", inaltime);
	dezalocare(radacina);
}