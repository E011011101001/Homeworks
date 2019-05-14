CC = g++
FLAGS = -Wall -O2 -v --std=c++11

snakysnakes : canvas.o graphic-tools.o main.o menu.o snake.o snake-components.o
	$(CC) $(FLAGS) $^ -o $@

main.o: main.cpp
	$(CC) $(FLAGS) -c $< -o $@

%.o: %.cpp %.hpp
	$(CC) $(FLAGS) -c $< -o $@
	
.PHONY: clean
clean:
	-rm -rf snakysnakes *.o

