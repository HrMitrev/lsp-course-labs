#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "calc_sum.h"

void *
new_thread (void *data)
{
  int *int_for_sum = data;

  int *sum = malloc (sizeof (int));
  *sum = calc_sum (int_for_sum[0], int_for_sum[1]);
  printf ("from new thread: sum = %d own ID: %lu\n", *sum, pthread_self ());
  pthread_exit ((void *) sum);
}

int
main (int argc, char *argv[])
{
  pthread_t thread_id;
  int int_for_sum[2];

  printf ("please input two integers\n");
  scanf ("%d %d", int_for_sum, int_for_sum + 1);

  if (pthread_create (&thread_id, NULL, new_thread, int_for_sum))
    {
      printf ("pthread_create() was unsucessful\n");
      return -1;
    }

  printf ("My new thread ID is: %lu and the main thread: %lu\n", thread_id,
	  pthread_self ());

  //I don't need the return value of new_thread() for now since it returns NULL anyway
  int *sum;
  pthread_join (thread_id, (void **)&sum);

  printf ("the sum from the helping thread is %d\n", *sum);

  free (sum);

  pthread_exit (NULL);
}
