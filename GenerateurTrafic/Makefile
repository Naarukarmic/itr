CC = gcc
CFLAGS = -g -std=c99 -O0 -Wall -pedantic

SOURCES = dataPacket.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))

BINS=server_traffic client_traffic

all: $(BINS)

help:
	@echo "Available targets"
	@echo "* server_traffic: server program"
	@echo "* clean/distclean: workspace cleanup"

server_traffic: $(OBJS) server_traffic.c
	$(CC) -o server_traffic server_traffic.c $(OBJS) $(CFLAGS)

client_traffic: $(OBJS) client_traffic.c
	$(CC) -o client_traffic client_traffic.c $(OBJS) $(CFLAGS)

clean:
	-rm -f *.o *~ *.dSYM

distclean: clean
	-rm -f $(BINS)

%.o : %.c
	$(CC) -c $(CFLAGS)  $<
