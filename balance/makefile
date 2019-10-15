main:main.o
	g++ -g main.o -o main -levent -ljson -lpthread -lssl
main.o:main.cpp
	g++ main.cpp -c -g
clean:
	rm *.o -rf
	rm main
