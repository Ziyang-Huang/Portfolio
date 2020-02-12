#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

typedef struct cmdarr{
	char cmd[20];
	int mode;
}cmdarr;

void run(char **argu);
void help(char *command);
void cntfile(int *l, int *w, int *c, FILE *fp);
void cwd(void);
void myerror(char *command, char *file);
int readcmdlist(cmdarr cmdlist[]);
int checkcmd(char cmd[]);

#endif // COMMON_H
