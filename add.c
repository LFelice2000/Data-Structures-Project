#include <stdio.h>
#include <stdlib.h>
#include "add.h"

Book *get_book(char *datafile, long int position, size_t size);
void update_index(Array *index, int *inidpos);

bool add_data(Book book, char *datafile, char *indexfile, indexbook *indexbook, Array *index, Arraydel *delindex, int strategy, char *listfile) {

    int lenght, code,i, pos = 0, space_found = 0, number_of_elements = 0, elements_compared = 0;
    bool search_over = false;
    FILE *file = NULL;

    if (!datafile || !indexfile) {
        return false;
    }
    
    if(delindex->used > 0) {
        file = fopen(datafile, "rb+");
        if (!file) {
            return NULL;
        }

        if(strategy == NEXTFIT && (int)delindex->lastPosition > -1) {
            i = delindex->lastPosition;
            number_of_elements = delindex->used;

            while (search_over == false)
            {
                if(i > ((int)delindex->used - 1)) {
                    i = 0;
                }

                if(elements_compared == number_of_elements) {
                    search_over = true;
                    break;
                }

                if(delindex->array[i].registersize == book.offset) {
                    
                    pos = delindex->array[i].offset;
                    if(i == ((int)delindex->used - 1)) {

                        i = 0;

                    } else {

                        delindex->lastPosition = i;
                    }

                    update_delList(delindex, &i);

                    dump_list(delindex, listfile, strategy);  
                    
                    space_found = 1;

                    search_over = true;
                }
                else {
                     i++;
                    elements_compared++;
                }
            }
            
        }
        else  {
            for(i = 0; i < (int)delindex->used && space_found == 0; i++){
                if(delindex->array[i].registersize == book.offset){
                    /*set pointer to empty space */
                    pos = delindex->array[i].offset;

                    if (strategy == NEXTFIT && (int)delindex->lastPosition == -1) {
                        if(i == ((int)delindex->used - 1)) {

                            delindex->lastPosition = 0;

                        } else {

                            delindex->lastPosition = i;
                        }
                    }

                    update_delList(delindex, &i);
                    if(strategy == BESTFIT) {
                        sort_best_fit(delindex);
                    }
                    else if (strategy == WORSTFIT) {
                        sort_worst_fit(delindex);
                    }
                    
                    dump_list(delindex, listfile, strategy);

                    space_found = 1;
                }
            }
        }

        fclose(file);
    }

    if(space_found == 0) {

        file = fopen(datafile, "a+b");
        if (!file) {
            return NULL;
        }

        /*write values into data file*/
        fwrite((const void *)&book.offset, sizeof(book.offset), 1, file);
        
        code = atoi(book.code);
        fwrite((const void *)&code, sizeof(code), 1, file);

        lenght = strlen(book.isbn);
        fwrite(book.isbn, lenght * sizeof(char), 1, file);

        fwrite(book.title, strlen(book.title), 1, file);

        lenght = strlen(book.printedBy);
        fwrite(book.printedBy, lenght * sizeof(char), 1, file);

        fclose(file);

        file = fopen(datafile, "rb");
        if (!file)
            return NULL;

        /** We get the position of the node */
        fseek(file, 0, SEEK_END);
        pos = ftell(file);
        indexbook->size = book.offset;
        indexbook->offset = abs((book.offset + 8) - pos);
        fseek(file, 0, SEEK_SET);

        fclose(file);

    }
    else {

        file = fopen(datafile, "rb+");
        if (!file) {
            return NULL;
        }

        fseek(file, pos, SEEK_SET);

        /*write values into data file*/
        fwrite((const void *)&book.offset, sizeof(book.offset), 1, file);
        
        code = atoi(book.code);
        fwrite((const void *)&code, sizeof(code), 1, file);

        lenght = strlen(book.isbn);
        fwrite(book.isbn, lenght * sizeof(char), 1, file);

        fwrite(book.title, strlen(book.title), 1, file);

        lenght = strlen(book.printedBy);
        fwrite(book.printedBy, lenght * sizeof(char), 1, file);

        indexbook->size = book.offset;
        indexbook->offset = pos;

        fclose(file);

    }

    /**Then we add the element to the index*/
    insertArray(index, *indexbook);
    sort_index(index);

    return true;
}

void sort_index(Array *index) {
    indexbook tmp;
    size_t i, j;

    for(i = 0; i < index->used; i++) {
        for(j = i + 1; j < index->used; j++) {
            if(index->array[i].key > index->array[j].key) {
                tmp = index->array[i];
                index->array[i] = index->array[j];
                index->array[j] = tmp;
            }
        }
    }

}

void sort_best_fit(Arraydel *index) {
    indexdeletedbook tmp;
    size_t i, j;

    for(i = 0; i < index->used; i++) {
        for(j = i + 1; j < index->used; j++) {
            if(index->array[i].registersize > index->array[j].registersize) {
                tmp = index->array[i];
                index->array[i] = index->array[j];
                index->array[j] = tmp;
            }
        }
    }

}

