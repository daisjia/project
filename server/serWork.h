/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-12-12 18:43

		>Email			:	      1242200789@qq.com
		>Filename		:	      serWork.h
************************************************************************/
#ifndef _SERWORK_H
#define _SERWORK_H
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

class serWork
{
public:
	static serWork* GetSerWork()
	{
		
		if(--count == 0)
		{
			_serwork = new serWork();
		}
		return _serwork;
	}

	int GetListenFd()
	{
		return _sockfd;
	}

	int AcceptCli()
	{

		struct sockaddr_in cli;
		socklen_t len = sizeof(cli);
		//cout<<"Connecting..."<<endl;
		int clifd = accept(_sockfd, (struct sockaddr*)&cli, &len);
		assert(clifd != -1);
		return clifd;
	}

	int RecvFromCli(int fd, string &tmp)
	{
		sockaddr_in cli;
		char buff[128] = {0};
		int res = recv(fd, (void *)tmp.c_str(), 127, 0);
		return res;
	}

	
private:
	serWork()
	{
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		assert(sockfd != -1);
		_sockfd = sockfd;
		sockaddr_in ser, cli;
		string ip;
		int port = 0;
	//	sleep(0.5);
	//	cout<<"loadind..."<<endl;
		sleep(1);
		cout<<"please input server ip: ";
		cin>>ip;
		cout<<"please input server port: ";
		cin>>port;
		ser.sin_family = AF_INET;
		ser.sin_port = htons(port);
		ser.sin_addr.s_addr = inet_addr(ip.c_str());
		int res = bind(sockfd, (struct sockaddr*)&ser, sizeof(ser));
		assert(res != -1);
		listen(sockfd, 20);
		
	}
	static serWork* _serwork;
	static int count;
	int _sockfd;
};

int serWork::count = 1;
serWork* serWork::_serwork = NULL;

#endif
