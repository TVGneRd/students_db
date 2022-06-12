#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <Windows.h>
#include "Students.h"
#include "Disciplines.h"
#include "Reports.h"
#include "utilities.h"

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ESC 27
#define ENTER 13

void initDataBases() {

	initStuentsDataBase();
	initDisciplinesDataBase();
	initReportsDataBase();
}

void saveAndCloseDataBases() {

	saveAndCloseStudentsDataBase();
	saveAndCloseDisciplinesDataBase();
	saveAndCloseReportsDataBase();
}

float getAverageScoreForStudentByID(int studentID) {
	float scoreSum = 0;
	int scoreCount = 0;

	for (int i = 0; i < numberOfReports; i++) {
		if (Reports[i].studentID == studentID) {
			scoreSum += Reports[i].score;
			scoreCount++;
		}
	}

	return scoreCount ? scoreSum / scoreCount : 0.f;
}

void sortStudentsByIdASC() {
	for (int i = 0; i < numberOfStudents; i++) {
		// сравниваем два соседних элемента.
		for (int j = 0; j < numberOfStudents - i - 1; j++) {

			if (Students[j].id > Students[j + 1].id) {
				// если они идут в неправильном порядке, то  
				//  меняем их местами. 
				Student tmp = Students[j];
				Students[j] = Students[j + 1];
				Students[j + 1] = tmp;
			}
		}
	}
}

void sortStudentsByNameASC() {
	for (int i = 0; i < numberOfStudents; i++) {
		// сравниваем два соседних элемента.
		for (int j = 0; j < numberOfStudents - i - 1; j++) {

			if (_stricmp(&Students[j].name[0], &Students[j + 1].name[0]) > 0) {
				// если они идут в неправильном порядке, то  
				//  меняем их местами. 
				Student tmp = Students[j];
				Students[j] = Students[j + 1];
				Students[j + 1] = tmp;
			}
		}
	}
}

void sortStudentsByScoreASC() {
	for (int i = 0; i < numberOfStudents; i++) {
		// сравниваем два соседних элемента.
		for (int j = 0; j < numberOfStudents - i - 1; j++) {
			char score1 = getAverageScoreForStudentByID(Students[j].id);
			char score2 = getAverageScoreForStudentByID(Students[j+1].id);

			if (score1 < score2) {
				// если они идут в неправильном порядке, то  
				//  меняем их местами. 
				Student tmp = Students[j];
				Students[j] = Students[j + 1];
				Students[j + 1] = tmp;
			}
		}
	}
}

void printStudentStatus(Student *student) {
	int totalReports = 0;

	for (int i = 0; i < numberOfReports; i++) {

		if (Reports[i].studentID != (*student).id) {
			continue;
		}
		
		totalReports++;

		if (Reports[i].score == 1.f) {
			 printf("\033[31m(Candidate for expulsion)\033[0m\0");
			 return;
		}

		if (Reports[i].score == 2.f) {
			printf("\033[33m(Retake the exams: \033[0m");
			int disciplinesCounter = 0;
			for (int j = i; j < numberOfReports; j++) {
				if (Reports[j].score == 2.f) {

					if (disciplinesCounter) {
						printf(", ");
					}

					Discipline discipline = Disciplines[searchDisciplineByID(Reports[j].disciplineID)];
					printf(discipline.name);
					
					disciplinesCounter++;
				}
			}

			printf("\033[33m)\033[0m");

			return;
		}

		if (Reports[i].score < 4.f) {
			printf("\033[36m(No scholarship)\033[0m\0");
			return;
		}
	}

	if (totalReports) {
		printf("\033[32m(With a scholarship)\033[0m\0");
	} else {
		printf("\033[3m(No data)\033[0m\0");
	}
}

