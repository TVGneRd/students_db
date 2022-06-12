typedef struct
{
	int id;
	char name[256];
	int group;

} Student;

int numberOfStudents;

Student *Students;

void initStuentsDataBase();

void readStudentsBD();

int getPrimaryKeyForStudentsBD();

void addStudent(Student student);

void removeStudent(int pos);

int searchStudentByID(int studentID);

void saveAndCloseStudentsDataBase();