
CC = gcc

all: client server
	gcc server.c -o server
	gcc client.c -o client

server: server.c 
	$(CC) $(CFLAGS) server.c -o server 
	./server 7890

client: client.c
	$(CC) $(CFLAGS) client.c -o client
	./client 127.0.0.1 7890
clean:
	rm *.o all
