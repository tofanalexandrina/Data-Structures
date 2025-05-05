#include <stdio.h>
#include <malloc.h>

typedef struct {
	int id;
	int* cod;
	char* numeProdus;
	float pret;
}produs;

typedef struct {
	produs* vector;
	int nrElemente;
}heap;

void filtrare(heap h, int i) {
	int indexRadacina = i;
	int indexStanga = 2 * i + 1;
	int indexDreapta = 2 * i + 2;

	//daca fiul stang al nodului curent exista in heap si are prioritatea(id) > cea a nodului radacina
	if (indexStanga<h.nrElemente && h.vector[indexStanga].id>h.vector[indexRadacina].id) {
		indexRadacina = indexStanga;
	}

	if (indexDreapta<h.nrElemente && h.vector[indexDreapta].id>h.vector[indexRadacina].id) {
		indexRadacina = indexDreapta;
	}

	//daca nodul curent(de la care am inceput) nu e cu cea mai mare prioritate dupa prelucrarile de mai sus
	if (i != indexRadacina) {
		//retinem pozitia nodului curent in aux
		produs aux = h.vector[i];
		//ne mutam pe pozitia copilului cu prioritate mai mare
		h.vector[i] = h.vector[indexRadacina];
		//nodul original se muta in pozitia copilului
		h.vector[indexRadacina] = aux;

		//apel recursiv pentru toate elementele structurii 
		filtrare(h, indexRadacina);
	}
}

void afisareHeap(heap h) {
	for (int i = 0;i < h.nrElemente;i++) {
		printf("\nPrioritate(Id)=%d, Cod=%d, Denumire=%s, Pret=%5.2f\n", h.vector[i].id, *h.vector[i].cod, h.vector[i].numeProdus, h.vector[i].pret);
	}
}

void dezalocareHeap(heap h) {
	for (int i = 0;i < h.nrElemente;i++) {
		free(h.vector[i].cod);
		free(h.vector[i].numeProdus);
	}
	free(h.vector);
}

void main() {
	heap h;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");

	fscanf(f, "%d", &h.nrElemente);
	h.vector = (produs*)malloc(h.nrElemente * sizeof(produs));
	for (int i = 0;i < h.nrElemente; i++) {
		fscanf(f, "%d", &h.vector[i].id);
		h.vector[i].cod = (int*)malloc(sizeof(int));
		fscanf(f, "%d", h.vector[i].cod);
		fscanf(f, "%s", buffer);
		h.vector[i].numeProdus = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(h.vector[i].numeProdus, buffer);
		fscanf(f, "%f", &h.vector[i].pret);
	}
	fclose(f);

	//doar primii n/2 -1 indici pot avea copii
	//iteram de jos in sus, verificand doar nodurile care au copii
	for (int i = (h.nrElemente-1) / 2;i >= 0;i-- ) {
		filtrare(h, i);
	}
	afisareHeap(h);
}

