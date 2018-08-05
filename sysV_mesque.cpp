#include <iostream>

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>

int main()
{
    key_t 	key=ftok("/tmp/msg.temp", 1);
    int		res;
    pid_t 	pid;
    int 	msqid;

    struct msgbuf
    {
        long mtype;       /* message type, must be > 0 */
        char msg[80];    /* message data */
    }msgbuf;


    msqid=msgget(key, IPC_CREAT|0666);
    assert(msqid!=-1);

    pid=fork();
    assert(pid!=-1);
    if(!pid)
    {
        //child's part
        res=msgrcv(msqid,&msgbuf,80,0,0);
        assert(-1!=res);
        std::cout<<msgbuf.msg<<std::endl;
    }
    else
    {
        //parent's part
        msgbuf.mtype=1;
        strcpy(msgbuf.msg, "hellO");
        res=msgsnd(msqid, &msgbuf, 80, 0);
        assert(-1!=res);
    }
}
