CC = gcc
RM = rm

client: client.c
	$(CC) -o client client.c

server: server.c
	$(CC) -o server server.c

all: client.c server.c
	$(CC) -o client client.c	
	$(CC) -o server server.c

clean: server client
	$(RM) server
	$(RM) client