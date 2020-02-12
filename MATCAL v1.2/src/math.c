#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Define.h"
#include "stack.h"
#include "fraction.h"
#include "matrix.h"
#include "interact.h"
#include "math.h"

extern mat *head;
extern sign err;

void math(char exp[])
{
	int n1, n2;
	sign *nifix = NULL, *postfix = NULL, result;

	if((nifix = stdnifix(exp, &n1)) == NULL)
		return ;
//	printexp(nifix, n1);
//	printf("n1 = %d\n", n1);
	if(legal(nifix, n1) < 0)
		return ;
//	printf("pass legal\n");
	if((postfix = nifix_to_postfix(nifix, n1, &n2)) == NULL)
		return ;
//	printf("pass trans\n");
//	printf("n2 = %d\n", n2);
//	printexp(postfix, n2);
	result = calculator(postfix, n2);
//	printf("pass calculator\n");
	if(result.stype == ERR)
		return;
	printf("%s =\n", exp);
	if(result.stype == NUM){
		printfrac(result.tok.num);
		printf("\n");
	}
	if(result.stype == MAT)
		printmatrix(result.tok.mat->matrix, result.tok.mat->row, result.tok.mat->col);
}

sign calculator(sign *postfix, int n)
{
	int i;
	sign tmp[2], result;
	frac tmpfrac[2];
	Stack *p = NULL;

	if((p = init_stack()) == NULL)
		return err;

	for(i = 0; i < n; i ++){
//		if(i != 0){
//			printf("(math)Stack(%d):", p->top);
//			if(p->data[p->top].stype == NUM)
//				printfrac(p->data[p->top].tok.num);
//			if(p->data[p->top].stype == MAT)
//				printmat(p->data[p->top].tok.mat);
//		}
//		printf("(math)i == %d\n(math)", i);
//		printmat(head);
//		printf("(math)postfix[%d] = ", i);
//		printexp(postfix + i, 1);
		if(postfix[i].stype == NUM || postfix[i].stype == MAT){
			if(push_stack(postfix[i], p) == 0)
				return err;
			continue;
		}
		if(postfix[i].stype == LROP1){
			tmp[0] = pop_stack(p);
			tmp[1] = pop_stack(p);
			if(tmp[0].stype == ERR || tmp[1].stype == ERR || legal_LROP1(tmp[0], tmp[1]) == false)
				return err;
			if(tmp[0].stype == NUM){
				result.stype = NUM;
				tmpfrac[0] = tmp[0].tok.num;
				tmpfrac[1] = tmp[1].tok.num;
				if(postfix[i].tok.optype == SUB)
					tmpfrac[0].mark *= -1;
				result.tok.num = sum_frac(tmpfrac, 2);
				push_stack(result, p);
				continue;
			}
			if(tmp[0].stype == MAT){
				result.stype = MAT;
				if(postfix[i].tok.optype == ADD){
					if((result.tok.mat = sum_mat(tmp[1].tok.mat, tmp[0].tok.mat)) == NULL)
						return err;
					push_stack(result, p);
					continue;
				}
				if(postfix[i].tok.optype == SUB){
					if((result.tok.mat = sub_mat(tmp[1].tok.mat, tmp[0].tok.mat)) == NULL)
						return err;
					push_stack(result, p);
					continue;
				}
			}
		}
		if(postfix[i].stype == LROP2){
//			printf("Now LROP2\n");
			tmp[0] = pop_stack(p);
//			printexp(&tmp[0], 1);
			tmp[1] = pop_stack(p);
//			printexp(&tmp[1], 1);
			if(tmp[0].stype == ERR || tmp[1].stype == ERR)
				return err;
			if(tmp[0].stype == tmp[1].stype){
				if(tmp[0].stype == NUM){
					result.stype = NUM;
					tmpfrac[0] = tmp[0].tok.num;
					tmpfrac[1] = tmp[1].tok.num;
					if(postfix[i].tok.optype == DIV){
						if(tmpfrac[0].nume == 0){
							printf("Division by zero: floating point exception!\n");
							return err;
						}
						int a;
						a = tmpfrac[0].nume;
						tmpfrac[0].nume = tmpfrac[0].deno;
						tmpfrac[0].deno = a;
					}
					result.tok.num = mul_frac(tmpfrac, 2);
					push_stack(result, p);
					continue;
				}
				if(tmp[0].stype == MAT){
					result.stype = MAT;
					if(postfix[i].tok.optype == MUL){
						if((result.tok.mat = mul_mat(tmp[1].tok.mat, tmp[0].tok.mat)) == NULL)
							return err;
						push_stack(result, p);
						continue;
					}
					if(postfix[i].tok.optype == DIV){
						if((result.tok.mat = div_mat(tmp[1].tok.mat, tmp[0].tok.mat)) == NULL)
							return err;
						push_stack(result, p);
						continue;
					}
				}
			}
			if(tmp[0].stype != tmp[1].stype){
				result.stype = MAT;
				if(postfix[i].tok.optype == MUL){
					if(tmp[0].stype == NUM){
//						printmat(tmp[1].tok.mat);
						if((result.tok.mat = nummul_mat(tmp[0].tok.num, tmp[1].tok.mat)) == NULL)
							return err;
						push_stack(result, p);
						continue;
					}
					if(tmp[1].stype == NUM){
//						printmat(tmp[0].tok.mat);
						if((result.tok.mat = nummul_mat(tmp[1].tok.num, tmp[0].tok.mat)) == NULL)
							return err;
						push_stack(result, p);
						continue;
					}
				}
				if(postfix[i].tok.optype == DIV){
					if(tmp[0].stype == NUM){
						int a;
						a = tmp[0].tok.num.nume;
						tmp[0].tok.num.nume = tmp[0].tok.num.deno;
						tmp[0].tok.num.deno = a;
						if((result.tok.mat = nummul_mat(tmp[0].tok.num, tmp[1].tok.mat)) == NULL)
							return err;
						push_stack(result, p);
						continue;
					}
					if(tmp[1].stype == NUM){
						mat *pt = NULL;
						if((pt = inv_mat(tmp[0].tok.mat)) == NULL)
							return err;
						if((result.tok.mat = nummul_mat(tmp[1].tok.num, pt)) == NULL)
							return err;
						push_stack(result, p);
						continue;
					}
				}
			}
		}
		if(postfix[i].stype == LOP){
			tmp[0] = pop_stack(p);
			if(tmp[0].stype == ERR)
				return err;
			if(tmp[0].stype == NUM){
				printf("Not supported for now.\n");
				return err;
			}
			result.stype = MAT;
//			printmat(tmp[0].tok.mat);
			if((result.tok.mat = inv_mat(tmp[0].tok.mat)) == NULL)
				return err;
			push_stack(result, p);
			continue;
		}
	}
	if((result = pop_stack(p)).stype == ERR)
		return err;

	if(IS_EMPTY_STACK(p) == false)
		return err;

	return result;
}

