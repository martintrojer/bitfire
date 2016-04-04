#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

void cyg_hal_plf_serial_putc( void )
{
}

void listdir(char *name)
{
	int err;
	DIR *dirp;
	int num=0;

	dirp = opendir( name );
	if( dirp == NULL )
 		return;
	
	printf("%-20s %s\n","Filename","Size");
	while(1)
	{
		struct dirent *entry = readdir( dirp );
		struct stat sbuf;
        
		if( entry == NULL )
			break;
		
		err = stat(entry->d_name, &sbuf);
		if (err>=0) {
			printf("%-20s %d\n",entry->d_name,sbuf.st_size);
		}
		num++;
	}
	
	printf("Directory contains %d files.\n",num);
	err = closedir( dirp );
}

#define BLOCK_SIZE 512

int main(void)
{
	int i;
	
	FILE *so = fopen("/dev/ser1","w");
	stdout = so;
	
	printf("Hello, eCos world!\n");
	mount(0,"/","mtffs");

#if 0
	{
		FILE *f,*f2;
		char buf[BLOCK_SIZE+4];
		char buf2[BLOCK_SIZE+4];
		char wbufA[BLOCK_SIZE/2];
		char wbufB[BLOCK_SIZE/2];
		char wbuf[4] = {1,2,3,4};
		
		memset(wbufA,0x55,BLOCK_SIZE/2);
		memset(wbufB,0xAA,BLOCK_SIZE/2);
	
		
		f = fopen("/kalle","w");
		f2 = fopen("/olle","w");
		fwrite(wbuf,1,4,f2);
		fclose(f2);
		
		fwrite(wbufA,1,BLOCK_SIZE/2,f);
		fwrite(wbufB,1,BLOCK_SIZE/2,f);
		fwrite(wbuf,1,4,f);
			
		fclose(f);
	
		f = fopen("/kalle","r");
		f2 = fopen("/olle","r");
		fread(buf,1,BLOCK_SIZE+4,f);
		fread(buf2,1,BLOCK_SIZE+4,f2);
		fclose(f);
		fclose(f2);
	}
#endif
	
	{
		// 'xmodem' test
#define BUFSIZE 128
		char xbuf[BUFSIZE];
		int i;
		FILE *f;
	
		f = fopen("/xmodem","w");
		for (i=0;i<BUFSIZE*100;i+=BUFSIZE)
			fwrite(xbuf,1,BUFSIZE,f);
		fclose(f);
	}
	
	listdir("/");

	umount("/");
	
	while(1)
	{
		i = hal_clock_read();
	}

	return 0;
}
