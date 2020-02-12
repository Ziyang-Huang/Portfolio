#ifndef MATH_H
#define MATH_H

void math(char exp[]);
sign calculator(sign *postfix, int n);
bool legal_LROP1(sign a, sign b);
sign *nifix_to_postfix(sign *nifix, int n1, int *n2);
int legal(sign *nifix, int n);
bool legal_brackets(sign *nifix, int n);
bool legal_afterlblrop(sign *nifix, int n);
bool legal_afternum(sign *nifix, int n);
bool legal_aftermatlbr(sign *nifix, int n);
bool legal_afterlop(sign *nifix, int n);
sign *stdnifix(char exp[], int *cnt);
int stdbuff(char exp[], char buff[]);
int IS_OP(char c, char d);
int IS_BR(char c);
int IS_LROP1(char c);
int IS_LROP2(char c);
int IS_LOP(char c, char d);
int cntword(char buff[]);
sign gettoken(char token[], int len);
enum OpType getOP(char token[]);
int pickupminus(sign *nifix, int n);

#endif //MATH_H
