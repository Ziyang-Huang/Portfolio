#ifndef CMD_H
#define CMD_H

void start(void);
mat *getmat_name(char cmd[], char name[]);
int addmat(char cmd[]);
int CMD_TYPE(char cmd[]);
int IS_INPUT(char cmd[], int len);
int stdinput(char input[], char name[], char buff[], int *row, int *col);
int checkinput(char input[], int row);

#endif //CMD_H