void sort_worst_fit(Arraydel *index) {
    indexdeletedbook tmp;
    size_t i, j;

    for(i = 0; i < index->used; i++) {
        for(j = i + 1; j < index->used; j++) {
            if(index->array[i].registersize < index->array[j].registersize) {
                tmp = index->array[i];
                index->array[i] = index->array[j];
                index->array[j] = tmp;
            }
        }
    }

}

void dump_index(Array *index, char *indexfile)
{
    FILE *file = NULL;
    size_t i;

    file = fopen(indexfile, "w+b");
    if(!file) return;

    for(i = 0; i < index->used; i++) {
        fwrite((const void*)&index->array[i].key, sizeof(index->array[i].key), 1, file);
        fwrite((const void*)&index->array[i].offset, sizeof(index->array[i].offset), 1, file);
        fwrite((const void*)&index->array[i].size, sizeof(index->array[i].size), 1, file);
    }

    fclose(file);
    
}

void printInd(Array *index)
{
    size_t i;
    
    for(i = 0; i < index->used; i++) {
        printf("Entry #%ld\n", i);
        printf("    key: #%d\n", index->array[i].key);
        printf("    offset: #%ld\n", index->array[i].offset);
        printf("    size: #%ld\n", index->array[i].size);
    }

    return;
}

Book *binary_search(Array *index, int *elem, char *datafile, int *pos)
{   
    bool found = false;
    int lower_bound = 0, upper_bound = 0, mid = 0;
    Book *book = NULL;

    upper_bound = index->used;

    while(found == false) {
        if(lower_bound > upper_bound) {
            break;
        }

        mid = lower_bound + ((upper_bound - lower_bound)/ 2);

        if(index->array[mid].key < *elem) {
            lower_bound = (mid + 1);
        }

        if(index->array[mid].key > *elem) {
            upper_bound = (mid - 1);
        }

        if(index->array[mid].key == *elem) {
            found = true;
            break;
        }
    }

    if(found==true) {
        *pos=mid;
        book = get_book(datafile, index->array[mid].offset, index->array[mid].size);
    }

    return book;
    
}


void initArray(Array *a, size_t initialSize)
{
    /*create initial empty array of size initial Size */
    a->array = (indexbook*)malloc(initialSize*sizeof(indexbook));
    a->array->key = 0;
    a->array->offset = 0;
    a->array->size = 0;

    a->used = 0;
    a->size = initialSize;
}

void insertArray(Array *a, indexbook element)
{
    /* insert item ”element” in array
    a−>used is the number of used entries,
    a−>size is the number of entries */
    if(a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size*sizeof(indexbook));
    }
    a->array[a->used] = element;
    a->used++;
}

void freeArray(Array *a) {

    /* free memory allocatedforarray */
    free(a->array) ;
    a->array = NULL;
    a->used = a->size = 0 ;
}

Book *get_book(char *datafile, long int position, size_t size) {
    FILE *file = NULL;
    char *info = NULL, **fields = NULL, *token = NULL, isbn[17] = "\0";
    int  count, book_info_len, i, id;
    size_t offset;
    Book *book = NULL;

    book_info_len = size - 20;
    
    info = (char*)malloc((book_info_len+1)*sizeof(char));
    if(!info) return NULL;

    fields = (char**)malloc(2*sizeof(char*));
    if(!fields) return NULL;
    fields[0] = NULL;
    fields[1] = NULL;

    file = fopen(datafile, "rb");
    if(!file) return NULL;

    fseek(file, position, SEEK_SET);

    fread(&offset, sizeof(size_t), 1, file);
    fread(&id, sizeof(int), 1, file);
    fread(isbn, 16, 1, file);
    fread(info, book_info_len, 1, file); 

    book = (Book*)malloc(sizeof(Book));
    if(!book) return NULL;

    book->offset = offset;

    book->code = (char*)malloc(7*sizeof(char));
    if(!book->code) return NULL;
    sprintf(book->code, "%d", id);
    
    book->isbn = (char*)malloc((strlen(isbn)+1)*sizeof(char));
    if(!book->isbn) return NULL;
    isbn[17] = '\0';
    strcpy(book->isbn, isbn);
    
    token = strtok(info, "|");
    count = 0;
    while (token != NULL)
    {
        fields[count] = (char*)malloc((strlen(token)+1)*sizeof(char));
        if(!fields[count]) return NULL;
        strcpy(fields[count], token);
        count++;

        token = strtok(NULL, "\0");
    }
    
    book->title = (char*)malloc((strlen(fields[0])+1)*sizeof(char));
    if(!book->title) return NULL;
    strcpy(book->title, fields[0]);

    book->printedBy = (char*)malloc((strlen(fields[1])+1)*sizeof(char));
    if(!book->printedBy) return NULL;
    strcpy(book->printedBy, fields[1]);

    for(i = 0; i < (count - 1); i++) {
        free(fields[i]);
    }
    free(fields);
    free(token);

    return book;
}

/*reload index*/
void reload_index(FILE *indexfile, Array *index) {
    int key;
    long int offset;
    size_t size;
    indexbook entry;

    while (fread(&key, 4, 1, indexfile) != 0)
    {
        entry.key = key;

        fread(&offset, 8, 1, indexfile);
        entry.offset = offset;
        
        fread(&size, 8, 1, indexfile);
        entry.size = size;

        insertArray(index, entry);
    }

}

