#include "head.h"

int selltictotic(tra *trahead, int cnt_tra, char dep[50], char des[50])
{
    ttot **head = NULL;
    tra *tra_select = NULL;
    int n, connect, ordernum = 0, cnt, ticnum = 0;
    char mid[50];
    tic *orders = NULL, *ptr = NULL;

    if((head = recommend(trahead, cnt_tra, dep, des, &n)) == NULL)
        return -1;
    displayTtoTarr(head, n);
    if(n > 0){
        getmid(head, n, mid);
        printf("%s---%s段：\n", dep, mid);
        DeleteTList(&tra_select);
        if((cnt = selectTra_city(trahead, &tra_select, dep, mid)) == -1){
            sys_error();
            return -1;
        }
        if((ordernum = Selltic(tra_select, dep, mid, &ticnum)) == -1)
            return -1;

        printf("%s---%s段：\n", mid, des);
        DeleteTList(&tra_select);
        if((cnt = selectTra_city(trahead, &tra_select, mid, des)) == -1){
            sys_error();
            return -1;
        }
        if((connect = Selltic(tra_select, mid, des, &ticnum)) == -1)
            return -1;

        if(connect == 0)
            DeleteONote(ordernum);
        else{
            loadorders(&orders);
            ptr = SCOrderN_ONum(ordernum, orders);
            ptr->connect = connect;
            ptr = SCOrderN_ONum(connect, orders);
            ptr->connect = ordernum;
            ptr = SCOrderN_ONum(ordernum, orders);
            saveOrderData(orders);
            DeleteOList(&orders);
            loadorders(&orders);
            displayONote(ptr);
            DeleteOList(&orders);
        }
    }
    return ordernum;
}

int Selltic(tra *tra_select, char dep[50], char des[50], int *n)
{
    int LS, OrderNum, ticnum;
    char TarTraN[20];
    tra *p1 = NULL;
    tic *orders = NULL;

    memset(TarTraN, origin, 20);
    printf("请输入所选的车次号：");
    scanf("%s", TarTraN);

    if((p1 = SCTraNote_traNum(TarTraN, tra_select)) != NULL)
        LS = SCTraN_city_LS(dep, des, p1);
    while(p1 == NULL || LS == 0){
        puts("输入错误，请重新输入所选车次号：");
        scanf("%s", TarTraN);
        if((p1 = SCTraNote_traNum(TarTraN, tra_select)) != NULL)
            LS = SCTraN_city_LS(dep, des, p1);
    }
    if(*n == 0){
        printf("请输入购票数量：");
        scanf("%d", &ticnum);
        while(ticnum <= 0 || (SCTraN_city_LS(dep, des, p1) < ticnum)){
            printf("输入非正或余票不足！请重新输入购票数量：");
            scanf("%d", &ticnum);
        }
        *n = ticnum;
        loadorders(&orders);
        if((OrderNum = sellT_order(p1, &orders, ticnum, dep, des)) == -1){
            sys_error();
            return -1;
        }
        saveOrderData(orders);
        DeleteOList(&orders);
    }
    else{
        ticnum = *n;
        if(SCTraN_city_LS(dep, des, p1) < ticnum){
            printf("%s车次余票不足，请重新购票！\n", p1->traNum);
            OrderNum = 0;
        }
        else{
            loadorders(&orders);
            if((OrderNum = sellT_order(p1, &orders, ticnum, dep, des)) == -1){
                sys_error();
                return -1;
            }
            saveOrderData(orders);
            DeleteOList(&orders);
        }
    }

    return OrderNum;
}

int DeleteONote(int tar)
{
    int cnt, connect;
    tic *orders = NULL, *ptr = NULL, *tmp = NULL;

    loadorders(&orders);
    ptr = SCOrderN_ONum(tar, orders);
    if((connect = ptr->connect) != 0){
        tmp = SCOrderN_ONum(connect, orders);
        tmp->connect = 0;
        saveOrderData(orders);
        DeleteOList(&orders);
        loadorders(&orders);
        DeleteONote(connect);
        DeleteOList(&orders);
        loadorders(&orders);
    }
    if((cnt = Delete_ONoteOP(&orders, tar)) < 0){
        sys_error();
        return -1;
    }
    else if(cnt == 1)
        saveOrderData(orders);

    DeleteOList(&orders);

    return 0;
}

int switch_SCtoTIC(void)
{
    char confirm = origin;

    while(confirm != 'Y' && confirm != 'N' && confirm != 'y' && confirm != 'n'){
        printf("是否需要转入票务系统？（Y/N）");
        scanf("%c", &confirm);
        clstdin();
    }
    if(confirm == 'Y' || confirm == 'y'){
        ticketsys();
        return 1;
    }
    else
        return 0;
}

