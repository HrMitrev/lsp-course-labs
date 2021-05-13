#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

void exit_err(void)
{
	fprintf(stderr, "unexpected error\r\n");
	exit(EXIT_FAILURE);
}

int
main (int argc, char *argv[])
{
	size_t len;
	struct stat status;	
	char *mapped_file;
	int fd;
	
	if(argc != 2)
	{
		exit_err();
	}
	
	fd = open(argv[1], O_RDONLY);
	
	if(fd == 0 || fstat(fd, &status))
	{
		exit_err();
	}
	
	len = status.st_size;
	
	if ((mapped_file = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		exit_err();
	}
	
	printf("the mapped file pointer is 0x%p\r\n", mapped_file);
	
	printf("the file contents are:");
	for(int i=0; i<len; ++i)
	{
		if(i%16 == 0) printf("\r\n");
		printf("%X ", mapped_file[i]);
	}
	
	printf("\r\n");
	
	if (munmap(mapped_file, len) || close(fd))
	{
		fprintf(stderr, "file close err\r\n");
	}
	
	printf("press any key to exit...\r\n");
	
	//use system call to make terminal send all keystrokes directly to stdin
	system ("/bin/stty raw");
	char fictive = getchar();
	//use system call to set terminal behaviour to more normal behaviour
	system ("/bin/stty cooked");
	
	printf("\r\n");
	
	exit(EXIT_SUCCESS);	
}
