#include <iostream>

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <assert.h>

size_t receive_fd(int read_from, int*fd)
{
    int		size;
    char	data[1];

    msghdr	msg;
    iovec	iov;
    union{
        struct cmsghdr cmsghdr;
        char control[CMSG_SPACE(sizeof(int))];
    }cmsgu;

    cmsghdr*cmsg;

    iov.iov_base=data; iov.iov_len=sizeof(data);
    msg.msg_name=NULL; msg.msg_namelen=0;
    msg.msg_iov=&iov; msg.msg_iovlen=1;
    msg.msg_control=cmsgu.control;
    msg.msg_controllen=sizeof(cmsgu.control);

    size=recvmsg(read_from, &msg, 0);
    assert(size!=-1);

    cmsg=CMSG_FIRSTHDR(&msg);

    if(cmsg && cmsg->cmsg_len==CMSG_LEN(sizeof(int)))
    {
        assert(cmsg->cmsg_level==SOL_SOCKET);
        assert(cmsg->cmsg_type==SCM_RIGHTS);

        *fd=*((int*)CMSG_DATA(cmsg));
    }
    else
        *fd=-1;

    return size;
}

size_t send_fd(int send_to, int fd)
{
    char 	data[1];
    size_t 	size;
    msghdr 	msg;
    iovec 	iov;
    cmsghdr	*cmsg;

    union{
        struct cmsghdr	cmsghdr;
        char	control[CMSG_SPACE(sizeof(int))];
    }cmsgu;

    iov.iov_base=data; iov.iov_len=sizeof(data);
    msg.msg_name=NULL; msg.msg_namelen=0;
    msg.msg_iov=&iov; msg.msg_iovlen=1;

    if(fd!=-1)
    {
        msg.msg_control=cmsgu.control;
        msg.msg_controllen=sizeof(cmsgu.control);
        cmsg=CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_len=CMSG_LEN(sizeof(int));
        cmsg->cmsg_level=SOL_SOCKET;
        cmsg->cmsg_type=SCM_RIGHTS;

        *((int*)CMSG_DATA(cmsg))=fd;
    }
    else
    {
        msg.msg_control=NULL;
        msg.msg_controllen=0;
        printf("not passing fd\n");
    }
    size=sendmsg(send_to, &msg, 0);
    assert(size!=-1);
    return size;
}

void child_function(int slave)
{
    //receive fd from parent
    //read a request
    //exec one
    //sent result to a client

    int fd;
    receive_fd(slave, &fd);
    assert(fd>0);
//    char buf[1024]; bzero(buf, sizeof(buf));
//    read(fd,buf,sizeof(buf));
//    write(fd, "Test\n", 5);
//    std::cout<<fd<<std::endl;
}

void parent_function(int send_to)
{
    //wait a connection, accept one
    //sent slave fd to child process

    //assume we received some slave
    //(for example I'll use some fd)
    int slave;
//    slave=open("file.txt", O_RDONLY);
    slave=1;
    assert(slave!=-1);
    int size = send_fd(send_to, slave);
    assert(size!=-1);
}

int main()
{
    int 	fds[2], res;
    pid_t 	pid;
    res=socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    assert(-1!=res);
    pid=fork();
    assert(pid!=-1);
    if(!pid)
    {
        //child's part
        child_function(fds[0]);
    }
    else
    {
        //parent's part
        parent_function(fds[1]);
    }
}
