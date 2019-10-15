main:main.o 
	g++ -g main.o -o main -L /usr/lib/mysql -lmysqlclient -ljson -levent -lpthread
main.o:main.cpp
	g++ main.cpp -c -g
cc:
	rm *.o -rf
	rm main
