CL_FLAGS     = -Wall -coverage -O0
CL          = clang

all: server client

client: client.c
	$(CL) $(CL_FLAGS) -o client client.c -lzmq

server: server.c
	$(CL) $(CL_FLAGS) -o server server.c -lzmq

clean:
	rm -rf server client *.gcno *.gcov *.gcda
