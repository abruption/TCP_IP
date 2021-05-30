#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	
	pid_t pid;
	struct sigaction act; //sigaction을 이용해서 자식 프로세스의 소멸을 확인한다.
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1)
			continue;
		else
			puts("new client connected...");
		pid=fork(); //새로운 클라이언트가 접속되면, 자식 프로세스에게 새로운 클라이언트를 넘겨준다. 
		if(pid==-1)
		{
			close(clnt_sock); //자식 프로세스가 종료되면, 클라이언트 접속을 끊는다. 
			continue;
		}
		if(pid==0) //자식 프로세스 실행 영역
		{
			close(serv_sock); //부모 프로세스에서 서버 소켓을 그대로 가져오는데, 자식 프로세스에서는 쓰지 않으므로 닫았다. 
			while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0)
				write(clnt_sock, buf, str_len);
			
			close(clnt_sock); //통신이 완료되면 클라이언트 소켓을 닫는다. 
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock); //클라이언트 소켓은 자식 프로세스에서 사용한다.
            					// 부모 프로세스의 클라이언트 소켓은 닫아준다. 
	}
	close(serv_sock);
	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d \n", pid);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