bool legal_LROP1(sign a, sign b)
{
	if(a.stype == b.stype)
		return true;
	printf("format error: sum with num and mat\n");
	return false;
}

sign *nifix_to_postfix(sign *nifix, int n1, int *n2)
{
	int i, j;
	sign *postfix = NULL, tmp;
	Stack *p = NULL;

	if((p = init_stack()) == NULL)
		return NULL;
	if((postfix = (sign *)malloc(n1 * sizeof(sign))) == NULL){
		perror("transform: mem:");
		return NULL;
	}

	for(i = 0, j = 0; i < n1; i ++){
//		if(i != 0){
//			printf("postfix[%d] = ", j);
//			printexp(postfix + j, 1);
//			printf("Stack(%d):", p->top);
//		}
//		printf("i == %d\n", i);
//		printf("nifix[%d] = ", i);
//		printexp(nifix + i, 1);
//		printexp(p->data, p->top);
		if(nifix[i].stype == MAT || nifix[i].stype == NUM){
			postfix[j ++] = nifix[i];
			continue;
		}
		if(nifix[i].stype == LBR){
			if(push_stack(nifix[i], p) == 0){
				free(postfix);
				return NULL;
			}
			continue;
		}
		if(nifix[i].stype == RBR){
			while(1){
				if(IS_EMPTY_STACK(p) == 1){
					free(postfix);
					return NULL;
				}
				if(peek_stack(p).stype == LBR)
					break;
				postfix[j ++] = pop_stack(p);
			}
			pop_stack(p);
			continue;
		}
		if(nifix[i].stype == LOP){
			if(push_stack(nifix[i], p) == 0){
				free(postfix);
				return NULL;
			}
			continue;
		}
		if(nifix[i].stype == LROP2){
			if(IS_FULL_STACK(p) == 1){
				free(postfix);
				return NULL;
			}
			if(IS_EMPTY_STACK(p) == 1){
				push_stack(nifix[i], p);
				continue;
			}
			if(peek_stack(p).stype == LROP1 || peek_stack(p).stype == LBR){
				push_stack(nifix[i], p);
				continue;
			}
			if(peek_stack(p).stype == LROP2 || peek_stack(p).stype == LOP){
				while(1){
					if(IS_EMPTY_STACK(p) == 1 || peek_stack(p).stype == LROP1 || peek_stack(p).stype == LBR)
						break;
//					if(peek_stack(p).stype == LROP2 || peek_stack(p).stype == LOP)
						postfix[j ++] = pop_stack(p);
				}
				push_stack(nifix[i], p);
				continue;
			}
		}
		if(nifix[i].stype == LROP1){
			if(IS_FULL_STACK(p) == 1){
				free(postfix);
				return NULL;
			}
			if(IS_EMPTY_STACK(p) == 1){
				push_stack(nifix[i], p);
				continue;
			}
			if(peek_stack(p).stype == LBR){
				push_stack(nifix[i], p);
				continue;
			}
			if(peek_stack(p).stype == LROP1 || peek_stack(p).stype == LROP2 || peek_stack(p).stype == LOP){
				while(1){
					if(IS_EMPTY_STACK(p) == 1 || peek_stack(p).stype == LBR)
						break;
//					if(peek_stack(p).stype == LROP1 || peek_stack(p).stype == LROP2 || peek_stack(p).stype == LOP)
						postfix[j ++] = pop_stack(p);
				}
				push_stack(nifix[i], p);
				continue;
			}
		}
	}

	while(p->top != 0)
		postfix[j ++] = pop_stack(p);

	free_stack(p);
	*n2 = j;

	return postfix;
}

