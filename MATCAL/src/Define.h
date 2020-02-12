#ifndef DEFINE_H
#define DEFINE_H

typedef struct fraction{
	int mark;
	int nume;
	int deno;
}frac;

typedef struct matrix{
	char name[20];
	int row;
	int col;
	int r;
	frac **matrix;
	frac **transmat;
	frac **normat;
	frac **invmat;
	struct matrix *next;
}mat;

//数字、矩阵、操作符、括号
enum SType{NUM, MAT, LROP1, LROP2, LOP, LBR, RBR, ERR};
//加减乘除逆
enum OpType{ADD, SUB, MUL, DIV, INV, BR};

typedef union token{
	mat *mat;							//矩阵
	frac num;
	enum OpType optype;					//操作符的类型
}TOK;

typedef struct signal{          		//表达式数组成员
	enum SType stype;	 				//成员的类型，用于判定表达式合法性
	TOK tok; 
}sign;

#endif //DEFINE_H
