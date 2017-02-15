CPPFLAGS = -O3 -Wall -Werror
CC = g++

PROGS = main
default: $(PROGS)

<<<<<<< HEAD
main: main.o PBIL.o GA.o
	$(CC) $(CPPFLAGS) -o $@ main.o PBIL.o GA.o
=======
main: main.o MAXSAT.o
	$(CC) $(CPPFLAGS) -o $@ main.o MAXSAT.o
>>>>>>> 084618d87f3e490d3e7df641c1030e8268514ba4

main.o: main.cpp MAXSAT.h
	$(CC) -c main.cpp -o $@

MAXSAT.o: MAXSAT.cpp MAXSAT.h
	$(CC) -c MAXSAT.cpp -o $@

GA.o: GA.cpp GA.h
	$(CC) -c GA.cpp -o $@

clean:
	rm *.o
	rm -f main
