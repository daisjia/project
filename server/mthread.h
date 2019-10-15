/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-12-12 20:02
#include<iostream>
		>Email			:	      1242200789@qq.com
		>Filename		:	      mthread.h
************************************************************************/
#ifndef _MTHREAD_H
#define _MTHREAD_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<pthread.h>
#include<event.h>
#include<map>
#include<errno.h>
#include<string>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<iostream>
#include"./serWork.h"
#include"./Contral.h"
using namespace std;

class pthread
{
public:
	pthread(int socketPair1)
	{
		_base = event_base_new();

		//_contral = new contral();
		_cliEventMap = new map<int, struct event*>();
		
		struct event* socketPair1Event = event_new(_base, socketPair1, EV_READ | EV_PERSIST, pthread::socketPair1Cb, this);

		if(NULL == socketPair1Event)
		{
			cerr<<"sock pair 1 event create fail; errno:"<<errno<<endl;
			exit(0);
		}

		event_add(socketPair1Event, NULL);

		Run();
	}

	static void socketPair1Cb(int fd, short event, void *argv)
	{
		pthread *pth = static_cast<pthread*>(argv);
		int cliFd;
		recv(fd, (char*)&cliFd, 4, 0);

		struct event*cliEvent = event_new(pth->_base, cliFd, EV_READ | EV_PERSIST, cliCb, argv);

		if(NULL == cliEvent)
		{
			cerr<<"cli event create fail ; errno: "<<errno<<endl;
			return;
		}
		event_add(cliEvent, NULL);

		pth->_cliEventMap->insert(make_pair(cliFd, cliEvent));

	}

	void Run()
	{
		pthread_t pid = 0;
		pthread_create(&pid, NULL, pthread::pthRun, _base);
	}

	static void *pthRun(void *argv)
	{
		cout<<"pthread "<<pthread_self()<<" run"<<endl;
		struct event_base* base = static_cast<struct event_base*>(argv);
		event_base_dispatch(base);
	}

	static void cliCb(int fd, short event, void *argv)
	{
		cout<<"clifd: "<<fd<<"new message"<<endl;
		cout<<"process pthread: "<<pthread_self()<<endl;

		pthread* pth = static_cast<pthread*>(argv);

		string str;

		int flag = serWork::GetSerWork()->RecvFromCli(fd, str);

		if(flag > 0)
		{
			_contral = new Contral();
			_contral->Process(fd, str);
		}
		else if(flag <= 0)
		{
			cerr<<"cli down cliFd == "<<fd<<";"<<endl;
			event_free((*pth->_cliEventMap)[fd]);
			pth->_cliEventMap->erase(fd);
			close(fd);
			return ;
		}
		
		//cout<<str<<endl;
	}

private:
	struct event_base* _base;

	map<int, struct event*> *_cliEventMap;
	static Contral *_contral;

};

Contral *pthread::_contral = NULL;







#endif
