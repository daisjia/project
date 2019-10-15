/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2019-03-28 17:59

		>Email			:	      1242200789@qq.com
		>Filename		:	      Model.h
************************************************************************/
#ifndef MODEL_H
#define MODEL_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include"./Enum.h"
#include"./View_base.h"
#include<map>
class Model
{
public:
	Model()
	{
		_map = new map<int , View_base*>;
		Insert_Model();
	}
	
	void Insert_Model()
	{
		_map->insert(map<int, View_base*>::value_type(INSERT, new Insert_View));
		_map->insert(map<int, View_base*>::value_type(DELETE, new Delete_View));
		_map->insert(map<int, View_base*>::value_type(UPDATE, new Update_View));
		_map->insert(map<int, View_base*>::value_type(SELECT, new Select_View));
		_map->insert(map<int, View_base*>::value_type(LOGIN, new Login_View));
		_map->insert(map<int, View_base*>::value_type(REGISTER, new Register_View));
	}
	map<int, View_base*> *_map;
	
};

#endif
