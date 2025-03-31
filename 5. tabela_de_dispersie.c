#include <stdio.h>
#include <malloc.h>

typedef struct {
	int varsta; 
	char* nume; 
	float medie; 
}student;

typedef struct {
	student info; 
	struct nodLS* next; 
}nodLS;

typedef struct {
	struct nodLS** vector;
	int nrElem;
}hashT;

int hash1(int cheie, hashT tabela) {
	return cheie % tabela.nrElem;
}

int hash2(char* nume, hashT tabela) {
	return nume[0] % tabela.nrElem;
}

void inserareLS(nodLS** capLS, student s) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.varsta = s.varsta;
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;
	nou->next = NULL;

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

void inserareHashT(hashT tabela, student s) {
	//int pozitie = hash1(s.varsta, tabela);
	int pozitie = hash2(s.nume, tabela);
	inserareLS(&tabela.vector[pozitie], s);
}

void traversareLS(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		printf("Varsta=%d, Nume=%s, Medie=%5.2f\n", temp->info.varsta, temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void traversareHashT(hashT tabela) {
	for (int i = 0;i < tabela.nrElem;i++) {
		if (tabela.vector[i] != NULL) {
			printf("\nPozitia %d\n", i);
			traversareLS(tabela.vector[i]);
		}
	 }
}

void dezalocareLS(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		nodLS* tempNext = temp->next;
		free(temp->info.nume);
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

//Sa se determine media mediilor pentru studentii cu varstele cuprinse intre 21 si 23 de ani
void calculMediaMediilor(hashT tabela) {
	int count = 0;
	float sumaMedii = 0.0;

	for (int i = 0;i < tabela.nrElem;i++) {
		nodLS* temp = tabela.vector[i];
		while (temp) {
			if (temp->info.varsta >= 21 && temp->info.varsta <= 23) {
				sumaMedii += temp->info.medie;
				count++;
			}
			temp = temp->next;
		}
	}

	if (count > 0) {
		printf("\nMedia mediilor studentilor cu varsta cuprinsa intre 21 si 23 ani: %5.2f\n", sumaMedii / count);
	}
	else {
		printf("\nNu exista studenti cu varsta intre 21 si 23 ani.\n");
	}
}

void main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(sizeof(nodLS*) * tabela.nrElem);
	for (int i = 0;i < tabela.nrElem;i++) {
		tabela.vector[i] = NULL;
	}
	
	student s;
	int nrStud;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	for (int i = 0;i < nrStud;i++) {
		fscanf(f, "%d", &s.varsta);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);
		inserareHashT(tabela, s);
		free(s.nume);
	}
	fclose(f);

	printf("\n------------Tabela De Dispersie------------\n");
	traversareHashT(tabela);
	calculMediaMediilor(tabela);
	dezalocareHashT(tabela);
}
