#include <stdio.h>
#include <malloc.h>

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

//LA TESTUL 2 NE DA SA FACEM FUNCTIA MAXIM
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

nodArbore* stergeRadacina(nodArbore* radacina) {
	nodArbore* aux = radacina;
	//daca am subarbore stang, noua radacina va fi copil stanga
	//legare noduri intre ele si setare noua radacina
	if (aux->stanga != NULL) {
		radacina = aux->stanga;

		if (aux->dreapta != NULL) {
			nodArbore* temp = aux->stanga;
			while (temp->dreapta != NULL) {
				temp = temp->dreapta;
			}
			temp->dreapta = aux->dreapta;
		}
	}
	else {
		if (aux->dreapta != NULL) {
			radacina = aux->dreapta;
		}
		else {
			radacina = NULL;
		}
	}

	//stergere
	free(aux->info.titlu);
	free(aux);

	return radacina;
}

nodArbore* stergeNod(nodArbore* radacina, int cheieDeSters) {
	if (radacina != NULL) {
		if (radacina->info.cod == cheieDeSters) {
			radacina = stergeRadacina(radacina);
			return radacina;
		}
		else {
			if (cheieDeSters < radacina->info.cod) {
				radacina->stanga = stergeNod(radacina->stanga, cheieDeSters);
				return radacina;
			}
			else {
				radacina->dreapta = stergeNod(radacina->dreapta, cheieDeSters);
				return radacina;
			}
		}
	}
	else {
		return NULL;
	}
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
	}
	fclose(f);

	printf("\n------Preordine-------\n");
	preordine(radacina);
	printf("\n------Inordine-------\n");
	inordine(radacina);
	printf("\n------Postordine-------\n");
	postordine(radacina);

	int inaltime = inaltimeArbore(radacina);
	printf("\n------Inaltime arbore=%d-------\n", inaltime);

	/*radacina = stergeRadacina(radacina);
	printf("\n------Arbore dupa stergere radacina:---------\n");*/
	radacina = stergeNod(radacina, 10);
	printf("\n------Arbore dupa stergere nod valoare 10:---------\n");
	preordine(radacina);
	printf("\n------Inaltime subarbore stang = %d ---------\n", inaltimeArbore(radacina->stanga));
	printf("\n------Inaltime subarbore drept = %d ---------\n", inaltimeArbore(radacina->dreapta));

	dezalocare(radacina);
}