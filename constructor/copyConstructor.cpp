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
			len=strlen(a);
			name=new char[len+1];
			strcpy(name,a);
		}
		void display()
		{
			cout << "Name= "<< name << endl;
			cout << "Length= "<< len << endl;
		}
};
int main()
{
	String X("Rama");
	String y = X;
	X.display();
	y.display();

	return 0;
}
