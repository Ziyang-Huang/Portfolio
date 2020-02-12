#include <stdio.h>
#include <string.h>
#include "common.h"

int chkcmp(int argc, char *argu[])
{
	if(argc == 1){
		printf("cmp: missing operand after 'cmp'\ncmp: Try 'cmp --help' for more information.\n") ;
		return -1;
	}

	if(argc > 3){
//		int i;
//		for(i = 2; i < cnt; i ++)
		printf("cmp: invalid --ignore-initial value '%s'\ncmp: Try 'cmp --help' for more information.\n", argu[3]) ;
		return -1;
	}

	return 0;
}

int main(int argc, char *argu[])
{
//	char command[20] = {'\0'}, **argu = NULL;
//	int cnt, mode = 0;

//	if(getcmd(1, command, &argu, &cnt) == -1)
//		return ;
	if(chkcmp(argc, argu) == -1)
		return 0;

	FILE *fp1 = NULL, *fp2 = NULL;
	int flag = 1;

	if((fp1 = fopen(argu[1], "r")) == NULL){
		myerror(argu[0], argu[1]);
		flag = 0;
	}

	if(argc == 2){
		if(strcmp(argu[1], "--help") == 0){
			help(argu[0]);
			return 0;
		}

		if((fp2 = fopen("-", "w")) == NULL){
			myerror(argu[0], "-");
			return 0;
		}

		char c;
		while((c = getc(stdin)) != EOF)
			putc(c, fp2);
		fclose(fp2);

//		if((fp2 = fopen("-", "r")) == NULL){
//			myerror(argu[0], "-");
//			remove("-");
//			return 0;
//		}

		argu[2] = "-";
	}

	if((fp2 = fopen(argu[2], "r")) == NULL){
		myerror(argu[0], argu[2]);
		flag = 0;
	}

	if(flag == 0){
		if(argc == 2){
			fclose(fp2);
			remove("-");
		}
		return 0;
	}

	char line1[500], line2[500];
	int line = 0, i, flag1 = 0, flag2 = 0, byte = 1;
	memset(line1, '\0', 500);
	memset(line2, '\0', 500);

	while(1){
		if(fgets(line1, 500, fp1) != NULL)
			flag1 = 1;
		if(fgets(line2, 500, fp2) != NULL)
			flag2 = 1;
		if(flag1 + flag2 < 2)
			break;

		line ++;
		for(i = 0; line1[i] == line2[i]; i++)
			if(line1[i] == '\0' && line2[i] == '\0'){
				flag1 = flag2 = 0;
				break;
			}
		byte += i;
		if(line1[i] != line2[i])
			break;
	}

	if(flag1 + flag2 == 1){
		printf("cmp: EOF on ");
		if(flag1 == 0)
			printf("%s.\n", argu[1]);
		else
			printf("%s.\n", argu[2]);
	}

	else if(flag1 + flag2 == 0){
		fclose(fp1);
		fclose(fp2);
		if(argc == 2)
			remove("-");
		return 0;
	}

	else{
		fclose(fp1);
		fclose(fp2);	
		printf("%s %s differ: byte %d, line %d\n", argu[1], argu[2], byte, line);
		if(argc == 2)
			remove("-");
	}

	return 0;
}
