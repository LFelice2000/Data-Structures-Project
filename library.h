#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>

typedef struct Book
{
    char *code;
    char *isbn;
    char *title;
    char *printedBy;
} Book;

typedef struct {
int *key ; /*book id */
long int offset; /*book is stored in disk at this position */
size_t size; 
}indexbook;

typedef struct {
size_t registersize; /*size of the deleted register*/
size_t offset; /*Record’s offset in file */
}indexdeletedbook ;

/***Functions***/
bool add_data(Book book,const char * datafile, const char * indexfile );
