#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Define.h"
#include "interact.h"
#include "matrix.h"
#include "cmd.h"

#define TRANS 3
#define NOR 4
#define INV 5
#define R 6
#define REMOVE 7

extern mat *head, *tmphead;
extern int cnt_mat, max_token;

void start(void)
{
	char *cmd = NULL;
	char name[20];
	int type;
	mat *p = NULL;

	welcome(cnt_mat);
	
	while(1){
		printf(">>");
		if((cmd = getcmd()) == NULL)
			break;
		if(strcmp(cmd, "exit") == 0)
			break;
		if(strcmp(cmd, "matlist") == 0){
			printmatlist();
			free(cmd);
			cmd = NULL;
			continue;
		}
		if((type = CMD_TYPE(cmd)) == -1)
			continue;
//		printf("type = %d\n", type);
		if(type == 0){
			p = getmatptr(cmd);
			printmat(p);
			free(cmd);
			cmd = NULL;
			continue;
		}
		if(type == 1){
			math(cmd);
			free(cmd);
			cmd = NULL;
			continue;
		}	
		if(type == 2){
			int tmp;
			tmp = addmat(cmd);
			if(tmp == -2)
				break;
			if(tmp == -1){
				free(cmd);
				cmd = NULL;
				continue;
			}
			free(cmd);
			cmd = NULL;
			continue;
		}
		mat *p = NULL;
		memset(name, '\0', 20);
		if(type == TRANS){
			if((p = getmat_name(cmd, name)) == NULL){
				printf("No such matrix: %s\n", name);
				free(cmd);
				cmd = NULL;
				continue;
			}
			printf("%s = \n", cmd);
			printmatrix(p->transmat, p->col, p->row);
			free(cmd);
			cmd = NULL;
			continue;
		}
		if(type == NOR){
			if((p = getmat_name(cmd, name)) == NULL){
				printf("No such matrix: %s\n", name);
				free(cmd);
				cmd = NULL;
				continue;
			}
			printf("%s = \n", cmd);
			printmatrix(p->normat, p->row, p->col);
			free(cmd);
			cmd = NULL;
			continue;
		}
		if(type == INV){
			if((p = getmat_name(cmd, name)) == NULL){
				printf("No such matrix: %s\n", name);
				free(cmd);
				cmd = NULL;
				continue;
			}
			if(checkmatinv(p) < 0){
				free(cmd);
				cmd = NULL;
				continue;	
			}
			printf("%s = \n", cmd);
			printmatrix(p->invmat, p->row, p->col);
			free(cmd);
			cmd = NULL;
			continue;
		}
		if(type == R){
			if((p = getmat_name(cmd, name)) == NULL){
				printf("No such matrix: %s\n", name);
				free(cmd);
				cmd = NULL;
				continue;
			}
			printf("%s = %d\n", cmd, p->r);
			free(cmd);
			cmd = NULL;
			continue;
		}
		if(type == REMOVE){
			sscanf(cmd, "%*s %s", name);
			deletemat(name);
			printf("%d matrixes are in matlist.\n", -- cnt_mat);
			free(cmd);
			cmd = NULL;
		}
	}
}

mat *getmat_name(char cmd[], char name[])
{
	int i, j;

	for(i = 0; cmd[i] != '\0'; i ++)
		if(cmd[i] == '(')
			break;
	i ++;
	for(j = 0; cmd[i] != '\0'; i ++){
		if(cmd[i] == ')')
			break;
		name[j ++] = cmd[i];
	}
//	puts(name);

	return getmatptr(name);
}

int addmat(char cmd[])
{
	int row, col;
	char buff[1000], name[20];
	mat *p = NULL;

	if(stdinput(cmd, name, buff, &row, &col) < 0){
		free(cmd);
		cmd = NULL;
		return -1;
	}

	if((p = creatmatptr(name, row, col, buff)) == NULL)
		return -2;
	insertmat(p);
	cnt_mat ++;
	printmat(p);
	
	return 1;
}

int CMD_TYPE(char cmd[])
{
	int len, i;

	if(getmatptr(cmd) != NULL)
		return 0;

	char tmp[100];
	memset(tmp, '\0', 10);
	sscanf(cmd, "%s", tmp);
	if(strcmp(tmp, "remove") == 0)
		return REMOVE;
	memset(tmp, '\0', 10);
	for(i = 0; cmd[i] != '\0'; i ++){
		if(cmd[i] == '(')
			break;
		tmp[i] = cmd[i];
	}
	if(strcmp(tmp, "trans") == 0)
		return TRANS;
	if(strcmp(tmp, "nor") == 0)
		return NOR;
	if(strcmp(tmp, "inv") == 0)
		return INV;
	if(strcmp(tmp, "r") == 0)
		return R;

	len = strlen(cmd);

	for(i = 0; i < len; i ++)
		if(IS_OP(cmd[i], cmd[i + 1]) == 1)
			return 1;

	if(IS_INPUT(cmd, len) == 1)
		return 2;

	return -1;
}

int IS_INPUT(char cmd[], int len)
{
	int i, flag = 0;

	for(i = 0; i < len; i ++){
		if(cmd[i] == '=')
			flag ++;
		if(cmd[i] == '{' && flag == 1)
			flag ++;
		if(cmd[i] == '}' && flag == 2)
			flag ++;
	}

	if(flag == 3)
		return 1;
	return 0;
}

int stdinput(char input[], char name[], char buff[], int *row, int *col)
{
	int i, j;

	for(i = 0, *row = 1; input[i] != '\0'; i ++)
		if(input[i] == ';')
			(*row) ++;

	if((*col = checkinput(input, *row)) < 0)
		return -1;

	memset(buff, '\0', 1000);
	memset(name, '\0', 20);

	for(i = 0; input[i] != '\0'; i ++)
		if(input[i] == '=')
			input[i] = ' ';

	sscanf(input, "%s", name);
	if(checkrename(name) == -1){
		printnamelist();
		return -1;
	}

	for(i = 0; i < input[i] != '\0'; i ++)
		if(input[i] == '{')
			j = i + 1;
	for(i = 0; input[i] != '\0'; i ++)
		input[i] = input[i + j];

	for(j = 0, i = 0; input[i] != '\0'; i++){
		if((input[i] <= '9' && input[i] >= '0') || input[i] == '/' || input[i] == '-')
			buff[j ++] = input[i];
		if(input[i] == ';' || input[i] == ',')
			buff[j ++] = ' ';
	}

	return 1;
}

int checkinput(char input[], int row)
{
	int i, j, col, *tmp;

	if((tmp = (int *)malloc(row * sizeof(int))) == NULL){
		perror("stdinput:");
		return -1;
	}
	for(i = 0; i < row; i ++)
		tmp[i] = 1;

	for(i = 0, j = 0; input[i] != '\0'; i ++){
		if(input[i] == ',')
			tmp[j] ++;
		if(input[i] == ';')
			j ++;
	}
	col = tmp[0];
	for(i = 0; i < row; i ++)
		if(tmp[i] != col){
			printf("Not a matrix: different cols in some rows!\n");
			return -2;
		}

	return col;
}

