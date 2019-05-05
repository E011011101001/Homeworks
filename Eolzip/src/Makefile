CC = g++
FLAGS =  -Wall -v -O2 -std=c++11
INSTALL_PATH = /usr/local/bin/

ezip: eolzip.o compress.o bitIO.o
	$(CC) $(FLAGS) eolzip.o compress.o bitIO.o -o ezip

eolzip.o: eolzip.cpp compress.hpp
	$(CC) $(FLAGS) -c eolzip.cpp -o eolzip.o

compress.o: compress.cpp compress.hpp freqcounter.hpp htree.hpp bitIO.hpp
	$(CC) $(FLAGS) -c compress.cpp -o compress.o

bitIO.o: bitIO.cpp bitIO.hpp
	$(CC) $(FLAGS) -c bitIO.cpp -o bitIO.o

.PHONY: clean install uninstall
install:
	cp ezip $(INSTALL_PATH)

uninstall:
	rm $(INSTALL_PATH)ezip

clean:
	-rm -rf ezip *.o *.out

