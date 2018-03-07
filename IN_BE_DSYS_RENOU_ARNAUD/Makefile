CC = clang
CFLAGS = -Wall
CLIBS =

all: libisocket.o client.o server.o
	${CC} client.o libisocket.o ${CLIBS} -o client
	${CC} server.o libisocket.o ${CLIBS} -o server

libisocket.o: libisocket.c
	${CC} ${CFLAGS} -c libisocket.c -o libisocket.o

client.o: client.c
	${CC} ${CFLAGS} -c client.c -o client.o

server.o: server.c
	${CC} ${CFLAGS} -c server.c -o server.o

clean:
	rm -f *.o client server libisocket.o
