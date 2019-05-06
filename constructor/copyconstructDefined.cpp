#include<iostream>
#include<string.h>
using namespace std;
class String
{
	private:
		char *name;
		int len;
	public:
		String(char *a)
		{
			if(a == NULL)
			{
				name = NULL;
				len = 0 ;
			}
			else
			{
				len = strlen(a);
				name = new char[len+10];
				strcpy(name,a);
			}
		}
		String(const String &x)
		{
			if(x.name == NULL)
			{
				name = NULL;
				len = 0;
			}
			else
			{
				len = strlen(x.name);
				name = new char[len+1];
				strcpy(name,x.name);
			}
		}
		void display()
		{
			cout << "Name = " << name << endl;
			cout << "Length = " << len << endl;
		}
};
int main()
{
	String x("Rama");
	String y(x);
	x.display();
	y.display();
	return 0;
}
