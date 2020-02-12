#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Define.h"
#include "stack.h"

Stack *init_stack(void)
{
	Stack *p = NULL;

	if((p = (Stack *)malloc(sizeof(Stack))) == NULL){
		perror("Stack:mem");
		return NULL;
	}

	p->top = 0;

	return p;
}

void free_stack(Stack *s)
{
	if(s != NULL)
		free(s);
}

void clear_stack(Stack *s)
{
	s->top = 0;
}

int IS_FULL_STACK(Stack *s)
{
	return (s->top == SIZE);
}

int IS_EMPTY_STACK(Stack *s)
{
	return (s->top == 0);
}

int push_stack(sign data, Stack *s)
{
	if(IS_FULL_STACK(s) == 1){
		printf("Stack full!\n");
		return 0;
	}

	s->data[++ s->top] = data;
	
	return 1;
}

sign pop_stack(Stack *s)
{
	sign a;
	a.stype = ERR;

	if(IS_EMPTY_STACK(s) == 1){
		printf("Stack empty!\n");
		return a;
	}
	return s->data[s->top --];
}

sign peek_stack(Stack *s)
{
	sign a;
	a.stype = ERR;

	if(IS_EMPTY_STACK(s) == 1){
		printf("Stack empty!\n");
		return a;
	}
	return s->data[s->top];
}
