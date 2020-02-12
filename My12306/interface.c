#include "head.h"

void TraInfoLF(void)
{
    int cnt;
    tra *trains = NULL;
    tic *orders = NULL, *ptr = NULL, *tmp = NULL;

    if((cnt = LoadTraInfo('F' ,&trains)) == -1){
        puts("发生错误！");
        exit(1);
    }
    printf("成功读取%d条班次信息!",cnt);

    trains = sort_trains_TNum(trains);
    saveTraData(trains);
    DeleteTList(&trains);

    if((cnt = loadorders(&orders)) == -1){
        puts("发生错误！");
        exit(1);
    }
    if(cnt == 0)
        printf("订单信息为空！");
    else{
        printf("成功读取%d条订单信息：\n",cnt);
        for(ptr = orders; ptr != NULL; ptr = ptr->next){
            tmp = CutTraLS_ONote(ptr);
            if(tmp != NULL)
                if(tmp->OrderNum == -1)
                    puts("订单票数信息与车次余座信息不符！");
        }
    }
}

void MainMenu(int n)
{
    int mode = 0;
    if(n != 0)
        system("cls");
    puts("\n\n                           欢迎使用火车票管理系统！\n\n");
    puts("                         |========================|");
    puts("                         | 1.班次信息管理系统     |");
    puts("                         | 2.票务系统             |");
    puts("                         | 3.退出系统             |");
    puts("                         |========================|");

    while(mode != 1 && mode != 2 && mode != 3){
        printf(getorder);
        scanf("%d", &mode);
        clstdin();
    }

    switch(mode)
    {
        case 1: TraInfo();break;
        case 2: ticketsys();break;
        case 3: puts("已退出系统！");return;
    }
}

void TraInfo(void)
{
    int mode = 0;
    char password[20];

    system("cls");
    puts("\n\n\n\n\n                         |========================|");
    puts("                         | 1.操作班次信息         |");
    puts("                         | 2.查询班次信息         |");
    puts("                         | 3.显示所有班次信息     |");
    puts("                         | 4.主菜单               |");
    puts("                         |========================|");

    while(mode != 1 && mode != 2 && mode != 3 && mode != 4){
        printf(getorder);
        scanf("%d", &mode);
        clstdin();
    }

    switch(mode)
    {
        case 1: {
            puts("请输入密码：");
            scanf("%s", password);
            if(strcmp(password, PW) == 0)
                TraInfoOP();
            else{
                puts("密码错误！无操作权限！");
                returntomenu
                getch();
                TraInfo();
            }
        }break;
        case 2: TraDataSC();break;
        case 3: showTraData();break;
        case 4: MainMenu(1);break;
    }
}

void TraInfoOP(void)
{
    int mode = 0;

    system("cls");
    puts("\n\n\n\n\n                         |========================|");
    puts("                         | 1.添加/修改班次        |");
    puts("                         | 2.删除班次             |");
    puts("                         | 3.上一级菜单           |");
    puts("                         | 4.主菜单               |");
    puts("                         |========================|");

    while(mode != 1 && mode != 2 && mode != 3 && mode != 4){
        printf(getorder);
        scanf("%d", &mode);
        clstdin();
    }

    switch(mode)
    {
        case 1: TraDataOP_AC();break;
        case 2: TraDataOP_D();break;
        case 3: TraInfo();break;
        case 4: MainMenu(1);break;
    }
}

void TraDataSC(void)
{
    int mode = 0;

    system("cls");
    puts("\n\n\n\n\n                         |========================|");
    puts("                         | 1.按车次号查找         |");
    puts("                         | 2.按出发/到达站查找    |");
    puts("                         | 3.上一级菜单           |");
    puts("                         | 4.主菜单               |");
    puts("                         |========================|");

    while(mode != 1 && mode != 2 && mode != 3 && mode != 4){
        printf(getorder);
        scanf("%d", &mode);
        clstdin();
    }

    switch(mode)
    {
        case 1: TDSC_traNum();break;
        case 2: TDSC_tra_city();break;
        case 3: TraInfo();break;
        case 4: MainMenu(1);break;
    }
}

void ticketsys(void)
{
    int mode = 0;

    system("cls");
    puts("\n\n\n\n\n                         |========================|");
    puts("                         | 1.售票系统             |");
    puts("                         | 2.退票系统             |");
    puts("                         | 3.查看订单             |");
    puts("                         | 4.主菜单               |");
    puts("                         |========================|");

    while(mode != 1 && mode != 2 && mode != 3 && mode != 4){
        printf(getorder);
        scanf("%d", &mode);
        clstdin();
    }

    switch(mode)
    {
        case 1: sellTicket();break;
        case 2: returnTicket();break;
        case 3: showorders();break;
        case 4: MainMenu(1);break;
    }
}

