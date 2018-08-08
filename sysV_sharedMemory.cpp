#include <iostream>

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#define MEGABYTE 1048576

int main()
{
    int		res;
    key_t	key;
    int 	shmid;
    char	*shmp;
    FILE	*file;

    file=fopen("/tmp/mem.temp", "w");
    assert(file!=NULL);
    fclose(file);

    key=ftok("/tmp/mem.temp", 1);
    assert(key!=-1);

    shmid=shmget(key, MEGABYTE, IPC_CREAT|0666);
    assert(key!=-1);

    shmp=(char*)shmat(shmid, NULL, 0);
    assert(*(int*)shmp!=-1);

    memset(shmp, 42, MEGABYTE);

    res=shmdt(shmp);
    assert(res!=-1);

    sleep(5);
    res=shmctl(shmid, IPC_RMID, 0);
    assert(res!=-1);
}