void printStudentsList(int sortType, int selectID) {

	switch (sortType) {
		case 1:
			sortStudentsByNameASC();
			break;
		case 2:
			sortStudentsByScoreASC();
			break;
		case 0:
			sortStudentsByIdASC();
			break;
	}

	printf("\nID\t%s\tGroup\tAverage score\tStatus\n", stringAtrim("Name\0", 40));
	
	for (int i = 0; i < numberOfStudents; i++) {
		float averageScore = getAverageScoreForStudentByID(Students[i].id);

		if (selectID == i) printf("\033[4;36m");

		printf("%d\t%s\t%.4d\t%.1f\t\t", Students[i].id, stringAtrim(Students[i].name, 40), Students[i].group, averageScore);
		printf("\033[0m");
		printStudentStatus(&Students[i]);
		printf("\n");

	}
	printf("\n");
}

int printStudentsListLoop(int canSelect) {

	int sortType = 0;
	int selectedStudent = canSelect ? 0 : -1;

	char commandsList[][256] = { "ID (ASC)", "Name (ASC)", "Score (DESC)" };

	while (1) {
		clearConsole();

		printStudentsList(sortType, selectedStudent);
		printf("Sorted by: \033[36m%s\033[0m \nUse \033[35mARROW_LEFT\033[0m or \033[35mARROW_RIGHT\033[0m to switch between sorting modes.\nPress \033[35mESC\033[0m to exit.\n", commandsList[sortType]);

		int command = getPressedKey();

		if (command == ARROW_LEFT) {
			sortType = sortType == 0 ? 2 : sortType - 1;
		}

		if (command == ARROW_RIGHT) {
			sortType = sortType == 2 ? 0 : sortType + 1;
		}

		if (canSelect && command == ARROW_UP) {
			selectedStudent = selectedStudent == 0 ? numberOfStudents - 1 : selectedStudent - 1;
		}

		if (canSelect && command == ARROW_DOWN) {
			selectedStudent = selectedStudent == numberOfStudents-1 ? 0 : selectedStudent + 1;
		}

		clearConsole();

		if (command == ESC) {

			return -1;
		}

		if (canSelect && command == ENTER) {
			return Students[selectedStudent].id;
		}

	}
}

int printDisciplinesList(int canSelect) {
	int selectID = canSelect ? 0 : -1;

	while (1) {
		clearConsole();
		printf("\nID \t Name\n");

		for (int i = 0; i < numberOfDisciplines; i++) {
			if (selectID == i) printf("\033[4;36m");

			printf("%d \t %s\n\033[0m", Disciplines[i].id, Disciplines[i].name);
		}

		printf("\nPress \033[35mESC\033[0m to exit.\n");

		int command = getPressedKey();


		if (canSelect && command == ARROW_UP) {
			selectID = selectID == 0 ? numberOfDisciplines - 1 : selectID - 1;
		}

		if (canSelect && command == ARROW_DOWN) {
			selectID = selectID == numberOfDisciplines - 1 ? 0 : selectID + 1;
		}

		clearConsole();

		if (command == ESC) {
			return -1;
		}

		if (canSelect && command == ENTER) {
			return Disciplines[selectID].id;
		}

	}


	
	printf("\n");
}

int printReportsList(int canSelect) {
	int selectID = canSelect ? 0 : -1;

	while (1) {
		clearConsole();

		printf("\nID\t%s\t%s\tScore\n", stringAtrim("Student ID/Name(Group)\0", 40), stringAtrim("Descipline\0", 24));

		for (int i = 0; i < numberOfReports; i++) {
			Report report = Reports[i];
			Student student = Students[searchStudentByID(report.studentID)];

			if (selectID == i) printf("\033[4;36m");

			char* studentData[40];
			snprintf(studentData, 40, "%0.4d/%s (%d)", student.id, &student.name, student.group);

			printf("%d\t%s\t%s\t%.1f\n\033[0m", i, stringAtrim(studentData, 40), stringAtrim(Disciplines[searchDisciplineByID(report.disciplineID)].name, 24), report.score);
		}
		printf("\n");
		printf("\nPress \033[35mESC\033[0m to exit.\n");

		int command = getPressedKey();

		if (canSelect && command == ARROW_UP) {
			selectID = selectID == 0 ? numberOfReports - 1 : selectID - 1;
		}

		if (canSelect && command == ARROW_DOWN) {
			selectID = selectID == numberOfReports - 1 ? 0 : selectID + 1;
		}

		clearConsole();

		if (command == ESC) {
			return -1;
		}

		if (canSelect && command == ENTER) {
			return selectID;
		}
	}
}

