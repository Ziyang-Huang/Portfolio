#ifndef SEARCH_SORT_H
#define SEARCH_SORT_H

tra *SCTraNote_traNum(char tar[20], tra *head);
sta *SCcity_trains(tra *trahead, char tar[50]);
int SCTraNum_inOrders(char tar[20]);
int SCTraN_city_LS(char dep_city[50], char des_city[50], tra* p);
int selectTra_city(tra *source, tra **result, char dep[50], char des[50]);
int SCdep_des(tra *p, sta **dep, sta **des, char dep_city[50], char des_city[50]);
tic *SCOrderN_ONum(int tar, tic *head);
tic *sort_orders_ONum(tic *head);
tra *sort_trains_TNum(tra *head);

#endif // SEARCH
