CC = gcc
FLAGS = -g -Wall -std=c90
all: ru_na_pandemia

ru_na_pandemia: ru_na_pandemia.o libpilha.o libfila.o liblista.o
	$(CC) $(FLAGS) ru_na_pandemia.o libpilha.o libfila.o liblista.o -o ru_na_pandemia
ru_na_pandemia.o: ru_na_pandemia.c libpilha.h libfila.h liblista.h
	$(CC) $(FLAGS) -c ru_na_pandemia.c
libpilha.o: libpilha.c libpilha.h
	$(CC) $(FLAGS) -c libpilha.c
libfila.o: libfila.c libfila.h
	$(CC) $(FLAGS) -c libfila.c
liblista.o: liblista.c liblista.h
	$(CC) $(FLAGS) -c liblista.c
clean:
	rm *.o ru_na_pandemia
