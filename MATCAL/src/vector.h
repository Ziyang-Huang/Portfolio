#ifndef VECTOR_H
#define VECTOR_H

frac mul_vector1(frac v1[], frac v2[], int n);
void nummul_vec(frac *v, frac num, int col);
void sum_vec(frac *v1, frac *v2, int col);
void normalizevec(frac *v, int tar, int col);
void switchvec(frac **mat, int tar1, int tar2);
void set0(frac **mat, int src, int tar, frac factor, int col);
bool IS_0VEC(frac *v, int col);

#endif //VECTOR_H
