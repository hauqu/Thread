#pragma once
/*
��������ҽ������ӣ�ֻ�д��������е���ҿ��Լ��뷿�䲢��ʼ��Ϸ

*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS //ͷ�ļ�����ԭ����ԭ�к����������������Ա���
#include <stdio.h>
#include <winsock2.h>//windows �µ�socket2.2
#include<iostream>
#include<thread>
#include<vector>
#include<string>
#pragma comment(lib,"ws2_32.lib")  //ʹ��socket������ؿ�
using namespace std;


class Link
{
public:
	string r;//���ܵ�����
	string s;//���͵�����
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
	
	vector<Link*>players;//���ӵ������
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
	//��ʼ��WSA  
	//����һ��2.2����Э��
	sockVersion = MAKEWORD(2, 2);
	
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "�����Э��2.2����ʧ��" << endl;
	}
	else
	{
		cout << "�����Э��2.2�����ɹ�" << endl;
	}
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//ʹ��TCP ����ͨ��
	if (slisten == INVALID_SOCKET)
	{
		cout << "������׽��ִ���ʧ��" << endl;
		
	}
	else
	{
		cout << "������׽��ִ����ɹ�" << endl;
	}
	//��IP�Ͷ˿�  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "������׽��ְ�ʧ�� " << endl;
	}
	else
	{
		cout << "������׽��ְ󶨳ɹ� "<< endl;
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
	if (listen(ser.slisten, SOMAXCONN) == SOCKET_ERROR)//SOMAXCONN ���еȴ�����������Ӹ���
	{
		cout << "�޷���������" << endl;
	}
	else
	{
		cout << "�ɹ��򿪼���" << endl;
	}
	
	while (true)
	{
		SOCKET sClient;
		sockaddr_in remoteAddr;
		int nAddrlen = sizeof(remoteAddr);
		cout << "<���ڼ���>" << endl;
		sClient = accept(ser.slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		
		cout << "��ʼ����" << endl;
		if (sClient == INVALID_SOCKET)
		{
			this_thread::sleep_for(100ms);
			cout << "����ʧ��" << endl;
			continue;//
		}
		else// ���ӳɹ�
		{
			printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			cout << "���ӳɹ�" << endl;
			send(sClient, "�����Ƿ�����\n",15,0);
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
		if (rec>0)//���յ�����
		{
			l.revData[rec] = '\0';
			l.r = l.revData;
			cout << "�������ݣ�" << l.r << endl;
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
