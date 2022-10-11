#include "kernel/types.h"
#include "user/user.h"

#define PRIME_NUM 35
#define READ 0
#define WIRTE 1

void child(int *pp);

int main(int argc, char* argv[]){
    int pp[2];
    pipe(pp);
    if(fork() == 0){
        child(pp);
    }else{
        close(pp[READ]);
        for(int i = 2; i < PRIME_NUM + 1; i++){
            write(pp[WIRTE], &i, sizeof(int));
        }
        close(pp[WIRTE]);
        wait((int *) 0);
    }
    exit(0);
}

void child(int* pp){
    close(pp[WIRTE]); // 关闭父进程写
    int num; // 保存读到的数

    // 首先检查一下管道中是否有数据，没有代表都传完了直接退出
    int read_result = read(pp[READ], &num, sizeof(int)); 
    if(read_result == 0){
        exit(0);
    }

    // 为当前进程开辟与其子进程通信的管道
    int cp[2];
    pipe(cp);


    if(fork() == 0){
        child(cp);
    }else{
        close(cp[READ]);
        printf("prime %d\n", num);
        // 保存读到的第一个数，用作素数判断
        int prime = num; 
        // 将管道中的数读完，从pp中读到num，再从num写到cp
        while(read(pp[READ], &num, sizeof(int)) != 0){
            if(num % prime != 0){
                write(cp[WIRTE], &num, sizeof(int));
            }
        } 
        close(cp[WIRTE]);
        // 每个子进程都需要等待其子进程结束后才可结束，
        // 从而确保root进程退出时所有子进程都已退出
        wait((int *) 0);
        exit(0);
    }
}