#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <wait.h>

#include <mqueue.h>

int main()
{
    int     res;
    mqd_t   mqd;
    mq_attr attr;
    char    msg[1024];

    attr.mq_flags=0;       /* Flags (ignored for mq_open()) */
    attr.mq_maxmsg=10;     /* Max. # of messages on queue */
    attr.mq_msgsize=1024;  /* Max. message size (bytes) */
    attr.mq_curmsgs=0;     /* # of messages currently in queue
                                                   (ignored for mq_open()) */


    mqd=mq_open("/test.mq", O_CREAT|O_RDWR, 0666, &attr);
    if(mqd==-1)
    {
        switch(errno)
        {
        case EACCES : std::cout<<"EACCES \n";break;
        case EEXIST: std::cout<<"EEXIST\n";break;
        case EINVAL : std::cout<<"EINVAL\n";break;
        case ENOENT: std::cout<<"ENOENT \n";break;
        case EFAULT: std::cout<<"EFAULT \n";break;
        default: std::cout<<"something else\n";
        }
        EXIT_FAILURE;
    }

    pid_t pid=fork();
    if(!pid)
    {
        //child
        res=mq_receive(mqd,msg,1024,NULL);
        assert(res!=-1);
        std::cout<<msg;
    }
    else
    {
        //parent
        res=snprintf(msg, sizeof(msg), "Hello!\n");
        mq_send(mqd, msg,res,0);

        wait(&res);
        res=mq_close(mqd);
        assert(res!=-1);
        res=mq_unlink("/test.mq");
        assert(res!=-1);
    }

//    res=write(open("/home/box/message.txt", O_CREAT|O_WRONLY|0666), msgbuf.msg, 80);
//    assert(-1!=res);
}
