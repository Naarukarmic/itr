EXECS=image_conv
MPICC=mpicc

all: ${EXECS}

image_conv: image_conv.c 
	${MPICC} -o image_conv image_conv.c image_processing.c

clean:
	rm -f ${EXECS}