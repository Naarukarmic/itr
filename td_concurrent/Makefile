CC = gcc
CFLAGS = -g -std=c99 -O0 -Wall -pedantic
LDFLAGS = -lpthread

SOURCES = threadpool.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))

BINS=test_threadpool tp_web_1 lf_web web_client

all: $(BINS)

help:
	@echo "Available targets"
	@echo "* test_threadpool: test program"
	@echo "* simple_web_1: dummy server"
	@echo "* web_client: web_client"
	@echo "* clean/distclean: workspace cleanup"

test_threadpool: $(OBJS) test_threadpool.c
	$(CC) -o test_threadpool test_threadpool.c $(OBJS) $(CFLAGS) $(LDFLAGS)

simple_web_1: simple_web_1.c $(OBJS)
	$(CC) -o simple_web_1 simple_web_1.c $(OBJS) $(CFLAGS) $(LDFLAGS)

tp_web_1: tp_web_1.c $(OBJS)
	$(CC) -o tp_web_1 tp_web_1.c $(OBJS) $(CFLAGS) $(LDFLAGS)

lf_web: lf_web.c $(OBJS)
	$(CC) -o lf_web lf_web.c $(OBJS) $(CFLAGS) $(LDFLAGS)

web_client: web_client.c $(OBJS)
	$(CC) -o web_client web_client.c isocket.c $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	-rm -f *.o *~ *.dSYM

distclean: clean
	-rm -f $(BINS)

%.o : %.c
	$(CC) -c $(CFLAGS)  $<

###############################################################################
# Packaging of the lab

STUDENT_DIR=threadpool_src
PROF_DIR=correction

dist: package_student

package_student: distclean
	-rm -rf $(STUDENT_DIR)
	mkdir $(STUDENT_DIR)
	cp Makefile $(STUDENT_DIR)
	cp utils.h $(STUDENT_DIR)
	cp threadpool.? $(STUDENT_DIR)
	cp test_threadpool.c $(STUDENT_DIR)
	cp simple_web_1.c $(STUDENT_DIR)
	tar zcvf $(STUDENT_DIR).tar.gz $(STUDENT_DIR)
