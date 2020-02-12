#include "head.h"

void TraDataOP_AC(void)
{
    char line[1000];
    tra *trains = NULL, *p1 = NULL, *p2 = NULL, *p = NULL;
    int cnt_sta;

    memset(line, origin, 1000);
    puts("请输入要修改的信息：");
    gets(line);
    if(((p = LoadTraNote('F', line, &cnt_sta)) == NULL) || (cnt_sta == -1)){
        sys_error();
        return ;
    }

    if((check_trainNote(p) == 1) && (cnt_sta != -2)){
        if(SCTraNum_inOrders(p->traNum) == 1)
            puts("有该班次号的订单，不可修改！");
        else if(p->maxPass == 0)
            puts("输入错误！");
        else if(p->maxPass != 0){
            if(p == NULL || (LoadTraInfo('D' ,&trains) == -1)){
                sys_error();
                return ;
            }
            p2 = p1 = trains;
            while(strcmp(p1->traNum, p->traNum) != 0 && p1->next != NULL){
                p2 = p1;
                p1 = p2->next;
            }
            if(p1->next == NULL)
                trains = insert_train(trains, p);
            if(strcmp(p1->traNum, p->traNum) == 0){
                if(p1 == trains){
                    p->next = trains->next;
                    DeleteTNote(&p1);
                    trains = p;
                }
                else{
                    p2->next = p;
                    p->next = p1->next;
                    DeleteTNote(&p1);
                }
            }
            puts("修改成功！");
            displayTra('F',trains);

            saveTraData(trains);
            //saveOPTraData(trains);
            DeleteTList(&trains);
        }
    }
    returntomenu
    getch();
    TraInfoOP();
}

void TraDataOP_D(void)
{
    tra *trains = NULL;
    int cnt, flag;
    char DtraNum[8], confirm = origin;

    cnt = LoadTraInfo('D' ,&trains);
    if(cnt == -1){
        sys_error();
        return ;
    }
    if(cnt == 0)
        puts("班次信息为空！");
    else{
        memset(DtraNum, origin, 8);
        printf("请输入要删除的班次号:");
        scanf("%s", DtraNum);
        if(SCTraNote_traNum(DtraNum, trains) == NULL)
            puts("无该班次！");
        else if((flag = SCTraNum_inOrders(DtraNum)) == 1)
            puts("有该班次号的订单，不可删除！");
        else{
            puts("要删除的班次信息为：");
            displayTraNote('F', SCTraNote_traNum(DtraNum, trains));
            clstdin();
            while(confirm != 'Y' && confirm != 'N' && confirm != 'y' && confirm != 'n'){
                puts("确定删除？（Y/N）");
                scanf("%c", &confirm);
            }
            if(confirm == 'Y' || confirm == 'y'){
                trains = Delete_TNote(trains, DtraNum);
                saveTraData(trains);
                //saveOPTraData(trains);
            }
        }
        DeleteTList(&trains);
        clstdin();
    }
    returntomenu
    getch();
    TraInfoOP();
}

void TDSC_traNum(void)
{
    tra *trains = NULL, *p1 = NULL;
    int cnt;
    char TarTraN[8];

    memset(TarTraN, origin, 8);
    printf("请输入要查找的车次号：");
    scanf("%s", TarTraN);

    cnt = LoadTraInfo('D' ,&trains);
    if(cnt == -1){
        sys_error();
        return ;
    }
    if(cnt == 0)
        puts("班次信息为空！无法查找！");
    else{
        p1 = SCTraNote_traNum(TarTraN, trains);
        if(p1 == NULL)
            printf("未找到%s的班次信息！\n",TarTraN);
        else
            displayTraNote('T',p1);

        DeleteTList(&trains);
        clstdin();
        if(p1 != NULL)
            if(switch_SCtoTIC() == 1)
                return ;
    }
    returntomenu
    getch();
    TraDataSC();
}

