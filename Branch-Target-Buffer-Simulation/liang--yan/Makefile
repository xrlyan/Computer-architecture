CC=g++
CFLAGS=-Wall -std=c++0x

main: main.cpp saturaPredictor.cpp globalPredictor.cpp tournaPredictor.cpp  btBuffer.cpp
	${CC} ${CFLAGS} -g -o main main.cpp saturaPredictor.cpp globalPredictor.cpp tournaPredictor.cpp btBuffer.cpp

.PHONY: clean stripped cppcheck

stripped:
	${CC} ${CFLAGS} -static -o main main.cpp
	strip main

cppcheck:
	cppcheck --enable=all --inconclusive *.cpp


clean:
	rm -f main *.o result.txt