int getdep_des(char dep[50], char des[50], tra *trahead)
{
    sta *pt1 = NULL, *pt2 = NULL;
    int flag1 = 0, flag2 = 0;

    memset(dep, origin, 50);
    memset(des, origin, 50);
    printf("出发站：");
    scanf("%s", dep);
    std_city(dep);
    printf("到达站：");
    scanf("%s", des);
    std_city(des);
    if(strcmp(dep, des) == 0){
        puts("出发站与到达站相同！");
        return 0;
    }
    if((pt1 = SCcity_trains(trahead, dep)) != NULL)
        flag1 ++;
    if((pt2 = SCcity_trains(trahead, des)) != NULL)
        flag2 ++;
    if(flag1 == 1 && flag2 == 1)
        return 1;
    if(flag1 == 1 && flag2 == 0){
        puts("班次信息中无到达站！");
        return -1;
    }
    if(flag1 == 0 && flag2 == 1){
        puts("班次信息中无出发站！");
        return -2;
    }
    puts("班次信息中无出发站和到达站！");
    return -3;
}

void getmid(ttot **head, int n, char mid[50])
{
    int flag = 1, i;

    printf("请输入中转站：");
    memset(mid, origin, 50);
    scanf("%s", mid);
    std_city(mid);
    while(flag){
        for(i = 0; i < n; i ++)
            if(strcmp(head[i]->destination, mid) == 0)
                flag = 0;
        if(flag == 1){
            printf("中转站输入错误，请重新输入：");
            memset(mid, origin, 50);
            scanf("%s", mid);
            std_city(mid);
        }
    }
}

void sys_error(void)
{
    error_sys
    getch();
    MainMenu(1);
}

void displayTra(char mode, tra *head)
{
    tra *p;

    if(head == NULL){
        printf("车次信息为空！");
        return ;
    }
    p = head;

    while(p != NULL){
        displayTraNote(mode, p);
        p = p->next;
    }
}

void displayTraNote(char mode, tra *p)
{
    sta *pt;
    int i;

    printf("%s, %d", p->traNum, p->maxPass);

    pt = p->stations;
    while(pt != NULL){
        if(mode == 'F')
            printf("| %s", pt->city);
        if(mode == 'T')
            printf("| %s, %d", pt->city, pt->leftSeat);
        if(mode == 'A'){
            printf("| %s(", pt->city);
            for(i = 0; i < pt->cnt_t; i++)
                printf("%s,", pt->tnums[i]);
            printf(")");
        }
        pt = pt->next;
    }
    printf("|\n");
}

void displayorder(tic *head)
{
    tic *p;

    if(head == NULL){
        puts("订单信息为空！");
        return ;
    }
    p = head;
    puts("订单信息为：");
    puts("订单号  班次号    出发站      到达站      票数  单价      总价    关联订单");
    while(p != NULL){
        printf_orderNum1(p->OrderNum);
        printf("%-10s%-12s%-12s%-6d%-10.2f%-10.2f", p->traNum, p->departure, p->destination, p->ticNum, p->pricepertic, p->ticNum * p->pricepertic);
        if(p->connect != 0)
            printf_orderNum1(p->connect);
        printf("\n");
        p = p->next;
    }
}

void displayONote(tic *p)
{
    tic *orders = NULL, *ptr = NULL;

    if(p == NULL){
        puts("订单信息为空！");
        return ;
    }
    puts("订单信息为：");
    puts("订单号  班次号    出发站      到达站      票数  单价    总价    关联订单");
    printf_orderNum1(p->OrderNum);
    printf("%-10s%-12s%-12s%-6d%-8.2f%-8.2f", p->traNum, p->departure, p->destination, p->ticNum, p->pricepertic, p->ticNum * p->pricepertic);
    printf_orderNum1(p->connect);
    printf("\n");
    if(p->connect != 0){
        loadorders(&orders);
        ptr = SCOrderN_ONum(p->connect, orders);
        printf_orderNum1(ptr->OrderNum);
        printf("%-10s%-12s%-12s%-6d%-8.2f%-8.2f", ptr->traNum, ptr->departure, ptr->destination, ptr->ticNum, ptr->pricepertic, ptr->ticNum * ptr->pricepertic);
        printf_orderNum1(ptr->connect);
        printf("\n");
        DeleteOList(&orders);
    }
}

void printf_orderNum1(int OrderNum)
{
    if(OrderNum >= 100)
        printf("%-8d", OrderNum);
    if(OrderNum < 100 && OrderNum > 9)
        printf("0%-7d", OrderNum);
    if(OrderNum < 10 && OrderNum > 0)
        printf("00%-6d", OrderNum);
}

void printf_orderNum2(int OrderNum)
{
    if(OrderNum >= 100)
        printf("%-3d", OrderNum);
    if(OrderNum < 100 && OrderNum > 9)
        printf("0%-2d", OrderNum);
    if(OrderNum < 10 && OrderNum > 0)
        printf("00%d", OrderNum);
}
