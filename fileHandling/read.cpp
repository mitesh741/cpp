#include<fstream>
#include<iostream>

using namespace std;

int main()
{
int count = 0;
	ifstream fin;
	fin.open("out.txt");

	char ch;

	while(!fin.eof())
	{
		fin.get(ch);
		cout << ch;
		count++;
	}

	cout << "count " << count << endl;

	fin.close();
	return 0 ;
}
