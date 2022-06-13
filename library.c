#include "library.h"

bool add_data(Book book,const char * datafile, const char * indexfile ){

    FILE *f=NULL;
    int lenght;
    int code;

    if(!datafile||!indexfile) return false;

    /*if book (key) exists, don't insert*/

    /*open file*/
    f=fopen(datafile,"a+b");
    if(!f) return false;

    /*write values into data file*/
    code=atoi(book.code);
    fwrite((const void *)&code,sizeof(int),1,f);
    lenght=strlen(book.isbn);
    fwrite(book.isbn, lenght*sizeof(char),1,f);
    lenght=strlen(book.title);
    fwrite(book.title,lenght*sizeof(char),1,f);
    fwrite("|",sizeof(char),1,f);
    lenght=strlen(book.printedBy);
    fwrite(book.printedBy,lenght*sizeof(char),1,f);

    fclose(f);

    return true;

}

