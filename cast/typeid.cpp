#include<iostream>
using namespace std;
class Employee 
{ 
	private:
		int id;
	public:
		void show_id(){}
};

class Programmer : public Employee 
{
	public:
		void coding(int i){ cout << i << endl; }
};

#include <typeinfo>

int main() 
{
	Employee lee;
	Programmer park;

	Employee *pEmpA = &lee;
	Employee *pEmpB = &park;

	/* check if two object is the same */
	if(typeid(Programmer) == typeid(lee)) 
	{
		Programmer *pProg = (Programmer *)&lee; 
		pProg->coding(1);
	}
	if(typeid(Programmer) == typeid(park))
	{
		Programmer *pProg = (Programmer *)&park; 
		pProg->coding(2);
	}

	pEmpA->show_id();
	pEmpB->show_id();

	return 0;
}

