run: main.o union-find.o hash-table.o
	c99 -Wall -O3 main.o union-find.o hash-table.o -o run

main.o: main.c
	c99 -c -O3 main.c

union-find.o: union-find.c union-find.h
	c99 -c -O3 union-find.c

hash-table.o: hash-table.c hash-table.h
	c99 -c -O3 hash-table.c

clean:
	rm *.o run
