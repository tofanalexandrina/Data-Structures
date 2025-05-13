#include <stdio.h>
#include <malloc.h>

typedef struct {
	int cod;
	char* titlu;
	int nrAutori;
	float pret;
}carte;

typedef struct {
	int BF; //balance factor
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
		}
		else
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
	//postordine
	if (radacina != NULL) {
		dezalocare(radacina->stanga);
		dezalocare(radacina->dreapta);
		free(radacina->info.titlu);
		free(radacina);
	}
}

int maxim(int a, int b) {
	int max = a;
	if (max < b) {
		max = b;
	}
	return max;
}

int inaltimeArbore(nodArbore* radacina) {
	if (radacina != NULL) {
		return 1 + maxim(inaltimeArbore(radacina->stanga), inaltimeArbore(radacina->dreapta));
	}
	else {
		return 0;
	}
}


void calculBF(nodArbore* radacina) {
	//functie de parcurgere arbore, in care setam BF=h(SD)-h(SS)
	if (radacina != NULL) {
		radacina->BF = inaltimeArbore(radacina->dreapta) - inaltimeArbore(radacina->stanga);
		calculBF(radacina->stanga);
		calculBF(radacina->dreapta);
	}
}

nodArbore* rotatieDreapta(nodArbore* radacina) {
	printf("\nRotatie dreapta\n");
	nodArbore* nod1 = radacina->stanga;
	radacina->stanga = nod1->dreapta;
	nod1->dreapta = radacina;
	radacina = nod1;
	return radacina;

}

nodArbore* rotatieStanga(nodArbore* radacina) {
	printf("\nRotatie stanga\n");
	nodArbore* nod1 = radacina->dreapta;
	radacina->dreapta = nod1->stanga;
	nod1->stanga = radacina;
	radacina = nod1;
	return radacina;

}

nodArbore* rotatieDreaptaStanga(nodArbore* radacina) {
	printf("\nRotatie dreapta-stanga\n");
	nodArbore* nod1 = radacina->dreapta;
	nodArbore* nod2 = nod1->stanga;
	nod1->stanga = nod2->dreapta;
	nod2->dreapta = nod1;
	radacina->dreapta = nod2->stanga;
	nod2->stanga = radacina;
	radacina = nod2;
	return radacina;
}

nodArbore* rotatieStangaDreapta(nodArbore* radacina) {
	printf("\nRotatie stanga-dreapta\n");
	nodArbore* nod1 = radacina->stanga;
	nodArbore* nod2 = nod1->dreapta;
	nod1->dreapta = nod2->stanga;
	nod2->stanga = nod1;
	radacina->stanga = nod2->dreapta;
	nod2->dreapta = radacina;
	radacina = nod2;
	return radacina;
}

nodArbore* reechilibrare(nodArbore* radacina) {
	calculBF(radacina);
	nodArbore* stanga = radacina->stanga;
	nodArbore* dreapta = radacina->dreapta;
	if (radacina->BF == -2 && stanga->BF == -1) {
		radacina = rotatieDreapta(radacina);
		calculBF(radacina);
	}
	else {
		if (radacina->BF == 2 && dreapta->BF == 1) {
			radacina = rotatieStanga(radacina);
			calculBF(radacina);
		}
		else {
			if (radacina->BF == 2 && dreapta->BF == -1) {
				radacina = rotatieDreaptaStanga(radacina);
				calculBF(radacina);
			}
			else if (radacina->BF == -2 && stanga->BF == 1) {
				radacina = rotatieStangaDreapta(radacina);
				calculBF(radacina);
			}
		}
	}
	return radacina;
}


void main() {
	nodArbore* radacina = NULL;
	carte c;
	int nrCarti;
	char buffer[30];

	FILE* f = fopen("file.txt", "r");
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

		radacina = reechilibrare(radacina);
	}
	fclose(f);

	printf("\n------Preordine-------\n");
	preordine(radacina);
	printf("\n------Preordine Stanga-------\n");
	preordine(radacina->stanga);
	printf("\n------Preordine Dreapta-------\n");
	preordine(radacina->dreapta);
	printf("\n------Inaltime subarbore stang = %d ---------\n", inaltimeArbore(radacina->stanga));
	printf("\n------Inaltime subarbore drept = %d ---------\n", inaltimeArbore(radacina->dreapta));

	int inaltime = inaltimeArbore(radacina);
	printf("\n------Inaltime arbore=%d-------\n", inaltime);

	dezalocare(radacina);
}