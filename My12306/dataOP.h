#ifndef DATAOP_H
#define DATAOP_H

tra *LoadTraNote(char mode, char line[1000], int *cnt_sta);
int LoadStaInfo(char mode, char line[1000], sta **p, int LSeat);
void std_station(char source[1000], char result[1000]);
int sellT_order(tra *p1, tic **orders, int ticnum, char dep[50], char des[50]);
tra *Delete_TNote(tra *head, char tar[20]);
int Delete_ONoteOP(tic **p, int tar);
int distance_count(char dep_city[50], char des_city[50], tra* p);
void std_city(char city[50]);

#endif // DATAOP_H
