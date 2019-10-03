LIBS = -lyajl
DEFS = 
CFLAGS = -g
TARGET = 
OBJS = obj
BINS = bin
CC = g++

clean_stub:
	test -f ${OBJS}/stub.o && rm ${OBJS}/stub.o || exit 0

prepare:
	test -d ${OBJS} || mkdir ${OBJS}
	test -d ${BINS} || mkdir ${BINS}

${OBJS}/%.o: %.cpp
	${CC} ${CFLAGS} ${DEFS} -c $< -o $@ ${LIBS}
	

flat_rows: DEFS += -DFLAT_ROWS
flat_rows: prepare clean_stub ${OBJS}/flat_rows.o ${OBJS}/stub.o flat_rows.h
	${CC} ${OBJS}/flat_rows.o ${OBJS}/stub.o -o ${BINS}/flat_rows ${LIBS}

simple_array: DEFS += -DSIMPLE_ARRAY
simple_array: prepare clean_stub ${OBJS}/simple_array.o ${OBJS}/stub.o simple_array.h
	${CC} ${OBJS}/simple_array.o ${OBJS}/stub.o -o ${BINS}/simple_array ${LIBS}

complex_object: DEFS += -DCOMPLEX_OBJECT
complex_object: prepare clean_stub ${OBJS}/complex_object.o ${OBJS}/stub.o complex_object.h
	${CC} ${OBJS}/complex_object.o ${OBJS}/stub.o -o ${BINS}/complex_object ${LIBS}

clean:
	test -d ${OBJS} && rm -rf ${OBJS} || exit 0
	test -d ${BINS} && rm -rf ${BINS} || exit 0

