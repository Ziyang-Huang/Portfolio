#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <curses.h>

void wcdisplay(int order[], int l, int w, int c);
void cntfile(int *l, int *w, int *c, FILE *fp);
void copyfile(char *fn1, char *fn2);
void copydir(char *path, char *dir1, char *dir2);
void stdpath(char *path_rsc, char *fn, char *path_des);
void initial(void);

void man(void)
{
	char command[20] = {'\0'}, **argu = NULL;
	int cnt;

	if(getcmd(1, command, &argu, &cnt) == -1)
		return ;

	if(cnt == 0){
		printf("What manual page do you want?\n");
		return;
	}

	FILE *fp;
	char path[1000], manual[100];

	memset(path, '\0', 1000);
	memset(manual, '\0', 100);
	getcwd(path, 1000);
	strcpy(manual, path);
	strcat(manual, "/help/");
	strcat(manual, *argu);
	strcat(manual, ".help");

	if((fp = fopen(manual, "r")) == NULL){
		printf("No manual entry for %s\n", *argu);
		return;
	}

	char line[1000];
	int i, ch = 0, n = 1, l, w, c;

	initial();

	cntfile(&l, &w, &c, fp);
	while(ch != 'q'){
		if(ch == KEY_UP)
			n --;
		if(ch == KEY_DOWN)
			n ++;
		if(n >= l - LINES)
			n = l - LINES;
		if(n < 0)
			n = 0;

		rewind(fp);
		for(i = 0; i < n; i++)
			if(fgets(line, 1000, fp) == NULL)
				break;

		for(i = 1; i < LINES; i ++){
			memset(line, '\0', 1000);
			if((fgets(line, 1000, fp)) != NULL)
				addstr(line);
		}

		attron(A_REVERSE);
		mvprintw(LINES-1, 0, " Manual page %s(1) line %d press 'q' to quit", *argu, n + 1);
		attroff(A_REVERSE);

		refresh();
		ch = getch();
		clear();
	}
	endwin();
}

void initial(void)
{
	initscr();
//	cbreak();
//	nonl();
//	noecho();
//	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	refresh();
}

void cp(void)
{
	char command[20] = {'\0'}, **argu = NULL;
	int cnt, mode = 0;
	struct stat buff1, buff2;

	if(getcmd(1, command, &argu, &cnt) == -1)
		return ;

	if(cnt == 0){
		printf("cp: missing file operand\nTry 'cp --help' for more information.\n");
		return;
	}

	else if(cnt == 1){
		printf("cp: missing destination file operand after ‘%s’\nTry 'cp --help' for more information.\n", *argu);
		return;
	}

	int i, mark, flag;
	for(i = 0; i < cnt; i ++)
		if(strcmp(argu[i], "-r") == 0){
			mode = 1;
			mark = i;
		}

	char path[1000], fpath1[1000], fpath2[1000];

	memset(path, '\0', 1000);
	getcwd(path, 1000);

	if(mode == 0){
		if(cnt == 2){
			stdpath(path, argu[0], fpath1);
			stdpath(path, argu[1], fpath2);

			stat(fpath1, &buff1);
			if(S_ISDIR(buff1.st_mode) == 1){
				printf("cp: omitting directory ‘%s’\n", argu[0]);				return;
			}

			else if(stat(fpath2, &buff2) == -1){
				copyfile(fpath1, fpath2);
				return;
			}

			else if(S_ISDIR(buff2.st_mode) == 1){
				strcat(fpath2, "/");
				strcat(fpath2, argu[0]);
				copyfile(fpath1, fpath2);
				return;
			}

			else
				copyfile(fpath1, fpath2);
		}
		else if(cnt > 2){
			stdpath(path, argu[cnt - 1], fpath2);
			if(stat(fpath2, &buff2) == -1){
				printf("cp: target ‘%s’ is not a directory", argu[cnt - 1]);
				return;
			}
			else if(S_ISDIR(buff2.st_mode) != 1){
				printf("cp: target ‘%s’ is not a directory", argu[cnt - 1]);
				return;
			}

			int i;
			char dpath[1000];
			memset(dpath, '\0', 1000);
			strcpy(dpath, fpath2);

			for(i = 0; i < cnt - 1; i ++){
				stdpath(path, argu[i], fpath1);
				stdpath(dpath, argu[i], fpath2);
				copyfile(fpath1, fpath2);
			}
		}
		return;
	}

	for(i = 0; i < cnt; i ++){
		if(mark != cnt - 1)
			if(i != mark && i != cnt -1)
				copydir(path, argu[i], argu[cnt - 1]);
		if(mark == cnt - 1)
			if(i != mark && i != cnt - 2)
				copydir(path, argu[i], argu[cnt - 2]);
	}
}

