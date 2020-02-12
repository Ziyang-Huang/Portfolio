#include <stdio.h>
#include <stdbool.h>
#include "Define.h"
#include "fraction.h"
#include "vector.h"

frac mul_vector1(frac v1[], frac v2[], int n)
{
	frac result, tmp1[2], tmp2[2];
	int i;
	
	result.mark = 1;
	result.nume = 0;
	result.deno = 1;

	for(i = 0; i < n; i ++){
		tmp1[0] = v1[i];
		tmp1[1] = v2[i];
		tmp2[0] = result;
		tmp2[1] = mul_frac(tmp1, 2);
		result = sum_frac(tmp2, 2);
	}

	return result;
}

void nummul_vec(frac *v, frac num, int col)
{
	int i;
	frac tmp[2];

	for(i = 0; i < col; i ++){
		tmp[0] = v[i];
		tmp[1] = num;
		v[i] = mul_frac(tmp, 2);
	}
}

void sum_vec(frac *v1, frac *v2, int col)
{
	int i = 0;
	frac tmp[2];

	for(i = 0; i < col; i ++){
		tmp[0] = v1[i];
		tmp[1] = v2[i];
		v1[i] = sum_frac(tmp, 2);
	}
}

void normalizevec(frac *v, int tar, int col)
{
	frac a, tmp[2];
	int i;

	a.mark = v[tar].mark;
	a.nume = v[tar].deno;
	a.deno = v[tar].nume;
	tmp[0] = a;
	
	for(i = 0; i < col; i ++){
		tmp[1] = v[i];
		v[i] = mul_frac(tmp, 2);
	}
}

void switchvec(frac **mat, int tar1, int tar2)
{
	frac *tmp = NULL;

	tmp = mat[tar1];
	mat[tar1] = mat[tar2];
	mat[tar2] = tmp;
}

void set0(frac **mat, int src, int tar, frac factor, int col)
{
	int tmp;

	nummul_vec(mat[src], factor, col);
	sum_vec(mat[tar], mat[src], col);
	
	tmp = factor.nume;
	factor.nume = factor.deno;
	factor.deno = tmp;

	nummul_vec(mat[src], factor, col);
}

bool IS_0VEC(frac *v, int col)
{
	int i;

	for(i = 0; i < col; i ++)
		if(v[i].nume != 0)
			return false;

	return true;
}
