#ifndef FRACTION_H
#define FRACTION_H

frac sum_frac(frac arr[], int n);
frac mul_frac(frac arr[], int n);
frac getfrac(char num[], int len);
bool IS_INT(char num[], int len);
int IS_FRAC(char num[], int len);
int GCD(int n, int m);
int LCM(int n, int m);
frac resort(frac num);
frac inttofrac(int a);
frac get_aij(frac **mat, int i, int j);

#endif //FRACTION_H
