/*
2016_12_8
133333 정광현
리눅스시스템_명령어_hostname
*/

/*
명령어 : hostname
기능 : 호스트네임을 출력 및 변경
사용법 : hostname [옵션]	: 출력
	hostname [새로운 호스트 네임] : 변경
옵션
-f, --fqdn : 호스트 네임 출력 [기본 옵션]
    --help : 도움말 출력
    --version : 버전 정보 출력 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void readLine(int fd, char *str);
void getHostname();
void setHostname();
void printHelp();
void printVersion();

int main(int argc, char *argv[])
{
	int i;
	char hostname[1024];

	getHostname(hostname);
	
	if(argc == 1)
		printf("%s", hostname);
	else if(argc == 2)
	{
		if(strcmp(argv[1], "--help") == 0)
			printHelp();
		else if(strcmp(argv[1], "--version") == 0)
			printVersion();
		else if(strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--fqdn") == 0)
			printf("%s", hostname);
		else
			setHostname(argv[1]);	
	}
	else
	{
		printHelp();
	}
	exit(0);
}
	

void readLine(int fd, char *str)
{
	int n;
	do
	{
		n = read(fd, str, 1);
	}while(n > 0 && *str++ != NULL);
	*str = '\0';
}

void getHostname(char *name)
{
	int fd;
	fd = open("/proc/sys/kernel/hostname", O_RDONLY);

	readLine(fd, name);
	close(fd);
}
void setHostname(char *name)
{
	int fd1;
	int fd2;
	int length = strlen(name);

	fd1 = open("/proc/sys/kernel/hostname", O_WRONLY | O_TRUNC);
	fd2 = open("/etc/hostname", O_WRONLY | O_TRUNC);

	write(fd1, name, length);
	write(fd2, name, length);

	close(fd1);
	close(fd2);
}
void printHelp()
{
	printf("사용법 : hostname [옵션]\n");
	printf("사용법 : hostname [새로운 호스트 네임]\n");
	printf("호스트 네임 출력 및 변경\n");
	printf("-f, --fqdn\t\t전체 호스트 네임 (default option)\n");
	printf("    --help\t\t도움말\n");
	printf("    --version\t\t버전 정보\n");
	printf("\n");
}
void printVersion()
{
	printf("리눅스시스템 명령어 구현 1.0\n");
	printf("명령어 : hostname\n");
	printf("제작자 : 133333 정광현\n");
	printf("제작일 : 2016_12_09\n");
	printf("\n");
}
