#include "kernel/types.h"
#include "user/user.h"

#define READED 0
#define WRITTEND 1

int main(int argc, char* argv[]){
    int pid;
    int p1[2];
    int p2[2];
    char buf[1];

    pipe(p1);
    pipe(p2);

    pid = fork();

    if(pid < 0) exit(1);
    else if(pid == 0){
        close(p1[WRITTEND]);
        close(p2[READED]);
        read(p1[READED], buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[WRITTEND], " ", 1);
        close(p1[READED]);
        close(p2[WRITTEND]);
        exit(0);
    }else{
        close(p2[WRITTEND]);
        close(p1[READED]);
        write(p1[WRITTEND], " ", 1);
        read(p2[READED], buf, 1);
        printf("%d: received pong\n", getpid());
        close(p2[READED]);
        close(p1[WRITTEND]);
        exit(0);
    }
}