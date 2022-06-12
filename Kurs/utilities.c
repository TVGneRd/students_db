#include <stdlib.h>

/* Расширяет строку до нужного количесвто знаков добавляя пробелы в конец строки */
char* stringAtrim(char* str, int len) {
	char *newStr = (char*)malloc(len * sizeof(char));
	int flag = 0;

	for (int i = 0; i < len - 1; i++) {
		if (str[i] == '\0') {
			flag = 1;
		}
		newStr[i] = flag ? ' ' : str[i];
	}

	newStr[len - 1] = '\0';

	return newStr;
}

void clearConsole()
{
	system("@cls||clear");
}

int getPressedKey() {
	int k1, k2;
	for (;;)
	{
		k1 = getch();
		if (k1 == 0xE0 || k1 == 0x00)
		{
			k2 = getch();
			return k2;
		} 

		return k1;
	}
}