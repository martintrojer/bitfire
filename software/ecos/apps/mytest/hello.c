#include <stdio.h>
//#include <string.h>

int main(void)
{
	int i;
	FILE *f;
	//char s[] = "Hejsan!\n";
	//fwrite(s,strlen(s),1,f)

	f = fopen("/dev/ser1", "w");
	stdout = f;			// Redirect stdout

	printf("Hello, eCos world!\n");

	while(1)
	{
		i = hal_clock_read();
	}

	fclose(f);
	return 0;
}
