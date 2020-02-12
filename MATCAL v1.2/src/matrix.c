#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Define.h"
#include "fraction.h"
#include "vector.h"
#include "matrix.h"
#include "interact.h"

extern mat *head;
int step = 0;

mat *init_matptr(int mode, char name[], int row, int col)
{
	mat *p = NULL;
	int i, j;
	
	if((p = (mat *)malloc(sizeof(mat))) == NULL){
		perror("mem: init_matptr:");
		return NULL;
	}

	memset(p->name, '\0', 20);
	strcpy(p->name, name);
	p->row = row;
	p->col = col;
	p->r = 0;
	p->next = NULL;
	if((p->matrix = init_matrix(row, col)) == NULL)
		return NULL;
	if((p->transmat = init_matrix(col, row)) == NULL)
		return NULL;
	if(mode == 1){
		p->normat = NULL;
		p->r = 0;
		p->invmat = NULL;
		return p;
	}
	if((p->normat = init_matrix(row, col)) == NULL)
		return NULL;
	if(row == col)
		if((p->invmat = init_matrix(row, col)) == NULL)
			return NULL;
	if(row != col)
		p->invmat = NULL;

	return p;
}

frac **init_matrix(int row, int col)
{
	frac **matrix = NULL;
	int i, j;

	if((matrix = (frac **)malloc(row * sizeof(frac *))) == NULL){
		perror("mem: init_matrix1");
		return NULL;
	}
	for(i = 0; i < row; i ++){
		if((matrix[i] = (frac *)malloc(col * sizeof(frac))) == NULL){
			perror("mem: init_matrix2");
			return NULL;
		}
	}

	for(i = 0; i < row; i ++)
		for(j = 0; j < col; j ++){
			matrix[i][j].mark = 1;
			matrix[i][j].nume = 0;
			matrix[i][j].deno = 1;
		}

	return matrix;
}

mat *creatmatptr(char name[], int row, int col, char buff[])
{
	mat *p = NULL;
	int i, j;

	if((p = init_matptr(0, name, row, col)) == NULL)
		return NULL;
	if(getmat(name, p->matrix, row, col, buff) == -1)
		return NULL;
//printmatrix(p->matrix, p->row, p->col);	
	gettrans_mat(p->matrix, p->transmat, row, col);
//printmatrix(p->transmat, p->row, p->col);	
	
	for(i = 0; i < row; i ++)
		for(j = 0; j < col; j ++)
			p->normat[i][j] = p->matrix[i][j];
	
	p->r = get_r(p->normat, row, col);
//printf("r = %d\n", p->r);
	
	if(p->r == row && p->invmat != NULL){
		if((getinv_mat(p->matrix, p->invmat, row, col)) == -1)
			return NULL;
	
//printmatrix(p->invmat, p->row, p->col);
	}
	return p;
}

int getmat(char name[], frac **mat, int row, int col, char buff[])
{
	int i, j, k, tmplen;
	char tmp[50];

	for(i = 0; i < row; i ++)
		for(j = 0; j < col; j ++){
			memset(tmp, '\0', 50);
			sscanf(buff, "%s", tmp);
			tmplen = strlen(tmp);
			for(k = 0; buff[k] != '\0'; k ++)
				buff[k] = buff[k + tmplen + 1];

			mat[i][j] = getfrac(tmp, tmplen);
			if(mat[i][j].mark == -2){
				printf("format error: mat:%s  row:%d  col:%d\n", name, i, j);
				return -1;
			}
		}
	return 0;
}

void gettrans_mat(frac **mat, frac **transmat, int row, int col)
{
	int i, j;

	for(i = 0; i < row; i ++)
		for(j = 0; j < col; j ++)
			transmat[j][i] = mat[i][j];
}

int get_r(frac **mat, int row, int col)
{
	int full;

	full = row;
	if(full > col)
		full = col;

	return full - normalizemat(mat, row, col);
}

int getinv_mat(frac **mat, frac **invmat, int row, int col)
{	
	frac **tmp = NULL, e;
	int i, j, r;

	if((tmp = init_matrix(row, 2 * col)) == NULL)
		return -1;
	for(i = 0; i < row; i ++)
		for(j = 0; j < col; j ++)
			tmp[i][j] = mat[i][j];

	e.mark = e.nume = e.deno = 1;
	for(i = 0; i < row; i ++)
		tmp[i][i + col] = e;

	normalizemat(tmp, row, 2 * col);

	for(i = 0; i < row; i ++)
		for(j = 0; j < col; j ++)
			invmat[i][j] = tmp[i][j + col];

	return 0;
}

