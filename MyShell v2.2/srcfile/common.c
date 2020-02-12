#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "common.h"

void run(char **argu)
{
	int status;
	pid_t pid;
	char exe[20] = {"./"};
//	char *argv[] = {"cp", "test2", "test7", NULL};

//	printf("(%d)%s\n", 0, argu[0]);
//	int i;
//	for(i = 0; i < 3; i ++)
//		printf("%s|", argu[i]);
//	printf("\n");

	strcat(exe, argu[0]);
//	printf("%s|\n", exe);

	pid = fork();
	if(pid < 0){
		myerror("Fail to fork", NULL);
		return;
	}
	if(pid == 0){
//		printf("Now child_P.(%d), pid = %d\n", getpid(), pid);
		if(execv(exe, argu) == -1)
			myerror(argu[0], NULL);
//		printf("Child_P end.\n");
		exit(0);
	}
	else if(pid > 0){
		waitpid(pid, &status, 0);
//		printf("Now father_P.(%d), pid = %d\n", getpid(), pid);
	}
}

void help(char *command)
{
	int status;
	pid_t pid;
	char *argu[] = {"man", NULL, NULL}, exe[] = {"./man"};

	argu[1] = command;

	pid = fork();
	if(pid < 0){
		myerror("Fail to fork", NULL);
		return;
	}
	if(pid == 0){
		if(execv(exe, argu) == -1)
			myerror(argu[0], NULL);
		exit(0);
	}
	else if(pid > 0){
		waitpid(pid, &status, 0);
	}
}

void cntfile(int *l, int *w, int *c, FILE *fp)
{
	int i = 0, l1 = 0, w1 = 0, c1 = 0;
/*	char line[1000], tmp[1000];
	memset(line, '\0', 1000);
	memset(tmp, '\0', 1000);

	while(1){
		if(fgets(tmp, 1000, fp) == NULL)
			break;
		l1 ++;
		for(i = 0; tmp[i] != '\0'; i ++);
		c1 += i;
		w1 += stdline(tmp, line);
//		puts(line);
//		printf("(l%d)(w%d)(c%d)", l1, w1, c1);
		memset(line, '\0', 1000);
		memset(tmp, '\0', 1000);	
	}*/
	int ch, STATE = 0;
	while((ch = getc(fp)) != EOF){
		if(ch == '\n')
			l1 ++;
		c1 ++;
		if(ch == ' ' || ch == '\t' || ch == '\n'){
			STATE = 0;
		}
		else if(STATE == 0){
			STATE = 1;
			w1 ++;
		}
	}
	*l = l1, *w = w1, *c = c1;
}

void cwd(void)
{
	int i, cnt = 0, len;
	char path[1000], HN[100], base[100], homepath[100] = {"/home/"};
	struct passwd *pwd_st = getpwuid(getuid());
	
	memset(HN, '\0', 100);
	memset(base, '\0', 100);
	memset(path, '\0', 1000);
	gethostname(HN, 100);
	getcwd(path, 1000);
	strcat(homepath, pwd_st->pw_name);

	for(i = 0; path[i] != '\0'; i ++){
		if(path[i] == '/')
			cnt ++;
		if(cnt >= 3)
			break;
		base[i] = path[i];
	}

	if(strcmp(base, homepath) == 0){
		len = strlen(homepath);
		path[0] = '~';
		for(i = 1; path[i] != '\0'; i ++)
			path[i] = path[i + len - 1];
	}

	printf("%s@%s:%s$ ", pwd_st->pw_name, HN, path);
}

void myerror(char *command, char *file)
{
	char err[100];

	memset(err, '\0', 100);
	strcpy(err, command);

	if(file != NULL){
		int i, j, cnt = 0, len;
		char filename[100] = {": "};

		for(i = 0; file[i] != '\0'; i ++)
			if(file[i] == '/')
				cnt ++;

		if(cnt > 0){
			len = strlen(file);
			for(i = len - 1; i >= 0; i --)
				if(file[i] == '/')
					break;
			i ++;

			for(j = 2; file[i] != '\0'; i ++, j ++)
				filename[j] = file[i];
		}

		else
			strcat(filename, file);

		strcat(err, filename);
	}

	perror(err);
}

int readcmdlist(cmdarr cmdlist[])
{
	FILE *fp = NULL;

	if((fp = fopen("cmd.ls", "r")) == NULL){
		printf("No command list file 'cmd.ls'!\n");
		return -1;
	}

	char line[100];
	int i, n = 0;

	memset(line, '\0', 100);
	while(fgets(line, 100, fp) != NULL){
		sscanf(line, "%s%d", cmdlist[n].cmd, &cmdlist[n].mode);
		n ++;
		memset(line, '\0', 100);
	}
	fclose(fp);

	return n;
}

int checkcmd(char cmd[])
{
	int i, n;
	cmdarr cmdlist[100];

	if((n = readcmdlist(cmdlist)) == -1)
		return -1;

//	printf("cmdlist(%d)\n", n);
	for(i = 0; i < n; i ++)
//		printf("(%s--%d)\n", cmdlist[i].cmd, cmdlist[i].mode);
		if(strcmp(cmdlist[i].cmd, cmd) == 0)
			return cmdlist[i].mode;
	
	return 0;
}
