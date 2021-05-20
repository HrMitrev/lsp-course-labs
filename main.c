#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <sys/ioctl.h>

void exit_err(void)
{
	fprintf(stderr, "unexpected error\r\n");
	exit(EXIT_FAILURE);
}

int
main (int argc, char *argv[])
{
	struct stat status;
	int fd;
	
	fd = open("/dev/tty", O_RDONLY);
	
	if(fd == 0 || fstat(fd, &status))
	{
		exit_err();
	}
	
	struct winsize ws;
	ioctl(fd, TIOCGWINSZ, &ws);
	
	printf ("lines %d\n", ws.ws_row);
	printf ("columns %d\n", ws.ws_col);
		
	
	printf("press any key to exit...\r\n");
	
	//use system call to make terminal send all keystrokes directly to stdin
	system ("/bin/stty raw");
	char fictive = getchar();
	//use system call to set terminal behaviour to more normal behaviour
	system ("/bin/stty cooked");
	
	printf("\r\n");
	
	exit(EXIT_SUCCESS);	
}
