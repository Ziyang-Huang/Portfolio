#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Define.h"
#include "interact.h"
#include "matrix.h"
#include "cmd.h"

mat *head = NULL, *tmphead = NULL;
sign err;
int cnt_mat, max_token;

int main(void)
{
	err.stype = ERR;
	cnt_mat = 0;

//	loadconfig();
	loadmat_origin();

	start();

	savemat();
	deletematlist();

	return 0;
}
