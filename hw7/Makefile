CC = g++
CFLAGS = -g -Wall -Wextra -std=c++11
INC_FILES = ringbuffer.hpp ringbuffer.ipp
SRC_FILES = main.cpp

all: ${INC_FILES} ${SRC_FILES}
	${CC} ${CFLAGS} -o main ${SRC_FILES}

handin:
	rm -f hw7.tar && tar cvf hw7.tar ringbuffer.ipp
clean:
	rm -f main

.PHONY: clean