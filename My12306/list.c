#include "head.h"

void DeleteTList(tra **head)
{
    tra *p = NULL;
    sta *pt = NULL;

    if(*head == NULL)
        return ;

    while(*head != NULL){
        p = *head;

        while(p->stations != NULL){
            pt = p->stations;
            p->stations = p->stations->next;
            free(pt);
        }

        *head = (*head)->next;
        free(p);
    }
}

void DeleteTNote(tra **p)
{
    sta *pt1;
    pt1 = (*p)->stations;
    while((*p)->stations != NULL){
        (*p)->stations = pt1->next;
        free(pt1);
        pt1 = (*p)->stations;
    }
    free(*p);
}

void DeleteOList(tic **head)
{
    tic *p;

    if(head == NULL)
        return ;

    while(*head != NULL){
        p = *head;
        *head = (*head)->next;
        free(p);
    }
}

tic *insert_order(tic *head, tic *p0)
{
    tic *p1 = NULL, *p2 = NULL;

    if(head == NULL)
        head = p0;

    else{
        p1 = head;
        while((p0->OrderNum > p1->OrderNum) && (p1->next != NULL)){
            p2 = p1;
            p1 = p1->next;
        }
        if(p1 == head){
            if(p0->OrderNum > p1->OrderNum){
                head->next = p0;
                p0->next = NULL;
            }
            else{
                p0->next = head;
                head = p0;
            }
        }
        else if(p0->OrderNum < p1->OrderNum){
            p2->next = p0;
            p0->next = p1;
        }
        else{
            p1->next = p0;
            p0->next = NULL;
        }
    }
    return head;
}

tra *insert_train(tra *head, tra *p)
{
    tra *p1 = NULL, *p2 = NULL;
    if(head == NULL){
        head = p;
        p->next = NULL;
    }
    else{
        p1 = head;
        while((strcmp(p->traNum , p1->traNum) > 0 )&& (p1->next != NULL)){
            p2 = p1;
            p1 = p1->next;
        }
        if(p1 == head){
            if(strcmp(p->traNum , p1->traNum) > 0){
                head->next = p;
                p->next = NULL;
            }
            else{
                p->next = head;
                head = p;
            }
        }
        else if(strcmp(p->traNum , p1->traNum) < 0){
            p->next = p1;
            p2->next = p;
        }
        else{
            p1->next = p;
            p->next = NULL;
        }
    }
    return head;
}

int create_OrderNum(tic *head)
{
    int num = 1, flag = 1;
    tic *p = NULL;
    while(flag){
        if(head == NULL)
            break;
        p = head;
        while(p != NULL){
            if(num == p->OrderNum){
                flag = 1;
                num ++;
                break;
            }
            else{
                p = p->next;
                flag = 0;
            }
        }
    }
    return num;
}

