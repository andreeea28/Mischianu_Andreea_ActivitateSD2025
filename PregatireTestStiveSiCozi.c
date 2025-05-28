#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct structuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct structuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	aux = strtok(NULL, sep);
	m.nrUsi = atoi(aux);
	aux = strtok(NULL, sep);
	m.pret = atof(aux);
	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);
	m.serie = *strtok(NULL, sep);
	return m;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//stack cu LSI

struct NodStiva {
	Masina info;
	struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

void pushStack(NodStiva** varf, Masina m) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->info = m;
	nou->next = *varf;
	*varf = nou;
}

Masina popStack(NodStiva** varf) {
	Masina m;
	if (varf != NULL) {
		NodStiva* aux = *varf;
		m = aux->info;
		*varf = aux->next;
		free(aux);
	}
	else {
		printf("stiva este goala!\n");
		m.model = NULL;
	}
	return m;
}

int emptyStack(NodStiva* varf) {
	return varf == NULL;
}

void* citireStackMasiniDinFisier(NodStiva** varf, const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		pushStack(varf, m);
	}
	fclose(file);
}

void dezalocareStivaDeMasini(NodStiva** varf) {
	while (*varf) {
		NodStiva* aux = *varf;
		*varf = aux->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}

}

int sizeStack(NodStiva* varf) {
	int count = 0;
	while (varf) {
		count++;
		varf = varf->next;
	}
	return count;
}

int main() {
	NodStiva* varf = NULL;
	citireStackMasiniDinFisier(&varf, "masini.txt");
	printf("Avem %d elemente in stiva\n", sizeStack(varf));
	while (!emptyStack(varf)) {
		afisareMasina(popStack(&varf));
	}
	dezalocareStivaDeMasini;
	return 0;
}