void copydir(char *path, char *dir1, char *dir2)
{
	DIR *d1, *d2;
	struct dirent *p;
	char path1[1000], path2[1000];
	struct stat buff;

	stdpath(path, dir1, path1);
	stdpath(path, dir2, path2);

	if((d1 = opendir(path1)) == NULL){
		printf("%s: cannot find.\n", dir1);
		return;
	}
	if((d2 = opendir(path2)) == NULL){
		stat(path1, &buff);
		mkdir(path2, buff.st_mode);
	}

	int i = 0;
	while((p = readdir(d1)) != NULL){
		if(strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0){
//			printf("%s/%s\n", dir1, p->d_name);
			continue;}

		struct stat buf;
		char nextpath1[1000], nextpath2[1000];

		stdpath(path1, p->d_name, nextpath1);
		stdpath(path2, p->d_name, nextpath2);
		stat(nextpath1, &buf);

//		printf("(%d)\n", ++ i);

		if(S_ISREG(buf.st_mode) == 1){
//			printf("file: %s\n", p->d_name);
			copyfile(nextpath1, nextpath2);
		}

		if(S_ISDIR(buf.st_mode) == 1){
//			printf("dir: %s\n", p->d_name);
//			puts(nextpath1);
			copydir(path, nextpath1, nextpath2);
		}
	}
}

void stdpath(char *path_src, char *fn, char *path_des)
{
	int len;

	len = strlen(fn);
	if(fn[len - 1] == '/'){
		fn[len - 1] = '\0';
		len --;
	}

	memset(path_des, '\0', 1000);
	if(fn[0] == '/'){
		strcpy(path_des, fn);
		return ;
	}

	int lenth, i;

	strcpy(path_des, path_src);
	lenth = strlen(path_des);
	
	if(fn[0] == '.' && fn[1] == '/')
		for(i = 0; i < len; i ++)
			fn[i] = fn[i + 2];

	if(fn[0] == '.' && fn[1] == '.' && fn[2] == '/'){
		for(i = 0; i < len; i ++)
			fn[i] = fn[i + 3];
		for(i = lenth - 1; path_des[i] != '/'; i --);
		for(; path_des[i] != '\0'; i++)
			path_des[i] == '\0';
	}

	strcat(path_des, "/");
	strcat(path_des, fn);
}

void copyfile(char *fn1, char *fn2)
{
/*	FILE *fp1 = NULL, *fp2 = NULL;

	if((fp1 = fopen(fn1, "r")) == NULL){
		printf("%s: No such file or directory\n", fn1);
		return;
	}
	if((fp2 = fopen(fn2, "w")) == NULL){
		printf("cp: Error when create %s\n", fn2);
		return;
	}

	char line[1000];
	memset(line, '\0', 1000);
	while(fgets(line, 1000, fp1) != NULL){
		fputs(line, fp2);
		memset(line, '\0', 1000);
	}

	fclose(fp1);
	fclose(fp2);*/

	int f1,f2;
	struct stat src;

	if((f1 = open(fn1, O_RDONLY)) == -1){
		printf("%s: No such file or directory\n", fn1);
		return;
	}

	if(fstat(f1, &src) == -1){
		printf("cp: stat error when read stat of %s\n", fn1);
		return;
	}

	if((f2 = open(fn2, O_CREAT|O_WRONLY|O_TRUNC, src.st_mode)) == -1){
		printf("cp: error when creat %s", fn2);
		return;
	}

	int len_r, len_w;
	char line_r[1000], *line_w;

	memset(line_r, '\0', 1000);
	while((len_r = read(f1, line_r, 1000))){
		if(len_r == -1){
			printf("cp: error when read %s\n", fn1);
			return; 
		}
		else if(len_r > 0){
			line_w = line_r;
			while(len_w = write(f2, line_w, len_r)){
				if(len_w == len_r)
					break;
				else if(len_w > 0){
					line_w += len_w;
					len_r -= len_w;
				}
				if(len_w == -1){
					printf("cp: error when write %s\n", fn2);
					return;
				}
			}
		}
		memset(line_r, '\0', 1000);
	}
}

