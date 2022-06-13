#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "add.h"

#define MAX_INPUT 200
#define MAX_FIELDS 5
#define INT_OFFSET 4
#define ISBN_OFFSET 16
#define HEADER_OFFSET 8

char *create_datafile(char *filename);
char *create_indexfile(char *filename);
char *create_listfile(char *filename);
char *create_nextRecordFile(char *filename);
void register_las_position(int pos, char *filename);
int get_last_nextfit_position(FILE* filename);

int main(int argc, char *argv[]) {
    char input[MAX_INPUT], *operation = NULL, **fields = NULL;
    char *datafile = NULL, *indexfilename = NULL, *listfile =NULL, *nextRecordFileName = NULL;
    int counter, delete_strategy = 0;
    int book_key, field, key;
    bool exit = false, deleted_nodes = false;
    FILE *indexfile = NULL, *deleted_nodes_file = NULL, *nextRecordFile = NULL;
    Book book, *aux_book = NULL;
    indexbook indexb;
    Array index;
    Arraydel delindex;
    int ret;
    int pos=0;

    if(argc != 3) {
        printf("Missing argument");
        return 0;
    }

    if(!(strcmp(argv[1], "first_fit") == 0 || strcmp(argv[1], "worst_fit") == 0 || strcmp(argv[1], "best_fit") == 0 || strcmp(argv[1], "next_fit") == 0)) {
        printf("Unknown search strategy %s", argv[1]);
        return 0;
    }

    /**allocte search startegy**/
    if(strcmp(argv[1], "first_fit") == 0) {
        delete_strategy = FIRSTFIT;
    }
    else if(strcmp(argv[1], "worst_fit") == 0) {
        delete_strategy = WORSTFIT;
    }
    else if(strcmp(argv[1], "best_fit") == 0) {
        delete_strategy = BESTFIT;
    }
    else {
        delete_strategy = NEXTFIT;
    }

    fields = (char**)malloc(MAX_FIELDS*sizeof(char*));

    /**We create the files */
    datafile = create_datafile(argv[2]);
    indexfilename = create_indexfile(argv[2]);
    listfile = create_listfile(argv[2]);
    nextRecordFileName = create_nextRecordFile(argv[2]);
    
    index.array = NULL;
    initArray(&index, 10);

    indexfile = fopen(indexfilename, "r");
    if(indexfile != NULL) {
        reload_index(indexfile, &index);
    }

    /*reload deleted records file*/
    deleted_nodes_file = fopen(listfile, "r");
    if(deleted_nodes_file) {
        initlist(&delindex, 10);
        reload_deleted_nodes_list(deleted_nodes_file, &delindex, delete_strategy);
        fclose(deleted_nodes_file);
    }else{
        delindex.array = NULL;
        initlist(&delindex, 10);
    }

    nextRecordFile = fopen(nextRecordFileName, "r");
    if(nextRecordFile != NULL) {
        delindex.lastPosition = get_last_nextfit_position(nextRecordFile);
        fclose(nextRecordFile);
    }

    /*loop*/
    printf("Type command and argument/s.\n");
    while(!exit) {
        printf("exit add del printRec printLst printInd\n");
    
        fgets(input, MAX_INPUT, stdin);
         if(strcmp(input, "exit\n") == 0) {
            exit = true;
        }

        operation = strtok(input, " ");
        if(operation == NULL) {
            operation = strtok(input, "\n");
        }
        if(!exit) {
            counter = 0;
            while(operation != NULL) {
                fields[counter] = operation;
                counter++;
                operation = strtok(NULL, "|");
            }
            
            if (strcmp(fields[0], "add") == 0) {
                
                key = atoi(fields[1]);
                aux_book = binary_search(&index, &key, datafile, &pos);
                if (aux_book) {
                    printf("\033[0;31mBook already in the database\033[0m\n");
                    free_book(aux_book);
                } 
                else {
                    book.offset = 0;

                    book.code = (char*)malloc((strlen(fields[1])+1)*sizeof(char));
                    strcpy(book.code, fields[1]);

                    /**add key to index sstruct */
                    book_key = atoi(fields[1]);
                    indexb.key = book_key;

                    book.isbn = (char*)malloc((strlen(fields[2])+1)*sizeof(char));
                    strcpy(book.isbn, fields[2]);
                    
                    book.title = (char*)malloc((strlen(fields[3])+3)*sizeof(char));
                    strcpy(book.title, fields[3]);
                    strcat(book.title, "|");
                    
                    book.printedBy = (char*)malloc((strlen(fields[4])+1)*sizeof(char));
                    strcpy(book.printedBy, fields[4]);
                    book.printedBy[strlen(book.printedBy) - 1] = '\0';
                
                    book.offset = INT_OFFSET + ISBN_OFFSET + strlen(book.title) + strlen(book.printedBy); 
                    
                    /**Agregar libro*/
                    add_data(book, datafile,indexfilename, &indexb, &index, &delindex, delete_strategy, listfile);

                    /*If success on add*/
                    printf("\033[0;32mRecord with BookID=%s has been added to the database\033[0m\n",book.code);
                    free(book.code);
                    free(book.isbn);
                    free(book.printedBy);
                    free(book.title);
                }
            }

            else if(strcmp(fields[0],"printInd\n") == 0) {
                printInd(&index);
            }

            else if(strcmp(fields[0],"find") == 0) {
                Book *bs_book = NULL;
                field = atoi(fields[1]);
                bs_book = binary_search(&index, &field, datafile,&pos);
                if(bs_book) {
                    printf("%s|%s|%s|%s\n", bs_book->code, bs_book->isbn, bs_book->title, bs_book->printedBy);
                }
                else
                {
                    printf("\033[0;31Item with key %s does not exist\033[0m\n", fields[1]);
                }
                free(bs_book); 
            }

            else if (strcmp(fields[0],"del")==0){
                field= atoi(fields[1]);
                pos = -1;

                ret=delete_data(&delindex,&index,&field,datafile, delete_strategy);
                if(ret==1){
                    printf("\033[0;32Record with BookID=%d has been deleted\033[0m\n",field);
                    deleted_nodes = true;
                      
                }else if(ret==0){
                    printf("Item with key %d does not exist\n", field);
                }
                
            }
            else if(strcmp(fields[0],"printLst\n")==0){
                printLst(&delindex);
            }
            else if (strcmp(fields[0],"printRec\n")==0){
                printRec(&index,datafile);
            }

        }
    }

    dump_index(&index, indexfilename);

    if(delindex.lastPosition > 0) {
        register_las_position(delindex.lastPosition, nextRecordFileName);
    }
    
    if(deleted_nodes) {
        dump_list(&delindex,listfile, delete_strategy);
        freelist(&delindex);
    }

    freeArray(&index);
    free(fields);
    free(datafile);
    free(indexfile);
    free(nextRecordFileName);

    printf("all done\n");

    return 1;
}


