#include <iostream>

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>

int main()
{
    key_t key=ftok("/tmp/msg.temp", 1);
    int		msgflg, res;
    pid_t 	pid;
    int msqid;

    struct msgbuf
    {
        long mtype=1;       /* message type, must be > 0 */
        char msg[80];    /* message data */
    }msgbuf;

    strcpy(msgbuf.msg, "hellO");

    msqid=msgget(key, IPC_CREAT|0666);
    assert(msqid!=-1);

    pid=fork();
    assert(pid!=-1);
    if(!pid)
    {
        //child's part
        res=msgrcv(msqid,&msgbuf,80,1,0);
        assert(-1!=res);
        std::cout<<msgbuf.msg<<std::endl;
    }
    else
    {
        //parent's part
        res=msgsnd(msqid, &msgbuf, 80, 0);
        assert(-1!=res);
    }
}

