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


char* search_char_forward(int len, char *str, char c)
{
	char *ret = NULL;
	for(int i=0; i<len && ret; ++i)
	{
		if( *(str+i) == c) ret = str+i;
	}
	
	return ret;
}

char* search_char_backward(int len, char *str, char c)
{
	if(len > 1)
	{
		str -= 2;
		len -= 2;
	}
	char *ret = NULL;
	for(int i=0; i<len && ret; ++i)
	{
		if( *(str-i) == c) ret = str+i;
	}
	
	return ret;
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
	
	fd = open("/usr/share/dict/words", O_RDONLY);
	
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
	
	char *start = (mapped_file + (len/2));
	int len_to_end = len - (start - mapped_file);
	start = search_char_forward(len_to_end, mapped_file, '\n') + 1;
	
	int word_len = strlen(argv[1]);
	
	for(int i=0; i<word_len; ++i)
	{
		int flag;
		while((flag = (*(start + i) - *(argv[1] + i))))
		{
			if(flag < 1)
			{
				int len_to_end = len - (start - mapped_file);
				start = search_char_forward(len_to_end, mapped_file, '\n') + 1;
			}
			else
			{
				int len_to_end = (start - mapped_file);
				start = search_char_backward(len_to_end, mapped_file, '\n') + 1;
			}
		}
	}
	
	printf("word - %s\r\n", start);
	
	
	char *word = strstr(mapped_file, argv[1]);
	if(word != NULL && *(word + strlen(argv[1])) != '\r' && *(word + strlen(argv[1])) != '\n') word = NULL;
	
	printf("the word %s is %s in the dictionary\r\n", argv[1], word == NULL ? "absent" : "present");
	
	if(word)
	{
		
	}
	
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
