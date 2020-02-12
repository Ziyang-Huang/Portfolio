#ifndef INTERACT2_H
#define INTERACT2_H

int selltictotic(tra *trahead, int cnt_tra, char dep[50], char des[50]);
int Selltic(tra *tra_select, char dep[50], char des[50], int *n);
int DeleteONote(int tar);
int switch_SCtoTIC(void);
int getdep_des(char dep[50], char des[50], tra *trahead);
void getmid(ttot **head, int n, char mid[50]);
void sys_error(void);
void displayTra(char mode, tra *head);
void displayTraNote(char mode, tra *p);
void displayorder(tic *head);
void displayONote(tic *p);
void printf_orderNum1(int OrderNum);
void printf_orderNum2(int OrderNum);

#endif // INTERACT2_H
