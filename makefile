simpleshell: simpleshell.o
	gcc simpleshell.o -o simpleshell

simpleshell.o: simpleshell.c
	gcc -c simpleshell.c -Wall

clean_csv:
	rm *.csv

clean:
	rm *.o simpleshell a.out