void addStudentLoop() {
	char _сontinue = '0';

	while (_сontinue != ENTER) {
		Student student;

		student.id = getPrimaryKeyForStudentsBD();

		printf("Enter student name: ");
		gets_s(&student.name, 256);

		printf("Enter student group: ");
		scanf_s("%d", &student.group);

		addStudent(student);

		printf("Press ENTER to finish or ANY KEY to add more.\n");

		_сontinue = getPressedKey();

	}
}

void addDisciplineLoop() {
	char _сontinue = '0';
	while (_сontinue != ENTER) {
		Discipline discipline;

		discipline.id = getPrimaryKeyForDisciplinesBD();

		printf("Enter discipline name: ");
		gets_s(&discipline.name, 256);

		addDiscipline(discipline);

		printf("Press ENTER to finish or ANY KEY to add more.\n");

		_сontinue = getPressedKey();
	}
}

void addReportLoop() {
	char _сontinue = '0';
	printf("\033[32mCreating new report\n\033[0m");

	if (numberOfStudents == 0) {
		printf("\033[31mStudents DB is empty!\n\033[0m");
		return;
	}

	if (numberOfDisciplines == 0) {
		printf("\033[31mDisciplines DB is empty!\n\033[0m");
		return;
	}

	while (_сontinue != ENTER) {
		Report report;
		printf("\033[36mSelect list:\n\033[0m");

		report.disciplineID = printDisciplinesList(1);
		if (report.disciplineID == -1) {
			return;
		}

		printf("\033[36mSelect student:\n\033[0m");

		report.studentID = printStudentsListLoop(1);
		
		if (report.studentID == -1) {
			return;
		}

		while (1) {
			printf("\033[36mEnter Score [2..5] or 1 if student failed the retake:\033[0m ");
			scanf_s("%f", &report.score);

			if (report.score < 1 || report.score > 5) {
				printf("\033[31mScore must be in the range from 2 to 5 or 1 if student failed the retake!\n\033[0m");
				continue;
			}

			break;
		}

		addReport(report);
		printf("\033[36m=== New report ===\n Student: \033[0m%s\n \033[36mDiscipline: \033[0m%s\n \033[36mScore: \033[0m%.1f\033[36m \n\033[0m", Students[searchStudentByID(report.studentID)].name, Disciplines[searchDisciplineByID(report.disciplineID)].name, report.score);

		printf("Press ENTER to finish or ANY KEY to add more.\n");

		_сontinue = getPressedKey();
	}
}

void removeStudentLoop() {
	char _сontinue = '0';

	if (numberOfStudents == 0) {
		printf("\033[31mStudents DB is empty!\n\033[0m");
		return;
	}

	while (_сontinue != ENTER) {
		int studentID;

		printf("\033[36mStudent list:\n\033[0m");
		
		int pos = printStudentsListLoop(1);
		if (pos == -1) {
			return;
		}
		removeStudent(searchStudentByID(pos));
		
		printf("\033[32mStudent removed successfully.\n\033[0m");

		printf("Press ENTER to finish or ANY KEY to add more.\n");

		_сontinue = getPressedKey();
	}
}

void removeDisciplineLoop() {
	char _сontinue = '0';

	if (numberOfDisciplines == 0) {
		printf("\033[31mDisciplines DB is empty!\n\033[0m");
		return;
	}

	while (_сontinue != ENTER) {
		int disciplineID;

		printf("\033[36mDiscipline list:\n\033[0m");
		
		int pos = printDisciplinesList(1);
		if (pos == -1) {
			return;
		}

		pos = searchDisciplineByID(pos);

		removeDiscipline(pos);
		printf("\033[32mDiscipline removed successfully.\n\033[0m");
		printf("Press ENTER to finish or ANY KEY to add more.\n");

		_сontinue = getPressedKey();
	}

}