int legal(sign *nifix, int n)
{
	if(legal_brackets(nifix, n) == false)
		return -1;
	if(legal_afterlblrop(nifix, n) == false)
		return -2;
	if(legal_afternum(nifix, n) == false)
		return -3;
	if(legal_aftermatlbr(nifix, n) == false)
		return -4;
	if(legal_afterlop(nifix, n) == false)
		return -5;
	return 1;
}

bool legal_brackets(sign *nifix, int n)
{
	int i, flag = 0;

	for(i = 0; i < n; i ++){
		if(nifix[i].stype == LBR)
			flag ++;
		if(nifix[i].stype == RBR)
			flag --;

		if(flag < 0){
			printf("expression error: right bracket before left bracket!\n");
			return false;
		}
	}

	if(flag == 0)
		return true;

	printf("(flag = %d)expression error: lost right bracket!\n", flag);
	return false;
}

bool legal_afterlblrop(sign *nifix, int n)
{
	int i;
	for(i = 0; i < n - 1; i ++){
		if(nifix[i].stype == LBR || nifix[i].stype == LROP1 || nifix[i].stype == LROP2)
			if(nifix[i + 1].stype != LBR && nifix[i + 1].stype != NUM && nifix[i + 1].stype != MAT){
				printexp(nifix + i, 1);
				printexp(nifix + 1 + i, 1);
				printf("Format error(001): position in expression :%d!\n", i + 2);
				return false;
			}
	}
	return true;
}

bool legal_afternum(sign *nifix, int n)
{
	int i;
	for(i = 0; i < n; i ++)
		if(nifix[i].stype == NUM){
			if(i + 1 == n)
				break;
			if(nifix[i + 1].stype != LROP1 && nifix[i + 1].stype != LROP2 && nifix[i + 1].stype != RBR){
				printexp(nifix + 1 + i, 1);
				printf("Format error(002): position in expression :%d!\n", i + 2);
				return false;
			}
		}
	return true;
}

bool legal_aftermatlbr(sign *nifix, int n)
{
	int i;
	for(i = 0; i < n; i ++)
		if(nifix[i].stype == MAT){
			if(i + 1 == n)
				break;
			if((nifix[i + 1].stype != LROP1) && (nifix[i + 1].stype != LROP2) && (nifix[i + 1].stype != RBR) && (nifix[i + 1].stype != LOP)){
				printexp(nifix + i + 1, 1);
				printf("Format error(003): position in expression :%d!\n", i + 2);
				return false;
			}
		}
	return true;
}

bool legal_afterlop(sign *nifix, int n)
{
	int i;
	for(i = 0; i < n; i ++)
		if(nifix[i].stype == LOP){
			if(i + 1 == n)
				break;
			if(nifix[i + 1].stype != LROP1 && nifix[i + 1].stype != LROP2 && nifix[i + 1].stype != RBR){
				printexp(nifix + 1 + i, 1);
				printf("Format error(004): position in expression :%d!\n", i + 2);
				return false;
			}
		}
	return true;
}

sign *stdnifix(char exp[], int *cnt)
{
	char buff[1000], tmp[20];
	int i, j, n, len;
	sign *expression = NULL;

	if((n = stdbuff(exp, buff)) == -1)
		return NULL;
//	printf("n = %d, exp = %s, buff = %s\n", n, exp, buff);
	if((expression = (sign *)malloc(n * sizeof(sign))) == NULL){
		perror("stdexp: mem:");
		return NULL;
	}

	for(i = 0; i < n; i ++){
		memset(tmp, '\0', 20);
		sscanf(buff, "%s", tmp);
		len = strlen(tmp);
		for(j = 0; buff[j] != '\0'; j ++)
			buff[j] = buff[j + len + 1];
//		printf("(i == %d) %s -- %d(tmp = %s)\n", i, buff, len, tmp);
		expression[i] = gettoken(tmp, len);
		if(expression[i].stype == ERR)
			return NULL;
//		printexp(expression + i, 1);
	}

	*cnt = pickupminus(expression, n);

	return expression;
}

