CPPFLAGS = -O3 -Wall -Werror
CC = g++

PROGS = main
default: $(PROGS)

main: main.o PBIL.o GA.o
	$(CC) $(CPPFLAGS) -o $@ main.o PBIL.o GA.o

main.o: main.cpp PBIL.h
	$(CC) -c main.cpp -o $@

PBIL.o: PBIL.cpp PBIL.h
	$(CC) -c PBIL.cpp -o $@

GA.o: GA.cpp GA.h
	$(CC) -c GA.cpp -o $@

clean:
	rm *.o
	rm -f main
