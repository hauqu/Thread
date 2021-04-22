#pragma once
/*
用来与玩家进行连接，只有处于连接中的玩家可以加入房间并开始游戏

*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS //头文件升级原因导致原有函数报错，加上这句忽略报错
#include <stdio.h>
#include <winsock2.h>//windows 下的socket2.2
#include<iostream>
#include<thread>
#include<vector>
#include<string>
#pragma comment(lib,"ws2_32.lib")  //使用socket必须加载库
using namespace std;


class Link
{
public:
	string r;//接受的数据
	string s;//发送的数据
	SOCKET sClient;
	thread client;
	char revData[255];
	const char *sendData;
	
public:
	Link(SOCKET c);
	void startLink();
	
};
void creatLink(SOCKET& s, vector<Link>&players);
void communicate(Link& l);
Link::Link(SOCKET c)
{
	sClient = c;
	
}
inline void Link::startLink()
{
	client = thread(communicate, std::ref(*this));
}

class Server 
{
public:
	Server();
	
	vector<Link*>players;//连接的玩家们
public:
	~Server();
public:
	WORD sockVersion;
	WSADATA wsaData;
	SOCKET slisten;
	thread server;
	
};

void StartListen(Server& ser);
inline Server::Server()
{
	//初始化WSA  
	//启动一个2.2网络协议
	sockVersion = MAKEWORD(2, 2);
	
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "服务端协议2.2启动失败" << endl;
	}
	else
	{
		cout << "服务端协议2.2启动成功" << endl;
	}
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//使用TCP 进行通信
	if (slisten == INVALID_SOCKET)
	{
		cout << "服务端套接字创建失败" << endl;
		
	}
	else
	{
		cout << "服务端套接字创建成功" << endl;
	}
	//绑定IP和端口  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "服务端套接字绑定失败 " << endl;
	}
	else
	{
		cout << "服务端套接字绑定成功 "<< endl;
	}
	server = thread(StartListen, std::ref(*this));
}
inline Server::~Server()
{
	closesocket(slisten);
	WSACleanup();
	
}
inline void StartListen(Server& ser)
{
	if (listen(ser.slisten, SOMAXCONN) == SOCKET_ERROR)//SOMAXCONN 队列等待处理最大连接个数
	{
		cout << "无法继续监听" << endl;
	}
	else
	{
		cout << "成功打开监听" << endl;
	}
	
	while (true)
	{
		SOCKET sClient;
		sockaddr_in remoteAddr;
		int nAddrlen = sizeof(remoteAddr);
		cout << "<正在监听>" << endl;
		sClient = accept(ser.slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		
		cout << "开始连接" << endl;
		if (sClient == INVALID_SOCKET)
		{
			this_thread::sleep_for(100ms);
			cout << "连接失败" << endl;
			continue;//
		}
		else// 连接成功
		{
			printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			cout << "连接成功" << endl;
			send(sClient, "这里是服务器\n",15,0);
			ser.players.push_back(&Link(sClient));
		}
		this_thread::sleep_for(100ms);
	}
}

void communicate(Link&l)
{
	int rec = 0;
	while (true)
	{
		rec = recv(l.sClient, l.revData, 255, 0);
		if (rec>0)//接收到数据
		{
			l.revData[rec] = '\0';
			l.r = l.revData;
			cout << "接受数据：" << l.r << endl;
			rec = 0;
		}
		if (!l.s.empty())
		{
			l.sendData = l.s.c_str();
			send(l.sClient, l.sendData, strlen(l.sendData), 0);
			l.s = "";
		}
		this_thread::sleep_for(50ms);
		
	}
}