int stdbuff(char exp[], char buff[])
{
	int i, j, k;

	memset(buff, '\0', 1000);

	for(i = 0, j = 0; exp[i] != '\0'; i ++){
		if((k = IS_OP(exp[i], exp[i + 1])) == -1)
			return -1;
		if(k == 1){
			if(exp[i] == '|'){
				if(buff[j - 1] != ' ')
					buff[j ++] = ' ';
				buff[j ++] = exp[i];
				buff[j ++] = exp[++ i];
				buff[j ++] = ' ';
				continue;
			}
			if(buff[j - 1] != ' ')
				buff[j ++] = ' ';
			buff[j ++] = exp[i];
			buff[j ++] = ' ';
			continue;
		}
		if((exp[i] <= 'Z' && exp[i] >= 'A') || (exp[i] <= 'z' && exp[i] >= 'a') || (exp[i] <= '9' && exp[i] >= '0'))
			buff[j ++] = exp[i];
	}

	return cntword(buff);
}

int IS_OP(char c, char d)
{
	if(IS_LOP(c, d) < 0)
		return -1;
	if(IS_BR(c) > 0 || IS_LROP1(c) > 0 || IS_LROP2(c) > 0 || IS_LOP(c, d) > 0)
		return 1;
	return 0;
}

int IS_BR(char c)
{
	if(c == '(')
		return 1;
	if(c == ')')
		return 2;
	return 0;
}

int IS_LROP1(char c)
{
	if(c == '+' || c == '-')
		return 1;
	return 0;
}

int IS_LROP2(char c)
{
	if(c == '*' || c == '/')
		return 1;
	return 0;
}
int IS_LOP(char c, char d)
{
	if(c == '|'){
		if(d != '1'){
			printf("Format error: use '|1' to inverse!\n");
			return -1;
		}
		return 1;
	}
	return 0;
}

int cntword(char buff[])
{
	int i = 0, w = 0, STATE = 0;

	for(i = 0; buff[i] != '\0'; i ++){
		if(buff[i] == ' ' || buff[i] == '\t' || buff[i] == '\n')
			STATE = 0;
		else if(STATE == 0){
			STATE = 1;
			w ++;
		}
	}

	return w;
}

sign gettoken(char token[], int len)
{
	sign a;

	switch(IS_BR(token[0])){
		case 1: {
			a.stype = LBR;
			a.tok.optype = BR;
			return a;
		}
		case 2: {
			a.stype = RBR;
			a.tok.optype = BR;
			return a;
		}
		default: break;
	}

	if(IS_LROP1(token[0]) == 1){
		a.stype = LROP1;
		a.tok.optype = getOP(token);
		return a;
	}
	
	if(IS_LROP2(token[0]) == 1){
		a.stype = LROP2;
		a.tok.optype = getOP(token);
		return a;
	}

	if(IS_LOP(token[0], token[1]) == 1){
		a.stype = LOP;
		a.tok.optype = getOP(token);
		return a;
	}

	if(IS_INT(token, len) == 1){
		a.stype = NUM;
		sscanf(token, "%d", &a.tok.num.nume);
		a.tok.num.mark = a.tok.num.deno = 1;
		return a;
	}

	if((a.tok.mat = getmatptr(token)) == NULL){
		printf("No such matrix: %s\n", token);
		a.stype = ERR;
		return a;
	}
	a.stype = MAT;
	return a;
}

enum OpType getOP(char token[])
{
	if(strcmp(token, "+") == 0)
		return ADD;
	if(strcmp(token, "-") == 0)
		return SUB;
	if(strcmp(token, "*") == 0)
		return MUL;
	if(strcmp(token, "/") == 0)
		return DIV;
	if(strcmp(token, "|1") == 0)
		return INV;
}

int pickupminus(sign *nifix, int n)
{
	int i, j, cnt;

	cnt = n;
	for(i = 0; i < n; i ++){
		if(nifix[i].stype == LROP1 && nifix[i].tok.optype == SUB){
			if(nifix[i + 1].stype == NUM){
				if(i == 0 || nifix[i - 1].stype == LBR){
					nifix[i + 1].tok.num.mark *= -1;
					for(j = i; j < n - 1; j ++)
						nifix[j] = nifix[j + 1];
					cnt --;
				}
			}
		}
	}
	return cnt;
}
