#include <stdio.h>
#include <stdlib.h> 

typedef struct
{
	int id;
	char name[256];

} Discipline;

FILE *disciplinesDB;
Discipline *Disciplines = NULL;

int numberOfDisciplines = 0;

void readDesciplinesBD() {
	
	fscanf_s(disciplinesDB, "%i", &numberOfDisciplines);

	Disciplines = (Discipline*)realloc(Disciplines, numberOfDisciplines * sizeof(Discipline));

	for (int i = 0; i < numberOfDisciplines; i++)
		fscanf_s(disciplinesDB, "%d|%s\n", &Disciplines[i].id, Disciplines[i].name, 256);
}

/**
 * Возвращает ID для добавления новой дисциплины в базу данных дисциплин.
 * Нужно чтобы значения ID всегда были уникальынми.
 *
 * @return int $id - ID
 */
int getPrimaryKeyForDisciplinesBD() {
	int maxId = 0;

	for (int i = 0; i < numberOfDisciplines; i++) {
		if (Disciplines[i].id > maxId) {
			maxId = Disciplines[i].id;
		}
	}

	return ++maxId;
}

void addDiscipline(Discipline discipline) {
	Disciplines = (Discipline*)realloc(Disciplines, (numberOfDisciplines + 1) * sizeof(Discipline));
	Disciplines[numberOfDisciplines++] = discipline;
	printf("\033[36mNew discipline (ID: %d): %s\n\033[0m", discipline.id, discipline.name);
}

void initDisciplinesDataBase() {
	if (fopen_s(&disciplinesDB, "db/disciplines.db", "r+") != NULL)
	{
		printf("Data base Disciplines unavailable!");
		exit(0);
	}

	readDesciplinesBD();
}

int searchDisciplineByID(int disciplinID) {

	for (int i = 0; i < numberOfDisciplines; i++) {
		if (Disciplines[i].id == disciplinID) {
			return i;
		}
	}

	return -1;
}

void removeDiscipline(int pos) {

	for (int i = pos; i < numberOfDisciplines - 1; i++) {
		Disciplines[i] = Disciplines[i + 1];
	}

	numberOfDisciplines--;
}

void saveAndCloseDisciplinesDataBase() {
	fclose(disciplinesDB);

	fopen_s(&disciplinesDB, "db/disciplines.db", "w");
	
	fprintf(disciplinesDB, "%i\n", numberOfDisciplines);

	for (int i = 0; i < numberOfDisciplines; i++)
		fprintf(disciplinesDB, "%d|%s\n", Disciplines[i].id, Disciplines[i].name);

	fclose(disciplinesDB);

	free(Disciplines);
}