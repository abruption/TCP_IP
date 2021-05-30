#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	pid_t pid = fork();

	if(pid == 0)
            // 자식 프로세스일 경우
            puts("Hi I'm a child process");
    else
            // 자식 프로세스의 값을 받지 못하게 sleep
            printf("Child process ID : %d\n", pid); sleep(30);

    if(pid == 0)
            puts("End child process");
    else
            puts("End parent process");
    return 0;
}	
