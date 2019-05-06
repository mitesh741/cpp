#include<iostream>
using namespace std;

class Test 
{
	private:
	public:
		const int t;
/*				Test(int x)
				{
				t = x;
				}*/
				

		Test(int t):t(t)
		{
		}  //For Const memeber Initializer list must be used
		int getT() 
		{
			return t; 
		}
};

int main() 
{
	Test t1(10);
	cout << t1.getT() << endl;
	return 0;
}

/* OUTPUT:
 *    10 
 *    */
