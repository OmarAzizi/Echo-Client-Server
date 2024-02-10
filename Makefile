CC = gcc

server: common.o
	$(CC) -c -g server.c -o server.o
	$(CC) -g common.o server.o -o server

client: common.o
	$(CC) -c -g client.c -o client.o
	$(CC) -g common.o client.o -o client

common.o:
	$(CC) -c -g common.c -o common.o

clean:
	rm -rf *.o
