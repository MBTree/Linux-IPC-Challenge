DEBUG=-g3 -O0 -z execstack

all: server client
	@echo done
	
server: server.cpp
	g++ -Wall $(DEBUG) -o server server.cpp

client: client.cpp
	g++ -Wall $(DEBUG) -o client client.cpp

clean:
	rm -f server client *.txt

test: server client
	./test.sh
