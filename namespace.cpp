#include<iostream>
using namespace std;
namespace mitesh
{
	int m = 5;
}
namespace patel
{
	float m = 3.14;
}
int main()
{

	cout << mitesh::m << endl;
	mitesh::m = 40;
	cout << mitesh::m <<endl;

	cout << patel::m << endl;
	patel::m = 2.3;
	cout << patel::m << endl;
	return 0;
}
