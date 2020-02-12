#include <stdio.h>

int main(int argc, char *argu[])
{
//	char command[20] = {'\0'}, **argu = NULL;
//	int cnt;

//	if(getcmd(1, command, &argu, &cnt) == -1)
//		return ;

	if(argc > 1){
		if(strcmp(argu[1], "--help") == 0){
			help(argu[0]);
			return 0;
		}

		FILE *fp = NULL;
		int i;
		
		for(i = 1; i < argc; i ++){
			if((fp = fopen(argu[i], "r")) == NULL){
				myerror(argu[0], argu[i]);
//				printf("cat: %s: No such file or directory.\n", argu[i]);
				return ;
			}

			char c;
			while((c = getc(fp)) != EOF)
				printf("%c", c);

			fclose(fp);
		}
	}

	else{
		char c;
		while((c = getc(stdin)) != EOF)
			putc(c, stdout);
	}

	return 0;
}
