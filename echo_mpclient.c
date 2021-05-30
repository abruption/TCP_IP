#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);  
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	pid=fork();
	if(pid==0) //자식 프로세스는 write routine 호출
		write_routine(sock, buf);
	else //부모 프로세스는 read routine 호출
		read_routine(sock, buf);

	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	while(1) //부모 프로세스는 읽기명령어를 호출하고, 읽을 내용이 없으면 return 한다.
	{
		int str_len=read(sock, buf, BUF_SIZE);
		if(str_len==0)
			return;

		buf[str_len]=0;
		printf("Message from server: %s", buf);
	}
}
void write_routine(int sock, char *buf)
{
	while(1)
	{
		fgets(buf, BUF_SIZE, stdin); //자식 프로세스는 최대 30개 크기의 문자를 입력받는다. 
		if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{	
			shutdown(sock, SHUT_WR); //쓰기 소켓(출력 소켓)을 닫고, return
			return;
		}
		write(sock, buf, strlen(buf)); //버퍼 크기 만큼 쓰기를 진행한다.
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
