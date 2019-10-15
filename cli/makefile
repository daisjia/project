main:main.o
	g++ -g main.o -o main -levent -ljson -lpthread
main.o:main.cpp
	g++ main.cpp -c -g
clean:
	rm *.o -rf
	rm main
