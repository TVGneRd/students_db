#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

FILE *reportsDB;

typedef struct
{
	int studentID;
	int disciplineID;
	float score;

} Report;

Report *Reports = NULL;

int numberOfReports = 0;

void readReportsBD() {

	fscanf_s(reportsDB, "%i", &numberOfReports);

	Reports = (Report*)realloc(Reports, numberOfReports * sizeof(Report));

	for (int i = 0; i < numberOfReports; i++)
		fscanf_s(reportsDB, "%d|%d|%f", &Reports[i].disciplineID, &Reports[i].studentID, &Reports[i].score);
}

void initReportsDataBase() {
	if (fopen_s(&reportsDB, "db/reports.db", "r+") != NULL)
	{
		printf("Data base Report Card unavailable!");
		exit(0);
	}
	readReportsBD();
}

void addReport(Report report) {
	Reports = (Report*)realloc(Reports, (numberOfReports + 1) * sizeof(Report));
	Reports[numberOfReports++] = report;
}



void removeReport(int pos) {

	for (int i = pos; i < numberOfReports - 1; i++) {
		Reports[i] = Reports[i + 1];
	}

	numberOfReports--;
}

void saveAndCloseReportsDataBase() {
	fclose(reportsDB);

	fopen_s(&reportsDB, "db/reports.db", "w");
	fseek(reportsDB, 0, SEEK_SET);

	fprintf(reportsDB, "%i\n", numberOfReports);

	for (int i = 0; i < numberOfReports; i++)
		fprintf(reportsDB, "%d|%d|%f\n", Reports[i].disciplineID, Reports[i].studentID, Reports[i].score);
	//fwrite(Reports, sizeof(Report), numberOfReports, reportsDB);

	fclose(reportsDB);

	free(Reports);
}