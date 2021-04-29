#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int
main (int argc, char *argv[])
{
  int fd_parent[2];
  int fd_child[2];

  int ret_pipe_parent = pipe (fd_parent);
  int ret_pipe_child = pipe (fd_child);

  if (ret_pipe_parent < 0 || ret_pipe_child < 0)
    {
      fprintf (stderr, "error - pipe() failed child - %d parent - %d\r\n",
	       ret_pipe_child, ret_pipe_parent);
      return EXIT_FAILURE;
    }

  //fd[0] will be as stdin and fd[1] will be as stdout
  int fd_stdin_from_parent = fd_parent[0];
  int fd_parent_stdout2 = fd_parent[1];

  int fd_stdin_from_child = fd_child[0];
  int fd_child_stdout = fd_child[1];

  pid_t pid = fork ();

  if (pid < 0)
    {
      fprintf (stderr, "error - fork() failed %d\r\n", pid);
      return EXIT_FAILURE;
    }
  else if (pid == 0)
    {
      printf ("I am the child, now I will invoke bc\r\n");
      dup2 (fd_stdin_from_parent, STDIN_FILENO);
      dup2 (fd_child_stdout, STDOUT_FILENO);

      int ret = execlp ("bc", "bc", (char *) NULL);
      if (ret)
	{
	  fprintf (stderr, "error - execlp() failed %d\r\n", ret);
	  return EXIT_FAILURE;
	}
    }
  else
    {
      printf ("I'm the parent - I have to pipe data to my child\r\n");

      const char buff_for_child[] = "2 + 2\n";
      write (fd_parent_stdout2, buff_for_child, strlen (buff_for_child));
      char buff_from_child[20];
      read (fd_stdin_from_child, buff_from_child, 20);

      fprintf (stdout, "2 + 2 = %s", buff_from_child);
    }
}
