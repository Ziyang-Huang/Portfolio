#include "head.h"

int check_trainNote(tra *p)
{
    sta *pt = NULL;
    int flag, cnt_sta;

    if(p->maxPass <= 0){
        displayTraNote('F', p);
        puts("最大载客数非正！");
        return -1;
    }
    for(pt = p->stations, cnt_sta = 0; pt!= NULL; pt = pt->next, cnt_sta ++);
    if(cnt_sta < 2){
        displayTraNote('F', p);
        puts("车站数小于2！");
        return -1;
    }
    for(pt = p->stations, flag = 0; pt->next!= NULL; pt = pt->next){
        if(strcmp(pt->city,pt->next->city) == 0){
            flag = 1;
            displayTraNote('T', p);
            printf("相邻站名重复（%s）\n", pt->city);
        }
    }
    if(flag == 1)
        return -1;
    for(pt = p->stations, flag = 0; pt!= NULL; pt = pt->next){
        if(pt->leftSeat < 0){
            flag = 1;
            displayTraNote('T', p);
            printf("%s的余座数为负！", pt->city);
        }
    }
    if(flag == 1)
        return -1;

    return 1;
}

tic *CutTraLS_ONote(tic *order)
{
    tra *trains = NULL, *p = NULL;
    int cnt;
    sta *dep = NULL, *des = NULL, *pt = NULL;
    tic *wrong = NULL, *right = NULL;

    if(init_ticP(&wrong) == -1)
        return NULL;
    if(init_ticP(&right) == -1)
        return NULL;
    wrong->ticNum = -1;
    right->ticNum = 0;

    cnt = LoadTraInfo('D' ,&trains);
    if(cnt < 0){
        DeleteTList(&trains);
        return NULL;
    }
    if(cnt == 0){
        DeleteTList(&trains);
        return right;
    }
    if((p = SCTraNote_traNum(order->traNum, trains)) == NULL){
        puts("订单班次号信息有误！");
        DeleteTList(&trains);
        return order;
    }
    if(SCdep_des(p, &dep, &des, order->departure, order->destination) < 0){
        puts("订单出发/到达站信息有误！");
        DeleteTList(&trains);
        return order;
    }
    for(pt = dep; pt != des; pt = pt->next){
        if(order->ticNum <= pt->leftSeat)
            pt->leftSeat -= order->ticNum;
        else{
            puts("订单票数信息与车次余座信息不符！");
            displayONote(order);
            displayTraNote('T', p);
            DeleteTList(&trains);
            return wrong;
        }
    }
    saveTraData(trains);
    DeleteTList(&trains);
    return right;
}

tic *AddTraLS_ONote(tic *order)
{
    tra *trains = NULL, *p = NULL;
    int cnt;
    sta *dep = NULL, *des = NULL, *pt = NULL;
    tic *wrong = NULL, *right = NULL;

    if(init_ticP(&wrong) == -1){
        DeleteTList(&trains);
        return NULL;
    }
    if(init_ticP(&right) == -1){
        DeleteTList(&trains);
        return NULL;
    }
    wrong->ticNum = -1;
    right->ticNum = 0;

    cnt = LoadTraInfo('D' ,&trains);
    if(cnt < 0){
        DeleteTList(&trains);
        return NULL;
    }
    if(cnt == 0){
        DeleteTList(&trains);
        return right;
    }

    if((p = SCTraNote_traNum(order->traNum, trains)) == NULL){
        puts("订单信息有误！");
        DeleteTList(&trains);
        return order;
    }
    if(SCdep_des(p, &dep, &des, order->departure, order->destination) < 0){
        puts("订单信息有误！");
        DeleteTList(&trains);
        return order;
    }
    for(pt = dep; pt != des; pt = pt->next){
        if(pt->leftSeat + order->ticNum <= p->maxPass)
            pt->leftSeat += order->ticNum;
        else{
            DeleteTList(&trains);
            return wrong;
        }
    }
    saveTraData(trains);
    DeleteTList(&trains);
    return right;
}

