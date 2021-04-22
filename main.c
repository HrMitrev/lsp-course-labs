#include <stdio.h>
#include <pthread.h>
#include "calc_sum.h"

void* new_thread(void* data)
{ //data will be NULL (I don't need it)

	//this would also work, but without synchronizations in the future
    //pthread_detach(pthread_self());
	
	printf("from new thread: sum = %d own ID: %lu\n", calc_sum(5, 5), pthread_self());
    pthread_exit(NULL);
}

/* like any C program, program's execution begins in main */
int main(int argc, char* argv[])
{
    pthread_t  thread_id;

    if(pthread_create(&thread_id, NULL, new_thread, (void*)NULL))
    {
        printf("pthread_create() was unsucessful\n");
        return -1;
    }
	
    printf("My new thread ID is: %lu and the main thread: %lu\n", thread_id, pthread_self());
	
	//I don't need the return value of new_thread() for now since it returns NULL anyway
	pthread_join(thread_id, NULL);
    
    pthread_exit(NULL);
}
