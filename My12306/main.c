#include "head.h"

int main(void)
{
    //system("color f9");
    TraInfoLF();
    MainMenu(0);
    return 0;
}

void clstdin(void)
{
    char c = '\0';
    while (c != '\n')
        c = getchar();
}

int init_traP(tra **p)
{
    if ((*p = (tra *)malloc(sizeof(tra))) == NULL)
    {
        spacenotenough return -1;
    }
    //    memset((*p)->date, origin, 20);
    //    memset((*p)->time, origin, 20);
    memset((*p)->traNum, origin, 20);
    (*p)->maxPass = 0;
    (*p)->LS = 0;
    (*p)->stations = NULL;
    (*p)->next = NULL;

    return 0;
}

int init_staP(sta **p)
{
    if ((*p = (sta *)malloc(sizeof(sta))) == NULL)
    {
        spacenotenough return -1;
    }

    memset((*p)->city, origin, 50);
    (*p)->distance = 0;
    (*p)->cnt_t = 0;
    (*p)->tnums = NULL;
    (*p)->leftSeat = 0;
    (*p)->next = NULL;

    return 0;
}

int init_ticP(tic **p)
{
    if ((*p = (tic *)malloc(sizeof(tic))) == NULL)
    {
        spacenotenough return -1;
    }

    memset((*p)->traNum, origin, 20);
    memset((*p)->departure, origin, 50);
    memset((*p)->destination, origin, 50);
    (*p)->pricepertic = 0;
    (*p)->ticNum = 0;
    (*p)->OrderNum = 0;
    (*p)->connect = 0;
    (*p)->next = NULL;

    return 0;
}

int init_tnums(char ***arr, int n)
{
    int i;
    *arr = (char **)malloc(n * sizeof(char *));
    for (i = 0; i < n; i++)
    {
        if (((*arr)[i] = (char *)malloc(20 * sizeof(char))) == NULL)
        {
            spacenotenough return -1;
        }
        else
            memset((*arr)[i], origin, 20);
    }
    return 0;
}
