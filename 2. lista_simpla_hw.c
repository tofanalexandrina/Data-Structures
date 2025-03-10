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
	struct nodLS* next;
}nodLS;

nodLS* initializareNod(produs p) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.id = p.id;
	nou->info.cod = (int*)malloc(sizeof(int));
	*(nou->info.cod) = *(p.cod);
	nou->info.numeProdus = (char*)malloc((strlen(p.numeProdus) + 1) * sizeof(char));
	strcpy(nou->info.numeProdus, p.numeProdus);
	nou->info.pret = p.pret;

	nou->next = NULL;

	return nou;
}

nodLS* inserareNod_nodLS(produs p, nodLS* capLS) {
	nodLS* nou = initializareNod(p);
	if (capLS == NULL) {
		capLS = nou;
	}
	else {
		nodLS* temp = capLS;
		while (temp->next) {
			temp = temp->next;
		} //ne oprim pe ultimul nod
		temp->next = nou; //next-ul ultimului nod devine nodul nou
	}
	return capLS;
}

void inserareNod_void(produs p, nodLS** capLS) {
	nodLS* nou = initializareNod(p);
	if (*capLS == NULL) {
		*capLS = nou;
	}
	else {
		nodLS* temp = *capLS;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void traversare(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		printf("\nId=%d, Cod=%d, Produs=%s, Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
		temp = temp->next;
	}
}

void salvareVector(nodLS* capLS, produs* vector, int* nrElem, float pretPrag) {
	nodLS* temp = capLS;
	while (temp) {
		if (temp->info.pret <= pretPrag) {
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
}

void dezalocare(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		nodLS* temp_next = temp->next;
		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		temp = temp_next;
	}
}

void main() {
	int nrProd;
	produs p;
	nodLS* capLS = NULL;
	char buffer[20]; //pentru citire char*

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
	for (int i = 0;i < nrProd;i++) {
		fscanf(f, "%d", &p.id);
		p.cod = (int*)malloc(sizeof(int));
		fscanf(f, "%d", p.cod);
		fscanf(f, "%s", buffer);
		p.numeProdus = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.numeProdus, buffer);
		fscanf(f, "%f", &p.pret);

		capLS = inserareNod_nodLS(p, capLS);
		//inserareNod_void(p, &capLS);

		free(p.cod);
		free(p.numeProdus);
	}
	fclose(f);

	traversare(capLS);

	printf("\n-------------------Salvare in Vector - produse cu pret <= 9 lei-------------------");
	produs* vector = (produs*)malloc(nrProd * sizeof(produs));
	int nrElem = 0;
	salvareVector(capLS, vector, &nrElem, 9.0f);
	for (int i = 0;i < nrElem;i++) {
		printf("\nId=%d, Cod=%d, Produs=%s, Pret=%5.2f\n", vector[i].id, *(vector[i].cod), vector[i].numeProdus, vector[i].pret);
	}
	for (int i = 0;i < nrElem;i++) {
		free(vector[i].cod);
		free(vector[i].numeProdus);
	}
	free(vector);

	dezalocare(capLS);
}