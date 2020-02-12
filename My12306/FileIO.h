#ifndef FILEIO_H
#define FILEIO_H

int LoadTraInfo(char mode, tra **trains);
void saveTraData(tra *head);
void saveOPTraData(tra *head);
int loadorders(tic **head);
void saveOrderData(tic *head);

#endif // FILEIO_H
