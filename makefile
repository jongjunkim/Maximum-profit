all: program1

program1: main.o
			g++ main.o -o program1

program1.o: main.cpp
			g++ -c main.cpp

clean:
	rm *.o program1