#compiler and flags 
CC := ${CC}
CFLAGS := ${CFLAGS}


#TARGET ?= makefileexample
#libs ?= lib-add.c lib-sub.c lib-mul.c

#we can use EXTRA_OEMAKE in the recipe
TARGET ?= ''
libs ?= ''
MAIN_SRC ?= ${TARGET}.c

#source files
SRCS := ${MAIN_SRC} ${libs}

#object files
OBJS := ${SRCS:.c=.o}

#binary output

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} $^ -o $@ -Wl,--hash-style=gnu
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@ 
	
install:
	install -d ${DESTDIR}${BINDIR}
	install -m 0755 ${TARGET} ${DESTDIR}${BINDIR}
	
clean:
	rm -f ${OBJS} ${TARGET} 
