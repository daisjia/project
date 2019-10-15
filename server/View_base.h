/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2019-01-15 12:08

		>Email			:	      1242200789@qq.com
		>Filename		:	      View_base.h
************************************************************************/
#ifndef VIEW_BASE
#define VIEW_BASE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<json/json.h>
#include<mysql/mysql.h>

class View_base
{
public:
	virtual void process(int fd, MYSQL *mpcon, const string &message) = 0;
};

class Insert_View : public View_base
{
public:
	void process(int fd, MYSQL *mpcon, const string &message)
	{

	}
};

class Delete_View : public View_base
{
public:	
	void process(int fd, MYSQL *mpcon, const string &message)
	{

	}
};


class Update_View : public View_base
{
public:
	void process(int fd, MYSQL *mpcon, const string &message)
	{

	}
};


class Select_View : public View_base
{
public:	
	void process(int fd, MYSQL *mpcon, const string &message)
	{

	}
};


class Login_View : public View_base
{
public:
	void process(int fd, MYSQL *mpcon, const string &message)
	{

	}
};


class Register_View : public View_base
{
public:
	void process(int fd, MYSQL *mpcon, const string &message)
	{

	}
};




#endif





#if 0

class View_base
{
public:
	virtual void View_Insert(MYSQL *mpcon, const string message) = 0;
	virtual void View_Delete(MYSQL *mpcon, const string message) = 0;
	virtual void View_Update(MYSQL *mpcon, const string message) = 0;
	virtual void View_Select(MYSQL *mpcon, const string message) = 0;
	virtual void View_Login(int fd, MYSQL *mpcon, const string message) = 0;
	virtual void View_Register(int fd, MYSQL *mpcon, const string message) = 0;
};

class View : public View_base
{
	void View_Insert(MYSQL *mpcon, const string message)
	{
		Json::Value val;
		Json::Reader read;
		if(-1 == read.parse(message.c_str(), val))
		{
			cout<<"read fail"<<endl;
		}
		string id = val["id"].asString();
		string name = val["name"].asString();
		string start_time = val["start_time"].asString();
		int limit_month = val["limit_month"].asInt();

		char cmd[100] = "insert into info values(id.c_str(), name.c_str(), start_time.c_str(), limit_month);";
		if(!mysql_real_query(mpcon, cmd, strlen(cmd)))
		{
			cout<<"query fail"<<endl;
			return ;
		}
	}

	void View_Delete(MYSQL *mpcon, const string message)
	{
		/*
		Json::Value val;
		Json::Reader read;
		if(read.parse(message.c_str(), val))
		{
			cout<<"read fail"<<endl;
		}
		string id = val["id"];

		char cmd[100] = "delete from info where id = id.c_str();";
		if(mysql_real_query(mpcon, cmd, strlen(cmd)))
		{
			cout<<"query fail"<<endl;
			return ;
		}
		*/
	}

	void View_Update(MYSQL *mpcon, const string message)
	{
		/*
		Json::Value val;
		Json::Reader read;
		if(read.parse(message.c_str(), val))
		{
			cout<<"read fail"<<endl;
		}
		string id = val["id"];
		string name = val["name"];
		string start_time = val["start_time"];
		int limit_month = val["limit_month"];

		char cmd[100] = "insert into info values(id.c_str(), name.c_str(), start_time.c_str(), limit_month);";
		if(mysql_real_query(mpcon, cmd, strlen(cmd)))
		{
			cout<<"query fail"<<endl;
			return ;
		}
		*/
	}

	void View_Select(MYSQL *mpcon, const string message)
	{
		/*
		Json::Value val;
		Json::Reader read;
		if(read.parse(message.c_str(), val))
		{
			cout<<"read fail"<<endl;
		}
		string id = val["id"];
		string name = val["name"];
		string start_time = val["start_time"];
		int limit_month = val["limit_month"];

		char cmd[100] = "insert into info values(id.c_str(), name.c_str(), start_time.c_str(), limit_month);";
		if(mysql_real_query(mpcon, cmd, strlen(cmd)))
		{
			cout<<"query fail"<<endl;
			return ;
		}
		*/
	}

	void View_Login(int fd, MYSQL *mpcon, const string message)
	{
		/*
		Json::Value val;
		Json::Reader read;
		if(read.parse(message.c_str(), val))
		{
			cout<<"read fail"<<endl;
		}
		string id = val["id"];
		string name = val["name"];
		string start_time = val["start_time"];
		int limit_month = val["limit_month"];

		char cmd[100] = "insert into info values(id.c_str(), name.c_str(), start_time.c_str(), limit_month);";
		if(mysql_real_query(mpcon, cmd, strlen(cmd)))
		{
			cout<<"query fail"<<endl;
			return ;
		}
		*/
		
	}

	void View_Register(int fd, MYSQL *mpcon, const string message)
	{
		Json::Value val;
		Json::Reader read;
		if(-1 == read.parse(message.c_str(), val))
		{
			cout<<"read fail"<<endl;
			return;
		}
		string user = val["username"].asString();
		string passwd = val["passwd"].asString();

		char cmd[100] = {0};
		sprintf(cmd, "insert into admi values('%s','%s');", user.c_str(), passwd.c_str());
		cout<<cmd<<endl;
		if(0 != mysql_query(mpcon, cmd))
		{
			cout<<"query fail"<<endl;
			char buff[] = {"register fail!"};
			int len = strlen(buff);
			send(fd, buff, len, 0);
			return ;
		}
		char buff[] = {"register success!"};
		int len = strlen(buff);
		send(fd, buff, len, 0);

	}
};
#endif
