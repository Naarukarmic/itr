CC = gcc
FLAGS = -Wall
RM = rm

client: client.c
	$(CC) $(FLAGS) -o client client.c

server: server.c
	$(CC) $(FLAGS) -o server server.c

all: client.c server.c
	$(CC) $(FLAGS) -o client client.c	
	$(CC) $(FLAGS) -o server server.c

clean: server client
	$(RM) server
	$(RM) client