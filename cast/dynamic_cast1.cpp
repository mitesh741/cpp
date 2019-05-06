#include <iostream>
using namespace std;

class Employee 
{ 
	private:
		int id;
	public:
		void show_id()
		{
		}
};

class Programmer : public Employee 
{
	public:
		void coding()
		{
		}
};

int main() 
{
	Employee employee;
	Programmer programmer;

	/* upcast - implicit upcast allowed */
	Employee *pEmp = &programmer;

	/* downcast - explicit type cast required */
	Programmer *pProg = (Programmer *)&employee;


	/* Upcasting: safe - progrommer is an Employee */
	/* and has his id to do show_id(). */
	pEmp->show_id();
	pProg->show_id();

	/* Downcasting: unsafe - Employee does not have */
	/* the method, coding(). */
	/* compile error: 'coding' : is not a member of 'Employee'  */
	/* pEmp->coding(); */
	pProg->coding();

	return 0;
}

