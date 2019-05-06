


#include<iostream>
#include <string> // for string
using namespace std;
int main()
{

	string s = "mitesh";


	cout << s.length() << endl ;

	s.insert(s.begin(), 6 - s.size(), 'X');

	cout << s << endl ;
	return 0;
}

