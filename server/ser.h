/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-12-08 19:22
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      ser.h
************************************************************************/
#ifndef _SER_H
#define _SER_H
#include<stdio.h>
#include<stdlib.h>
#include<event.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include"./pthreadPool.h"


class Ser
{
public:
	
	Ser()
	{
		_base = event_base_new();
		
		_pthreadpool = new pthreadPool(_base);
		/*
		string ip;
		int port = 0;
		cout<<"please input server ip: ";
		cin>>ip;
		cout<<"please input server port: ";
		cin>>port;
		serWork::GetSerWork(ip.c_str, port);
		*/
	}

	void Run()
	{
		int fd = serWork::GetSerWork()->GetListenFd();

		struct event* listenEvent = event_new(_base, fd, EV_READ | EV_PERSIST, Ser::listenCb, _pthreadpool);
	
		if(NULL == listenEvent)
		{
			cerr<<"listen event create fail; errno: "<<errno<<endl;
			exit(0);

		}
		event_add(listenEvent, NULL);
		cout<<"server listen event create success"<<endl;

		cout<<"server libevent start"<<endl;
		event_base_dispatch(_base);
	}

	/*
	void listen_Cb(int sockfd, short event, void *argv)
	{
		struct event_base *lib_base = (struct event_base*)argv;
		struct sockaddr_in cli;
		socklen_t len = sizeof(cli);
		int cli_fd = accept(sockfd, (struct sockaddr*)&cli, &len);

		if(-1 == cli_fd)
		{
			cerr<<"accept fail; errno: "<<errno<<endl;
			return ;
		}

		_pthreadpool->SendCliFdToWorkThread(cli_fd);
	}
	*/

	static void listenCb(int fd, short event, void *argv)
	{
		
		////////////////cout<<"connecting...";
		pthreadPool *threadpool = static_cast<pthreadPool*>(argv);
		

		int clifd = serWork::GetSerWork()->AcceptCli();
		cout<<"cli connect success, clifd == "<<clifd<<endl;
		threadpool->SendCliFdToPthread(clifd);
	}
private:
	pthreadPool *_pthreadpool;
	struct event_base *_base;
};




#endif
