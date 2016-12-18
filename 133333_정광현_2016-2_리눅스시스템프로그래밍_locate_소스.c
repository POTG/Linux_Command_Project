/*
2016_12_16
133333 정광현
리눅스시스템_명령어_locate
*/
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

void update(int fd, char *pos);
void saveFind();
void search(char *option, char *name, char *file);
void printHelp();
void printVersion();

int main(int argc, char* argv[])
{
	//int fd;
	if(argc == 2)
	{
		if(strcmp(argv[1], "--update") == 0)
		{
			//fd = open("mLocate.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			//update(fd, "/home/potg/linux/Project/locate");
			//close(fd);
			saveFind();
			exit(0);
		}
		else if(strcmp(argv[1], "--help") == 0)
		{
			printHelp();
			exit(0);
		}
		else if(strcmp(argv[1], "--version") == 0)
		{
			printVersion();
			exit(0);
		}
		search(NULL, argv[argc-1], "/home/potg/linux/Project/locate/mFind.txt");
	}

	search(argv[argc-2] ,argv[argc-1], "/home/potg/linux/Project/locate/mFind.txt");
	return 0;
}

int readLine(int fd, char *str)
{
	int n;
	int i = 0;
	do
	{
		str = (char *)realloc(str, sizeof(str) + 1); 
		n = read(fd, str+i, 1);
		i++;
	}while(n > 0 && *str++ != NULL);
	*str = '\0';
	return (n>0);
}

void search(char *option, char *name, char *file)
{
	FILE* fp = fopen(file, "r");
	char str[1024];
	char *find = NULL;
	char *opN = option+1;
	int c = 0;
	int n;
	if(option == NULL)
	{
		while(!feof(fp))
		{
			fscanf(fp, "%s", str);
			find = strstr(str, name);
			if(find != NULL)
			{
				c++;
				printf("%s \n", str);
			}
		}
		return;
	}
	if(option[1] == 'c')
	{
		while(!feof(fp))
		{
			fscanf(fp, "%s", str);
			find = strstr(str, name);
			if(find != NULL)
			{
				c++;
				printf("%s \n", str);
			}
		}
		printf("Count : %d \n", c);
		return;
	}
	else if(option[1] != c)
	{
		n = atoi(opN);
		while(c < n && !feof(fp))
		{
			fscanf(fp, "%s", str);
			find = strstr(str, name);
			if(find != NULL)
			{
				printf("%s \n", str);
				c++;
			}
		}
		return;		
	}
}

void update(int fd,char *pos)
{
	
	DIR *dp;
	struct dirent *dir;
	struct stat stat;
	int length;
	char *buf;

	if((dp = opendir(pos)) == NULL)
	{
		perror(pos);
		return;
	}

	while(dir = readdir(dp))
	{
		memset(&stat, 0, sizeof(struct stat));
		lstat(dir->d_name, &stat);
		if(S_ISDIR(stat.st_mode))
		{
			if(strncmp(".", dir->d_name, 1) == 0 || strncmp("..", dir->d_name, 2) == 0)
			{
				continue;
			}
			update(fd, dir->d_name);
		}
		else
		{
			printf("mode : %s %d %x\n", dir->d_name, stat.st_mode, stat.st_mode);
			length = strlen(dir->d_name);
			buf = (char *)malloc(length + 1);
			strcpy(buf, dir->d_name);
			buf[length] = '\n';
			write(fd, buf, length+1);
			free(buf);
		}
	}
	closedir(dp);	
}

void saveFind()
{
	system("find / * > mFind.txt");
}

void printHelp()
{
	printf("사용법 : locate [옵션] [파일명]\n");
	printf("파일 검색\n");
	printf("-n          \t\tn개의 결과 출력\n");
	printf("-c          \t\t결과 카운트\n");
	printf("    --update\t\tDB갱신\n");
	printf("    --help\t\t도움말\n");
	printf("    --version\t\t버전 정보\n");
	printf("\n");
}
void printVersion()
{
	printf("리눅스시스템 명령어 구현 1.0\n");
	printf("명령어 : locate\n");
	printf("제작자 : 133333 정광현\n");
	printf("제작일 : 2016_12_15\n");
	printf("\n");
}