mat *getmatptr(char name[])
{
	if(head == NULL){
		printf("Matlist NULL!\n");
		return NULL;
	}

	mat *p = NULL;

	p = head;
	while(p->next != NULL){
		if(strcmp(name, p->name) == 0)
			break;
		p = p->next;
	}
	if(strcmp(name, p->name) == 0)
		return p;
	return NULL;
}

mat *sum_mat(mat *p1, mat *p2)
{
	mat *result = NULL;
	int i, j;
	frac tmp[2];
	char nametmp[20];

	if(checkmatsum(p1, p2) == -1)
		return NULL;

	memset(nametmp, '\0', 20);
	sprintf(nametmp, "tmp%d", step ++);
	if((result = init_matptr(1, nametmp, p1->row, p1->col)) == NULL)
		return NULL;

	for(i = 0; i < p1->row; i ++)
		for(j = 0; j < p1->col; j ++){
			tmp[0] = p1->matrix[i][j];
			tmp[1] = p2->matrix[i][j];
			result->matrix[i][j] = sum_frac(tmp, 2);
		}
	gettrans_mat(result->matrix, result->transmat, result->row, result->col);
	
//	printmat(p1);
//	printmat(p2);
//	printmat(result);

	return result;
}

int checkmatsum(mat *p1, mat *p2)
{
	if(p1->row != p2->row || p1->col != p2->col){
		printf("format error: %s +/- %s\n", p1->name, p2->name);
		return -1;
	}

	return 1;
}

mat *mul_mat(mat *p1, mat *p2)
{
	mat *result = NULL;
	int i, j, k;
	char nametmp[20];

	if(checkmatmul(p1, p2) == -1)
		return NULL;

	memset(nametmp, '\0', 20);
	sprintf(nametmp, "tmp%d", step ++);
	if((result = init_matptr(1, nametmp, p1->row, p2->col)) == NULL)
		return NULL;

	for(i = 0; i < result->row; i ++)
		for(j = 0; j < result->col; j ++)
			result->matrix[i][j] = mul_vector1(p1->matrix[i], p2->transmat[j], p1->col);
	gettrans_mat(result->matrix, result->transmat, result->row, result->col);

//	printmat(p1);
//	printmat(p2);
//	printmat(result);

	return result;
}

int checkmatmul(mat *p1, mat *p2)
{
	if(p1->col != p2->row){
		printf("format error: %s * %s\n", p1->name, p2->name);
		return -1;
	}

	return 1;
}

mat *nummul_mat(frac num, mat *p)
{
	mat *result = NULL;
	frac tmp[2];
	int i, j;
	char nametmp[20];

//	printfrac(num);
//	printmat(p);
	memset(nametmp, '\0', 20);
	sprintf(nametmp, "tmp%d", step ++);
	if((result = init_matptr(1, nametmp, p->row, p->col)) == NULL)
		return NULL;

	tmp[0] = num;
	for(i = 0; i < p->row; i ++)
		for(j = 0; j < p->col; j ++){
			tmp[1] = p->matrix[i][j];
			result->matrix[i][j] = mul_frac(tmp, 2);
		}
	gettrans_mat(result->matrix, result->transmat, result->row, result->col);

//	printmat(result);

	return result;
}

mat *inv_mat(mat *p)
{
//	printmat(p);
	mat *result = NULL;
	frac **tmp = NULL, e;
	int i, j, r;
	char nametmp[20];

	if(checkmatinv(p) == -1)
		return NULL;

	if((tmp = init_matrix(p->row, 2 * p->col)) == NULL)
		return NULL;
	for(i = 0; i < p->row; i ++)
		for(j = 0; j < p->col; j ++)
			tmp[i][j] = p->matrix[i][j];

	e.mark = e.nume = e.deno = 1;
	for(i = 0; i < p->row; i ++)
		tmp[i][i + p->col] = e;

//	printmatrix(tmp, p->row, 2 * p->col);
	normalizemat(tmp, p->row, 2 * p->col);

	memset(nametmp, '\0', 20);
	sprintf(nametmp, "tmp%d", step ++);
	if((result = init_matptr(1, nametmp, p->row, p->col)) == NULL)
		return NULL;

	for(i = 0; i < result->row; i ++)
		for(j = 0; j < result->col; j ++)
			result->matrix[i][j] = tmp[i][j + p->col];
	gettrans_mat(result->matrix, result->transmat, result->row, result->col);

//	printmat(p);
//	printmat(result);

	return result;
}

