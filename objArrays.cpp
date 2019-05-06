#include<iostream>
using namespace std;
class Student
{
	private:
		char USN[10];
		char name[10];
	public:
		void read();
		void display();

};
void Student::read()
{
	cin >> USN >> name ;
}
void Student::display()
{
	cout << USN << name << endl;
}
int main()
{
	Student a[10];
	cout << "Enter the number of students" << endl;
	int n;
	cin  >> n;
	for(int i = 0 ; i < n ; i++ )
	{
		cout << " Enter USN and name" << endl;
		a[i].read();
	}
	cout << "USN  Name" << endl;
	for(int i = 0; i < n ; i++)
	{
		a[i].display();
	}
	return 0;
}
