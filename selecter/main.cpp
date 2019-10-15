/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-30 16:54
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      main_selecter.cpp
************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include"./tcpConnect.h"
#include"./selecter.h"

int main()
{
	string ip;
	int port;
	cout<<"please input selecter ip:";
	cin>>ip;
	cout<<"please input selecter port:";
	cin>>port;
	selecter Selecter(ip.c_str(), port);
	while(1)
	{
		Selecter.Run();
	}
	return 0;
}
