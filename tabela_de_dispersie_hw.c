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
	struct nodLS** vector;
	int nrElem;
}hashT;

//Componente necesare pentru inserare in hashtable
// 1. Functia hash
//1.1. functie hash cu int
int hashFunc1(int cheie, hashT tabela) {
	return cheie % tabela.nrElem;
}
//1.2. functie hash cu char*(grupare in functie de prima litera)
int hashFunc2(char* nume, hashT tabela) {
	return nume[0] % tabela.nrElem;
}
//2. Functia de inserare in lista simpla
void inserareLS(nodLS** cap, produs p) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.id = p.id;
	nou->info.cod = (int*)malloc(sizeof(int));
	*(nou->info.cod) = *(p.cod);
	nou->info.numeProdus = (char*)malloc((strlen(p.numeProdus) + 1) * sizeof(char));
	strcpy(nou->info.numeProdus, p.numeProdus);
	nou->info.pret = p.pret;
	nou->next = NULL;

	if (*cap==NULL) {
		*cap = nou;
	}
	else {
		nodLS* temp = (*cap);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

//2. Inserarea in hashtable
void inserareHashT(hashT tabela, produs p) {
	//int pozitie = hashFunc1(p.id, tabela);
	//sau
	int pozitie = hashFunc2(p.numeProdus, tabela);
	inserareLS(&tabela.vector[pozitie], p);
}

//3. Traversare hashtable
void traversareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp) {
		printf("\nId=%d, Cod=%d, Denumire=%s, Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
		temp = temp->next;
	}
}
void traversareHashT(hashT tabela) {
	for (int i = 0;i < tabela.nrElem;i++) {
		if (tabela.vector[i] != NULL) {
			printf("\nPozitia %d:", i);
			traversareLS(tabela.vector[i]);
		}
	}
}

//4. Dezalocare hashtable
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

void dezalocareHashT(hashT tabela) {
	for (int i = 0;i < tabela.nrElem;i++) {
		if (tabela.vector[i] != NULL) {
			dezalocareLS(tabela.vector[i]);
		}
	}
	free(tabela.vector);
}

//Sa se determine produsul cu cel mai mare pret din tabela de dispersie
void cautareProdusPret(hashT tabela) {
	produs* produsPretMax = NULL;
	for (int i = 0;i < tabela.nrElem;i++) {
		nodLS* temp = tabela.vector[i];
		while (temp) { 
			if (produsPretMax == NULL || temp->info.pret > produsPretMax->pret) {
				produsPretMax = &temp->info;
			}
			temp = temp->next;
		}
		
	}
	if (produsPretMax) {
		printf("\nProdusul cu pretul maxim este %s : %5.2f lei.\n",
			produsPretMax->numeProdus, produsPretMax->pret);
	}
	else {
		printf("\nTabela este goala.\n");
	}
}

void main() {
	nodLS* cap = NULL;
	produs p;
	char buffer[20];
	int nrProd;

	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(sizeof(nodLS*) * tabela.nrElem);
	for (int i = 0;i < tabela.nrElem;i++) {
		tabela.vector[i] = NULL;
	}

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
		inserareHashT(tabela, p);
		free(p.cod);
		free(p.numeProdus);
	}
	fclose(f);

	traversareHashT(tabela);
	cautareProdusPret(tabela);
	dezalocareHashT(tabela);




}





