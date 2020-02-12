#ifndef HEAD_H
#define HEAD_H

#ifdef __cplusplus
extern "c"{
#endif // __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define origin '\0'
#define PW "12306"
#define priceindex 0.75
#define spacenotenough puts("未能分配空间！");
#define formatwrong puts("班次信息格式错误！");
#define getorder "请输入您要执行的操作："
#define error_sys puts("发生错误！自动返回主菜单！");
#define returntomenu puts("输入任意键返回本级菜单。");

typedef struct station{
    int distance;
    int cnt_t;                 //班次数
    char **tnums;              //班次号
    char city[50];             //车站名
    int leftSeat;              //余票数
    struct station *next;
}sta;

typedef struct train{
    char traNum[20];           //班次号
    int maxPass;               //最大载客数
//    char date[20];             //发车日期
//    char time[20];             //发车时间
    int LS;                    //对应于某两个站点，该车次的余票数；为负时表示至少无其中一个对应站点
    sta *stations;
    struct train *next;
}tra;

typedef struct ticket{
    int OrderNum;              //订单号
    int connect;               //联程票
    char traNum[20];           //班次号
    char departure[50];        //出发站
    char destination[50];      //到达站
    int ticNum;                //票数
    float pricepertic;         //单价
    struct ticket *next;
}tic;

typedef struct tsta{
    char traNum[20];
    int leftseat;
    struct tsta *next;
}tsta;

typedef struct ttot{
    char departure[50];
    char destination[50];
    int cnt_tnum;
    tsta *tsta;
    struct ttot *next;
}ttot;

void clstdin(void);
int init_traP(tra **p);
int init_staP(sta **p);
int init_ticP(tic **p);
int init_tnums(char ***arr, int n);

#include "FileIO.h"
#include "list.h"
#include "dataOP.h"
#include "interact1.h"
#include "interact2.h"
#include "interface.h"
#include "search&sort.h"
#include "logic.h"
#include "recommend.h"

#include "FileIO.c"
#include "list.c"
#include "dataOP.c"
#include "interact1.c"
#include "interact2.c"
#include "interface.c"
#include "search&sort.c"
#include "logic.c"
#include "recommend.c"

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HEAD_H
