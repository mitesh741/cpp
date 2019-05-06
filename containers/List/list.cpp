#include<iostream>
#include<list>
using namespace std;
int main()
{

	list<int> first ; 


	for(int i = 0 ; i < 5 ; i++ )
		first.push_back(i + 10);

//	cout << "Size of List " << first.size() << endl;
//	cout << "Contents of List " << endl;

	list<int> ::iterator p = first.begin() ;
//	for(p = first.begin() ; p != first.end() ; p++)
//		cout << *p << endl;

while(p != first.end())
{
cout << "*p " << (*p) ;
cout << "list " << endl ;
}

//	for( p = first.begin() ; p != first.end() ; p++)
//		*p += 10;

//	cout << "Contents of modified list" << endl; 
//	for( p = first.begin() ; p != first.end() ; p++)		
//		cout << *p << endl;

			return 0 ;
}
