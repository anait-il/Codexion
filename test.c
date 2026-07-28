#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *a(void *l)
{
    printf("%ld\n", pthread_self());
    printf("%d\n", gettid());

    
    return NULL;
}

int main()
{
    
}