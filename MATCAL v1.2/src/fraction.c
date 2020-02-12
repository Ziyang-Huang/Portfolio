#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Define.h"
#include "interact.h"
#include "fraction.h"

frac sum_frac(frac arr[], int n)
{
	int i, lcm;
	frac result, tmp;

	result.mark = 1;
	result.nume = 0;
	result.deno = 1;

	for(i = 0; i < n; i ++){
		tmp = arr[i];
		
		lcm = LCM(tmp.deno, result.deno);
		result.nume *= (lcm / result.deno);
		tmp.nume *= (lcm / tmp.deno);
		result.deno = lcm;
		result.nume = result.mark * result.nume + tmp.mark * tmp.nume;
		
		if(result.nume < 0){
			result.mark = -1;
			result.nume *= -1;
		}
		else
			result.mark = 1;
		
		result = resort(result);
	}

	return result;
}

frac mul_frac(frac arr[], int n)
{
	int i, lcm;
	frac result, tmp;

	result.mark = 1;
	result.nume = 1;
	result.deno = 1;

	for(i = 0; i < n; i ++){
		result.nume *= arr[i].nume;
		result.deno *= arr[i].deno;
		result.mark *= arr[i].mark;
		result = resort(result);
	}

	return result;
}

frac getfrac(char num[], int len)
{
	int slash, i, j, a;
	char tmp[50];
	frac result;

	if((slash = IS_FRAC(num, len)) == -1){
		result.mark = -2;
		result.deno = 0;
		result.nume = 0;
	}

	if(slash == 0){
		sscanf(num, "%d", &a);
		result = inttofrac(a);
	}

	else{
		memset(tmp, '\0', 50);
		for(i = 0, j = 0; num[i] != '\0'; i ++){
			if(num[i] == '/'){
				tmp[j ++] = ' ';
				tmp[j ++] = num[i];
				tmp[j ++] = ' ';
			}
			else
				tmp[j ++] = num[i];
		}

		sscanf(tmp, "%d / %d", &result.nume, &result.deno);
		result.mark = 1;
		if(result.nume < 0){
			result.mark *= -1;
			result.nume *= -1;
		}
		if(result.deno < 0){
			result.mark *= -1;
			result.deno *= -1;
		}
		result = resort(result);
	}

	return result;
}

bool IS_INT(char num[], int len)
{
	if(len == 0)
		return false;

	int i;
	bool flag = true;
	
	for(i = 0; i < len; i ++)
		if((num[i] > '9' || num[i] < '0') && num[i] != '-')
			flag = false;

	return flag;
}

int IS_FRAC(char num[], int len)
{
	int i;

	for(i = 0; i < len; i ++)
		if(num[i] == '/')
			break;

	if(i == len){
		if(IS_INT(num, len) == true)
			return 0;
		else
			return -1;
	}

	if(IS_INT(num, i) == true && IS_INT(num + i + 1, len - i - 1) == true)
		return i;
	
	return -1;
}

int GCD(int n, int m)
{
	int tmp;
	if(n < m){
		tmp = n;
		n = m;
		m = tmp;
	}
	while((tmp = n % m) != 0){
		n = m;
		m = tmp;
	}
	return m;
}

int LCM(int n, int m)
{
	return n * m / GCD(n, m);
}

frac resort(frac num)
{
	frac result;
	int tmp;

	if(num.nume == 0){
		result.mark = result.deno = 1;
		result.nume = 0;
		return result;
	}

	tmp = GCD(num.nume, num.deno);
	result.mark = num.mark;
	result.nume = num.nume / tmp;
	result.deno = num.deno / tmp;

	return result;
}

frac inttofrac(int a)
{
	frac result;

	if(a < 0){
		result.mark = -1;
		a *= -1;
	}
	else
		result.mark = 1;

	result.nume = a;
	result.deno = 1;

	return result;
}

frac get_aij(frac **mat, int i, int j)
{
	frac result;

	result = mat[i][j];
	if(result.nume != 0)
		result.mark *= -1;

	return result;
}
