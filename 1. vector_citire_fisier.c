#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int cod;
	char* denumire;
	float pret;
	float cantitate;
}produs;

produs citireFisier(FILE* f) {
	produs p;
	char buffer[20];

	fgets(buffer, sizeof(buffer), f);
	p.cod = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	char* denumire = strtok(buffer, "\n");
	p.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(p.denumire, denumire);

	fgets(buffer, sizeof(buffer), f);
	p.pret = atof(buffer);

	fgets(buffer, sizeof(buffer), f);
	p.cantitate = atof(buffer);

	return p;
}

void afisare(produs* vp, int nr) {
	for (int i = 0;i < nr;i++) {
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, Cantitate=%5.2f", vp[i].cod, vp[i].denumire, vp[i].pret, vp[i].cantitate);
	}
}

int nrProd(FILE* f) {
	int nr = 0;
	char buffer[20];
	while (fgets(buffer, sizeof(buffer), f)) {
		nr++;
	}
	return nr / 4;
}

void dezalocare(produs* vp, int nr) {
	for (int i = 0;i < nr;i++) {
		free(vp[i].denumire);
	}
	free(vp);
}

void main() {
	FILE* f = fopen("produse.txt", "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului.");
	}
	int numarProduse = nrProd(f);
	rewind(f);
	produs* vp = (produs*)malloc(numarProduse * sizeof(produs));
	for (int i = 0;i < numarProduse;i++) {
		vp[i] = citireFisier(f);
	}
	fclose(f);
	afisare(vp, numarProduse);
	dezalocare(vp, numarProduse);
}