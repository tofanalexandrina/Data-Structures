#include <stdio.h>
#include <malloc.h>

typedef struct {
	int* varsta;
	char* nume;
	float medie;
}student;

typedef struct {
	student info;
	struct nodLD* next, * prev;
}nodLD;


nodLD* initializareNod(student s) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->info.varsta = (int*)malloc(sizeof(int));
	*(nou->info.varsta) = (*s.varsta);
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;

	nou->next = nou;
	nou->prev = nou;

	return nou;
}

nodLD* inserareNod1(nodLD* capLD, student s) {
	nodLD* nou = initializareNod(s);
	if (capLD == NULL) {
		capLD = nou;
	}
	else {
		nodLD* temp = capLD;
		while (temp->next!=capLD) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		nou->next = capLD;
		capLD->prev = nou;
	}
	return capLD;
}

void inserareNod2(nodLD** capLD, student s) {
	nodLD* nou = initializareNod(s);
	if (*capLD == NULL) {
		*capLD = nou;
	}
	else {
		nodLD* temp = *capLD;
		while (temp->next!=(*capLD)) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		nou->next = *capLD;
		(*capLD)->prev = nou;
	}
}

void traversareListaStangaDreapta(nodLD* capLD) {
	nodLD* temp = capLD;
	while (temp->next != capLD) {
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f\n", *(temp->info.varsta), temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
	printf("\nVarsta=%d, Nume=%s, Medie=%5.2f\n", *(temp->info.varsta), temp->info.nume, temp->info.medie);
}

void dezalocareLista(nodLD* capLD) {
	nodLD* temp = capLD;
	while (temp->next != capLD) {
		nodLD* temp2 = temp->next;
		free(temp->info.varsta);
		free(temp->info.nume);
		free(temp);
		temp = temp2;
	}
	free(temp->info.varsta);
	free(temp->info.nume);
	free(temp);
}

void salvareStudentiVector(nodLD* capLD, student* vector, int* nrElem, float mediePrag) {
	nodLD* temp = capLD;
	while (temp->next != capLD) {
		if (temp->info.medie >= mediePrag) {
			vector[*nrElem].varsta = (int*)malloc(sizeof(int));
			*(vector[*nrElem].varsta) = *(temp->info.varsta);
			vector[*nrElem].nume = (char*)malloc((strlen(temp->info.nume) + 1) * sizeof(char));
			strcpy(vector[*nrElem].nume, temp->info.nume);
			vector[*nrElem].medie = temp->info.medie;
			(*nrElem)++;
		}

		temp = temp->next;
	}
	if (temp->info.medie >= mediePrag) {
		vector[*nrElem].varsta = (int*)malloc(sizeof(int));
		*(vector[*nrElem].varsta) = *(temp->info.varsta);
		vector[*nrElem].nume = (char*)malloc((strlen(temp->info.nume) + 1) * sizeof(char));
		strcpy(vector[*nrElem].nume, temp->info.nume);
		vector[*nrElem].medie = temp->info.medie;
		(*nrElem)++;
	}
}


void main() {
	int nrStud;
	student s;
	nodLD* capLD = NULL, * coadaLD = NULL;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	for (int i = 0;i < nrStud;i++) {
		s.varsta = (int*)malloc(sizeof(int));
		fscanf(f, "%d", s.varsta);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);
		capLD = inserareNod1(capLD, s);
		//inserare varianta 2
		//inserareNod2(&capLD, &coadaLD, s);
		free(s.nume);
		free(s.varsta);
	}
	fclose(f);
	printf("\n------------Traversare-----------\n");
	traversareListaStangaDreapta(capLD);

	//salvare vector
	printf("\n------------LS->Vector-----------\n");
	//declarare vector
	student* vector = (student*)malloc(nrStud * sizeof(student)); //alocare pt nr max de studenti
	int nrElem = 0;
	salvareStudentiVector(capLD, vector, &nrElem, 9.0f);
	for (int i = 0;i < nrElem;i++) {
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f\n", *(vector[i].varsta), vector[i].nume, vector[i].medie);
	}
	for (int i = 0;i < nrElem;i++) {
		free(vector[i].varsta);
		free(vector[i].nume);
	}
	free(vector);

	printf("\n------------Traversare-----------\n");
	traversareListaStangaDreapta(capLD);
	dezalocareLista(capLD);
}
