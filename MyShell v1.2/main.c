#include <stdio.h>

void run(int mode);

int main(void)
{
	while(1){
		char command[20] = {'\0'}, **argu = NULL;
		int cnt, mode;
		if((mode = getcmd(0, command, &argu, &cnt)) == -1)
			continue;
		if(mode == 10)
			break;
		run(mode);
//		printf("(%d)\n%s\n", cnt, command);
//		int i;
//		for(i = 0; i < cnt; i ++)
//			printf("%s|", argu[i]);
//		printf("\n");
	}
	return 0;
}

void run(int mode)
{
	switch(mode){
		case 1: cp(); break;
		case 2: cmp(); break;
		case 3: wc(); break;
		case 4: cat(); break;
		case 5: man(); break;
	}
}
