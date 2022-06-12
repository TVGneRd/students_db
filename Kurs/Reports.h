FILE *reportsDB;

typedef struct
{
	int studentID;
	int disciplineID;
	float score;

} Report;

Report *Reports;

int numberOfReports;

void readReportsBD();

void initReportsDataBase();

void addReport(Report report);

void removeReport(int pos);

void saveAndCloseReportsDataBase();