char *create_datafile(char *filename) {
    char *datafile = NULL;

    datafile = (char*)malloc((strlen(filename) + 4)*sizeof(char));
    if(!datafile) return NULL;

    sprintf(datafile, "%s.db", filename);

    return datafile;
}

char *create_indexfile(char *filename) {
    char *datafile = NULL;

    datafile = (char*)malloc((strlen(filename) + 5)*sizeof(char));
    if(!datafile) return NULL;

    sprintf(datafile, "%s.ind", filename);

    return datafile;
}

char *create_listfile(char *filename){
    char *datafile = NULL;

    datafile = (char*)malloc((strlen(filename) + 5)*sizeof(char));
    if(!datafile) return NULL;

    sprintf(datafile, "%s.lst", filename);

    return datafile;
}

char *create_nextRecordFile(char *filename) {
    char *datafile = NULL;

    datafile = (char*)malloc((strlen(filename) + 5)*sizeof(char));
    if(! datafile) {
        return NULL;
    }

    sprintf(datafile, "%s.info", filename);

    return datafile;
}

void register_las_position(int pos, char *filename) {
    FILE *posFile = NULL;

    posFile = fopen(filename, "w");

    fwrite((const void *)&pos, sizeof(pos), 1, posFile);
    
    fclose(posFile);
}
 
int get_last_nextfit_position(FILE* filename) {
    int pos;

    fseek(filename, 0, SEEK_SET);

    fread(&pos, sizeof(int), 1, filename);

    return pos;
}