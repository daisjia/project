/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-29 21:51
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      selectSerThread.h
************************************************************************/
#ifndef SELECTSERTHREAD_H
#define SELECTSERTHREAD_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<arpa/inet.h>
#include<event.h>
#include"./tcpSer.h"
#include<map>
#include<string>
#include"./workSer.h"
#include<pthread.h>
#include"./view.h"
#include<iostream>
using namespace std;

class selectSerThread
{
public:
	//监听套接子的回调函数
	static void listenCb(int fd, short int event, void *argv)
	{
		cout<<"select mechine connect "<<endl;
		tcpSer *server = static_cast<tcpSer*>(argv);
		int cliFd = server->Accept();
		if(-1 == cliFd)
		{
			cerr<<"selecter cli accept fail ;: errno : "<<errno<<endl;
			return ;
		}

		struct event* cliEvent = event_new(_base, cliFd, EV_READ | EV_PERSIST, selectSerThread::cliCb, server);
		if(NULL == cliEvent)
		{
			cerr<<"select ser thread cli event new fail ; errno : "<<errno<<endl;
			return ;
		}

		_eventMap.insert(make_pair(cliFd, cliEvent));
		event_add(cliEvent, NULL);

	}

	//
	//客户端套接子的回调函数

	static void cliCb(int fd, short int event, void *argv)
	{
		tcpSer *server = static_cast<tcpSer*>(argv);

		string id;
		if(0 >= server->Recv(fd, id))
		{
			cerr<<"select cli shot down ; errno : "<<errno<<endl;
			event_free(_eventMap[fd]);
			_eventMap.erase(fd);
			close(fd);
			return ;
		}

		Json::Value val;
		val["type"] = SELECT;
		val["fd"] = fd;
		val["messege"] = id.c_str();

		//发送给工作服务器
		workSer::GetWorkSer()->SendToSer(val.toStyledString());
	}


	//启动线程循环监听:
	static void *selectThreadRun(void *argv)
	{
		event_base_dispatch(_base);
	}

	//

	selectSerThread()
	{
		//创建分布式信息采集机服务器
		_selecterSer = new tcpSer("127.0.0.1", 5000);
		cout<<"select ser create success ; ip:127.0.0.1, port:5000"<<endl;
		_base = event_base_new();

		//创建监听事件
		struct event *listenEvent = event_new(_base, _selecterSer->GetListenFd(), EV_READ | EV_PERSIST, selectSerThread::listenCb, _selecterSer);

		if(NULL == listenEvent)
		{
			cerr<<"select ser listen event create fail ; errno : "<<errno<<endl;
			return;
		}

		event_add(listenEvent, NULL);
		cout<<"selecter listen event create success!"<<endl;

		//启动线程进行循环监听
		pthread_t pid;
		pthread_create(&pid, NULL, selectSerThread::selectThreadRun, _base);
	}


	//发送数据给信息采集机
	void SendSelect(int fd, string &str)
	{
		_selecterSer->Send(fd, str);
	}


private:
	tcpSer* _selecterSer;  //分布式信息采集机的服务器
	static event_base *_base;
	static map<int, struct event*> _eventMap;
};

event_base *selectSerThread::_base = NULL;
map<int, struct event*> selectSerThread::_eventMap;


#endif
