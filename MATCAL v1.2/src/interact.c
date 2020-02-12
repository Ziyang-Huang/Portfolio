#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Define.h"
#include "interact.h"

extern mat *head;

void welcome(int cnt)
{
	printf("            **********************\n");
	printf("               Matrix Calculator\n");
	printf("                   Welcome !\n");
	printf("            **********************\n");
	printf("successfully load %d matrixes.\n", cnt);
	printf("input 'matlist' to check matrix list.\n");
}

char *getcmd()
{
	char buff[1000], *cmd = NULL;
	int len;

	memset(buff, '\0', 1000);
	fgets(buff, 1000, stdin);
	
	len = strlen(buff);
	if(buff[len - 1] == '\n')
		buff[len - 1] = '\0';

	if((cmd = (char *)malloc(len * sizeof(char))) == NULL){
		perror("mem");
		return ;
	}
	
	memset(cmd, '\0', len);
	strcpy(cmd, buff);

	return cmd;
}

void printmatlist(void)
{
	mat *p = NULL;

	for(p = head; p != NULL; p = p->next)
		printmat(p);
}

void printmat(mat *p)
{
	printf("%s =\n", p->name);
	printmatrix(p->matrix, p->row, p->col);
}

void printmatrix(frac **mat, int row, int col)
{
	int i;

	for(i = 0; i < row; i ++)
		printvec(mat[i], col);
}

void printvec(frac *v, int len)
{
	int i;

	for(i = 0; i < len; i ++)
		printfrac(v[i]);
	printf("\n");
}

void printfrac(frac a)
{
	if(a.mark == -1)
		printf("-");
	if(a.deno == 1)
		printf("%d\t", a.nume);
	else
		printf("%d/%d\t", a.nume, a.deno);
}

void printexp(sign* exp, int n)
{
	int i;
	for(i = 0; i < n; i ++){
		if(exp[i].stype == MAT)
			printf("%s", exp[i].tok.mat->name);
		if(exp[i].stype == NUM)
			printfrac(exp[i].tok.num);
		if(exp[i].stype == LOP)
			printf(" |1 ");
		if(exp[i].stype == LBR)
			printf(" ( ");
		if(exp[i].stype == RBR)
			printf(" ) ");
		if(exp[i].stype == LROP1){
			if(exp[i].tok.optype == ADD)
				printf(" + ");
			else
				printf(" - ");
		}
		if(exp[i].stype == LROP2){
			if(exp[i].tok.optype == MUL)
				printf(" * ");
			else
				printf(" / ");
		}
	}
	printf("\n");
}

void printnamelist(void)
{
	mat *p = NULL;
	for(p = head; p != NULL; p = p->next)
		printf("%s  ", p->name);
	printf("\n");
}
