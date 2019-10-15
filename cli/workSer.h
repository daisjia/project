/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-26 21:01
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      workServer.h
************************************************************************/
#ifndef WORKSERVER_H
#define WORKSERVER_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<json/json.h>
#include<errno.h>
#include<string>
#include"./tcpConnect.h"
#include<iostream>
using namespace std;

class workSer
{
public:
	//单例模式
	static workSer* GetWorkSer()
	{
		if(--count == 0)
		{
			string ip;
			cout<<"please input workSer ip:";
			cin>>ip;
			int port;
			cout<<"please input workSer port:";
			cin>>port;
			workSer::_workser = new workSer(ip, port);
			
		}
		return _workser;
	}

	//给主服务器发送数据
	bool SendToSer(const string &message)
	{
		cout<<"sending message..."<<endl;
		return  _ser->Send(message);
	}

	//接收主服务器的消息
	int RecvFromWorkSer(string &str)
	{
		cout<<"receiving message..."<<endl;
		return  _ser->Recv(str);
	}

	
private:

	workSer(const string &ip, int port)
	{
		_ser = new tcpConnect();
		_ser->Connect(ip, port);
	}
	tcpConnect *_ser;
	static workSer* _workser;
	static int count;
	
};

workSer* workSer::_workser = NULL;
int workSer::count = 1;






#endif

