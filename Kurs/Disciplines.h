typedef struct
{
	int id;
	char name[256];

} Discipline;

int numberOfDisciplines;
Discipline *Disciplines;

void initDisciplinesDataBase();

int getPrimaryKeyForDisciplinesBD();

void readDesciplinesBD();

int searchDisciplineByID(int disciplinID);

void removeDiscipline(int pos);

void addDiscipline(Discipline discipline);

void saveAndCloseDisciplinesDataBase();