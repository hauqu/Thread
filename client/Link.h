#pragma once
/*

�������շ����ݣ�ֻ�����ṩ���ݴ洢�����ñ�־λ

*/
#include<string>
#include <winsock2.h>//windows �µ�socket2.2
#pragma comment(lib,"ws2_32.lib")  //ʹ��socket������ؿ�
using namespace std;
class Link
{
public:
	string r;//���ܵ�����
	string s;//���͵�����
	SOCKET sClient;
	string name;
	char revData[255];
	const char* sendData;
	void sendS(string s)
	{
		this->s = s;
		ableS = true;
	}
	void recvR()
	{
		this->r = "";
		ableR = true;
	}
public:
	Link(SOCKET& c, string l_name);


	bool ableR;
	bool ableS;
};
Link::Link(SOCKET& c, string l_name)
{
	sClient = c;
	ableR = false;
	ableS = false;
	name = l_name;

}