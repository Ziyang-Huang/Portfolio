#include "head.h"

tra *SCTraNote_traNum(char tar[20], tra *head)
{
    tra *p = NULL;

    p = head;
    while(p != NULL){
        if(strcmp(p->traNum, tar) == 0)
            return p;
        p = p->next;
    }

    return NULL;
}

sta *SCcity_trains(tra *trahead, char tar[50])
{
    tra *p = NULL;
    sta *pttmp = NULL, *pt = NULL;

    for(p = trahead; p != NULL; p = p->next){
        for(pt = p->stations; (strcmp(pt->city, tar) != 0) && (pt->next != NULL); pt = pt->next);
        if(strcmp(pt->city, tar) == 0)
            pttmp = pt;
    }
    return pttmp;
}

int SCTraNum_inOrders(char tar[20])
{
    tic  *head = NULL, *ptr = NULL;
    int flag = 0;

    loadorders(&head);
    ptr = head;
    while(ptr != NULL){
        if(strcmp(ptr->traNum, tar) == 0){
            flag = 1;
        }
        ptr = ptr->next;
    }
    DeleteOList(&head);

    return flag;
}

int selectTra_city(tra *source, tra **result, char dep[50], char des[50])
{
    tra *p = NULL, *p1 = NULL, *p2 = NULL;
    int cnt;

    p = source;
    cnt = 0;
    while(p != NULL){
        p->LS = SCTraN_city_LS(dep, des, p);
        if(p->LS >= 0){
            if(init_traP(&p1) == -1)
                return -1;
            *p1 = *p;
            p1->next = NULL;
            if(*result == NULL)
                *result = p2 = p1;
            else{
                p2->next = p1;
                p2 = p1;
            }
            cnt ++;
        }
        p = p->next;
    }
    return cnt;
}

int SCTraN_city_LS(char dep_city[50], char des_city[50], tra* p)
{
    sta *dep = NULL, *des = NULL, *pt = NULL;
    int leftseat;

    leftseat = SCdep_des(p, &dep, &des, dep_city, des_city);

    if(leftseat == 1){
        leftseat = dep->leftSeat;
        for(pt = dep->next; pt != des; pt = pt->next)
            if(pt->leftSeat < leftseat)
                leftseat = pt->leftSeat;
    }
    return leftseat;
}

int SCdep_des(tra *p, sta **dep, sta **des, char dep_city[50], char des_city[50])
{
    int flag1 = 0, flag2 = 0;
    sta *pt1 = NULL, *pt2 = NULL, *pt3 = NULL;

    pt1 = p->stations;
    while(strcmp(pt1->city, dep_city) != 0 && pt1->next->next != NULL){
        pt2 = pt1;
        pt1 = pt2->next;
    }
    if(strcmp(pt1->city, dep_city) == 0)
        flag1 ++;

    pt2 = pt1->next;
    while(strcmp(pt2->city, des_city) != 0 && pt2->next != NULL){
        pt3 = pt2;
        pt2 = pt3->next;
    }
    if(strcmp(pt2->city, des_city) == 0)
        flag2 ++;

    if(flag1 == 1 && flag2 == 1){
        *dep = pt1;
        *des = pt2;
        return 1;
    }
    if(flag1 == 1 && flag2 == 0)
        return -1;
    if(flag1 == 0 && flag2 == 1)
        return -2;
    else
        return -3;
}

tic *SCOrderN_ONum(int tar, tic *head)
{
    tic *p = NULL;

    p = head;
    while(p != NULL){
        if(p->OrderNum == tar)
            return p;
        p = p->next;
    }

    return NULL;
}

tic *sort_orders_ONum(tic *head)
{
    tic *ptr = NULL, *tmp = NULL;
    if(head == NULL)
        return head;
    ptr = head->next;
    head->next = NULL;
    while(ptr != NULL){
        tmp = ptr->next;
        head = insert_order(head, ptr);
        ptr = tmp;
    }
    return head;
}

tra *sort_trains_TNum(tra *head)
{
    tra *p = NULL, *tmp = NULL;
    if(head == NULL)
        return head;
    p = head->next;
    head->next = NULL;
    while(p != NULL){
        tmp = p->next;
        head = insert_train(head, p);
        p = tmp;
    }
    return head;
}
