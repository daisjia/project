/************************************************************************
		>Author			:	      JiaDai
		>Last modified	:	      2019-01-08 18:40

		>Email			:	      1242200789@qq.com
		>Filename		:	      balance.cpp
************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<json/json.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<event.h>
#include<openssl/md5.h>
#include"./work.h"
#include<iostream>
#include<map>
using namespace std;

struct IpPort
{
	string ip;
	short port;
};

int ar[65535];
void InitArray()
{
	for(int i=0; i<65535; ++i)
	{
		ar[i] = 0;
	}
}

long GetHash(const string& str)
{
	long hash = 0;
	unsigned char digest[16];

	MD5_CTX md5state;
	MD5_Init(&md5state);
	MD5_Update(&md5state, str.c_str(), strlen(str.c_str()));
	MD5_Final(digest, &md5state);

	cout<<str<<endl;

	for(int i=0; i<4; ++i)
	{
		hash += ((long)(digest[i*4+3] & 0xFF) << 24)
			| ((long)(digest[i*4+2] & 0xFF) << 16)
			| ((long)(digest[i*4+1] & 0xFF) << 8)
			| ((long)(digest[i*4+0] & 0xFF));
	}

	return hash;
}


class balance
{
public:
	balance()
	{
		InitArray();
		_baseSer = event_base_new();
		_work = work::GetWork();
		
		pthread_t pid = 0;
		pthread_create(&pid, NULL, balance::dealCli, NULL);
		
		struct event *serEvent = event_new(_baseSer, _work->_serfd, EV_READ | EV_PERSIST, balance::listenSer, NULL);
		if(NULL == serEvent)
		{
			cout<<"serEvent create fail"<<endl;
			return;
		}
		event_add(serEvent, NULL);
		event_base_dispatch(_baseSer);
	}

	static void *dealCli(void *argv)
	{
		_baseCli = event_base_new();
		struct event *cliEvent = event_new(_baseCli, _work->_clifd, EV_READ | EV_PERSIST, balance::listenCli, NULL);
		if(NULL == cliEvent)
		{
			cout<<"cliEvent create fail"<<endl;
			return NULL;
		}
		event_add(cliEvent, NULL);
		
		event_base_dispatch(_baseCli);
	}

	static void listenSer(int fd, short int event, void *argv)
	{
		struct sockaddr_in ser;
		int len = sizeof(ser);
		int res = accept(fd, (struct sockaddr*)&ser, (socklen_t*)&len);
		assert(res != -1);
		cout<<"ser ip:"<<inet_ntoa(ser.sin_addr)<<endl;;
		cout<<"ser port:"<<ntohs(ser.sin_port)<<endl;
		string ip = inet_ntoa(ser.sin_addr);
		short port = ntohs(ser.sin_port);

		HashSer(ip, port);
	}

	static void HashSer(const string &ip, const short &port)
	{
		char port_str[10];
		sprintf(port_str, "%d", port);
		string node = ip + port_str;

		struct IpPort ipport;
		ipport.ip = ip;
		ipport.port = port;

		for(int i=0; i<5; ++i)
		{
			char buff[2];
			sprintf(buff, "%d", i);
			node = node + buff;
			long pos = GetHash(node);
			if(pos < 0)
			{
				pos = -pos;
			}
			ar[pos] = pos;
			_ipMap.insert(make_pair(pos, ipport));
		}
	}

	static void listenCli(int fd, short int event, void *argv)
	{
		struct sockaddr_in cli;
		int len = sizeof(cli);
		int res = accept(fd, (struct sockaddr*)&cli, (socklen_t*)&len);
		assert(res != -1);
		string ip = inet_ntoa(cli.sin_addr);
		short port = ntohs(cli.sin_port);
		HashCli(fd, ip, port);
	}

	static void HashCli(int fd, const string &ip, const short &port)
	{
		char port_str[10];
		sprintf(port_str, "%d", port);
		string node = ip + port_str;
		int pos = GetHash(node);
		int i = 0;
		if(pos < 0)
		{
			pos = -pos;
		}
		int index = pos;
		cout<<index<<endl;
		for(i=0; i<65535; ++i)
		{
			index = index % 65535;
			if(ar[index] != 0)
			{
				break;
			}
		}

		Json::Value val;
		struct IpPort _ipport;
		_ipport.ip = _ipMap[ar[index]].ip;
		_ipport.port = _ipMap[ar[index]].port;

		val["ip"] = _ipport.ip;
		val["port"] = _ipport.port;
		string message = val.toStyledString();
		int len = sizeof(message);
		send(fd, message.c_str(), len, 0);
	}
private:
	static work *_work;
	static struct event_base *_baseSer;
	static struct event_base *_baseCli;
	static map<int, struct IpPort> _ipMap;
};

work *balance::_work = work::GetWork();
map<int, struct IpPort> balance::_ipMap;

event_base *balance::_baseSer = NULL;
event_base *balance::_baseCli = NULL;


