/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-30 16:44
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      selecter.h
************************************************************************/
#ifndef SELECTER_H
#define SELECTER_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<iostream>
#include"./tcpConnect.h"
using namespace std;

class selecter
{
public:
	selecter(const string &ip, const int port)
	{
		_selectConnect = new tcpConnect();
		int res = _selectConnect->Connect(ip.c_str(), port);
		if(res == -1)
		{
			exit(0);
		}
	}


	void Run()
	{
		string id;
		cout<<"please input id:"<<endl;
		cin>>id;
		_selectConnect->Send(id);
		string buff;
		_selectConnect->Recv(buff);
		cout<<buff<<endl;
	}

private:
	tcpConnect * _selectConnect;
};


#endif
