#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addcmd(void);
void clstdin(void);

int main(void)
{
	while(1){
		char command[20] = {'\0'}, **argu = NULL;
		int cnt, mode;

		if((mode = getcmd(0, command, &argu, &cnt)) <= 0)
			continue;
		if(mode == 10)
			break;

		if(mode == 1)
			run(argu);
		if(mode == 2){
			addcmd();
			clstdin();
		}
//		printf("(%d)%s\n", cnt, command);
//		int i;
//		for(i = 0; i < cnt + 1; i ++)
//			printf("%s|", argu[i]);
//		printf("\n");
	}
	return 0;
}

int addcmd(void)
{
	char cmd[50], ch = 0;

	printf("Please input the name of the command you want to add to this shell:\n");
	scanf("%s", cmd);

	if(checkcmd(cmd) != 0){
		printf("command '%s' has already existed\n", cmd);
		return -1;
	}
	
	clstdin();
	printf("Press 'r' to show 'readme', anything else to continue\n");
	scanf("%c", &ch);

	if(ch == 'r' || ch == 'R')
		help("readme");
	
/*	char *argu[] = {"cp", NULL, NULL};

	if((argu[1] = (char *)malloc(200*sizeof(char))) == NULL){
		myerror("addcmd", NULL);
		return;
	}

	if((argu[2] = (char *)malloc(200*sizeof(char))) == NULL){
		myerror("addcmd", NULL);
		return;
	}

	memset(argu[1], '\0', 200);
	memset(argu[2], '\0', 200);

	getcwd(argu[2], 199);
	strcat(argu[2], "/");
	strcat(argu[2], cmd);
	
	printf("Please input the path of the exe:\n");

	scanf("%s", argu[1]);

	run(argu);
*/
	return 0;
}

void clstdin(void)
{
	int ch = 0;
	while(ch != '\n')
		ch = getchar();
}
