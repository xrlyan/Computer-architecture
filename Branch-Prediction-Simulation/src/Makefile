CC=g++
CFLAGS=-Wall -std=c++0x

main: main.cpp saturaPredictor.cpp globalPredictor.cpp gsharePredictor.cpp tournaPredictor.cpp combinePredictor.cpp
	${CC} ${CFLAGS} -g -o main main.cpp saturaPredictor.cpp globalPredictor.cpp gsharePredictor.cpp tournaPredictor.cpp combinePredictor.cpp

.PHONY: clean stripped cppcheck

stripped:
	${CC} ${CFLAGS} -static -o main main.cpp
	strip main

cppcheck:
	cppcheck --enable=all --inconclusive *.cpp


clean:
	rm -f main *.o
