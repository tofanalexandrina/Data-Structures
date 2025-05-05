#include <stdio.h>
#include <malloc.h>

typedef struct {
	int prioritate;
	char* denumire;
	float durata;
}activitate;

typedef struct {
	activitate* vect;
	int nrElem;
}heap;

typedef struct {
	activitate info;
	struct nodLS* next;
}nodLS;

//indexul este i de la care pornim
void filtrare(heap h, int index) {
	//cautam i - nod radacina, i - fiu stanga, i - fiu dreapta
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;

	if (indexSt<h.nrElem && h.vect[indexSt].prioritate>h.vect[indexRad].prioritate) {
		indexRad = indexSt;
	}

	if (indexDr<h.nrElem && h.vect[indexDr].prioritate>h.vect[indexRad].prioritate) {
		indexRad = indexDr;
	}

	if (index != indexRad) {
		activitate aux = h.vect[index];
		//interschimbare cu shallow copy
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

void afisareHeap(heap h) {
	for (int i = 0;i < h.nrElem;i++) {
		printf("\nPrioritate=%d, Denumire=%s, Durata=%5.2f", h.vect[i].prioritate, h.vect[i].denumire, h.vect[i].durata);
	}
}

void dezalocareHeap(heap h) {
	for (int i = 0;i < h.nrElem;i++) {
		free(h.vect[i].denumire);
	}
	free(h.vect);
}

heap inserare(heap h, activitate nodNou) {
	activitate* vectNou = (activitate*)malloc((h.nrElem + 1) * sizeof(activitate));
	for (int i = 0;i < h.nrElem;i++) {
		vectNou[i] = h.vect[i];
	}
	h.nrElem++;

	//punem activitatea noua pe ultima poz
	vectNou[h.nrElem - 1] = nodNou;
	free(h.vect);
	//refacem referinta
	h.vect = vectNou;

	for (int i = (h.nrElem - 1) / 2;i >= 0;i--) {
		filtrare(h, i);
	}

	return h;

}

heap extragere(heap h, activitate* extrasa) {
	activitate* vectNou = (activitate*)malloc((h.nrElem - 1) * sizeof(activitate));
	//interschimb primul cu ultimul elem vector
	activitate aux = h.vect[0];
	h.vect[0] = h.vect[h.nrElem - 1];
	h.vect[h.nrElem - 1] = aux;
	//ultimul elem din vector
	*extrasa = h.vect[h.nrElem - 1];

	h.nrElem--;
	for (int i = 0;i < h.nrElem;i++) {
		vectNou[i] = h.vect[i];
	}
	free(h.vect);
	h.vect = vectNou;

	for (int i = (h.nrElem - 1) / 2;i >= 0;i--) {
		filtrare(h, i);
	}

	return h;
}

void inserareFinal(nodLS** cap, activitate a) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.prioritate = a.prioritate;
	nou->info.durata = a.durata;
	nou->info.denumire = (char*)malloc(strlen(a.denumire) + 1);
	strcpy(nou->info.denumire, a.denumire);
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


void dezalocareLista(nodLS* cap) {
	while (cap) {
		nodLS* temp = cap;
		cap = cap->next;
		free(temp->info.denumire);
		free(temp);
	}
}

void afisareLista(nodLS* cap) {
	while (cap) {
		printf("\nPrioritate=%d, Denumire=%s, Durata=%5.2f", cap->info.prioritate, cap->info.denumire, cap->info.durata);
		cap = cap->next;
	}
}



void main() {
	heap h;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &h.nrElem);
	h.vect = (activitate*)malloc(h.nrElem * sizeof(activitate));
	for (int i = 0;i < h.nrElem;i++) {
		fscanf(f, "%d", &h.vect[i].prioritate);
		fscanf(f, "%s", buffer);
		h.vect[i].denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(h.vect[i].denumire, buffer);
		fscanf(f, "%f", &h.vect[i].durata);
	}
	fclose(f);
	//afisare fara filtrare e doar un vector
	for (int i = (h.nrElem - 1) / 2;i >= 0;i--) {
		filtrare(h, i);
	}
	printf("----------------------Structura HEAP filtrata:----------------------");
	afisareHeap(h);

	activitate activitateNoua;
	printf("\n\nIntroduceti date pentru activitate noua:\n");
	printf("Prioritate=");
	scanf("%d", &activitateNoua.prioritate);
	printf("Denumire=");
	scanf("%s", buffer);
	activitateNoua.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(activitateNoua.denumire, buffer);
	printf("Durata=");
	scanf("%f", &activitateNoua.durata);
	printf("\n----------------------Structura HEAP dupa inserare nod nou:----------------------");
	h = inserare(h, activitateNoua);
	afisareHeap(h);
	printf("\n\n----------------------Structura HEAP dupa extragere nod:----------------------");
	activitate activitateExtrasa;
	h = extragere(h, &activitateExtrasa);
	afisareHeap(h);
	printf("\n\nActivitatea extrasa:");
	printf("\nPrioritate=%d, Denumire=%s, Durata=%5.2f\n", activitateExtrasa.prioritate, activitateExtrasa.denumire, activitateExtrasa.durata);


	printf("\n\n----------------------Conversie HEAP - LS:----------------------");
	nodLS* capLS = NULL;
	activitate a;

	while (h.nrElem > 0) {
		h = extragere(h, &a);
		inserareFinal(&capLS, a);
	}

	afisareLista(capLS);
	dezalocareLista(capLS);
	dezalocareHeap(h);
	
}
