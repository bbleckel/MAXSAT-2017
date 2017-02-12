CPPFLAGS = -O3 -Wall -Werror
CC = g++

PROGS = main
default: $(PROGS)

main: main.o PBIL.o
	$(CC) $(CPPFLAGS) -o $@ main.o PBIL.o

main.o: main.cpp PBIL.h
	$(CC) -c main.cpp -o $@

PBIL.o: PBIL.cpp PBIL.h
	$(CC) -c PBIL.cpp -o $@

clean:
	rm *.o
	rm -f main
