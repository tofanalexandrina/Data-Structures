#include <stdio.h>
#include <malloc.h>

typedef struct {
	int id;
	int* cod;
	char* numeProdus;
	float pret;
} produs;

typedef struct {
	produs info;
	struct nodStiva* next;
}nodStiva;

typedef struct {
	produs info;
	struct nodCoada* next;
}nodCoada;

//stiva - push, pop
//coada - put, get


nodStiva* push1(nodStiva* varf, produs p) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->info.id = p.id;
	nou->info.cod = (int*)malloc(sizeof(int));
	*(nou->info.cod) = *(p.cod);
	nou->info.numeProdus = (char*)malloc((strlen(p.numeProdus) + 1) * sizeof(char));
	strcpy(nou->info.numeProdus, p.numeProdus);
	nou->info.pret = p.pret;
	nou->next = NULL;

	if (varf) {
		nou->next = varf;
		varf = nou;
	}
	else {
		varf = nou;
	}

	return nou;

}

void push2(nodStiva** varf, produs p) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->info.id = p.id;
	nou->info.cod = (int*)malloc(sizeof(int));
	*(nou->info.cod) = *(p.cod);
	nou->info.numeProdus = (char*)malloc((strlen(p.numeProdus) + 1) * sizeof(char));
	strcpy(nou->info.numeProdus, p.numeProdus);
	nou->info.pret = p.pret;
	nou->next = NULL;

	if (*varf) {
		nou->next = *varf;
		*varf = nou;
	}
	else {
		*varf = nou;
	}
}

int pop(nodStiva** varf, produs* p) {
	if (*varf == NULL) {
		return -1;
	}
	else {
		(*p).id = (*varf)->info.id;
		(*p).cod = (int*)malloc(sizeof(int));
		*((*p).cod) = *((*varf)->info.cod);
		(*p).numeProdus = (char*)malloc((strlen((*varf)->info.numeProdus) + 1) * sizeof(char));
		strcpy((*p).numeProdus, (*varf)->info.numeProdus);
		(*p).pret = (*varf)->info.pret;

		nodStiva* temp = *varf;
		(*varf) = (*varf)->next;
		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		return 0;
	}
}

void traversareStiva(nodStiva* varf) {
	nodStiva* temp = varf;
	while (temp) {
		printf("\nId=%d Cod=%d Denumire=%s Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
		temp = temp->next;
	}
}

void dezalocareStiva(nodStiva* varf) {
	nodStiva* temp = varf;
	while (temp) {
		nodStiva* tempNext = temp->next;
		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		temp = tempNext;
	}
}

void put(nodCoada** prim, nodCoada** ultim, produs p) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->info.id = p.id;
	nou->info.cod = (int*)malloc(sizeof(int));
	*(nou->info.cod) = *(p.cod);
	nou->info.numeProdus = (char*)malloc((strlen(p.numeProdus) + 1) * sizeof(char));
	strcpy(nou->info.numeProdus, p.numeProdus);
	nou->info.pret = p.pret;
	nou->next = NULL;

	if (*prim == NULL || *ultim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

void traversareCoada(nodCoada* varf) {
	nodCoada* temp = varf;
	while (temp) {
		printf("\nId=%d Cod=%d Denumire=%s Pret=%5.2f\n", temp->info.id, *(temp->info.cod), temp->info.numeProdus, temp->info.pret);
		temp = temp->next;
	}
}

int get(nodCoada** prim, nodCoada** ultim, produs* p) {
	if (*prim!=NULL && *ultim!=NULL) {
		(*p).id = (*prim)->info.id;
		(*p).cod = (int*)malloc(sizeof(int));
		*((*p).cod) = *((*prim)->info.cod);
		(*p).numeProdus = (char*)malloc((strlen((*prim)->info.numeProdus) + 1) * sizeof(char));
		strcpy((*p).numeProdus, (*prim)->info.numeProdus);
		(*p).pret = (*prim)->info.pret;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.cod);
		free(temp->info.numeProdus);
		free(temp);
		return 0;
	}
	if (*prim == NULL) {
		*ultim = NULL;
		return -1;
	}
}

void dezalocareCoada(nodCoada* prim, nodCoada* ultim) {
	produs p;
	while (get(&prim, &ultim, &p) == 0) {
		free(p.cod);
		free(p.numeProdus);
	}
}

//conversii - nu sunt testate in main
void stiva_in_coada(nodStiva* varf, nodCoada** prim, nodCoada** ultim) {
	produs p;
	while (pop(&varf, &p) == 0) {
		put(prim, ultim, p);
	}
}
void coada_in_stiva(nodCoada** prim, nodCoada** ultim, nodStiva** varf) {
	produs p;
	while (get(prim, ultim, &p) == 0) {
		push2(varf, p);
	}
}

void main() {
	int nrProduse;
	produs p;
	nodStiva* varfStiva = NULL;
	char buffer[20];

	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;

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

		//pentru stiva
		varfStiva = push1(varfStiva, p);
		//push2(&varfStiva, p);
		
		//pentru coada
		put(&prim, &ultim, p);

		free(p.cod);
		free(p.numeProdus);
	}
	fclose(f);

	printf("\n--------------Stiva--------------\n");
	traversareStiva(varfStiva);
	dezalocareStiva(varfStiva);

	printf("\n--------------Coada--------------\n");
	traversareCoada(prim);
	dezalocareCoada(prim, ultim);
}