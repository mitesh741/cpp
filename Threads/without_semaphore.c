#include <pthread.h>
#include <stdio.h>

void *f1(void *arg)
{
        printf("thread 1\n");
}

void *f2(void *arg)
{
        printf("thread 2\n");
}

int main()
{
        pthread_t myThread[2];
        pthread_create(&myThread[0], 0, f1, NULL);
        pthread_create(&myThread[1], 0, f2, NULL);
        pthread_join(myThread[0], 0);
        pthread_join(myThread[1], 0);
        return 0;
}
