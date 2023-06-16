.SSP:all
all: server client

server:1.server.c
	gcc -o $@ $^

client:2.client.c
	gcc -o $@ $^

.SSP:clean
clean:
	rm -f server client
