#include<iostream>
using namespace std;
enum dayOfWeek {M,TU,W,Th,F,SA,SN};
int main()
{
	dayOfWeek d = F;

	cout << d << endl;
	cout << dayOfWeek(SA) << endl;
	return 0;
}