void wc(void)
{
	char command[20] = {'\0'}, **argu = NULL;
	int cnt;

	if(getcmd(1, command, &argu, &cnt) == -1)
		return ;

	int i, j = 0, n = 0, *mark, order[3] = {0,0,0};
	FILE *fp;

	mark = (int *)malloc(cnt * sizeof(int));
	for(i = 0; i < cnt; i ++)
		mark[i] = 0;
	for(i = 0; i < cnt; i ++){
		if(strcmp(argu[i], "-l") == 0)
			order[0] = 1;
		else if(strcmp(argu[i], "-w") == 0)
			order[1] = 1;
		else if(strcmp(argu[i], "-c") == 0)
			order[2] = 1;
		else
			mark[j++] = i;	
	}
	n = j;

	int l = 0, w = 0, c = 0;
	if(n == 0){
		if((fp = fopen("tmp", "w")) == NULL){
			printf("Error.\n");
			return ;
		}
		char ch;
		while((ch = getc(stdin)) != EOF)
			putc(ch, fp);
		fclose(fp);

		fp = fopen("tmp", "r");
		cntfile(&l, &w, &c, fp);
		fclose(fp);

		wcdisplay(order, l, w, c);
		printf("\n");
		return;
	}

	for(j = 0; j < n; j ++){
		int l1 = 0, w1 = 0, c1 = 0;
		if((fp = fopen(argu[mark[j]], "r")) == NULL){
			printf("wc: %s: No such file or directory.\n", argu[mark[j]]);
			continue;
		}
		cntfile(&l1, &w1, &c1, fp);
		fclose(fp);

		l += l1, w += w1, c += c1;
		wcdisplay(order, l1, w1, c1);
		printf(" %s\n", argu[mark[j]]);
	}
	if(n > 1){
		wcdisplay(order, l, w, c);
		printf(" total\n");
	}
}

void cntfile(int *l, int *w, int *c, FILE *fp)
{
	int i = 0, l1 = 0, w1 = 0, c1 = 0;
/*	char line[1000], tmp[1000];
	memset(line, '\0', 1000);
	memset(tmp, '\0', 1000);

	while(1){
		if(fgets(tmp, 1000, fp) == NULL)
			break;
		l1 ++;
		for(i = 0; tmp[i] != '\0'; i ++);
		c1 += i;
		w1 += stdline(tmp, line);
//		puts(line);
//		printf("(l%d)(w%d)(c%d)", l1, w1, c1);
		memset(line, '\0', 1000);
		memset(tmp, '\0', 1000);	
	}*/
	int ch, STATE = 0;
	while((ch = getc(fp)) != EOF){
		if(ch == '\n')
			l1 ++;
		c1 ++;
		if(ch == ' ' || ch == '\t' || ch == '\n'){
			STATE = 0;
		}
		else if(STATE == 0){
			STATE = 1;
			w1 ++;
		}
	}
	*l = l1, *w = w1, *c = c1;
}

void wcdisplay(int order[], int l, int w, int c)
{
	if(order[0] == 0 && order[1] == 0 && order[2] == 0)
		printf("%-d\t%-d\t%-d", l, w, c);
	else if(order[0] == 1)
		printf("%d\t", l);
	else if(order[1] == 1)
		printf("%d\t", w);
	else if(order[2] == 1)
		printf("%d", c);
}

void cat(void)
{
	char command[20] = {'\0'}, **argu = NULL;
	int cnt;

	if(getcmd(1, command, &argu, &cnt) == -1)
		return ;

	if(cnt > 0){
		FILE *fp = NULL;
		int i;
		
		for(i = 0; i < cnt; i ++){
			if((fp = fopen(argu[i], "r")) == NULL){
				printf("cat: %s: No such file or directory.\n", argu[i]);
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
}

void cmp(void)
{
	char command[20] = {'\0'}, **argu = NULL;
	int cnt, mode = 0;

	if(getcmd(1, command, &argu, &cnt) == -1)
		return ;

	if(cnt == 0){
		printf("cmp: missing operand after 'cmp'\ncmp: Try 'cmp --help' for more information.\n") ;
		return ;
	}
	if(cnt > 2){
//		int i;
//		for(i = 2; i < cnt; i ++)
		printf("cmp: invalid --ignore-initial value '%s'\ncmp: Try 'cmp --help' for more information.\n", argu[2]) ;
		return ;
	}
	FILE *fp1 = NULL, *fp2 = NULL;
	int flag = 1;

	if((fp1 = fopen(*argu, "r")) == NULL){
		printf("cmp: %s: No such file or directory.\n", *argu);
		flag = 0;
	}
	if(cnt == 1){
		if((fp2 = fopen("-", "w")) == NULL){
			printf("Error.");
			return ;
		}
		char c;
		while((c = getc(stdin)) != EOF)
			putc(c, fp2);
		fclose(fp2);
		fp2 = fopen("-", "r");
		argu[1] = "-";
	}
	else if((fp2 = fopen(*(argu + 1), "r")) == NULL){
		printf("cmp: %s: No such file or directory.\n", *argu);
		flag = 0;
	}
	if(flag == 0)
		return ;

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
			printf("%s.\n", *argu);
		else
			printf("%s.\n", *(argu + 1));
	}
	else if(flag1 + flag2 == 0){
		if(cnt == 1)
			remove("-");
		return ;
	}
	else{
		printf("%s %s differ: byte %d, line %d\n", *argu, *(argu + 1), byte, line);
		if(cnt == 1)
			remove("-");
	}
}
