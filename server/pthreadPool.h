#ifndef _PTHREADPOOL_H
#define _PTHREADPOOL_H
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include"./mthread.h"

#include<iostream>
#include<map>
#include<sys/types.h>
#include<sys/socket.h>
using namespace std;

class pthreadPool
{
public:
	
	pthreadPool(struct event_base *lib_base)
	{
		//cout<<"please input thread number:";
		//int num = 0;
		//cin>>num;
		cout<<"pthread pool is creating ..."<<endl;
		
		for(int i=0; i<3; ++i)
		{
			int fd[2] = {0};
			socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
			struct event *sockPair0Event = event_new(lib_base, fd[0], EV_READ | EV_PERSIST, RecvEventNumFromThread, &_threadEventNumMap);
			if(NULL == sockPair0Event)
			{
				cerr<<"socketpair 0 event create fail; errno:"<<errno<<endl;
				exit(0);
			}
			event_add(sockPair0Event, NULL);
			cout<<"socket pair "<<i+1<<" create success"<<endl;
			cout<<"socket pair "<<i+1<<" 0 event create success"<<endl;
			_threadEventNumMap.insert(make_pair(fd[0],0));
			new pthread(fd[1]);
		}
	}
	void SendCliFdToPthread(int fd)
	{
		map<int,int>::iterator it = _threadEventNumMap.begin();
		map<int,int>::iterator itMin = it;
		int eventNum = it->second;

		for(; it != _threadEventNumMap.end(); it++)
		{  
			cout<<"pthread flage: "<<it->first<<"    event num:"<<it->second<<endl;
			if(it->second < eventNum)
			{
				itMin = it;
			}
		}

		send(itMin->first, (char *)&fd, 4, 0);
		itMin->second++;
		cout<<"send to pthread flage:"<<itMin->first<<endl;
	}

	static void RecvEventNumFromThread(int fd, short event, void *argv)
	{
		map<int,int> *_map= static_cast<map<int,int>*>(argv);
		int num;
		recv(fd, (char *)&num, 4, 0);
		(*_map)[fd] = num;
	}
		
private:
	map<int,int> _threadEventNumMap;
	struct event_base *lib_base_thread;
};




#endif
