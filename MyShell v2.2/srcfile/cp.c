#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

void copyfile(char *fn1, char *fn2);
void copydir(char *path, char *dir1, char *dir2);
void stdpath(char *path_rsc, char *fn, char *path_des);

int main(int argc, char *argu[])
{
//	char command[20] = {'\0'}, **argu = NULL;
//	int cnt, mode = 0;
	int mode = 0;
	struct stat buff1, buff2;
//
//	if(getcmd(1, command, &argu, &cnt) == -1)
//		return ;

	if(argc == 1){
		printf("cp: aaa missing file operand\nTry 'cp --help' for more information.\n");
		return 0;
	}

	if(argc == 2){
		if(strcmp(argu[1], "--help") == 0)
			help(argu[0]);
		else
			printf("cp: missing destination file operand after ‘%s’\nTry 'cp --help' for more information.\n", argu[1]);
		return 0;
	}

	int i, mark;

	for(i = 1; i < argc; i ++)
		if(strcmp(argu[i], "-r") == 0){
			mode = 1;
			mark = i;
		}

	char path[1000], fpath1[1000], fpath2[1000];

	memset(path, '\0', 1000);
	getcwd(path, 1000);
	
	if(mode == 0){
		if(argc == 3){
			stdpath(path, argu[1], fpath1);
			stdpath(path, argu[2], fpath2);

			if(stat(fpath1, &buff1) == -1){
				char err[100];
				memset(err, '\0', 100);
				sprintf(err, "cannot stat ‘%s’", argu[1]);
				myerror(argu[0], err);
				return 0;
			}
			if(S_ISDIR(buff1.st_mode) == 1){
				printf("cp: omitting directory ‘%s’\n", argu[1]);
				return 0;
			}

			if(stat(fpath2, &buff2) == -1){
				copyfile(fpath1, fpath2);
				return 0;
			}
			if(S_ISDIR(buff2.st_mode) == 1){
				strcat(fpath2, "/");
				strcat(fpath2, argu[1]);
				copyfile(fpath1, fpath2);
				return 0;
			}

			else
				copyfile(fpath1, fpath2);

			return 0;
		}

		stdpath(path, argu[argc - 1], fpath2);
		if(stat(fpath2, &buff2) == -1){
			if(errno == 2)
				printf("cp: target ‘%s’ is not a directory\n", argu[argc - 1]);
			else
				myerror(argu[0], argu[argc - 1]);
			return 0;
		}
		if(S_ISDIR(buff2.st_mode) != 1){			
			printf("cp: target ‘%s’ is not a directory\n", argu[argc - 1]);
			return 0;
		}

		int i;
		char dpath[1000];
		memset(dpath, '\0', 1000);
		strcpy(dpath, fpath2);

		for(i = 1; i < argc - 1; i ++){
			stdpath(path, argu[i], fpath1);
			stdpath(dpath, argu[i], fpath2);
			copyfile(fpath1, fpath2);
		}

		return 0;
	}

	for(i = 1; i < argc; i ++){
		if(mark != argc - 1)
			if(i != mark && i != argc -1)
				copydir(path, argu[i], argu[argc - 1]);
		if(mark == argc - 1)
			if(i != mark && i != argc - 2)
				copydir(path, argu[i], argu[argc - 2]);
	}
	return 0;
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
		myerror("cp", dir1);
//		printf("%s: No such file or directory\n", dir1);
		return;
	}

	if((d2 = opendir(path2)) == NULL){
		if(errno == 2){
			if(stat(path1, &buff) == -1){
				myerror("cp", dir1);
				closedir(d1);
				return;
			}

			mkdir(path2, buff.st_mode);
		}

		else{
			myerror("cp", dir2);
			closedir(d1);
			return;
		}
	}

//	int i = 0;
	while((p = readdir(d1)) != NULL){
		if(strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0){
//			printf("%s/%s\n", dir1, p->d_name);
			continue;
		}

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
	closedir(d1);
	closedir(d2);
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
		myerror("cp", fn1);
//		printf("%s: No such file or directory\n", fn1);
		return;
	}

	if(fstat(f1, &src) == -1){
		myerror("cp", fn1);
//		printf("cp: stat error when read stat of %s\n", fn1);
		return;
	}

	if((f2 = open(fn2, O_CREAT|O_WRONLY|O_TRUNC, src.st_mode)) == -1){
		myerror("cp", fn2);
//		printf("cp: error when creat %s", fn2);
		return;
	}

	int len_r, len_w;
	char line_r[1000], *line_w;

	memset(line_r, '\0', 1000);
	while((len_r = read(f1, line_r, 1000))){
		if(len_r == -1){
			myerror("cp", fn1);
//			printf("cp: error when read %s\n", fn1);
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
					myerror("cp", fn2);
//					printf("cp: error when write %s\n", fn2);
					return;
				}
			}
		}
		memset(line_r, '\0', 1000);
	}

	close(f1);
	close(f2);
}
