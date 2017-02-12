CPPFLAGS = -O3 -Wall -Werror
CC = g++

PROGS = main
default: $(PROGS)

main: main.cpp
	$(CC) $(CPPFLAGS) -o main main.cpp

clean:
	rm -f main