/*reload list*/
void reload_deleted_nodes_list(FILE *deleted_nodes_file, Arraydel *delindex, int strategy) {
    int old_strategy = 3;
    size_t offset = 0, size = 0;
    indexdeletedbook indexdelbook;

    fread(&old_strategy, 4, 1, deleted_nodes_file);

    while (fread(&offset, sizeof(size_t), 1, deleted_nodes_file) != 0)
    {
        fread(&size, sizeof(size_t), 1, deleted_nodes_file);
        
        /*assign values*/
        indexdelbook.offset= offset;
        indexdelbook.registersize= size;

        delindex->array[delindex->used]= indexdelbook;
        (delindex->used)++;
    }

    if(old_strategy != strategy) {
        fseek(deleted_nodes_file, 0, SEEK_SET);
        fwrite((const void*)&strategy, 4, 1, deleted_nodes_file);

        if(strategy == BESTFIT) {
            sort_best_fit(delindex);
        }
        else if (strategy == WORSTFIT) {
            sort_worst_fit(delindex);
        }
    }

    return;
}

void initlist(Arraydel *Ad, size_t initial_size){

    Ad->array=(indexdeletedbook *)malloc(initial_size *(sizeof(indexdeletedbook)));
    Ad->array->key = 0;
    Ad->array->offset = 0;
    Ad->array->registersize = 0;

    Ad->size=initial_size;
    Ad->used=0;
    Ad->lastPosition = -1;
}

/* -1: ERROR || 0: NOT found || 1: found and deleted*/
int delete_data(Arraydel *arraydel,Array *index, int *bookid,char *datafile, int delete_strategy){

    indexdeletedbook indexdelbook;
    int inidpos=-1, del = -1;
    FILE *db = NULL;

    if(!arraydel||!index) return -1;

    /*preform binary search on index*/
    binary_search(index,bookid,datafile,&inidpos);
    if(inidpos==-1) return 0;

    /*assign values*/
    indexdelbook.offset=index->array[inidpos].offset;
    indexdelbook.registersize=index->array[inidpos].size;
    indexdelbook.key=index->array[inidpos].key;

    /**We rewrite the datafile */
    db = fopen(datafile, "rb+");

    fseek(db, indexdelbook.offset + 6, SEEK_CUR);

    fwrite((const char *)&del, sizeof(int), 1, db);

    fclose(db);

    arraydel->array[arraydel->used]= indexdelbook;
    (arraydel->used)++;

    if(delete_strategy == BESTFIT) {
        sort_best_fit(arraydel);
    }
    else if (delete_strategy == WORSTFIT) {
        sort_worst_fit(arraydel);
    }

    /*update the index*/
    update_index(index,&inidpos);
    sort_index(index);

    return 1;

}

void update_index(Array *index, int *inidpos){
    
    int i,used;
    used= (int) index->used;

    for(i=(*inidpos);i<used-1;i++){
        index->array[i]=index->array[i+1];
    }

    (index->used)--;

    return;

}

void update_delList(Arraydel *index, int *inidpos) {
    
    int i,used;
    used= (int) index->used;

    if(index->used > 1) {
        for(i=(*inidpos);i<used-1;i++){
            index->array[i]=index->array[i+1];
        }
    }

    (index->used)--;

    return;

}

void dump_list(Arraydel *arraydel, char *listfile, int delete_strategy)
{
    FILE *file = NULL;
    size_t i;

    file = fopen(listfile, "w+b");
    if(!file) return;

    fwrite((const void*)&(delete_strategy),sizeof(delete_strategy),1,file);

    for(i = 0; i < arraydel->used; i++) {
        fwrite((const void*)&(arraydel->array[i].offset),sizeof(arraydel->array[i].offset),1,file);
        fwrite((const void*)&(arraydel->array[i].registersize),sizeof(arraydel->array[i].registersize),1,file);
    }

    fclose(file);
    
}

void freelist(Arraydel *arraydel){
    free(arraydel->array);
    arraydel->array=NULL;
    arraydel->used=arraydel->size=0;
}

void printLst(Arraydel *arraydel)
{
    size_t i;
    
    for(i = 0; i < arraydel->used; i++) {
        printf("Entry #%ld\n", i);
        printf("    offset: #%ld\n", arraydel->array[i].offset);
        printf("    size: #%ld\n", arraydel->array[i].registersize);
    }
    return;
}

void free_book(Book *book) {

    free(book->code);
    free(book->isbn);
    free(book->printedBy);
    free(book->title);
    free(book);
    book = NULL;
}

void printRec(Array *indexr,char *datafile){
    Book *rec = NULL;
    int i,used;

    if(indexr==NULL) return;
    
    used = (int)indexr->used;

    for(i=0;i<used;i++){
        rec = get_book(datafile,indexr->array[i].offset,indexr->array[i].size);
        printf("%s|%s|%s|%s\n", rec->code, rec->isbn, rec->title, rec->printedBy);
        free(rec);
        rec = NULL;
    }

    return;

}
