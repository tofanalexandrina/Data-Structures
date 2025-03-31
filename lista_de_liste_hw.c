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

typedef struct {
	struct nodLS* info;
	struct nodLP* next;
}nodLP;

void inserareLS(nodLS** cap, produs p) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.id = p.id;
	nou->info.cod = (int*)malloc(sizeof(int));
	*(nou->info.cod) = *(p.cod);
	nou->info.numeProdus = (char*)malloc((strlen(p.numeProdus) + 1) * sizeof(char));
	strcpy(nou->info.numeProdus, p.numeProdus);
	nou->info.pret = p.pret;
	nou->next = NULL;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodLS* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareLP(nodLP** capLP, nodLS* capLS) {
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->info = capLS;
	nou->next = NULL;

	if (*capLP == NULL) {
		*capLP = nou;
	}
	else {
		nodLP* temp = *capLP;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void traversareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp) {
		printf("\nId=%d, Cod=%d, Denumire=%s, Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
		temp = temp->next;
	}
}
void traversareLP(nodLP* cap) {
	nodLP* temp = cap;
	int nrSublista = 1;
	while (temp) {
		printf("\nSublista %d:", nrSublista);
		traversareLS(temp->info);
		temp = temp->next;
		nrSublista++;
	}
}

void dezalocareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp) {
		nodLS* tempNext = temp->next;
		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		temp = tempNext;
	}
}
void dezalocareLP(nodLP* cap) {
	nodLP* temp = cap;
	while (temp) {
		nodLP* tempNext = temp->next;
		dezalocareLS(temp->info);
		free(temp);
		temp = tempNext;
	}
}

void main() {
	produs p;
	nodLS* capLSIeftine=NULL;
	nodLS* capLSScumpe=NULL;
	char buffer[20];
	int nrProduse;

	nodLP* capLP = NULL;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProduse);
	for (int i = 0;i < nrProduse;i++) {
		fscanf(f, "%d", &p.id);
		p.cod = (int*)malloc(sizeof(int));
		fscanf(f, "%d", p.cod);
		fscanf(f, "%s", buffer);
		p.numeProdus = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.numeProdus, buffer);
		fscanf(f, "%f", &p.pret);
		if (p.pret > 5.0) {
			inserareLS(&capLSScumpe, p);
		}
		else {
			inserareLS(&capLSIeftine, p);
		}
		free(p.cod);
		free(p.numeProdus);

	}
	fclose(f);

	inserareLP(&capLP, capLSIeftine);
	inserareLP(&capLP, capLSScumpe);

	traversareLP(capLP);
	dezalocareLP(capLP);

}