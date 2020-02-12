#include "head.h"

ttot **recommend(tra *trahead, int cnt_tra, char dep[50], char des[50], int *cnt)
{
    ttot **head = NULL, *pointer = NULL;
    tra *p = NULL;
    sta *pt = NULL;
    int i, n, j, k, flag;
    char **tnum = NULL;

    creat_tnums(trahead, cnt_tra);
    pt = SCcity_trains(trahead, dep);
    n = pt->cnt_t;
    tnum = pt->tnums;

    head = (ttot **)malloc(n*sizeof(ttot *));
    for(i = 0; i < n; i ++)
        if((head[i] = (ttot *)malloc(sizeof(ttot))) == NULL){
            spacenotenough
            return NULL;
        }
    j = 0;

    for(i = 0; i < n; i ++){
        p = SCTraNote_traNum(tnum[i], trahead);
        for(pt = p->stations->next; pt != NULL; pt = pt->next)
            if((pointer = check_citytodes(trahead, pt->city, des)) != NULL){
                for(k = 0, flag = 1; k < j; k ++)
                    if(strcmp(head[k]->destination, pt->city) == 0)
                        flag = 0;
                if(flag == 1){
                    head[j] = check_citytodes(trahead, dep, pt->city);
                    head[j]->next = pointer;
                    j ++;
                }
        }
    }
    *cnt = j;

    return head;
}

ttot *check_citytodes(tra *trahead, char city[50], char des[50])
{
    tra *p = NULL;
    sta *pt = NULL;
    int i, j = 0, n;
    char **tnumtmp1 = NULL, **tnumtmp2 = NULL;

    pt = SCcity_trains(trahead, city);
    n = pt->cnt_t;
    tnumtmp1 = pt->tnums;

    if(init_tnums(&tnumtmp2, n) == -1)
        return NULL;

    for(i = 0; i < n; i ++){
        p = SCTraNote_traNum(tnumtmp1[i], trahead);
        for(pt = p->stations; (strcmp(pt->city, city) != 0) && (pt->next != NULL); pt = pt->next);
        for(pt = pt->next; pt != NULL; pt = pt->next)
            if(strcmp(pt->city, des) == 0)
                strcpy(tnumtmp2[j ++], p->traNum);
    }

    if(j > 0)
        return creat_ttotNote(trahead, city, des, j, tnumtmp2);
    return NULL;
}

ttot *creat_ttotNote(tra * trahead, char dep[50], char des[50], int n, char **tnum)
{
    ttot *pointer = NULL;

    if(init_ttotP(&pointer) == -1)
        return NULL;

    strcpy(pointer->departure, dep);
    strcpy(pointer->destination, des);
    pointer->tsta = creat_tstalist(trahead, dep, des, n, tnum);
    pointer->cnt_tnum = n;

    return pointer;
}

tsta *creat_tstalist(tra * trahead, char dep[50], char des[50], int n, char **tnum)
{
    tsta *head = NULL, *a1 = NULL, *a2 = NULL;
    int i;

    for(i = 0; i < n; i++){
        if(init_tstaP(&a1) == -1)
            return NULL;
        a1->leftseat = SCTraN_city_LS(dep, des, SCTraNote_traNum(tnum[i], trahead));
        strcpy(a1->traNum, tnum[i]);

        if(head == NULL)
            head = a2 = a1;
        else{
            a2->next = a1;
            a2 = a1;
        }
    }

    return head;
}

void creat_tnums(tra *trahead, int cnt_tra)
{
    tra *p = NULL;
    sta *pt = NULL;

    for(p = trahead; p != NULL; p = p->next)
        for(pt = p->stations; pt != NULL; pt = pt->next)
            pt->cnt_t = SC_City_traNum(trahead, cnt_tra, pt->city, &pt->tnums);
}

int SC_City_traNum(tra *trahead, int cnt_tra, char city[50], char ***tnum)
{
    char **tnumtmp = NULL;
    int i, n;
    tra *p = NULL;
    sta *pt = NULL;

    if(init_tnums(&tnumtmp, cnt_tra) == -1)
        return -1;

    i = 0;
    for(p = trahead; p != NULL; p = p->next)
        for(pt = p->stations; pt != NULL; pt = pt->next)
            if(strcmp(city, pt->city) == 0){
                strcpy(tnumtmp[i ++], p->traNum);
                break;
            }

    n = i;
    if(init_tnums(tnum, n) == -1)
        return -1;
    for(i = 0; i < n; i ++)
        strcpy((*tnum)[i], tnumtmp[i]);

    return n;
}

int init_ttotP(ttot **p)
{
    if((*p = (ttot *)malloc(sizeof(ttot))) == NULL){
        spacenotenough
        return -1;
    }

    (*p)->tsta = NULL;
    memset((*p)->departure, origin, 50);
    memset((*p)->destination, origin, 50);
    (*p)->cnt_tnum = 0;
    (*p)->next = NULL;

    return 0;
}

int init_tstaP(tsta **p)
{
    if((*p = (tsta *)malloc(sizeof(tsta))) == NULL){
        spacenotenough
        return -1;
    }

    memset((*p)->traNum, origin, 20);
    (*p)->leftseat = 0;
    (*p)->next = NULL;

    return 0;
}

void displayTtoTarr(ttot **head, int n)
{
    int i;

    if(n > 0){
        puts("转乘路径：");
        for(i = 0; i < n ; i ++){
            displayTratoTra(head[i]);
            if(i != n-1)
                printf("======================================\n\n");
        }
    }
    else
        puts("无转乘一次的联程车票！");
}

void displayTratoTra(ttot *head)
{
    printf("%s----->%s----->%s\n\n", head->departure, head->destination, head->next->destination);
    displaytsta(head);
    displaytsta(head->next);
    printf("\n");
}

void displaytsta(ttot *pointer)
{
    tsta *p = NULL;
    tra *trahead = NULL;

    LoadTraInfo('D', &trahead);
    printf("%s---%s段：\n", pointer->departure, pointer->destination);
    puts("车次           余票      单价");
    for(p = pointer->tsta; p != NULL; p = p->next)
        printf("%-15s%-10d%-10.2f\n", p->traNum, p->leftseat, priceindex * distance_count(pointer->departure, pointer->destination, SCTraNote_traNum(pointer->tsta->traNum, trahead)));
}
