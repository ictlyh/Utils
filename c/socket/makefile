C_FLAGS      = -Wall -coverage -O0
C           = gcc
MAKE        = make

all: server client

client: client.c
	$(C) $(C_FLAGS) -o client client.c

server: server.c
	$(C) $(C_FLAGS) -o server server.c

clean:
	rm -rf server client *.gcno *.gcov *.gcda
