#include"Server.h"
using namespace std;
int main()
{
	Server* test =new Server(12345);
	string temp="";
	int i = 0;
	while (true)
	{
		
		if (!test->players.empty())
		{
			for (int i = 0; i < test->players.size(); i++)
			{


				test->players[i]->recvR();

				if (!test->players[i]->r.empty())
				{
					if (temp != test->players[i]->r)
					{
						cout << "½ÓÊÕµ½£º" << test->players[i]->name << ": " << endl
							<< test->players[i]->r << endl;
						temp = test->players[i]->r;
					}
				}
				
			}
		}
		
		
	}
	return 0;
}