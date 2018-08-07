#include <iostream>

#include <unistd.h>
#include <assert.h>

#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
    int		res;
    key_t	key;
    int		sid;

    union semun
    {
        int val;
        semid_ds*buf;
        unsigned short array[16];
    }arg;

    
    key=ftok("/tmp/sem.temp", 1); //file is must be exists
    assert(key!=-1);

    sid=semget(key, 16, IPC_CREAT|0666);
    assert(sid!=-1);

    for(int i=0; i<16; ++i)
        arg.array[i]=i;

    res=semctl(sid,0,SETALL,arg.array);
    assert(res!=-1);

//    res=semctl(sid, 0, IPC_RMID); //removing a semaphore
//    assert(res!=-1);
}
