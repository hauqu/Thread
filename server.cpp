#include"Server.h"
using namespace std;
int main()
{
	Server* test =new Server();
	string temp="";
	int i = 0;
	while (true)
	{
		if (!test->players.empty())
		{
			
			test->players[0]->recvR();
			
			if (!test->players[0]->r.empty())
			{
				if (temp != test->players[0]->r)
				{
					cout << "���յ���" <<test->players[0]->name<<": "<<endl
						<< test->players[0]->r << endl;
					temp = test->players[0]->r;
				}
			}
			//test->players[0]->sendS(to_string(i++) + " :���пͻ���1");
			
		}
		
		
	}
	return 0;
}