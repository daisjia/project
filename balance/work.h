/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2019-01-08 18:45

		>Email			:	      1242200789@qq.com
		>Filename		:	      work.h
************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<iostream>
using namespace std;

class work
{
public:
	static work* GetWork()
	{
		if(--count == 0)
		{
			_work = new work();
			return _work;
		}
		return _work;
	}
private:
	work()
	{
		//string ip;
		//int port = 0;
		//cout<<"please input balance ip:";
		//cin>>ip;
		//cout<<"please input connect ser port:";
		//cin>>port;
		_serfd = socket(AF_INET, SOCK_STREAM, 0);
		assert(_serfd != -1);

		struct sockaddr_in ser, cli;
		ser.sin_port = htons(5000);
		ser.sin_family = AF_INET;
		ser.sin_addr.s_addr = inet_addr("127.0.0.1");
		int res = bind(_serfd, (struct sockaddr*)&ser, sizeof(ser));
		assert(res != -1);
		cout<<"serfd create success!"<<endl;
		listen(_serfd, 20);
		/*
		cout<<"please input connect cli port:";
		port = 0;
		cin>>port;
		*/
		_clifd = socket(AF_INET, SOCK_STREAM, 0);
		assert(_clifd != -1);
		cli.sin_port = htons(6000);
		cli.sin_addr.s_addr = inet_addr("127.0.0.1");
		cli.sin_family = AF_INET;
		res = bind(_clifd, (struct sockaddr*)&cli, sizeof(cli));
		assert(res != -1);
		cout<<"clifd create success!"<<endl;
		listen(_clifd, 20);
	}

	
	static work *_work;
	static int count;
	static int _clifd;
	static int _serfd;
	friend class balance;
};

work *work::_work = NULL;
int work::count = 1;
int work::_serfd = 0;
int work::_clifd = 0;
