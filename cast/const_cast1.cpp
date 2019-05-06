#include <iostream>
#include <cstring>
using namespace std;
int main ()
{
	string str("A123456789");
	const char *cstr = str.c_str();
	char *nonconst_cstr  = const_cast<char *> (cstr) ; 
	*nonconst_cstr ='B';
	cout << nonconst_cstr << endl;
	return 0;
}

