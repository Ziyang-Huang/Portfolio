#ifndef INTERACT_H
#define INTERACT_H

void welcome(int cnt);
char *getcmd(void);
void printmatlist(void);
void printmat(mat *p);
void printmatrix(frac **mat, int row, int col);
void printvec(frac *v, int len);
void printfrac(frac a);
void printexp(sign* exp, int n);
void printnamelist(void);

#endif //INTERACT_H
