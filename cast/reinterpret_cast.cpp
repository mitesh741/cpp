#include <iostream>
using namespace std;

struct data
{
	short a; 
	short b;
};

int main () 
{
	long value = 123456789;
	data* pdata = reinterpret_cast<data*> (&value);
	cout << pdata->a << endl;
	return 0;
}
/*It converts any pointer type to any other pointer type, even of unrelated classes. The operation result is a simple binary copy of the value from one pointer to the other. All pointer conversions are allowed: neither the content pointed nor the pointer type itself is checked.*/
