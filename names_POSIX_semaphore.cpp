#include <iostream>

#include <unistd.h>
#include <assert.h>

#include <fcntl.h>
#include <semaphore.h>

int main()
{
    int		res;
    sem_t	*sid;

    sid=sem_open("/test.sem", O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, 66);
    if(sid==SEM_FAILED)
    {
        std::cerr<<"SEM_FAILED\n";
        exit(1);
    }

    res=sem_close(sid);
    assert(res!=-1);

    res=sem_unlink("/test.sem");
    assert(res!=-1);
}
