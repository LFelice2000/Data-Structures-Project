#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BESTFIT 0
#define WORSTFIT 1
#define FIRSTFIT 2
#define NEXTFIT 3
#define FIRSTFITNOTEXECUTED 0
#define FIRSTFITEXECUTED 1

typedef struct Book
{
    size_t offset;
    char *code;
    char *isbn;
    char *title;
    char *printedBy;
} Book;

/**index**/
typedef struct {
    int key; /** book isbn */
    long int offset; /** book is stored in disk at this postion */
    size_t size; /** book recordsize. This is a redundant field that helps in the implemen ta tion */
} indexbook;

typedef struct {
    indexbook *array;
    size_t used;
    size_t size;
} Array;

/***delete**/
typedef struct {
    int key;  /*book id*/
    size_t registersize; /*size of the deleted register*/
    size_t offset; /*Record’s offset in file */
}indexdeletedbook ;

typedef struct {
    indexdeletedbook *array;
    size_t used;
    size_t size;
    int lastPosition;
} Arraydel;

/***Functions***/
bool add_data(Book book, char *datafile, char *indexfile, indexbook *indexbook, Array *index, Arraydel *delindex, int strategy, char *listfile);
void printInd(Array *index);
Book *binary_search(Array *index, int *elem, char *datafile, int *pos);
void initArray(Array *a, size_t initialSize);
void insertArray(Array *a, indexbook element);
void freeArray(Array *a);
void sort_index(Array *index);
void dump_index(Array *index, char *indexfile);
void reload_index(FILE *indexfile, Array *index);
void free_book(Book *book);

void initlist(Arraydel *Ad, size_t initial_size);
int delete_data(Arraydel *arraydel,Array *index, int *bookid,char *datafile, int delete_strategy);
void dump_list(Arraydel *arraydel, char *listfile, int delete_strategy);
void freelist(Arraydel *arraydel);
void printLst(Arraydel *arraydel);
void printRec(Array *indexr,char *datafile);
void sort_best_fit(Arraydel *index);
void sort_worst_fit(Arraydel *index);
void reload_deleted_nodes_list(FILE *deleted_nodes_file, Arraydel *delindex, int strategy);
void update_delList(Arraydel *index, int *inidpos);