/*
2016_12_5
133333 정광현
리눅스시스템_명령어_wc
*/

/*
명령어 : WC
기능 : 명령행 인자로 옵션과 파일명을 받아, 해당 파일의 바이트, 문자, 단어, 행, 최대길이 등을 구하는 명령어
사용법 : wc [옵션] [파일명]
옵션
-c, --bytes : 바이트수 출력
-m, --chars : 문자수 출력
-w, --words : 단어수 출력
-l, --lines : 행의수 출력
-L, --max_line_length : 행의 최대 길이 출력
    --help : 도움말 출력
    --version : 버전 정보 출력 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int getMode(int argc, char *argv[]);
void getCount(char *file, int *count); 
void printHelp();
void printVersion();
void printCount(char *file,int argc, char *argv[], int mode, int *count);

int main(int argc, char* argv[])
{
	int count[5] = {0, 0, 0, 0, 0};
	int mode;
	char *file;

	mode = getMode(argc, argv);	// 명령행의 입력 타입에 따른 분류

	switch(mode)
	{
	case 0:
		break;
	case 1:
		file = argv[1];
		break;
	case 2:
	case 3:
		file = argv[2];
		break;
	case 4:
		file = argv[argc-1];
		break;
	}
	if(mode != 0)
		getCount(file, count);	// 파일에서 바이트, 문자, 단어, 행의 수 등을 읽음
	printCount(file, argc, argv, mode, count);	// 읽어온 바이트, 문자, 단어, 행 등을 출력

	exit(0);
}

int getMode(int argc, char *argv[])
{
	if(argc == 2)
	{
		if(argv[1][0] == '-')	// wc --help, wc --version 일 경우
			return 0;
		else			// wc [파일명] 일 경우
			return 1;
	}
	else if(argc == 3)
	{
		if(argv[1][1] == '-' || strlen(argv[1]) == 2)	// wc [옵션] [파일명] 일 경우 ex) wc -c file
			return 2;
		else
			return 3;	// wc [옵션] [파일명] 일 경우 ex) wc -cwlL file
	}
	else
		return 4;		// wc [옵션1] [옵션2] ... [옵션n] [파일명] 일 경우 ex) wc -c -w -l -L file
}
void printHelp()
{
	printf("사용법 : wc [옵션] [파일]\n");
	printf("행, 단어, 바이트 수를 각 파일에서 카운트\n");
	printf("-c, --bytes\t\t바이트수 출력\n");
	printf("-w, --words\t\t단어수 출력\n");
	printf("-m, --chars\t\t문자수 출력\n");
	printf("-l, --lines\t\t라인수 출력\n");
	printf("-L, --max-line-length\t가장긴 행의 길이 출력\n");
	printf("    --help\t\t도움말\n");
	printf("    --version\t\t버전 정보\n");
	printf("\n");
}
void printVersion()
{
	printf("리눅스시스템 명령어 구현 1.0\n");
	printf("명령어 : wc\n");
	printf("제작자 : 133333 정광현\n");
	printf("제작일 : 2016_12_08\n");
	printf("\n");
}
void getCount(char* file, int* count)
{
	int fd;
	char buf[2];
	char pbuf = 0;
	int n= 0;
	
	if((fd = open(file, O_RDONLY)) == -1)
				perror(file);
	while(read(fd, buf, 1))	//	파일에서 1byte 씩 읽어들여, 바이트,문자, 단어, 행 등을 판
	{
		count[0]++;
		count[1]++;

		if(buf[0] == ' ' || buf[0] == '\n' || buf[0] == '\t')	//	단어 단위로 판별하기 위한 조건문
			if(pbuf != ' ' && pbuf != '\n' && pbuf != '\t')	//	공백,개행,탭 등을 이용하여 단어 단위로 판별, 연속으로 공백이 들어가는 경우를 필터
				count[2]++;

		if(buf[0] == '\n')	//	행의 수를 판별, 기존에 중간의 있는 내용없는 빈줄은 계산하지 않았으나, 기존 wc명령어와 동일하게 빈줄도 계산
			count[3]++;

		if(buf[0] != '\n')
			n++;
		else
			n = 0;
	
		if(count[4] < n)
			count[4] = n;
		pbuf = buf[0];
	}
	close(fd);
}
int isBytes(char *str)	//	들어온 옵션이 바이트수 옵션인지 확인
{
	if(strcmp(str, "-c") == 0 || strcmp(str, "--bytes") == 0)	
		return 1;
	return 0;
}
int isWords(char *str)	//	들어온 옵션이 단어수 옵션인지 확인
{
	if(strcmp(str, "-w") == 0 || strcmp(str, "--words") == 0)
		return 1;
	return 0;
}
int isChars(char *str)	//	들어온 옵션이 문자수 옵션인지 확인
{
	if(strcmp(str, "-m") == 0 || strcmp(str, "--chars") == 0)
		return 1;
	return 0;
}
int isLines(char *str)	//	들어온 옵션이 행의 수 옵션인지 확인 
{
	if(strcmp(str, "-l") == 0 || strcmp(str, "--lines") == 0)
		return 1;
	return 0;
}
int isLength(char *str)	//	들어온 옵션이 최대 행의 길이 인지 확인
{
	if(strcmp(str, "-L") == 0 || strcmp(str, "--max-line-length") == 0)
		return 1;
	return 0;
}
int isHelp(char *str)	//	옵션이 도움말 인지 확인
{
	if(strcmp(str, "--help") == 0)
		return 1;
	return 0;
}
int isVersion(char *str)	//	옵션이 버전정보 인지 확인
{
	if(strcmp(str, "--version") == 0)
		return 1;
	return 0;
}
void printCount(char *file,int argc, char *argv[] , int mode, int* count)	// 계산한 바이트,문자,단어,행, 최대길이 등을 옵션에 맞춰 출력시키는 함수
{
	int c = count[0];
	int m = count[1];
	int w = count[2];
	int l = count[3];
	int L = count[4];
	int n;
	int i;
	if(mode == 0)
	{
		if(isHelp(argv[1]))
			printHelp();
		else if(isVersion(argv[1]))
			printVersion();
	}
	else if(mode == 1)
	{
		printf(" %d %d %d %s\n", l, w, m, file);	//	wc [파일명] = 행 단어 문자 [파일명]
	}
	else if(mode == 2)
	{
		if(isBytes(argv[1]))
			printf(" %d %s\n", c, file);	//	wc -c [파일명] = 바이트 [파일명]	
		else if(isWords(argv[1]))
			printf(" %d %s\n", w, file);	//	wc -w [파일명] = 단어 [파일명]
		else if(isChars(argv[1]))		
			printf(" %d %s\n", m, file);	//	wc -m [파일명] = 문자 [파일명]
		else if(isLines(argv[1]))
			printf(" %d %s\n", l, file);	//	wc -l [파일명] = 행 [파일명]
		else if(isLength(argv[1]))
			printf(" %d %s\n", L, file);	//	wc -L [파일명] = 최대길이 [파일명]
	}
	else if(mode == 3)
	{
		n = strlen(argv[1]);
		for(i = 1; i <= n; i++)			//	옵션이 (-) 생략형으로 들어올 경우 처리 ex) wc -cwl [파일명] = 바이트 단어 행 [파일명]
		{
			if(argv[1][i] == 'c')
				printf(" %d", c);
			else if(argv[1][i] == 'w')
				printf(" %d", w);
			else if(argv[1][i] == 'm')
				printf(" %d", m);
			else if(argv[1][i] == 'l')
				printf(" %d", l);
			else if(argv[1][i] == 'L')
				printf(" %d", L);
		}
		printf(" %s\n", file);
	}
	else if(mode == 4)
	{
		for(i = 1; i < argc; i++)		//	옵션이 여러가지가 들어올 경우 처리 ex) wc -c -w -l [파일명] = 바이트 단어 행 [파일명]
		{
			if(isBytes(argv[i]))
				printf(" %d", c);
			else if(isWords(argv[i]))
				printf(" %d", w);
			else if(isChars(argv[i]))
				printf(" %d", m);
			else if(isLines(argv[i]))
				printf(" %d", l);
			else if(isLength(argv[i]))
				printf(" %d", L);
		}
		printf(" %s\n", file);
	}
}
