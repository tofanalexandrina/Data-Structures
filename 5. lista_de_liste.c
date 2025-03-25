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
	struct nodLS* inf;
	struct nodLP* next;
}nodLP;

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

void inserareLP(nodLP** capLP, nodLS* capLS) {
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->inf = capLS;
	nou->next = NULL;
	if (*capLP == NULL) {
		*capLP = nou;
	}
	else {
		nodLP* temp = (*capLP);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void traversareLS(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		printf("Varsta=%d, Nume=%s, Medie=%5.2f\n", temp->info.varsta, temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void traversareLP(nodLP* capLP) {
	nodLP* temp = capLP;
	int i = 1;
	while (temp) {
		printf("\nSublista %d\n", i);
		traversareLS(temp->inf);
		temp = temp->next;
		i++;
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

void dezalocareLP(nodLP* capLP) {
	nodLP* temp = capLP;
	while (temp) {
		nodLP* tempNext = temp->next;
		dezalocareLS(temp->inf);
		free(temp);
		temp = tempNext;
	}
}

void main() {
	nodLP* capLP=NULL;
	//sublista 1
	nodLS* capLSPromovati = NULL;
	//sublista 2
	nodLS* capLSNepromovati = NULL;

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
		if (s.medie >= 5.0) {
			inserareLS(&capLSPromovati, s);
		}
		else {
			inserareLS(&capLSNepromovati, s);
		}
		free(s.nume);
	}
	fclose(f);

	inserareLP(&capLP, capLSPromovati);
	inserareLP(&capLP, capLSNepromovati);

	traversareLP(capLP);
	dezalocareLP(capLP);
}
