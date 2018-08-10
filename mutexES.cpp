#include <iostream>

#include <unistd.h>


#include <stdio.h>

#include <pthread.h>

#include <assert.h>


#define cout(a) std::cout<<a<<std::endl;
#define non(a) assert(a>=0);

static pthread_mutex_t		m; //mutex
static pthread_spinlock_t	sm; //spinmutex
static pthread_rwlock_t		rm, wm; //read/writemutex

void* thread_mutex(void*arg)
{
    cout("mutex is running");

    int s;
    s=pthread_mutex_lock(&m);
    assert(s!=-1);

    s=pthread_mutex_unlock(&m);
    assert(s!=-1);

    cout("mutex finished");
}

void* thread_spinmutex(void*arg)
{
    cout("SPINmutex is running");

    int s;
    s=pthread_spin_lock(&sm);
    assert(s!=-1);

    s=pthread_spin_unlock(&sm);
    assert(s!=-1);

    cout("SPINmutex finished");
}

void* thread_readmutex(void*arg)
{
    cout("READmutex is running");

    int s;
    s=pthread_rwlock_rdlock(&rm);
    assert(s!=-1);

    s=pthread_rwlock_unlock(&rm);
    assert(s!=-1);

    cout("READmutex finished");
}

void* thread_writemutex(void*arg)
{
    cout("WRITEmutex is running");

    int s;
    s=pthread_rwlock_wrlock(&wm);
    assert(s!=-1);

    s=pthread_rwlock_unlock(&wm);
    assert(s!=-1);

    cout("WRITEmutex finished");
}

int main()
{
    pthread_t tid[4];
    int s;
    FILE*f;

    f=fopen("/home/adygha/Desktop/main.pid", "w+");
    assert(f!=NULL);
    fprintf(f, "%d", getpid());

    //init mutex
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    s=pthread_mutex_init(&m, NULL);
    non(s);

    s=pthread_spin_init(&sm, PTHREAD_PROCESS_SHARED);
    non(s);

    pthread_rwlockattr_t rwattr;
    pthread_rwlockattr_init(&rwattr);
    pthread_rwlockattr_setpshared(&rwattr, PTHREAD_PROCESS_SHARED);

    s=pthread_rwlock_init(&rm, &rwattr);
    non(s);
    s=pthread_rwlock_init(&wm, &rwattr);
    non(s);
    //=======================================

    //lock them==============================
    pthread_mutex_lock(&m);
    pthread_spin_lock(&sm);
    pthread_rwlock_rdlock(&rm);
    pthread_rwlock_wrlock(&wm);
    //=======================================

    cout("\nStarting all threads"
         "\n=======================================");


    //run threads============================
    s=pthread_create(&tid[0], NULL, thread_mutex, NULL);
    assert(s!=-1);
    s=pthread_create(&tid[1], NULL, thread_spinmutex, NULL);
    assert(s!=-1);
    s=pthread_create(&tid[2], NULL, thread_readmutex, NULL);
    assert(s!=-1);
    s=pthread_create(&tid[3], NULL, thread_writemutex, NULL);
    assert(s!=-1);
    //=======================================

    //unlock recources=======================
    sleep(3);
    pthread_mutex_unlock(&m);
    pthread_spin_unlock(&sm);
    pthread_rwlock_unlock(&rm);
    pthread_rwlock_unlock(&wm);
    //=======================================

    for(int i=0; i<4; ++i)
        pthread_join(tid[i], NULL);


    //destroy mutex&attr=====================
    pthread_mutexattr_destroy(&attr);
    pthread_rwlockattr_destroy(&rwattr);

    pthread_mutex_destroy(&m);
    pthread_spin_destroy(&sm);
    pthread_rwlock_destroy(&rm);
    pthread_rwlock_destroy(&wm);
    //=======================================
}

