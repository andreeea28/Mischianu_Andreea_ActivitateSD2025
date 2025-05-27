#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct structuraAvion {
	char* model;
	int nrLocuri;
	int nrLocuriOcupate;
	float* preturi; 
};
typedef struct structuraAvion Avion;

Avion citireAvionDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Avion a;
	aux = strtok(buffer, sep);
	a.model = malloc(strlen(aux) + 1);
	strcpy_s(a.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	a.nrLocuri = atoi(aux);
	aux = strtok(NULL, sep);
	a.nrLocuriOcupate = atoi(aux);
	a.preturi = malloc(sizeof(float)*a.nrLocuriOcupate);
	for (int i = 0; i < a.nrLocuriOcupate; i++) {
		aux = strtok(NULL, sep);
		a.preturi[i] = atof(aux);
	}
	return a;
}

void afisareAvion(Avion a) {
	printf("Avionul este modelul %s, cu %d locuri disponibile dintre care %d sunt ocupate. Pretul fiecarui loc este:\n", a.model, a.nrLocuri, a.nrLocuriOcupate);
	for (int i = 0; i < a.nrLocuriOcupate; i++) {
		printf("Locul %d are pretul de %5.2f euro\n", i + 1, a.preturi[i]);
	}
}

typedef struct NOD {
	Avion info;
	struct NOD* stanga;
	struct NOD* dreapta;
} nod;

nod* inserareAvionInArbore(nod* radacina, Avion a) {
	if (radacina) {
		if (radacina->info.nrLocuri > a.nrLocuri) {
			radacina->stanga = inserareAvionInArbore(radacina->stanga, a);
		}
		else {
			radacina->dreapta = inserareAvionInArbore(radacina->dreapta, a);
		}
		return radacina;
	}
	else {
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = a;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		return nou;
	}
}

//afisare inordine SRD
void afisareArbore(nod* radacina) {
	if (radacina) {
		afisareArbore(radacina->stanga);
		afisareAvion(radacina->info);
		afisareArbore(radacina->dreapta);
	}
}

Avion cautareAvionInArboreDupaNrLocuri(nod* radacina, int nrLocuri) {
	if (radacina) {
		if (radacina->info.nrLocuri == nrLocuri) {
			return radacina->info;
		}
		else {
			if (radacina->info.nrLocuri > nrLocuri) {
				return cautareAvionInArboreDupaNrLocuri(radacina->stanga, nrLocuri);
			}
			else {
				return cautareAvionInArboreDupaNrLocuri(radacina->dreapta, nrLocuri);
			}
		}
	}
}

//arbore binar de cautare echilibrat
int inaltimeArbore(nod* radacina) {
	if (radacina) {
		int inaltimeStanga = inaltimeArbore(radacina->stanga);
		int inaltimeDreapta = inaltimeArbore(radacina->dreapta);
		int maxim;
		if (inaltimeStanga > inaltimeDreapta) {
			maxim = inaltimeStanga;
		}
		else {
			maxim = inaltimeDreapta;
		}
		maxim += 1;
		return maxim;
	}
	else {
		return 0;
	}
}

nod* rotireLaDreapta(nod* radacina) {
	if (radacina) {
		nod* aux = radacina->stanga;
		radacina->stanga = aux->dreapta;
		aux->dreapta = radacina;
		return aux;
	}
	else return NULL;
}
nod* rotireLaStanga(nod* radacina) {
	if (radacina) {
		nod* aux = radacina->dreapta;
		radacina->dreapta = aux->stanga;
		aux->stanga = radacina;
		return aux;
	}else return NULL;
}

int calculareGradEchilibru(nod* radacina) {
	if (radacina) {
		int inaltimeStanga = inaltimeArbore(radacina->stanga);
		int inaltimeDreapta = inaltimeArbore(radacina->dreapta);
		return  inaltimeStanga - inaltimeDreapta;
	}
	else return 0;
}

nod* inserareAvionInArboreEchilibrat(nod* radacina, Avion a) {
	if (radacina) {
		if (radacina->info.nrLocuri > a.nrLocuri) {
			radacina->stanga = inserareAvionInArbore(radacina->stanga, a);
		}
		else {
			radacina->dreapta = inserareAvionInArbore(radacina->dreapta, a);
		}
		
		if (calculareGradEchilibru(radacina) == 2) {
			if (calculareGradEchilibru(radacina->stanga) != 1) {
				radacina->stanga = rotireLaStanga(radacina->stanga);
			}
			radacina = rotireLaDreapta(radacina);
			
		}
		if (calculareGradEchilibru(radacina) == -2) {
			if (calculareGradEchilibru(radacina->dreapta) != -1) {
				radacina->dreapta = rotireLaDreapta(radacina->dreapta);
			}
			radacina = rotireLaStanga(radacina);
		}
		return radacina;
	}
	else {
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = a;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		return nou;
	}
}


int main() {
	FILE* file = fopen("Avioane.txt","r");
	/*
	Avion a1 = citireAvionDinFisier(file);
	afisareAvion(a1);
	return 0;*/

	nod* radacina = NULL;
	for (int i = 0; i < 10; i++) {
		Avion a = citireAvionDinFisier(file);
		radacina = inserareAvionInArboreEchilibrat(radacina, a);
	}
	afisareArbore(radacina);
	printf("obtinere avion\n");
	Avion cautare = cautareAvionInArboreDupaNrLocuri(radacina, 180);
	afisareAvion(cautare);
	printf("Inaltime: %d \n", inaltimeArbore(radacina));
}