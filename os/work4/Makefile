.MAIN: all
CC = clang
CCP = clang++
CFLAGS = -std=c99 -Werror -pedantic-errors -lc -lpthread -L/usr/lib -L/lib
CPPFLAGS = -std=c++17 -Werror -pedantic-errors -DC_WITH_CLASSES -lpthread -L/usr/lib -L/lib
all: work4 work4p

clean:
	rm ${.CURDIR}/work4
work4:
	${CC} ${CFLAGS} -o ${.CURDIR}/work4 work4.c
work4p:
	${CCP} ${CPPFLAGS} -o ${.CURDIR}/work4p work4.cpp