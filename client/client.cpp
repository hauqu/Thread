#include"Client.h"
#include<iostream>
using namespace std;
string ip = "127.0.0.1";
int main()
{
	Client test(ip);
	
	int i = 1;
	while (true)
	{
		
		cout << "�ͻ������߳�ִ����..........." << endl;
		
		string s;
		getline(cin,s);
		test.player->sendS(s);
	}
	return 0;
}