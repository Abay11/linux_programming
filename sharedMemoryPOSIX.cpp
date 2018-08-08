#include <iostream>

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <sys/mman.h> //mmap(...) - map or unmap files or devices into memory
#include <fcntl.h>


#define MEGABYTE 1048576

int main()
{
    int		res;
    int 	shmfd;
    char	*shmp;

    shmfd=shm_open("/test1.shm", O_CREAT|O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);
    assert(shmfd!=-1);

    res=ftruncate(shmfd, MEGABYTE);
    assert(res!=-1);

    shmp=(char*)mmap(NULL, MEGABYTE, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
    assert(shmp!=MAP_FAILED);

    memset(shmp, 13, MEGABYTE);

//    for(u_char i=0; i<256; ++i)
//        std::cout<<(int)shmp[i]<<std::endl;

    sleep(10);
    res=shm_unlink("/test1.shm");
    assert(res!=-1);

    res=close(shmfd);
    assert(res!=-1);
}
