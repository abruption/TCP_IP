#include <stdio.h>
#include <unistd.h>

int gval = 10;  // Global 변수

int main(int argc, char *argv[]){
        // 프로세스 번호 (Process ID; PID)가 저장될 변수 
        pid_t pid;

        int lval = 20;
        gval++, lval += 5;

        // 자식 프로세스를 생성한다.
        // 성공 시 0, 실패 시 -1, 부모 프로세스이면 양수 (프로세스 ID)를 반환한다.
        pid = fork();
        if(pid == 0)    // pid가 0이면 자식 프로세스이다.
                gval += 2, lval += 2;
        else            // pid가 양수이면 부모 프로세스이다.
                gval -= 2, lval -= 2;

        if(pid == 0)
                printf("Child Proc : [%d, %d]\n", gval, lval);
        else
                printf("Parent Proc : [%d, %d]\n", gval, lval);
        return 0;
}
