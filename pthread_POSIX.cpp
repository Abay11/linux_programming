#include <iostream>

#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include <pthread.h>

void* thread_start(void*arg)
{
    std::cout<<"thread is running"<<std::endl;
    sleep(5);
}

int main()
{
    pthread_t tid;
    int s;
    FILE*f;

    f=fopen("/home/adygha/Desktop/main.pid", "w+");
    assert(f!=NULL);
    fprintf(f, "%d", getpid());

    s=pthread_create(&tid, NULL, thread_start, NULL);
    assert(s!=-1);

    pthread_join(tid, NULL);
}
