#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Define.h"
#include "fileIO.h"
#include "matrix.h"

extern mat *head, *tmphead;
extern int cnt_mat, max_token;

void loadmat_origin(void)
{
	FILE *fp;

	if((fp = fopen("../matrix/matrix.mat", "r")) == NULL){
		perror("open matrix.mat:");
		return;
	}
	
	char buff[1000];
	char nametmp[20], rowc[10], colc[10];
	int row, col, len, namelen, i, j;
	mat *p = NULL;

	while(1){
		memset(nametmp, '\0', 20);
		memset(buff, '\0', 1000);
		fgets(buff, 1000, fp);
		if(buff[0] == '\0')
			break;
		cnt_mat ++;
		sscanf(buff, "%s%s%s", nametmp, rowc, colc);

		if(head != NULL)
			if(checkrename(nametmp) == -1){
				printnamelist();
				continue;
			}

		namelen = strlen(nametmp);
		len = namelen + strlen(rowc) + strlen(colc) + 3;

		sscanf(rowc, "%d", &row);
		sscanf(colc, "%d", &col);

		for(j = 0; buff[j] != '\0'; j ++)
			buff[j] = buff[j + len];

		if((p = creatmatptr(nametmp, row, col, buff)) == NULL)
			break;

		insertmat(p);
//		p = NULL;
	}
	fclose(fp);
}

void insertmat(mat *p)
{
	if(head == NULL){
		head = p;
		return ;
	}

	mat *p1 = NULL;

	p1 = head;
	while(p1->next != NULL)
		p1 = p1->next;
	p1->next = p;
}

void deletemat(char name[])
{
	mat *p1 = NULL, *p2 = NULL;

	if(head == NULL){
		printf("Matlist NULL!\n");
		return;
	}
	p1 = head;
	
	while(p1->next != NULL && strcmp(p1->name, name) != 0){
		p2 = p1;
		p1 = p1->next;
	}

	if(p1 == head)
		if(strcmp(p1->name, name) == 0){
			head = p1->next;
			free(p1);
		}
	if(strcmp(p1->name, name) == 0){
		p2->next = p1->next;
		free(p1);
	}
}

void deletematlist(void)
{
	mat *p = NULL;

	if(head == NULL)
		return;

	while(head != NULL){
		p = head;
		head = head->next;
		free(p);
	}
}

void savemat(void)
{
	FILE *fp;

	if((fp = fopen("../matrix/matrix.mat", "w")) == NULL){
		perror("open matrix.mat:");
		return;
	}

	int i, j;
	mat *p = NULL;

	for(p = head; p != NULL; p = p->next){
		fprintf(fp, "%s %d %d ", p->name, p->row, p->col);
		for(i = 0; i < p->row; i ++)
			for(j = 0; j < p->col; j ++){
				if(p->matrix[i][j].mark == -1)
					fprintf(fp, "-");
				if(p->matrix[i][j].deno == 1)
					fprintf(fp, "%d ", p->matrix[i][j].nume);
				else
					fprintf(fp, "%d/%d ", p->matrix[i][j].nume, p->matrix[i][j].deno);
			}
		fputc('\n', fp);
	}
	fclose(fp);
}

//void loadconfig(void)
//{
//	FILE *fp;

//	if((fp = fopen("../config/config", "r")) == NULL){
//		perror("Config:");
//		return;
//	}

//	char buff[100];

//	memset(buff, '\0', 100);
//	fgets(buff, 100, fp);
//	sscanf(buff, "Max token of input expression = %d", &max_token);

//	fclose(fp);
//}