int checkmatinv(mat *p)
{
	if(IS_SQUARE(p) == false){
		printf("format error: cannot inverse %s(%d*%d)\n", p->name, p->row, p->col);
		return -1;
	}

	if((p->normat = init_matrix(p->row, p->col)) == NULL)
		return -1;
	int i, j;
	for(i = 0; i < p->row; i ++)
		for(j = 0; j < p->col; j ++)
			p->normat[i][j] = p->matrix[i][j];
	
	if((p->r = get_r(p->normat, p->row, p->col)) < p->row){
		printf("error: cannot inverse %s(r = %d)\n", p->name, p->r);
		return -1;
	}

	return 1;
}

bool IS_SQUARE(mat *p)
{
	if(p->row == p->col)
		return true;
	return false;
}

int normalizemat(frac **mat, int row, int col)
{
	int i, j, k, tmp, flag = 0, cnt = 0, full;
	frac a;

	for(i = 0; i < row; i ++){
//printf("line %d starts\n", i + 1);
//printmatrix(mat, row, col);
		tmp = i;
		j = i + flag;
		if(j >= col)
			break;
		if(mat[i][j].nume == 0){
			while(i < row && mat[i][j].nume == 0)
				i ++;
			if(i == row)
				break;
			if(mat[i][j].nume == 0){
				i = tmp - 1;
				flag ++;
				continue;
			}
			switchvec(mat, i, tmp);
			i = tmp;
		}
//printf("i = %d, j = %d\n", i, j);

		normalizevec(mat[i], j, col);
//printmatrix(mat, row, col);		
		for(k = 0; k < row; k ++){
			if(k == i)
				continue;
			a = get_aij(mat, k, j);
//printf("factor = ");
//printfrac(a);
//printf("\n");
			if(a.nume != 0)
				set0(mat, i, k, a, col);
		}
//printf("pass set0, line %d finished\n", i);
//printmatrix(mat, row, col);
	}
	full = row;
	if(full > col)
		full = col;

	for(i = 0; i < full; i ++)
		if(IS_0VEC(mat[i], col) == true)
			cnt ++;

	return cnt;
}

mat *sub_mat(mat *p1, mat *p2)
{
	char nametmp[20];
	frac factor;
	mat *result = NULL;

	if(checkmatsum(p1, p2) == -1)
		return NULL;

	factor.mark = -1;
	factor.nume = factor.deno = 1;

	memset(nametmp, '\0', 20);
	sprintf(nametmp, "tmp%d", step ++);
	if((result = init_matptr(1, nametmp, p1->row, p1->col)) == NULL)
		return NULL;
	result = sum_mat(p1, nummul_mat(factor, p2));
	gettrans_mat(result->matrix, result->transmat, result->row, result->col);

//	printmat(p1);
//	printmat(p2);
//	printmat(result);

	return result;
}

mat *div_mat(mat *p1, mat *p2)
{
	char nametmp[20];
	mat *result = NULL;

	if(checkmatmul(p1, p2) == -1)
		return NULL;
	if(checkmatinv(p2) == -1)
		return NULL;

	memset(nametmp, '\0', 20);
	sprintf(nametmp, "tmp%d", step ++);
	if((result = init_matptr(1, nametmp, p1->row, p2->col)) == NULL)
		return NULL;
	result = mul_mat(p1, inv_mat(p2));
	gettrans_mat(result->matrix, result->transmat, result->row, result->col);

//	printmat(p1);
//	printmat(p2);
//	printmat(result);

	return result;
}

int checkrename(char name[])
{
	if(getmatptr(name) != NULL){
		printf("Repeated name(%s)! Name list:\n", name);
		return -1;
	}
	return 1;
}
