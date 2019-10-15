/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-27 17:04
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      sys.h
************************************************************************/
#ifndef _SYS_H
#define _SYS_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"./view.h"
#include<json/json.h>
#include<iostream>
#include"./worker.h"
#include"./view.h"
using namespace std;

class sys
{
public:
	sys()
	{
		workSer::GetWorkSer();
		//构造一个工作组对象
		_worker = new worker();
	}

	void Put()
	{
		cout<<"1.Login"<<endl;
		cout<<"2.Register"<<endl;
		cout<<"3.Exit"<<endl;
	}

	void Register()   //注册
	{
		cout<<"registering..."<<endl;
		string name;
		string pwd;
		cout<<"please input name:";
		cin>>name;
		cout<<"please input pwd:";
		cin>>pwd;

		Json::Value val;
		val["type"] = REGISTER;
		val["username"] = name.c_str();
		val["passwd"] = pwd.c_str();
		//将数据发送给服务器
		workSer::GetWorkSer()->SendToSer(val.toStyledString());
		//等待服务器回应
		
		string recvbuff;
		workSer::GetWorkSer()->RecvFromWorkSer(recvbuff);

		cout<<recvbuff<<endl;

	}

	bool Login()   //登录
	{
		cout<<"loginning..."<<endl;
		string name;
		string pwd;
		cout<<"please input username:";
		cin>>name;
		cout<<"please input passwd:";
		cin>>pwd;
		Json::Value val;
		val["type"] = LOGIN;  //发送给服务器类型
		val["username"] = name.c_str();
		val["passwd"] = pwd.c_str();

		//将姓名和密码发送给服务器
		workSer::GetWorkSer()->SendToSer(val.toStyledString());  
		string recvbuff;

		//接受服务器消息，看是否姓名和密码相匹配
		workSer::GetWorkSer()->RecvFromWorkSer(recvbuff);
		cout<<recvbuff<<endl;

//		if(recvbuff == "login success")
//		{
//			cout<<"login success!"<<endl;
			//登录成功进入工作界面
			_worker->Run();
//		}
	}

	void Exit()
	{
		cout<<"exitting..."<<endl;
		exit(0);
	}

	~sys()
	{
		delete _worker;
	}
private:
	worker *_worker;
};



#endif
