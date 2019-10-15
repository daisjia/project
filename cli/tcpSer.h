/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-26 21:02
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      tcpSer.h
************************************************************************/
#ifndef _TCPSER_H
#define _TCPSER
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<assert.h>
#include<iostream>
using namespace std;
class tcpSer
{
public:
	tcpSer(const string &ip, const int port)
	{
		_ip = ip;
		_port = port;
		_listenFd = socket(AF_INET, SOCK_STREAM, 0);
		if(_listenFd == -1)
		{
			cerr<<"ser create fail ; errno : "<<errno<<endl;
			return;
		}
		struct sockaddr_in ser;
		memset(&ser, 0, sizeof(ser));

		ser.sin_family = AF_INET;
		ser.sin_port = htons(_port);
		ser.sin_addr.s_addr = inet_addr(_ip.c_str());

		int res =bind(_listenFd, (struct sockaddr*)&ser, sizeof(ser));
		if(res == -1)
		{
			cerr<<"bind fail; errno : "<<errno<<endl;
			return ;
		}

		if(-1 == listen(_listenFd, 20))
		{
			cerr<<"listen fail; errno : "<<errno<<endl;
			return ;
		}

	}

	int Accept()
	{
		struct sockaddr_in cli;
		memset(&cli, 0, sizeof(cli));
		int len = sizeof(cli);
		int cli_fd = accept(_listenFd, (struct sockaddr*)&cli, (socklen_t*)&len);
		if(cli_fd == -1)
		{
			cerr<<"accept fail ; errno : "<<errno<<endl;
		}
		return cli_fd;
	}

	int Send(int fd, const string &str)
	{
		int flag = send(fd, str.c_str(), strlen(str.c_str()), 0);
		return flag;
	}

	int Recv(int fd, string &str)
	{
		char buff[128] = {0};
		int flag = recv(fd, buff, 127, 0);
		str = buff;
		return flag;
	}

	string GetIp()
	{
		return _ip;
	}

	int GetPort()
	{
		return _port;
	}

	int GetListenFd()
	{
		return _listenFd;
	}

private:
	string _ip;
	int _port;
	int _listenFd;

};



#endif
