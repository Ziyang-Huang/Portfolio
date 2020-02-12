#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cmdarr{
	char cmd[20];
	int mode;
}cmdarr;

int getcmd(int type, char command[], char ***argu, int *cnt)
{
	/*int len = 0, read;
	char *line = NULL;

	read = getline(&line, &len, stdin);

	printf("(len = %d)(read = %d)\n", len, read);

	int i, cnt = 0;

	for(i = 0; i < read; i ++){
		command[i] = line[i];
		if(line[i] == ' ')
			break;
	}
	for(; i < read; i ++)

	puts(line);
	puts(command);
	printf("(cnt = %d)\n", cnt);*/
	char *line = NULL, *tmp = NULL, c = 0;
	int flag = 0, i = 0, mode;
	FILE *fp = NULL;
	
	line = (char *)malloc(1000);
	tmp = (char *)malloc(1000);
	memset(line, '\0', 1000);
	memset(tmp, '\0', 1000);

	if(type == 0){
		fgets(tmp, 1000, stdin);
		if((fp = fopen("cmd.his", "a")) == NULL){
			puts("Error!");
			return -1;
		}
		fputs(tmp, fp);
		fclose(fp);
	}

	if(type == 1){
		char c;
		if((fp = fopen("cmd.his", "r")) == NULL){
			puts("Cannot open cmd.his!");
			return -1;
		}
		fseek(fp, -2L, SEEK_END);
		while((c = getc(fp)) != '\n')
			fseek(fp, -2L, SEEK_CUR);
		fgets(tmp, 1000, fp);
		fclose(fp);
	}

	*cnt = stdline(tmp, line) - 1;
//	printf("argu(%d)\ntmp:%s|\nline:%s|\n", *cnt, tmp, line);
	sscanf(line, "%s", command);
//	printf("%s\n", command);
//	if(strcmp("his", command) == 0){
//		memset(line, '\0', 200);
//		memset(command, '\0', 20);
//		strcpy(line, "cat cmd.his");
//		strcpy(command, "cat");
//	}
	for(i = 0; line[i] != '\0'; i ++)
		line[i] = line[i + strlen(command) + 1];
//	printf("%s|\n", line);
	if((mode = checkcmd(command)) <= 0)
		return -1;

	if(*cnt > 0){
		*argu = (char **)malloc(*cnt * sizeof(char *));
		for(i = 0; i < *cnt; i++){
			(*argu)[i] = (char *)malloc(100);
			memset((*argu)[i], '\0', 100);
		}
		int j, k;
		for(i = 0, j = 0, k = 0; line[i] != '\0'; i ++){
			if(line[i] != ' ')
				(*argu)[j][k++] = line[i];
			else{
				j ++;
				k = 0;
			}
		}
		/*for(i = 0; i < cnt; i ++)
			printf("%s|", argu[i]);
		printf("\n");*/
	}

	free(tmp);
	free(line);

	return mode;
}

int stdline(char src[], char rst[])
{
	int i, j, cnt;
	
	for(i = 0; src[i] != '\0'; i++)
		if(src[i] == '\t'){
			for(j = i; src[j] != '\0'; j ++)
				src[j] = src[j + 1];
			i --;
		}

	for(i = 0; src[i] != '\0'; i ++)
		if(src[i] == '/' && src[i + 1] == '/')
			src[i + 1] = ' ';

	for(i = 0; src[i] != '\0'; i ++)
		if(src[i] == '\n')
			src[i] = ' ';

	for(i = 0, j = 0; src[i] != '\0'; i ++){
		if(src[i] == ' '){
			if(j != 0)
				rst[j ++] = src[i];
			while(src[i] == ' ')
				i ++;
			i --;
		}
		if(src[i] != ' ')
			rst[j ++] = src[i];
	}

	for(i = 0, cnt = 0; rst[i] != '\0'; i ++)
		if(rst[i] == ' ')
			cnt ++;

	return cnt;
}

int checkcmd(char cmd[])
{
	FILE *fp = NULL;

	if((fp = fopen("cmd.sh", "r")) == NULL){
		printf("No cmd file!\n");
		return -1;
	}

	cmdarr cmdlist[100];
	char line[25];
	int i, n = 0;

	memset(line, '\0', 25);
	while(fgets(line, 25, fp) != NULL){
		sscanf(line, "%s%d", cmdlist[n].cmd, &cmdlist[n].mode);
		n ++;
		memset(line, '\0', 25);
	}

//	printf("cmdlist(%d)\n", n);
	for(i = 0; i < n; i ++)
//		printf("(%s--%d)\n", cmdlist[i].cmd, cmdlist[i].mode);
		if(strcmp(cmdlist[i].cmd, cmd) == 0)
			return cmdlist[i].mode;

	printf("%s: command not found\n", cmd);
	return 0;
}
