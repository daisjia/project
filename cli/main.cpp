/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2018-11-30 17:07
		>Telephone		:	      15389668442
		>Email			:	      1242200789@qq.com
		>Filename		:	      main_sys.cpp
************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"./sys.h"
#include"./workSer.h"
#include<iostream>
using namespace std;

int main()
{
	sys *Sys = new sys();

	while(1)
	{
		Sys->Put();
		int choice;
		cout<<"please input choice:";
		cin>>choice;

		switch(choice)
		{
			case 1 : Sys->Login(); break;
			case 2 : Sys->Register(); break;
			case 3 : Sys->Exit(); break;
			default : cout<<"input error"<<endl; break;
		}
	}
	delete Sys;
	return 0;
}

