#ifndef STACK_H
#define STACK_H

#include "Define.h"

#define SIZE 50

typedef struct mystack{
	sign data[SIZE + 1];
	int top;
}Stack;

Stack *init_stack(void);
void free_stack(Stack *s);
void clear_stack(Stack *s);
int IS_FULL_STACK(Stack *s);
int IS_EMPTY_STACK(Stack *s);
int push_stack(sign data, Stack *s);
sign pop_stack(Stack *s);
sign peek_stack(Stack *s);

#endif //STACK_H
