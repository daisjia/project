/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-26 21:25
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      tcpConnect.h
************************************************************************/
#ifndef TCPCONNECT_H
#define TCPCONNECT_H
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include<string.h>
#include<errno.h>
#include<iostream>
using namespace std;
class tcpConnect
{
public:
	int Connect(const string &ip, const int port)
	{
		cout<<"connecting to Ser"<<endl;
		_ip = ip;
		_port = port;
		_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		assert(_sockfd != -1);
		sockaddr_in ser;
		memset(&ser, 0, sizeof(ser));
		ser.sin_family = AF_INET;
		ser.sin_port = htons(port);
		ser.sin_addr.s_addr = inet_addr(ip.c_str());
		int len = sizeof(ser);

		int res = connect(_sockfd, (struct sockaddr*)&ser, len);
		assert(res != -1);
		cout<<"connect success!"<<endl;
		return _sockfd;
	}

	bool Send(const string &message)
	{
		int len = strlen(message.c_str());
		int n = send(_sockfd, message.c_str(), len, 0);
		return n;
	}

	int Recv(string &str)
	{
		char buff[128] = {0};
		int n = recv(_sockfd, buff, 127, 0);
		str = buff;
		return n;
	}

	int GetSockfd()
	{
		return _sockfd;
	}

	string GetIp()
	{
		return _ip;
	}

	int GetPort()
	{
		return _port;
	}
private:
	int _sockfd;
	string _ip;
	int _port;
};



#endif
