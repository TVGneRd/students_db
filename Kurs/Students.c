#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> 

typedef struct
{
	int id;
	char name[256];
	int group;
} Student;

FILE *studentsDB;

int numberOfStudents = 0;

Student *Students = NULL;

void readStudentsBD() {

	fscanf_s(studentsDB, "%i\n", &numberOfStudents);

	Students = (Student*)realloc(Students, numberOfStudents * sizeof(Student));

	for (int i = 0; i < numberOfStudents; i++){
		fscanf(studentsDB, "%d\n", &Students[i].id);
		fscanf(studentsDB, "%[^\n]\n", Students[i].name);
		fscanf(studentsDB, "%d\n", &Students[i].group);
	}

}

void readStudentsBD1() {

	int fileSize;

	fseek(studentsDB, 0, SEEK_END);
	fileSize = ftell(studentsDB);


	numberOfStudents = fileSize / sizeof(Student);

	Students = (Student*)realloc(Students, numberOfStudents * sizeof(Student));

	fseek(studentsDB, 0, SEEK_SET);
	fread(Students, sizeof(Student), numberOfStudents, studentsDB);
}

void initStuentsDataBase() {
	if (fopen_s(&studentsDB, "db/students.db", "r+") != NULL)
	{
		printf("Data base Students unavailable!");
		exit(0);
	}
	readStudentsBD();
}

int getPrimaryKeyForStudentsBD() {

	int maxId = 0;

	for (int i = 0; i < numberOfStudents; i++) {
		if (Students[i].id > maxId) {
			maxId = Students[i].id;
		}
	}

	return ++maxId;
}

void addStudent(Student student) {
	Students = (Student*)realloc(Students, (numberOfStudents + 1) * sizeof(Student));
	Students[numberOfStudents++] = student;
	printf("\033[36mNew student (ID: %d): %s Group: %d\n\033[0m", student.id, student.name, student.group);
}

int searchStudentByID(int studentID) {

	for (int i = 0; i < numberOfStudents; i++) {
		if (Students[i].id == studentID) {
			return i;
		}
	}

	return -1;
}

void removeStudent(int pos) {

	for (int i = pos; i < numberOfStudents - 1; i++) {
		Students[i] = Students[i + 1];
	}

	numberOfStudents--;
}

void saveAndCloseStudentsDataBase() {
	fclose(studentsDB);

	// Открываем файлы для перезаписи
	fopen_s(&studentsDB, "db/students.db", "w");
	fseek(studentsDB, 0, SEEK_SET);

	fprintf(studentsDB, "%i\n", numberOfStudents);

	for (int i = 0; i < numberOfStudents; i++)
		fprintf(studentsDB, "%d\n%s\n%d\n", Students[i].id, Students[i].name, Students[i].group);

	fclose(studentsDB);

	free(Students);
}
