#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>

mat *init_matptr(int mode, char name[], int row, int col);
frac **init_matrix(int row, int col);
mat *creatmatptr(char name[], int row, int col, char buff[]);
int getmat(char name[], frac **mat, int row, int col, char buff[]);
void gettrans_mat(frac **mat, frac **transmat, int row, int col);
int get_r(frac **mat, int row, int col);
int getinv_mat(frac **mat, frac **invmat, int row, int col);
mat *getmatptr(char name[]);
mat *sum_mat(mat *p1, mat *p2);
int checkmatsum(mat *p1, mat *p2);
mat *mul_mat(mat *p1, mat *p2);
int checkmatmul(mat *p1, mat *p2);
mat *nummul_mat(frac num, mat *p);
mat *inv_mat(mat *p);
int checkmatinv(mat *p);
bool IS_SQUARE(mat *p);
int normalizemat(frac **mat, int row, int col);
mat *sub_mat(mat *p1, mat *p2);
mat *div_mat(mat *p1, mat *p2);
int checkrename(char name[]);

#endif //MATRIX_H
