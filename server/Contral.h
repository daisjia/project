/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2019-01-15 11:32

		>Email			:	      1242200789@qq.com
		>Filename		:	      Contral.h
************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<map>
#include<json/json.h>
#include"./View_base.h"
#include<mysql/mysql.h>
#include<iostream>
#include"./Model.h"
#include"./Enum.h"
using namespace std;

class Contral
{
public :
	Contral()
	{
		mpcon = mysql_init((MYSQL *)0);
		MYSQL_RES *mp_res;
		MYSQL_ROW mprow;

		if(!mysql_real_connect(mpcon, "127.0.0.1", "root", "", "person", 3306, NULL, 0))
		{
			cout<<"sql connect fail"<<endl;
			exit(0);
		}

		if(mysql_select_db(mpcon, "person"))
		{
			cout<<"select fail"<<endl;
		}

		_model = new Model();
		
	}

	void Process(int fd, const string &message)
	{
		_fd = fd;
		Json::Value val; 
		Json::Reader read;
		if(-1 == read.parse(message.c_str(), val))
		{
			cout<<"read fail"<<endl;
			return ;
		}
		map<int, View_base *>::iterator it = _model->_map->find(val["type"].asInt());
		switch(val["type"].asInt())
		{
			case INSERT : it->second->process(_fd, mpcon, message); break;
			case DELETE : it->second->process(_fd, mpcon, message); break;
			case LOGIN : it->second->process(_fd, mpcon, message); break;
			case REGISTER : it->second->process(_fd, mpcon, message); break;
			case SELECT : it->second->process(_fd, mpcon, message); break;
			case UPDATE : it->second->process(_fd, mpcon, message); break;
			default : break;
		}

	}

private:
	Model *_model;
	MYSQL *mpcon;
	int _fd;
};