void removeReportLoop() {
	char _сontinue = '0';

	if (numberOfReports == 0) {
		printf("\033[31mReports DB is empty!\n\033[0m");
		return;
	}

	while (_сontinue != ENTER) {

		printf("\033[36mReport list:\n\033[0m");
		
		int pos = printReportsList(1);

		if (pos == -1) {
			return;
		}
		removeReport(pos);

		printf("\033[32mReport removed successfully.\n\033[0m");
		printf("Press ENTER to finish or ANY KEY to add more.\n");

		_сontinue = getPressedKey();
	}
}

void editStudentLoop() {
	char _сontinue = '0';
	int studentID = 0;

	if (numberOfStudents == 0) {
		printf("\033[31mStudent DB is empty!\n\033[0m");
		return;
	}

	while (_сontinue != ENTER) {


		printf("\033[36mStudent list:\n\033[0m");
		int pos = printStudentsListLoop(1);

		if (pos == -1) {
			return;
		}

		pos = searchStudentByID(pos);

		printf("Enter new group: ");
		scanf_s("%d", &Students[pos].group);

		printf("Press ENTER to finish or ANY KEY to add more.\n");

		_сontinue = getPressedKey();
	}
}

void dropAllDatabases(){
	Students = NULL;
	Disciplines = NULL;
	Reports = NULL;

	numberOfStudents = 0;
	numberOfReports = 0;
	numberOfDisciplines = 0;

	printf("\033[31mAll tables have been cleared.\n\033[0m");
}

int main() {

	initDataBases();

	int selectedCommand = 0;
	int command;

	printf("\033[36mDatabase Students has \033[0m%d\033[36m rows.\n\033[0m", numberOfStudents);
	printf("\033[36mDatabase Reports has \033[0m%d\033[36m rows.\n\033[0m", numberOfReports);
	printf("\033[36mDatabase Disciplines has \033[0m%d\033[36m rows.\n\033[0m", numberOfDisciplines);
	printf("\033[32mReady!\n\033[0m", numberOfDisciplines);

	char commandsList[][256] = { "Show the list of students.", "Show the list of disciplines.", "Show the list of reports.", "Add a student.", "Add a discipline.", "Add a report.", "Remove a student.", "Remove a discipline.", "Remove a report.", "Edit a student.", };
	int commandsLen = sizeof(commandsList) / 256;

	getPressedKey();
	clearConsole();

	while (1) {

		printf("\033[36mSelect command:\n\033[0m");

		for (int i = 0; i < commandsLen; i++)
		{
			if(selectedCommand == i) printf("\033[32;4m");
			printf("%s", commandsList[i]);
			printf("\033[0m\n");
		}

		printf("==========================\n");

		//printf("DELETE - Drop all tables. \n");
		printf("ESC - End session. \n");

		int command = getPressedKey();

		clearConsole();

		if (command == ARROW_UP) {
			selectedCommand = selectedCommand == 0 ? commandsLen - 1 : selectedCommand - 1;
		} else if (command == ARROW_DOWN) {
			selectedCommand = selectedCommand == commandsLen - 1 ? 0 : selectedCommand + 1;
		} else if (command == ESC) {
			break;
		}
		else if (command == ENTER) {

			switch (selectedCommand)
			{
			case 0:
				printStudentsListLoop(0);
				break;

			case 1:
				printDisciplinesList(0);
				break;

			case 2:
				printReportsList(0);
				break;

			case 3:
				addStudentLoop();
				break;

			case 4:
				addDisciplineLoop();
				break;

			case 5:
				addReportLoop();
				break;

			case 6:
				removeStudentLoop();
				break;

			case 7:
				removeDisciplineLoop();
				break;

			case 8:
				removeReportLoop();
				break;

			case 9:
				editStudentLoop();
				break;

				//case 224: // удалить базу данных
				//	dropAllDatabases();
				//	state = 1;
				//	break;

			}

			clearConsole();
		}
	}

	saveAndCloseDataBases();

	printf("\033[32mAll databases have been successfully saved.\n\033[0m");
	return 0;
}