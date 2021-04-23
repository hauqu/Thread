#pragma once
/*

thread listenServer ���� ѭ��������������
ͨ�� Link ͨ�ţ����߳�LinkServer ���� communicate �����ݴ洢�� Link ������

*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS //ͷ�ļ�����ԭ����ԭ�к����������������Ա���
#include <stdio.h>

#include<iostream>
#include<thread>
#include<vector>
#include<string>
#include"Link.h"
#pragma comment(lib,"ws2_32.lib")  //ʹ��socket������ؿ�
using namespace std;




void communicate(Link& l);



class Server 
{
public:
	Server(u_short port);
	
	vector<Link*>players;//���ӵ������
public:
	~Server();
public:
	static const int MAX_LINK=4;
public:
	WORD sockVersion;
	WSADATA wsaData;
	SOCKET slisten;
public:
	thread listenServer;
	thread linkServer[MAX_LINK];

};

void StartListen(Server& ser);
inline Server::Server(u_short port)
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
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "������׽��ְ�ʧ�� " << endl;
	}
	else
	{
		cout << "������׽��ְ󶨳ɹ� "<< endl;
	}
	listenServer = thread(StartListen, std::ref(*this));
}
inline Server::~Server()
{
	closesocket(slisten);
	WSACleanup();
	
}
inline void StartListen(Server& ser)
{
	if (listen(ser.slisten, SOMAXCONN) == ser.MAX_LINK)//�ȴ�����������Ӹ���
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
		cout << "������ " << ser.players.size() << " ������" << endl;
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
			int i = ser.players.size();
			printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			cout << "���ӳɹ�" << endl;
			
			Link* temp = new Link(sClient, inet_ntoa(remoteAddr.sin_addr));
			ser.players.push_back(temp);
			ser.linkServer[i] = thread(communicate, std::ref(*ser.players[i]));
			
		}
		this_thread::sleep_for(100ms);
	}
}
void communicate(Link& l)
{
	while (true)
	{
		if (!l.ableR && !l.ableS)
		{
			this_thread::sleep_for(0.5s);
		}
		if (l.ableS)
		{
			l.sendData = l.s.c_str();
			send(l.sClient, l.sendData, strlen(l.sendData), 0);
		}
		if (l.ableR)
		{
			int rec = 0;
			rec = recv(l.sClient, l.revData, 255, 0);
			if (rec > 0)//���յ�����
			{
				l.revData[rec] = '\0';
				l.r = l.revData;
				rec = 0;
			}
		}

		

	}
}