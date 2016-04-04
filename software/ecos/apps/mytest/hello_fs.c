#include <stdio.h>
#include <cyg/io/flash.h>
//#include <string.h>

void cyg_hal_plf_serial_putc( void )
{
}

int main(void)
{
	int i;
	FILE *f;
	//char s[] = "Hejsan!\n";
	//fwrite(s,strlen(s),1,f)

	f = fopen("/dev/ser1", "w");
	stdout = f;			// Redirect stdout

	printf("Hello, eCos world!\n");

	mount(CYGDAT_IO_FLASH_BLOCK_DEVICE_NAME_1,"/","jffs2");

	while(1)
	{
		i = hal_clock_read();
	}

	fclose(f);
	return 0;
}
