#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>  


void *worker_thread(void *arg)
{
	pthread_exit((void*)12);
}

int main()
{
	int i;
	pthread_t thread;
	pthread_create(&thread, NULL, worker_thread, NULL);
	pthread_join(thread, (void **)&i);
	printf("%d\n",i);  // will print out 911

}


