#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void wcdisplay(int order[], int l, int w, int c);

int main(int argc, char *argv[])
{
//	char command[20] = {'\0'} , **argv = NULL;
//	int cnt;

//	if(getcmd(1, command, &argv, &cnt) == -1)
//		return ;

	int i, j = 0, n = 0, *mark = NULL, order[3] = {0,0,0};
	FILE *fp;
	
	if(argc > 1){
		if(strcmp(argv[1], "--help") == 0){
			help(argv[0]);
			return 0;
		}

		if((mark = (int *)malloc(argc * sizeof(int))) == NULL){
			myerror(argv[0], NULL);
			return 0;
		}

		for(i = 1; i < argc; i ++)
			mark[i] = 0;
	}

	for(i = 1; i < argc; i ++){
		if(strcmp(argv[i], "-l") == 0)
			order[0] = 1;
		else if(strcmp(argv[i], "-w") == 0)
			order[1] = 1;
		else if(strcmp(argv[i], "-c") == 0)
			order[2] = 1;
		else
			mark[j++] = i;
	}
	n = j;
	
	int l = 0, w = 0, c = 0;
	if(n == 0){
		if((fp = fopen("tmp", "w")) == NULL){
			myerror(argv[0], "tmp");
			return 0;
		}

		char ch;
		while((ch = getc(stdin)) != EOF)
			putc(ch, fp);
		fclose(fp);

		if((fp = fopen("tmp", "r")) == NULL){
			myerror(argv[0], "tmp");
			return 0;
		}

		cntfile(&l, &w, &c, fp);
		fclose(fp);
		remove("tmp");

		wcdisplay(order, l, w, c);
		printf("\n");
		return 0;
	}

	for(j = 0; j < n; j ++){
		int l1 = 0, w1 = 0, c1 = 0;

		if((fp = fopen(argv[mark[j]], "r")) == NULL){
//			char err[100];
//			memset(err, '\0', 100);
//			strcpy(err, argv[0]);
//			strcat(err, ": ");
//			strcat(err, argv[mark[j]]);
			myerror(argv[0], argv[mark[j]]);
//			printf("wc: %s: No such file or directory.\n", argu[mark[j]]);
			return 0;
		}
		cntfile(&l1, &w1, &c1, fp);
		fclose(fp);

		l += l1, w += w1, c += c1;
		wcdisplay(order, l1, w1, c1);
		printf(" %s\n", argv[mark[j]]);
	}
	if(n > 1){
		wcdisplay(order, l, w, c);
		printf(" total\n");
	}
}

void wcdisplay(int order[], int l, int w, int c)
{
	if(order[0] == 0 && order[1] == 0 && order[2] == 0)
		printf("%-d\t%-d\t%-d", l, w, c);
	else{
		if(order[0] == 1)
			printf("%d\t", l);
		if(order[1] == 1)
			printf("%d\t", w);
		if(order[2] == 1)
			printf("%d", c);
	}
}
