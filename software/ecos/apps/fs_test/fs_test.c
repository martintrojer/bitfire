#include <stdio.h>
#include <cyg/io/flash.h>
#include <malloc.h>

int main(void)
{
	int i;
	FILE *f;
	void *p1=0,*p2=0;
	//char s[] = "Hejsan!\n";
	//fwrite(s,strlen(s),1,f)

	f = fopen("/dev/ser1", "w");
	stdout = f;			// Redirect stdout
	stderr = f;

	printf("Hello, eCos world!\r\n");

	p1 = malloc(64);
	p2 = malloc(4096);

	mount(CYGDAT_IO_FLASH_BLOCK_DEVICE_NAME_1,"/","jffs2");

	while(1)
	{
		i = hal_clock_read();
	}

	fclose(f);
	return 0;
}
