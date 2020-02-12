#include <stdio.h>
#include <string.h>
#include <curses.h>
#include "common.h"

void initial(void);

int main(int argc, char *argu[])
{
//	char command[20] = {'\0'}, **argu = NULL;
//	int cnt;

//	if(getcmd(1, command, &argu, &cnt) == -1)
//		return ;

	if(argc == 1){
		printf("What manual page do you want?\n");
		return 0;
	}
	if(argc == 3){
		printf("Please man only one order at a time\n");
		return 0;
	}

	if(strcmp(argu[1], "--help") == 0){
		help(argu[0]);
		return 0;
	}

	FILE *fp;
	char path[1000], manual[100];

	memset(path, '\0', 1000);
	memset(manual, '\0', 100);
	getcwd(path, 1000);
	strcpy(manual, path);
	strcat(manual, "/help/");
	strcat(manual, argu[1]);
	strcat(manual, ".help");

	if((fp = fopen(manual, "r")) == NULL){
		printf("No manual entry for %s\n", argu[1]);
		return;
	}

	char line[1000];
	int i, ch = 0, n = 0, l, w, c;

	initial();

	cntfile(&l, &w, &c, fp);
	while(ch != 'q'){
		if(ch == KEY_UP || ch == 'k' || ch == 'K')
			n --;
		if(ch == KEY_DOWN || ch == 'j' || ch == 'J')
			n ++;
		if(n >= l - LINES + 1)
			n = l - LINES + 1;
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
		mvprintw(LINES-1, 0, " Manual page %s(1) line %d press 'q' to quit", argu[1], n + 1);
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
