#include <stdio.h>
#include <malloc.h>

typedef struct {
	int* varsta; 
	char* nume; 
	float medie; 
}student;

typedef struct {
	student info;
	struct nodLD* next, *prev;
}nodLD;


nodLD* initializareNod(student s) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->info.varsta = (int*)malloc(sizeof(int));
	*(nou->info.varsta) = (*s.varsta);
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;

	nou->next = NULL;
	nou->prev = NULL;

	return nou;
}

nodLD* inserareNod1(nodLD* capLD, nodLD** coadaLD, student s) {
	nodLD* nou = initializareNod(s);
	if (capLD == NULL) {
		capLD = nou;
		*coadaLD = nou;
	}
	else {
		/*nodLD* temp = capLD;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
		*coadaLD = nou;*/
		(*coadaLD)->next = nou;
		nou->prev = *coadaLD;
		(*coadaLD) = nou;
	}
	return capLD;
}

//varianta cu returnare void
void inserareNod2(nodLD** capLD, nodLD**coadaLD, student s) {
	nodLD* nou = initializareNod(s);
	if (*capLD == NULL) {
		*capLD = nou;
		*coadaLD = nou;
	}
	else {
		/*nodLD* temp = *capLD;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
		*coadaLD = nou;*/
		(*coadaLD)->next = nou;
		nou->prev = *coadaLD;
		(*coadaLD) = nou;
	}
}

void traversareListaStangaDreapta(nodLD* capLD) {
	nodLD* temp = capLD;
	while (temp) {
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f\n", *(temp->info.varsta), temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void traversareListaDreaptaStanga(nodLD* coadaLD) {
	nodLD* temp = coadaLD;
	while (temp) {
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f\n", *(temp->info.varsta), temp->info.nume, temp->info.medie);
		temp = temp->prev;
	}
}

//acelasi sablon ca la traversare
void dezalocareLista(nodLD* capLS) {
	nodLD* temp = capLS;
	while (temp) {
		nodLD* temp2 = temp->next;
		free(temp->info.varsta);
		free(temp->info.nume);
		free(temp);
		temp = temp2;
	}
}

//LS --> vector
void salvareStudentiVector(nodLD* capLD, student* vector, int* nrElem, float mediePrag) {
	nodLD* temp = capLD;
	while (temp != NULL) {
		if (temp->info.medie >= mediePrag) {
			//vector[*nrElem] = temp->info; //shallow copy - vectorul si lista partajeaza aceeasi zona de memorie
			vector[*nrElem].varsta = (int*)malloc(sizeof(int));
			*(vector[*nrElem].varsta) = *(temp->info.varsta);
			vector[*nrElem].nume = (char*)malloc((strlen(temp->info.nume) + 1) * sizeof(char));
			strcpy(vector[*nrElem].nume, temp->info.nume);
			vector[*nrElem].medie = temp->info.medie;
			(*nrElem)++;
		}

		temp = temp->next;
	}
}

void stergereNodDupaNume(nodLD** capLD, nodLD** coadaLD, char*nume) {
	if (strcmp((*capLD)->info.nume, nume) == 0) {
		nodLD* temp = *capLD;
		*capLD = (*capLD)->next;
		if (*capLD != NULL) {
			(*capLD)->prev = NULL;
		}
		free(temp->info.nume);
		free(temp->info.varsta);
		free(temp);
		return;
	}
	else if (strcmp((*coadaLD)->info.nume, nume) == 0) {
		nodLD* temp = *coadaLD;
		*coadaLD = (*coadaLD)->prev;
		if (*coadaLD != NULL) {
			(*coadaLD)->next = NULL;
		}
		free(temp->info.nume);
		free(temp->info.varsta);
		free(temp);
		return;
	}
	else {
		nodLD* temp = *capLD;
		while (temp) {
			if (strcmp(temp->info.nume, nume) == 0) {
				nodLD* tempPrev = temp->prev;
				nodLD* tempNext = temp->next;
				tempPrev->next = tempNext;
				tempNext->prev = tempPrev;
				free(temp->info.nume);
				free(temp->info.varsta);
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
		//inserare varianta 1
		capLD = inserareNod1(capLD, &coadaLD, s);
		//inserare varianta 2
		//inserareNod2(&capLD, &coadaLD, s);
		free(s.nume);
		free(s.varsta);
	}
	fclose(f);
	printf("\n------------Traversare cap->coada-----------\n");
	traversareListaStangaDreapta(capLD);
	printf("\n------------Traversare coada->cap-----------\n");
	traversareListaDreaptaStanga(coadaLD);

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

	printf("\n------------Stergere Nod-----------\n");
	stergereNodDupaNume(&capLD, &coadaLD, "Ana");
	printf("\n------------Traversare cap->coada-----------\n");
	traversareListaStangaDreapta(capLD);
	printf("\n------------Traversare coada->cap-----------\n");
	traversareListaDreaptaStanga(coadaLD);
	dezalocareLista(capLD);
}