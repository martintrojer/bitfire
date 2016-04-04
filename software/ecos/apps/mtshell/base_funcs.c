// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// Basic functions

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <cyg/fileio/fileio.h>

#include "mtshell.h"


int meminfo_func(char *s)
{
	struct mallinfo mi;
	 
	mi= mallinfo();
	
	printf("Total size: %d\n",mi.arena);
	printf("Free blocks: %d\n",mi.ordblks);
	printf("Total allocated: %d\n",mi.uordblks);
	printf("Total free: %d\n",mi.fordblks);
//	printf("Max free: %d",mi.maxfree);
}

int fsinfo_func(char *s)
{
	int err;
#ifdef CYGSEM_FILEIO_INFO_DISK_USAGE
	struct cyg_fs_disk_usage usage;	
	err = cyg_fs_getinfo(MOUNT_POINT, FS_INFO_DISK_USAGE, &usage, sizeof(usage));
	printf("Total size: %6lld blocks, %10lld bytes\n", usage.total_blocks, usage.total_blocks * usage.block_size); 
	printf("Free size:  %6lld blocks, %10lld bytes\n", usage.free_blocks, usage.free_blocks * usage.block_size); 
	printf("Block size: %6u bytes\n", usage.block_size);
#endif
}

int mkf_func(char *s)
{
	char arg1[ARG_SIZE];
	char arg2[ARG_SIZE];
	FILE *f;
	char *buf = 0;
	int size=0;
		
	get_arg(s,arg1,1);
	get_arg(s,arg2,2);
	
	size = atoi(arg2);
	if (size>0)
		buf = malloc(size);

	if (strlen(arg1)>0)
	{
		f = fopen(arg1,"w");
		if (buf != NULL) {
			fwrite(buf,1,size,f);
			free(buf);
		}
		fclose(f);
	}

	umount("/");					// Write out FAT to flash
	mount(0,MOUNT_POINT,"mtffs");
}

int rmf_func(char *s)
{
	char arg1[ARG_SIZE];
		
	get_arg(s,arg1,1);
	
	if (strlen(arg1)>0)
	{
		unlink(arg1);
	}
}

int cat_func(char *s)
{
	char arg1[ARG_SIZE];
		
	get_arg(s,arg1,1);
	
	if (strlen(arg1)>0)
	{
		struct stat sbuf;
		int err;
		err = stat(arg1, &sbuf);
		if (err>=0) {
			FILE *f = fopen(arg1,"r");
			char *buf = malloc(sbuf.st_size+10);
			int i,rs;
			rs=fread(buf,1,sbuf.st_size+10,f);
			
			printf("Read %d bytes\n",rs);
			for (i=0;i<rs;i++)
				printf("%c",buf[i]);
//				printf("%c(0x%x)",buf[i],buf[i]);
			printf("\n");
			
			free(buf);
			fclose(f);
		}
	}
}

int kill_func(char *s)
{
	cyg_handle_t th;
	char *erptr = NULL;
	char arg1[ARG_SIZE];
		
	get_arg(s,arg1,1);
	
	if (strlen(arg1)>0)
	{
		th = strtol(arg1, &erptr, 0);

		if(*erptr) {
			printf("Invalid thread handle\n");
			return;
		}
	
		cyg_thread_kill(th);
		cyg_thread_delete(th);
	}
}

int ls_func(char *s)
{
	int err;
	DIR *dirp;
	int num=0;

	dirp = opendir( MOUNT_POINT );
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
	
	printf("Directory contains %d file(s).\n",num);
	err = closedir( dirp );
}

int help_func(char *s)
{
	printf("List of commands\n");

	int ctr=0;
	command c = command_list[ctr++];
	
	while(c.name!=0) {
		printf("%-20s %s\n",c.name,c.expl);
		c = command_list[ctr++];		
	}
}
