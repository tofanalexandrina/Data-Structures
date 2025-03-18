#include <stdio.h>
#include <malloc.h>

typedef struct {
	int zi, luna, an; //12B
}dataReceptionare;

typedef struct {
	dataReceptionare data; //12B
	char* denumireAplicatie; //4B
	float durata; //4B
}notificare;

typedef struct {
	notificare info; //20B
	struct nodStiva* next; //4B
}nodStiva;

typedef struct {
	notificare info;
	struct nodCoada* next;
}nodCoada;

//inserare nod in stiva
void push(nodStiva** varf, notificare notif){
	//creare si initializare nod nou
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->info.data.zi = notif.data.zi;
	nou->info.data.luna = notif.data.luna;
	nou->info.data.an = notif.data.an;
	nou->info.denumireAplicatie = (char*)malloc((strlen(notif.denumireAplicatie) + 1) * sizeof(char));
	strcpy(nou->info.denumireAplicatie, notif.denumireAplicatie);
	nou->info.durata = notif.durata;
	nou->next = NULL;

	if (*varf == NULL) {
		*varf = nou;
	}
	else {
		nou->next = *varf;
		*varf = nou;
	}

}

//extragere nod din stiva
int pop(nodStiva** varf, notificare* notif) {
	if (*varf == NULL) {
		return -1;
	}
	else {
		(*notif).data.zi = (*varf)->info.data.zi;
		(*notif).data.luna = (*varf)->info.data.luna;
		(*notif).data.an = (*varf)->info.data.an;
		(*notif).denumireAplicatie = (char*)malloc((strlen((*varf)->info.denumireAplicatie) + 1) * sizeof(char));
		strcpy((*notif).denumireAplicatie, (*varf)->info.denumireAplicatie);
		(*notif).durata = (*varf)->info.durata;

		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->info.denumireAplicatie);
		free(temp);
		return 0;
	}
}

void afisareStiva(nodStiva* varf) {
	nodStiva* temp = varf;
	while (temp) {
		printf("\nData: %d/%d/%d, Denumire aplicatie: %s, Durata: %5.2f\n", temp->info.data.zi, temp->info.data.luna, temp->info.data.an, temp->info.denumireAplicatie, temp->info.durata);
		temp = temp->next;
	}
}

void dezalocareStiva(nodStiva* varf) {
	notificare notif;
	while (pop(&varf, &notif) == 0) {
		free(notif.denumireAplicatie);
	}
}

//inserare in coada
void put(nodCoada** prim, nodCoada** ultim, notificare notif) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->info.data.zi = notif.data.zi;
	nou->info.data.luna = notif.data.luna;
	nou->info.data.an = notif.data.an;
	nou->info.denumireAplicatie = (char*)malloc((strlen(notif.denumireAplicatie) + 1) * sizeof(char));
	strcpy(nou->info.denumireAplicatie, notif.denumireAplicatie);
	nou->info.durata = notif.durata;
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

//extragere din coada
int get(nodCoada** prim, nodCoada** ultim, notificare* notif) {
	if (*prim != NULL && *ultim != NULL) {
		(*notif).data.zi = (*prim)->info.data.zi;
		(*notif).data.luna = (*prim)->info.data.luna;
		(*notif).data.an = (*prim)->info.data.an;
		(*notif).denumireAplicatie = (char*)malloc((strlen((*prim)->info.denumireAplicatie) + 1) * sizeof(char));
		strcpy((*notif).denumireAplicatie, (*prim)->info.denumireAplicatie);
		(*notif).durata = (*prim)->info.durata;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.denumireAplicatie);
		free(temp);
		return 0;
	}
	if (*prim == NULL) {
		*ultim = NULL;
		return -1;
	}
}

void afisareCoada(nodCoada* prim) {
	nodCoada* temp = prim;
	while (temp) {
		printf("\nData: %d/%d/%d, Denumire aplicatie: %s, Durata: %5.2f\n", temp->info.data.zi, temp->info.data.luna, temp->info.data.an, temp->info.denumireAplicatie, temp->info.durata);
		temp = temp->next;
	}
}

void dezalocareCoada(nodCoada* prim, nodCoada* ultim) {
	notificare notif;
	while (get(&prim, &ultim, &notif) == 0) {
		free(notif.denumireAplicatie);
	}
}

void conversieStivaCoada(nodStiva* varf, nodCoada** prim, nodCoada** ultim) {
	notificare notif;
	while (pop(&varf, &notif) == 0) {
		put(prim, ultim, notif);
	}
}

//sa se determine din coada notificarile care au durata >= prag
void determinareNotificariDurata(nodCoada* prim, nodCoada* ultim, notificare* vect, int* nr, float prag) {
	notificare notif;
	while (get(&prim, &ultim, &notif) == 0) {
		if (notif.durata >= prag) {
			vect[*nr] = notif;
			(*nr)++;
		}
	}
}

void main() {
	int nrNotif;
	nodStiva* varf = NULL;
	notificare notif;
	char buffer[30];

	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrNotif);
	for (int i = 0;i < nrNotif;i++) {
		fscanf(f, "%d", &notif.data.zi);
		fscanf(f, "%d", &notif.data.luna);
		fscanf(f, "%d", &notif.data.an);
		fscanf(f, "%s", buffer);
		notif.denumireAplicatie = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(notif.denumireAplicatie, buffer);
		fscanf(f, "%f", &notif.durata);
		push(&varf, notif);
		//put(&prim, &ultim, notif);
		free(notif.denumireAplicatie);
	}

	fclose(f);
	printf("\n---------Stiva----------\n");

	afisareStiva(varf);

	//dezalocareStiva(varf); - dupa conversie in stiva

	printf("\n---------Coada----------\n");
	conversieStivaCoada(varf, &prim, &ultim);
	afisareCoada(prim);
	//dezalocareCoada(prim, ultim); - dupa conversie in vector

	printf("\n---------Vector----------\n");
	notificare* vect = (notificare*)malloc(nrNotif * sizeof(notificare));
	int nr = 0;
	determinareNotificariDurata(prim, ultim, vect, &nr, 2);
	for (int i = 0;i < nr;i++) {
		printf("\nData: %d/%d/%d, Denumire aplicatie: %s, Durata: %5.2f\n", vect[i].data.zi, vect[i].data.luna, vect[i].data.an, vect[i].denumireAplicatie, vect[i].durata);
	}
	for (int i = 0;i < nr;i++) {
		free(vect[i].denumireAplicatie);
	}
	free(vect);


}
