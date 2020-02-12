#ifndef RECOMMEND_H
#define RECOMMEND_H

ttot **recommend(tra *trahead, int cnt_tra, char dep[50], char des[50], int *cnt);
void creat_tnums(tra *trahead, int cnt_tra);
int SC_City_traNum(tra *trahead, int cnt_tra, char city[50], char ***tnum);
ttot *check_citytodes(tra *trahead, char city[50], char des[50]);
ttot *creat_ttotNote(tra * trahead, char dep[50], char des[50], int n, char **tnum);
tsta *creat_tstalist(tra * trahead, char dep[50], char des[50], int n, char **tnum);
int init_ttotP(ttot **p);
int init_tstaP(tsta **p);
void displayTtoTarr(ttot **head, int n);
void displayTratoTra(ttot *head);
void displaytsta(ttot *pointer);

#endif // RECOMMEND_H
