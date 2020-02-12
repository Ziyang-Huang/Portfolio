#ifndef FILEIO_H
#define FILEIO_H

void loadmat_origin(void);
void insertmat(mat *p);
void deletemat(char name[]);
void deletematlist(void);
void savemat(void);
void loadconfig(void);

#endif //FILEIO_H
