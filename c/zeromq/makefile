CL_FLAGS     = -Wall -coverage -O0
CL          = clang

all: server client

client: client.c
	$(CL) $(CL_FLAGS) -o client client.c -lzmq -L$(TRAVIS_BUILD_DIR)/zeromq/lib -I$(TRAVIS_BUILD_DIR)/zeromq/include

server: server.c
	$(CL) $(CL_FLAGS) -o server server.c -lzmq -L$(TRAVIS_BUILD_DIR)/zeromq/lib -I$(TRAVIS_BUILD_DIR)/zeromq/include

clean:
	rm -rf server client *.gcno *.gcov *.gcda
