#include <stdio.h>
#include <malloc.h>

typedef struct {
	int id;
	int* cod;
	char* numeProdus;
	float pret;
}produs;

typedef struct {
	produs info;
	struct nodLD* next, * prev;
}nodLD;

nodLD* initializareNod(produs p) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->info.id = p.id;
	nou->info.cod = (int*)malloc(sizeof(int));
	*(nou->info.cod) = *(p.cod); //aici sa nu uit *
	nou->info.numeProdus = (char*)malloc((strlen(p.numeProdus) + 1) * sizeof(char));
	strcpy(nou->info.numeProdus, p.numeProdus); //am uitat de randul asta
	nou->info.pret = p.pret;

	nou->next = nou;
	nou->prev = nou;

	return nou;
}

nodLD* inserareNod1(produs p, nodLD* cap, nodLD** coada) {
	nodLD* nou = initializareNod(p);
	if (!cap) {
		cap = nou;
		*coada = nou;
	}
	else {
		(*coada)->next = nou; 
		nou->prev = *coada;
		*coada = nou;
		//aici se adauga legaturile pentru circulara
		(*coada)->next = cap;
		cap->prev = (*coada);
	}
	return cap; 
}

void inserareNod2(produs p, nodLD** cap, nodLD** coada) {
	nodLD* nou = initializareNod(p);
	if (!(*cap)) {
		*cap = nou;
		*coada = nou;
	}
	else {
		(*coada)->next = nou;
		nou->prev = (*coada);
		(*coada) = nou;
		//aici se adauga legaturile pentru circulara
		(*coada)->next = *cap;
		(*cap)->prev = (*coada);
	}
}

void traversare(nodLD* cap) {
	nodLD* temp = cap;
	//aici in loc de temp, verificam temp->next
	while (temp->next!=cap) {
		printf("\nId=%d, Cod=%d, Produs=%s, Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
		temp = temp->next;
	}
	//pentru ultimul nod(de dinaintea lui cap)
	printf("\nId=%d, Cod=%d, Produs=%s, Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
}

void traversareInversa(nodLD* coada) {
	nodLD* temp = coada;
	while (temp->prev!=coada) {
		printf("\nId=%d, Cod=%d, Produs=%s, Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
		temp = temp->prev;
	}
	printf("\nId=%d, Cod=%d, Produs=%s, Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
}

void dezalocare(nodLD* cap) {
	nodLD* temp = cap;
	while (temp!=cap) {
		nodLD* tempNext = temp->next;
		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		temp = tempNext;
	}
	free(temp->info.cod);
	free(temp->info.numeProdus);
	free(temp);
}

void salvareInVector(produs* vector, nodLD* cap, int* nrElem, float prag) {
	nodLD* temp = cap;
	while (temp->next!=cap) {
		if (temp->info.pret <= prag) {
			vector[*nrElem].id = temp->info.id;
			vector[*nrElem].cod = (int*)malloc(sizeof(int));
			*(vector[*nrElem].cod) = *(temp->info.cod);
			vector[*nrElem].numeProdus = (char*)malloc((strlen(temp->info.numeProdus) + 1) * sizeof(char));
			strcpy(vector[*nrElem].numeProdus, temp->info.numeProdus);
			vector[*nrElem].pret = temp->info.pret;
			(*nrElem)++;
		}
		temp = temp->next;
	}
	if (temp->info.pret <= prag) {
		vector[*nrElem].id = temp->info.id;
		vector[*nrElem].cod = (int*)malloc(sizeof(int));
		*(vector[*nrElem].cod) = *(temp->info.cod);
		vector[*nrElem].numeProdus = (char*)malloc((strlen(temp->info.numeProdus) + 1) * sizeof(char));
		strcpy(vector[*nrElem].numeProdus, temp->info.numeProdus);
		vector[*nrElem].pret = temp->info.pret;
		(*nrElem)++;
	}
}

void stergereNodDupaNume(nodLD** cap, nodLD** coada, char* nume) {
	//daca e primul nod
	if (strcmp((*cap)->info.numeProdus, nume) == 0) {
		nodLD* temp = *cap;
		*cap = (*cap)->next;
		//stabilire legaturi noi inainte de free
		(*cap)->prev = *coada;
		(*coada)->next = *cap;
;		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		return;
	}
	//daca e ultimul nod
	else if (strcmp((*coada)->info.numeProdus, nume) == 0) {
		nodLD* temp = *coada;
		*coada = (*coada)->prev;
		(*coada)->next = *cap;
		(*cap)->prev = *coada;
		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		return;
	}
	//daca e oricare dintre celelalte noduri
	else {
		nodLD* temp = *cap;
		while (temp) {
			if (strcmp(temp->info.numeProdus, nume) == 0) {
				nodLD* tempPrev = temp->prev;
				nodLD* tempNext = temp->next;
				tempPrev->next = tempNext;
				tempNext->prev = tempPrev;
				free(temp->info.cod);
				free(temp->info.numeProdus);
				free(temp);
				return;
			}
			else {
				temp = temp->next;
			}
		}
	}
}

void main() {
	int nrProduse;
	produs p;
	nodLD* cap = NULL;
	nodLD* coada = NULL;
	char buffer[20];

	FILE* f = fopen("fisier_produs.txt", "r");
	fscanf(f, "%d", &nrProduse);
	for (int i = 0;i < nrProduse;i++) {
		fscanf(f, "%d", &p.id);

		p.cod = (int*)malloc(sizeof(int));
		fscanf(f, "%d", p.cod);

		fscanf(f, "%s", buffer);
		p.numeProdus = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.numeProdus, buffer);

		fscanf(f, "%f", &p.pret);

		cap = inserareNod1(p, cap, &coada);
		free(p.cod);
		free(p.numeProdus);
	}
	fclose(f);

	printf("\n------------------Traversare Cap->Coada------------------\n");
	traversare(cap);
	printf("\n\n------------------Traversare Coada->Cap------------------\n");
	traversareInversa(coada);
	printf("\n\n------------------Salvare in Vector------------------\n");
	produs* vector = (produs*)malloc(nrProduse * sizeof(produs));
	int nrElem = 0;
	salvareInVector(vector, cap, &nrElem, 9.0f);
	for (int i = 0;i < nrElem;i++) {
		printf("\nId=%d, Cod=%d, Produs=%s, Pret=%5.2f\n", vector[i].id, *(vector[i].cod), vector[i].numeProdus, vector[i].pret);
	}

	for (int i = 0;i < nrElem;i++) {
		free(vector[i].cod);
		free(vector[i].numeProdus);
	}
	free(vector);
	
	printf("\n\n------------------Stergere Nod dupa Denumire------------------\n");
	stergereNodDupaNume(&cap, &coada, "Fanta");
	traversare(cap);
	
	dezalocare(cap);
}