CC      = gcc
CFLAGS  = -Wall -std=c99
LDFLAGS = -lpthread

.SUFFIXES: .o .c .h .gch

###############################################################################
BINARIES = ex1 ex2 ex3

help:
	@echo The following targets are available
	@echo ex1 : exercise 1
	@echo ex2 : exercise 2
	@echo ex3 : exercise 3
	@echo train : exercise 4

all : $(BINARIES)

ex1: ex1.c
	$(CC) $(CFLAGS) -o $@ $@.c  $(LDFLAGS)

ex2: ex2.c
	$(CC) $(CFLAGS) -o $@ $@.c  $(LDFLAGS)

ex3: ex3.c
	$(CC) $(CFLAGS) -o $@ $@.c  $(LDFLAGS)

train: train.c
	$(CC) $(CFLAGS) -o $@ $@.c  $(LDFLAGS)

clean:
	-rm -rf *~ *.o *#

distclean: clean
	-rm -f $(BINARIES)

###############################################################################
# Packaging of the lab

STUDENT_DIR=posix_lab
PROF_DIR=correction

dist: package_student package_prof

package_student:
	-rm -rf $(STUDENT_DIR)
	mkdir $(STUDENT_DIR)
	cp debug.h $(STUDENT_DIR)
	cp Makefile $(STUDENT_DIR)
	-unifdef -D__STUDENT__ ex1.c > $(STUDENT_DIR)/ex1.c
	-unifdef -D__STUDENT__ ex2.c > $(STUDENT_DIR)/ex2.c
	-unifdef -D__STUDENT__ ex3.c > $(STUDENT_DIR)/ex3.c
	-unifdef -D__STUDENT__ train.c > $(STUDENT_DIR)/train.c
	tar zcvf $(STUDENT_DIR).tar.gz $(STUDENT_DIR)

package_prof:
	-rm -rf $(PROF_DIR)
	mkdir $(PROF_DIR)
	cp debug.h $(PROF_DIR)
	cp Makefile $(PROF_DIR)
	-unifdef -U__STUDENT__ ex1.c > $(PROF_DIR)/ex1.c
	-unifdef -U__STUDENT__ ex2.c > $(PROF_DIR)/ex2.c
	-unifdef -U__STUDENT__ ex3.c > $(PROF_DIR)/ex3.c
	-unifdef -U__STUDENT__ train.c > $(PROF_DIR)/train.c
	tar zcvf $(PROF_DIR).tar.gz $(PROF_DIR)
