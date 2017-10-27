CC = clang
CFLAGS = -Wall
CLIBS =

all: clean libisocket.o client_echo.o client_echo2.o server_echo.o server_echo2.o gent.o
	${CC} client_echo.o libisocket.o ${CLIBS} -o client_echo
	${CC} client_echo2.o libisocket.o ${CLIBS} -o client_echo2
	${CC} server_echo.o libisocket.o ${CLIBS} -o server_echo 
	${CC} server_echo2.o libisocket.o ${CLIBS} -o server_echo2
	${CC} gent.o libisocket.o ${CLIBS} -o gent

libisocket.o: libisocket.c
	${CC} ${CFLAGS} -c libisocket.c -o libisocket.o

client_echo.o: client_echo.c
	${CC} ${CFLAGS} -c client_echo.c -o client_echo.o

client_echo2.o: client_echo2.c
	${CC} ${CFLAGS} -c client_echo2.c -o client_echo2.o

server_echo.o: server_echo.c
	${CC} ${CFLAGS} -c server_echo.c -o server_echo.o

server_echo2.o: server_echo2.c
	${CC} ${CFLAGS} -c server_echo2.c -o server_echo2.o

server_echo3.o: server_echo3.c
	${CC} ${CFLAGS} -c server_echo3.c -o server_echo3.o

gent.o: gent.c
	${CC} ${CFLAGS} -c gent.c -o gent.o 

clean:
	rm -f *.o gent client_echo client_echo2 server_echo server_echo2 server_echo3 libisocket.o

