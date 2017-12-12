CC      = gcc
CFLAGS  = -Wall -g
LDFLAGS = -lpthread

.SUFFIXES: .o .c .h .gch

###############################################################################

help:
	@echo The following targets are available
	@echo - test_minteger  : test minteger.c
	@echo - test_msgbox    : test msg_box.c
	@echo - test_periodic  : test periodic_task.c
	@echo - minepump       : final binary, Tcl/Tk GUI
	@echo - minepump_nogui : final binary, no GUI
	@echo - minepump_rtems : final binary, no GUI, RTEMS variant
	@echo - rtems_run      : run RTEMS binary

C_SRCS = minepump.c minteger.c msg_box.c periodic_task.c	\
	simu.c utils.c

test_minteger: minteger.c minteger.h
	gcc -D__TEST__ $(CFLAGS) -o test_minteger minteger.c $(LDFLAGS)

test_msgbox: msg_box.h msg_box.c
	gcc -D__TEST__ $(CFLAGS) -o test_msgbox msg_box.c $(LDFLAGS)

test_periodic: periodic_task.h periodic_task.c
	gcc -D__TEST__ $(CFLAGS) -o test_periodic periodic_task.c utils.c $(LDFLAGS)

minepump: clean $(C_SRCS)
	gcc $(CFLAGS) -o minepump -D__SIMU_SOCKET__ $(C_SRCS) $(LDFLAGS)

minepump_nogui: clean $(C_SRCS)
	gcc $(CFLAGS) -o minepump_nogui $(C_SRCS) $(LDFLAGS)

minepump_rtems: clean $(C_SRCS)
	make -f Makefile.rtems

rtems_run:
	tsim-leon3 o-optimize/minepump.exe

clean:
	-rm -rf *~ *.o

distclean: clean
	-rm -rf test_minteger test_msgbox test_periodic minepump

###############################################################################
# Packaging of the lab

STUDENT_DIR=minepump
PROF_DIR=correction

dist: package_student package_prof

package_student:
	-rm -rf $(STUDENT_DIR)
	mkdir $(STUDENT_DIR)
	cp minepump.tcl $(STUDENT_DIR)
	cp utils.h $(STUDENT_DIR)
	cp Makefile $(STUDENT_DIR)
	cp Makefile.rtems $(STUDENT_DIR)
	-unifdef -D__STUDENT__ minepump.c > $(STUDENT_DIR)/minepump.c
	cp minteger.h $(STUDENT_DIR)
	-unifdef -D__STUDENT__ minteger.c > $(STUDENT_DIR)/minteger.c
	cp msg_box.h $(STUDENT_DIR)
	-unifdef -D__STUDENT__ msg_box.c > $(STUDENT_DIR)/msg_box.c
	cp periodic_task.h $(STUDENT_DIR)
	-unifdef -D__STUDENT__ periodic_task.c > $(STUDENT_DIR)/periodic_task.c
	cp simu.c $(STUDENT_DIR)
	cp simu.h $(STUDENT_DIR)
	cp utils.c $(STUDENT_DIR)
	cp utils.h $(STUDENT_DIR)
	tar zcvf $(STUDENT_DIR).tar.gz $(STUDENT_DIR)

package_prof:
	-rm -rf $(PROF_DIR)
	mkdir $(PROF_DIR)
	cp minepump.tcl $(PROF_DIR)
	cp utils.h $(PROF_DIR)
	cp Makefile $(PROF_DIR)
	cp Makefile.rtems $(PROF_DIR)
	-unifdef -U__STUDENT__ minepump.c > $(PROF_DIR)/minepump.c
	cp minteger.h $(PROF_DIR)
	-unifdef -U__STUDENT__ minteger.c > $(PROF_DIR)/minteger.c
	cp msg_box.h $(PROF_DIR)
	-unifdef -U__STUDENT__ msg_box.c > $(PROF_DIR)/msg_box.c
	cp periodic_task.h $(PROF_DIR)
	-unifdef -U__STUDENT__ periodic_task.c > $(PROF_DIR)/periodic_task.c
	cp simu.c $(PROF_DIR)
	cp simu.h $(PROF_DIR)
	cp utils.c $(PROF_DIR)
	cp utils.h $(PROF_DIR)
	tar zcvf $(PROF_DIR).tar.gz $(PROF_DIR)
