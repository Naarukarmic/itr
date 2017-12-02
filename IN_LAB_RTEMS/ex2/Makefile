###############################################################################
# Type:
#  - "make"     to compile the example
#  - "make run" to compile and run
###############################################################################

# C source names
CSRCS = init.c  

###############################################################################
# DO NOT EDIT AFTER THIS LINE

EXEC=test.exe
PGM=${ARCH}/$(EXEC)

COBJS_ = $(CSRCS:.c=.o)
COBJS = $(COBJS_:%=${ARCH}/%)

MANAGERS = all        # optional managers required
LIBS = -lrtemsall -lc # libraries

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

CFLAGS += -O0

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)

run: all
	tsim-leon3 ${ARCH}/$(EXEC)

help:
	@echo Type:
	@echo  - "make"     to compile the example
	@echo  - "make run" to compile and run
