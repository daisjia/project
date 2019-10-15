/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-28 19:32
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      worker.h
************************************************************************/
#ifndef _WORKER_H
#define _WORKER_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"./selectSerThread.h"
#include"./workSer.h"
#include<iostream>
using namespace std;

class worker
{
public:

	worker()
	{
		//创建分布式信息采集机服务器
		_selectSer = new selectSerThread();
	}

	//接收服务器的消息的线程
	static void *RecvFromWorkSer(void *argv)
	{
		//将传入的参数强转
		selectSerThread *selectSer = static_cast<selectSerThread*>(argv);
		
		while(1)
		{
			string str;

			//接受服务器的消息，如果是数据就处理，否则不管
			if(workSer::GetWorkSer()->RecvFromWorkSer(str));
			{
				Json::Value val;
				Json::Reader read;
				
				//解析数据包
				if(read.parse(str.c_str(), val))
				{
					cerr<<"read buff fail ; errno : "<<errno<<endl;
					continue;
				}


				//判断数据是否给信息采集机的服务器
				if(val["type"].asInt() == SELECT && val["fd"].asInt() != -1)
				{
					int fd = val["fd"].asInt();
					string str = val["message"].asString();
					//通过信息采集机服务器发送给相应的信息采集机
					selectSer->SendSelect(fd, str);
				}
				else
				{
					//本机消息，直接打印
					cout<<val["message"].asString()<<endl;
				}
			}
		}
	}

	void Put()
	{
		cout<<"1.Insert"<<endl;
		cout<<"2.Delete"<<endl;
		cout<<"3.Update"<<endl;
		cout<<"4.Selete"<<endl;
		cout<<"0.EXIT"<<endl;
	}

	void Run()
	{
		//启动线程用于接受主服务器信息
		RecvFromWorkSer();

		while(1)
		{
			Put();
			int choice = -1;
			cout<<"please choice:"<<endl;
			cin>>choice;

			switch(choice)
			{
				case 1 : Insert(); break;
				case 2 : Delete(); break;
				case 3 : Update(); break;
				case 4 : Select(); break;
				case 0 : Exit(); break;
				default : cout<<"input error" <<endl;
			}

		}
	}

	void Insert()
	{
		string name;
		cout<<"please input name:";
		cin>>name;

		string id;
		cout<<"please input id:";
		cin>>id;

		string start_time;
		cout<<"please input start time:";
		cin>>start_time;

		int month = 0;
		cout<<"please input limit time:";
		cin>>month;
	
		Json::Value val;
		val["type"] = INSERT;
		val["name"] = name.c_str();
		val["id"] = id.c_str();
		val["limit_month"] = month;
		val["start_time"] = start_time.c_str();
	
		workSer::GetWorkSer()->SendToSer(val.toStyledString());
	}

	void Delete()
	{
		string id;
		cout<<"please input id:";
		cin>>id;

		Json::Value val;
		val["type"] = DELETE;
		val["id"] = id.c_str();
		workSer::GetWorkSer()->SendToSer(val.toStyledString());
	}

	
	void Update()
	{
		string id;
		cout<<"please input id:";
		cin>>id;

		int month = 0;
		cout<<"please input new limit month:";
		cin>>month;

		Json::Value val;
		val["type"] = UPDATE;
		val["id"] = id.c_str();
		val["new_month"] = month;
		workSer::GetWorkSer()->SendToSer(val.toStyledString());
	}
	

	void Select()
	{
		string id;
		cout<<"please input id:"<<endl;
		cin>>id;

		Json::Value val;
		val["type"] = SELECT;
		val["fd"] = -1;
		val["id"] = id.c_str();

		workSer::GetWorkSer()->SendToSer(val.toStyledString());
	}

	void Exit()
	{
		Json::Value val;
		val["type"] = EXIT;
		workSer::GetWorkSer()->SendToSer(val.toStyledString());
	}


	void RecvFromWorkSer()
	{
		pthread_t pid;
		pid = pthread_create(&pid, NULL, worker::RecvFromWorkSer, _selectSer);
	}

private:
	selectSerThread* _selectSer;  //信息采集机服务器
};




#endif

