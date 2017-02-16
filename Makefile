CPPFLAGS = -O3 -Wall -Werror
CC = g++

PROGS = main
default: $(PROGS)

<<<<<<< Updated upstream
main: main.o MAXSAT.o
	$(CC) $(CPPFLAGS) -o $@ main.o MAXSAT.o
=======
main: main.o MAXSAT.o GA.o
	$(CC) $(CPPFLAGS) -o $@ main.o MAXSAT.o GA.o
>>>>>>> Stashed changes

main.o: main.cpp MAXSAT.h
	$(CC) -c main.cpp -o $@

MAXSAT.o: MAXSAT.cpp MAXSAT.h
	$(CC) -c MAXSAT.cpp -o $@

GA.o: GA.cpp GA.h
	$(CC) -c GA.cpp -o $@

clean:
	rm *.o
	rm -f main
