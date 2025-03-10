#include <stdio.h>
#include <malloc.h>

typedef struct {
	int* varsta; //4B
	char* nume; //4B
	float medie; //4B
}student;

typedef struct {
	student info; //info utila
	struct nodLS* next; //pointer de legatura 4B
}nodLS;

//=> structura ocupa 16B

nodLS* initializareNod(student s) {
	//INITIALIZARE INFO UTILA
	//aici se aloca spatiul de 16B
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	//nou->info = s; //shallow copy - componentele nodului nou partajeaza aceeasi zona de memorie
	//nou->info.varsta = s.varsta;
	nou->info.varsta = (int*)malloc(sizeof(int));
	*(nou->info.varsta) = (*s.varsta);
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume); //copie caracter cu caracter
	nou->info.medie = s.medie;

	//INITIALIZARE POINTER DE LEGATURA
	nou->next = NULL;

	return nou;
}

nodLS* inserareNod1(nodLS* capLS, student s) {
	nodLS* nou = initializareNod(s);
	if (capLS == NULL) {
		capLS = nou;
	}
	else {
		nodLS* temp = capLS;
		while (temp->next) 
			temp = temp->next;
		temp->next = nou;
	}
	return capLS;
}

//varianta cu returnare void
void inserareNod2(nodLS** capLS, student s) {
	nodLS* nou = initializareNod(s);
	if (*capLS == NULL) {
		*capLS = nou;
	}
	else {
		nodLS* temp = *capLS;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void traversareLista(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f\n", *(temp->info.varsta), temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

//acelasi sablon ca la traversare
void dezalocareLista(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		nodLS* temp2 = temp->next;
		//2 malloc => 2 free
		free(temp->info.varsta);
		free(temp->info.nume);
		free(temp);
		temp = temp2;
	}
}

//LS --> vector
void salvareStudentiVector(nodLS* capLS, student* vector, int* nrElem, float mediePrag) {
	//parcurgem lista
	nodLS* temp = capLS;
	while (temp != NULL) {
		//conditia de salvare in vector
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

void main() {
	int nrStud;
	student s;
	nodLS* capLS = NULL;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");
	//citim nr de studenti in variabila nrStud
	fscanf(f, "%d", &nrStud);
	for (int i = 0;i < nrStud;i++) {
		s.varsta = (int*)malloc(sizeof(int));
		fscanf(f, "%d", s.varsta);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);
		//inserare varianta 1
		//capLS = inserareNod1(capLS, s);
		//inserare varianta 2
		inserareNod2(&capLS, s);
		free(s.nume);
		free(s.varsta);
	}
	fclose(f);

	traversareLista(capLS);

	//salvare vector
	printf("\n------------LS->Vector-----------\n");
	//declarare vector
	student* vector = (student*)malloc(nrStud * sizeof(student)); //alocare pt nr max de studenti
	int nrElem = 0;
	salvareStudentiVector(capLS, vector, &nrElem, 9.0f);
	for (int i = 0;i < nrElem;i++) {
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f\n", *(vector[i].varsta), vector[i].nume, vector[i].medie);
	}
	for (int i = 0;i < nrElem;i++) {
		free(vector[i].varsta);
		free(vector[i].nume);
	}
	free(vector);

	dezalocareLista(capLS);
}
