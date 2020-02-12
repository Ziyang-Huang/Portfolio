#include "head.h"

tra *LoadTraNote(char mode, char line[1000], int *cnt_sta)
{
    char tmp[1000];
    int i;
    tra *p = NULL;

    if (init_traP(&p) == -1)
    {
        spacenotenough return NULL;
    }

    for (i = 0; i < 10; i++)
        if (line[i] == ',')
            line[i] = ' ';

    if (sscanf(line, "%s%d", p->traNum, &p->maxPass) != 2)
    {
        formatwrong
            *cnt_sta = -2;
    }
    else
    {
        memset(tmp, origin, 1000);
        std_station(line, tmp);
        *cnt_sta = LoadStaInfo(mode, tmp, &p->stations, p->maxPass);
    }

    return p;
}

int LoadStaInfo(char mode, char line[1000], sta **p, int LSeat)
{
    int i, j, k, a = 0, cnt = 0, flag = 1;
    sta *p1, *p2;
    char tmp1[10], tmp2[10];

    if (init_staP(&p1) == -1)
        return -1;
    *p = p2 = p1;

    while (1)
    {
        if (mode == 'D')
        {
            memset(tmp1, origin, 10);
            memset(tmp2, origin, 10);
            if (sscanf(line, "%s%s%s", p1->city, tmp2, tmp1) != 3)
            {
                formatwrong return -2;
            }
            for (j = 0, a = 0; tmp1[j] != origin; j++)
                a = a * 10 + (tmp1[j] - '0');
            p1->leftSeat = a;
        }

        if (mode == 'F')
        {
            memset(tmp2, origin, 10);
            if ((flag = sscanf(line, "%s%s", p1->city, tmp2)) == 0)
            {
                formatwrong return -2;
            }
            p1->leftSeat = LSeat;
        }

        for (j = 0, a = 0; tmp2[j] != origin; j++)
            a = a * 10 + (tmp2[j] - '0');
        p1->distance = a;

        std_city(p1->city);
        cnt++;
        for (i = 0; p1->city[i] != origin; i++)
            ;
        for (j = 0; tmp1[j] != origin; j++)
            ;
        for (k = 0; tmp2[k] != origin; k++)
            ;
        if (mode == 'D')
            i += (j + k + 2);
        if (mode == 'F')
            i += (k + 1);
        for (j = 0; j < 999 - i; j++)
            line[j] = line[j + i + 1];

        if (line[0] == origin)
            break;

        if (init_staP(&p1) == -1)
            return -1;
        p2->next = p1;
        p2 = p1;
    }
    p1->leftSeat = 0;
    p1->next = NULL;

    return cnt;
}

void std_station(char source[1000], char result[1000])
{
    int i = 0, j = 0;

    while (source[i] != '|')
        i++;

    for (j = 0, i += 1; i < 1000; i++)
    {
        if (source[i] == '|' || source[i] == ',')
            result[j++] = ' ';
        if ((source[i] >= 'a' && source[i] <= 'z') || (source[i] >= 'A' && source[i] <= 'Z') || (source[i] >= '0' && source[i] <= '9'))
            result[j++] = source[i];
    }
}

int sellT_order(tra *p1, tic **head, int ticnum, char dep[50], char des[50])
{
    tic *ptr = NULL, *tmp = NULL;

    if (init_ticP(&ptr) == -1)
    {
        return -1;
    }
    strcpy(ptr->departure, dep);
    strcpy(ptr->destination, des);
    strcpy(ptr->traNum, p1->traNum);
    ptr->ticNum = ticnum;
    ptr->OrderNum = create_OrderNum(*head);
    ptr->pricepertic = priceindex * distance_count(dep, des, p1);

    tmp = CutTraLS_ONote(ptr);
    if (tmp == NULL)
        return -1;
    if (tmp != NULL)
    {
        if (tmp->OrderNum == -1)
            puts("订单票数信息与车次余座信息不符！");
    }
    *head = insert_order(*head, ptr);
    //    if(*head == NULL)
    //        *head = ptr;
    //    else{
    //        for(tmp = *head; tmp->next != NULL; tmp = tmp->next);
    //        tmp->next = ptr;
    //    }
    //    *head = sort_orders_ONum(*head);

    return ptr->OrderNum;
}

tra *Delete_TNote(tra *head, char tar[20])
{
    tra *p1 = NULL, *p2 = NULL;
    p1 = head;
    while (strcmp(p1->traNum, tar) != 0 && p1->next != NULL)
    {
        p2 = p1;
        p1 = p2->next;
    }
    if (strcmp(p1->traNum, tar) == 0)
    {
        if (p1 == head)
            head = p1->next;
        else
            p2->next = p1->next;
        DeleteTNote(&p1);
        printf("%s的班次信息已删除！\n", tar);
    }
    else
        printf("未找到%s的班次信息！\n", tar);
    return head;
}

int Delete_ONoteOP(tic **head, int tar)
{
    tic *p1 = NULL, *p2 = NULL, *tmp = NULL;
    p2 = p1 = *head;
    int tar_Corder = 0;

    while (p1->OrderNum != tar && p1->next != NULL)
    {
        p2 = p1;
        p1 = p2->next;
    }
    if (p1->OrderNum == tar)
    {
        if (p1->connect != 0)
            tar_Corder = p1->connect;
        tmp = AddTraLS_ONote(p1);
        if (tmp == NULL)
            return -1;
        if (tmp->ticNum == 0)
        {
            if (p1 == *head)
                *head = p1->next;
            else
                p2->next = p1->next;
            free(p1);
            printf_orderNum2(tar);
            puts("的订单信息已删除！");
            return 1;
        }
        if (tmp->ticNum == -1)
            return -2;
        if (tar_Corder != 0)
        {
            if (Delete_ONoteOP(head, tar_Corder) != 1)
                printf("%d的联程票订单2（%d）删除失败", tar, tar_Corder);
            else
                printf("%d的联程票订单2（%d）删除成功", tar, tar_Corder);
        }
    }
    return 0;
}

int distance_count(char dep_city[50], char des_city[50], tra *p)
{
    sta *dep = NULL, *des = NULL, *pt = NULL;
    int distance = 0;

    SCdep_des(p, &dep, &des, dep_city, des_city);
    for (pt = dep->next; pt != des->next; pt = pt->next)
        distance += pt->distance;

    return distance;
}

void std_city(char city[50])
{
    int i;

    if (city[0] >= 'a' && city[0] <= 'z')
        city[0] -= 32;
    for (i = 1; city[i] != origin; i++)
        if (city[i] >= 'A' && city[i] <= 'Z')
            city[i] += 32;
}
