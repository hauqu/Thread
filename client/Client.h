#pragma once
/*
	ͨ�� Link ͨ�ţ����߳�client ���� communicate �����ݴ洢�� Link ������
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS //ͷ�ļ�����ԭ����ԭ�к����������������Ա���
#include <stdio.h>
#include"Link.h"
#include<iostream>
#include<thread>

using namespace std;


void communicate(Link& l);

class Client
{
public:
	WORD sockVersion;
	WSADATA wsaData;
	SOCKET sclient;
	string ServerIp;
	Link  *player;
	thread client;
public:
	Client(string ip);
	~Client();

};





Client::Client(string ip)
{

	//��ʼ��WSA  
	//����һ��2.2����Э��
	ServerIp = ip;
	sockVersion = MAKEWORD(2, 2);

	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		printf("Version error !");
	}else
	{
		cout << "�ͻ���Э��2.2�����ɹ�" << endl;
	}
	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket!");
	}
	else
	{
		cout << "Socket�����ɹ�" << endl;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(12345);
	const char* sip = ServerIp.c_str();
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//127.0.0.1 ���ػػ���ַ

	bool falg =true;
	while (falg)
	{
		if (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			cout << "�ͻ�������ʧ��" << endl;
			this_thread::sleep_for(1s);
		}
		else
		{
			falg = false;
			cout << "�ͻ������ӳɹ�" << endl;
			//send(sclient, "����ͻ���\n", 14, 0);
		}
	}
	
	player = new Link(sclient,"�ͻ�������");
	client = thread(communicate, std::ref(*player));
	
}

inline Client::~Client()
{
	closesocket(sclient);
	WSACleanup();
	
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
			l.ableS = false;
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
			l.ableR = false;
		}
		

	}
}