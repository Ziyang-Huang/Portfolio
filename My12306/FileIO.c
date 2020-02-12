#include "head.h"

int LoadTraInfo(char mode, tra **head)
{
    FILE *fp;
    char line[1000];
    int cnt_tra = 0, cnt_sta, i;
    tra *p = NULL;

    if(mode == 'F')
        if((fp = fopen("tra_info.txt","r")) == NULL){
            puts("读取班次信息失败！");
            puts("请将车次信息保存在“tra_info.txt”中，并放入本程序所在文件夹内！");
            clstdin();
            exit(1);
        }
    if(mode == 'D')
        if((fp = fopen("traInfo.txt","r")) == NULL){
            puts("读取班次信息失败！");
            return -1;
        }
    while(1){
        memset(line, origin, 1000);
        if(fgets(line, 1000, fp) == NULL)
            break;

        if(((p = LoadTraNote(mode, line, &cnt_sta)) == NULL) || (cnt_sta == -1) || (cnt_sta == -2)){
            puts("错误信息为（只显示前100个字符）：");
            for(i = 0; i < 100; i++)
                printf("%c", line[i]);
            return -1;
        }
        if(check_trainNote(p) == 1){
            *head = insert_train(*head, p);
            cnt_tra ++;
        }
    }
    if(cnt_tra == 0){
        puts("班次信息为空！");
        *head = NULL;
    }
    fclose(fp);
    return cnt_tra;
}

void saveTraData(tra *head)
{
    FILE *fp;
    tra *p;
    sta *pt;

    if((fp = fopen("traInfo.txt","w")) == NULL){
        puts("保存信息失败！");
        exit(1);
    }

    if(head != NULL){
        p = head;
        while(p != NULL){
            fprintf(fp, "%s, %d", p->traNum, p->maxPass);
            pt = p->stations;
            while(pt != NULL){
                fprintf(fp, "| %s, %d, %d", pt->city, pt->distance, pt->leftSeat);
                pt = pt->next;
            }
            fprintf(fp,"|\n");
            p = p->next;
        }
    }

    fclose(fp);
}

void saveOPTraData(tra *head)
{
    FILE *fp;
    tra *p;
    sta *pt;

    if((fp = fopen("tra_info.txt","w")) == NULL){
        puts("保存信息失败！");
        exit(1);
    }

    if(head != NULL){
        p = head;
        while(p != NULL){
            fprintf(fp, "%s, %d", p->traNum, p->maxPass);
            pt = p->stations;
            while(pt != NULL){
                fprintf(fp, "| %s, %d, %d", pt->city, pt->distance, pt->leftSeat);
                pt = pt->next;
            }
            fprintf(fp,"|\n");
            p = p->next;
        }
    }

    fclose(fp);
}

int loadorders(tic **head)
{
    FILE *fp;
    char line[100];
    int cnt = 0;
    tic *ptr = NULL, *ptr2 = NULL;

    if((fp = fopen("orders.txt","r")) == NULL){
        puts("读取信息失败！");
        return -1;
    }

    while(1){
        memset(line, origin, 100);
        if(fgets(line, 100, fp) == NULL)
            break;
        cnt ++;
        if(init_ticP(&ptr) == -1)
            return -1;
        sscanf(line, "%d%s%s%s%d%f%d", &ptr->OrderNum, ptr->traNum, ptr->departure, ptr->destination, &ptr->ticNum, &ptr->pricepertic, &ptr->connect);
        //*head = insert_order(*head, ptr);
        if(cnt == 1)
            *head = ptr2 = ptr;
        else{
            ptr2->next = ptr;
            ptr2 = ptr;
        }
    }
    fclose(fp);
    if(cnt == 0)
        *head = NULL;

    return cnt;
}

void saveOrderData(tic *head)
{
    FILE *fp;
    tic *p = NULL;

    if((fp = fopen("orders.txt","w")) == NULL){
        puts("保存信息失败！");
        exit(1);
    }

    if(head != NULL){
        p = head;
        while(p != NULL){
            fprintf(fp,"%-5d%-10s%-15s%-15s%-5d%-8.2f%-5d\n", p->OrderNum, p->traNum, p->departure, p->destination, p->ticNum, p->pricepertic, p->connect);
            p = p->next;
        }
    }

    fclose(fp);
}
