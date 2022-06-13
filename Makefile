CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi
LDLIBS = # -lform
HEADERS = commands.h

# recompile if this header changes

EXE = library
OBJ =main.o add.o #find.o  del.o print.o dinamicArray.o

all : $(EXE)

%.o: %.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE): $(DEPS) $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDLIBS)

clean :
	rm -f *.o core $(EXE) add main

bf:
	./$(EXE) --bestfit test 

valgrind:
	valgrind --leak-check=full ./$(EXE) first_fit test

best_fit:
	./library best_fit test

first_fit:
	./library first_fit test

worst_fit:
	./library worst_fit test

next_fit:
	./library next_fit test