void TDSC_tra_city(void)
{
    tra *trains = NULL, *p = NULL;
    int cnt, LeftSeat;
    char dep[50], des[50];


    cnt = LoadTraInfo('D' ,&trains);
    if(cnt == -1){
        sys_error();
        return ;
    }
    if(cnt == 0)
        puts("班次信息为空！无法查找！");
    else if(getdep_des(dep, des, trains) == 1){
        puts("车次           余票      单价");
        p = trains;
        while(p != NULL){
            LeftSeat = SCTraN_city_LS(dep, des, p);
            if(LeftSeat >= 0)
                printf("%-15s%-10d%-10.2f\n", p->traNum, LeftSeat, priceindex * distance_count(dep, des, p));
            p = p->next;
        }
        DeleteTList(&trains);
        clstdin();
        if(cnt > 0)
            if(switch_SCtoTIC() == 1)
                return ;
    }
    returntomenu
    getch();
    TraDataSC();
}

void showTraData(void)
{
    tra *trains = NULL;
    int cnt;

    cnt = LoadTraInfo('D' ,&trains);
    if(cnt == -1){
        sys_error();
        return ;
    }
    if(cnt == 0)
        puts("班次信息为空！无法查找！");
    else{
        displayTra('T',trains);
        DeleteTList(&trains);
    }
    returntomenu
    getch();
    TraInfo();
}

void sellTicket(void)
{
    tra *trains = NULL, *tra_select = NULL, *p = NULL;
    int cnt1, cnt2, ordernum, tmp = 0;
    char dep[50], des[50], confirm = origin;
    tic *orders = NULL;

    if((cnt1 = LoadTraInfo('D' ,&trains))== -1){
        sys_error();
        return ;
    }
    if(cnt1 == 0)
        puts("班次信息为空！无法查找！");
    else if(getdep_des(dep, des, trains) == 1){
        if((cnt2 = selectTra_city(trains, &tra_select, dep, des)) == -1){
            sys_error();
            return ;
        }
        if(cnt2 == 0){
            puts("未找到直达班次！");
            clstdin();
            while(confirm != 'Y' && confirm != 'N' && confirm != 'y' && confirm != 'n'){
                puts("是否需要联程推荐？（Y/N）");
                scanf("%c", &confirm);
            }
            if(confirm == 'Y' || confirm == 'y')
                if(selltictotic(trains, cnt1, dep, des) == -1)
                    return ;
        }
        else{
            puts("车次           余票      单价");
            for(p = tra_select; p != NULL; p = p->next)
                printf("%-15s%-10d%-10.2f\n", p->traNum, p->LS, priceindex * distance_count(dep, des, p));
            if((ordernum = Selltic(tra_select, dep, des, &tmp)) == -1)
                return ;
            loadorders(&orders);
            displayONote(SCOrderN_ONum(ordernum, orders));
            DeleteOList(&orders);
        }
        clstdin();
    }
    DeleteTList(&trains);
    returntomenu
    getch();
    ticketsys();
}

void returnTicket(void)
{
    int orderNum_D, cnt;
    tic *orders = NULL;
    char confirm = origin;

    cnt = loadorders(&orders);
    if(cnt == 0)
        puts("订单信息为空！");
    else{
        printf("请输入订单号：");
        scanf("%d", &orderNum_D);
        if(SCOrderN_ONum(orderNum_D, orders) != NULL){
            puts("要删除的订单信息为：");
            displayONote(SCOrderN_ONum(orderNum_D, orders));
            clstdin();
            while(confirm != 'Y' && confirm != 'N' && confirm != 'y' && confirm != 'n'){
                puts("确定删除？（Y/N）");
                scanf("%c", &confirm);
            }
            if(confirm == 'Y' || confirm == 'y')
                DeleteONote(orderNum_D);
        }
        else{
            printf("未找到");
            printf_orderNum2(orderNum_D);
            puts("的订单信息！");
        }
    }
    DeleteOList(&orders);
    clstdin();
    returntomenu
    getch();
    ticketsys();
}

void showorders(void)
{
    int cnt;
    tic *orders;

    if((cnt = loadorders(&orders)) == -1){
        sys_error();
        return ;
    }
    if(cnt != 0)
        printf("成功读取%d条订单信息：\n",cnt);

    displayorder(orders);
    DeleteOList(&orders);
    returntomenu
    getch();
    ticketsys();
}
