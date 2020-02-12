#ifndef LIST_H
#define LIST_H

void DeleteTList(tra **head);
void DeleteTNote(tra **p);
void DeleteOList(tic **head);
tic *insert_order(tic *head, tic *ptr);
tra *insert_train(tra *head, tra *p);
int create_OrderNum(tic *head);

#endif // LIST_H
